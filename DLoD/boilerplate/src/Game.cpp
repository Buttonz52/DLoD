#include "Game/Game.h"

GEO* initGroundPlane()
{
  GEO* plane = new GEO();
 // plane->setFilename("plane.obj");
  if (!plane->initMesh("plane.obj")) {
    cout << "Failed to initialize mesh." << endl;
  }
  if (!plane->initTexture("textures/ground.png", GL_TEXTURE_2D)) {
    cout << "Failed to initialize plane." << endl;
  }
  plane->addShaders("shaders/tex2D.vert", "shaders/tex2D.frag");
  plane->setScale(vec3(150.f));
  plane->setPosition(vec3(0, -0.7, 0));
  plane->updateModelMatrix();

  return plane;
}

Game::Game(GLFWwindow *w, Audio audio, const string &skyboxFilepath, const string &arenaFilepath, const string &starObjFilename, const string &arenaMapFile, const vector<int> *humanVehicleChoice, const int numPlayers, const vector<vec3> spawnPoints)
{

	pause = false, restart = false;
	menuIndex = 0;
	//set cameras so overtop and far-ish away from cars
	overheadCam.setAlt(-90);
	overheadCam.setRadius(450);

	winningCam.setAlt(-90);
	winningCam.setRadius(50);

  window = w;
  numPlayerScreens = numPlayers;
  glfwGetWindowSize(window, &width, &height);

	//windows = w;
  physX.init(4);

  initSkyBox(skyboxFilepath);
  //skybox->children.push_back(initArena());
  //create the starnode
  vector<vec3> starNodes;
  GEO aStarGEO;
  aStarGEO.initMesh(starObjFilename);	//load vertices
  gameState = new GameState(aStarGEO.getMesh().vertices);


  arena = initArena(arenaTexFilename, arenaFilepath);
  arenaMap = arenaMapFile;
  //skybox->children.push_back(initGroundPlane());

  for (int i = 0; i < numPlayers; i++) {
	  Human* human = new Human(i);
	  human->ChooseVehicle(humanVehicleChoice->at(i));
	  human->vehicle->setPosition(spawnPoints[i]);
	  human->vehicle->setEnvironmentMap(skybox->getTexture());
	  initVehicle(human->vehicle, humanVehicleChoice->at(i));
	  skybox->children.push_back(human->vehicle);
	  players.push_back(human);

    // Eventually this should be the only list of the players
    gameState->players.push_back(human);
  }

  //Adding a few AIs for the time being
  //random number generator to choose AI cars
  //create AIs so that there are 8 cars in the arena
  //NOTE: This gets pretty slow, might want to think about multi-threading or trying to run in release mode (but need to link those libraries)

  srand(time(NULL));
  for (int i = numPlayers; i < 4; i++) {
	  AI* ai = new AI(i);
	  int aiRNGChoose = rand() % 3;
	  ai->ChooseVehicle(aiRNGChoose);
	  ai->vehicle->setPosition(spawnPoints[i]);
	  ai->vehicle->setEnvironmentMap(skybox->getTexture());
	  initVehicle(ai->vehicle, aiRNGChoose);
	  skybox->children.push_back(ai->vehicle);
	  players.push_back(ai);

    // Eventually this should be the only list of the players
    gameState->players.push_back(ai);
  }
  vector<vec3> positions;
  for (int i = 0; i < 4; i++) {
	  positions.emplace_back();
  }
	gameHud.InitializeHud(*players[0]->vehicle->getColour(), &positions, arenaMap);
	gameHud.InitializeMenu(vec3(1, 0, 1));
}

bool Game::start()
{

  // start the game loop
  gameState->timer.start();

  pauseText.setScale(vec3(4.f));
  pauseText.InitializeGameText("PAUSE", vec3(-0.4, 0.5, 0), vec3(1, 0.5, 0.3), 30);
  gameLoop();

  // Clean up and Display the win screen

  ///TODO: implmement for all screens -> lose/win for appropriate player
  ScreenOverlay endGameText;
  endGameText.setScale(vec3(4.f));
  if (!gameOver) {
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//  for (int i = 0; i < numPlayerScreens; i++) {
		  ResizeViewport(0, 1, width, height);
		  endGameText.InitializeGameText("RETIRE", vec3(-0.4, 0, 0), vec3(1,0.5,0.3), 30);
		  endGameText.Render(GL_TRIANGLES, endGameText.getColour());
	//  }
  }
  else {
	  for (int i = 0; i < numPlayerScreens; i++) {
		  ResizeViewport(i, numPlayerScreens, width, height);
		  if (players[i]->isDead()) {
			  endGameText.InitializeGameText("LOSE!", vec3(-0.35, 0, 0), vec3(1, 0, 0), 30);
		  }
		  else {
			  endGameText.InitializeGameText("WIN!", vec3(-0.3, 0, 0), vec3(0, 1, 0), 30);
		  }
		  endGameText.Render(GL_TRIANGLES, endGameText.getColour());
	  }
  }
  glfwSwapBuffers(window);

	//	audio.PlaySfx(winSFX);
	//	audio.PlaySfx(loseSFX);

  bool pause = true;

  while (!restart && pause && !glfwWindowShouldClose(window))
  {
	  for (Player* p : players)
	  {
		  Human* human = dynamic_cast<Human*> (p);
		  if (human != nullptr)
			  human->getGameOverInput(window,pause);
	  }

	  glfwPollEvents();
  }
  endGameText.Destroy();
  pauseText.Destroy();
  gameHud.Destroy();
  delete skybox;
  delete arena;
  physX.cleanupPhysics(true);
  return restart;
}

void Game::gameLoop()
{
	vector<vec3> positions;
	for (Player *p : players) {
			positions.push_back(p->vehicle->getModelMatrix()[3]);
	}
  //Initialize hud

  int frameCtr = 0;

  while (!glfwWindowShouldClose(window) && !gameOver && !restart)
  {
    //game paused
    while (pause && !glfwWindowShouldClose(window)) {
	    goToGamePausedState();
    }	 

    // Add items to the scene
    vector<pair<Player*, int>>::iterator itr = itemsToAdd.begin();
    while (itr != itemsToAdd.end()) {
      if (itr->second < gameState->timer.getTicks()) {
        itr->first->ableToTrap = true;
        itr->first->layTrap = false;
        itr = itemsToAdd.erase(itr);
        break;
      }
      else {
        ++itr;
      }
    }

    // remove the deleted geometry
    for (GEO* g : physX.deletedGeos)
      skybox->removeChild(g);

    physX.deletedGeos.clear();

    // Step Physx
    physX.stepPhysics(true, physXObjects);

    //clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //render screens
    //TODO: Make less.. ugly and long
    int viewports = numPlayerScreens;
    if (numPlayerScreens >= 3) {
	    viewports = 4;
    }
    mat4 projectionMatrix, viewMatrix;
    string healthStr, armourStr, velocityStr;
    bool canLayTrap;
    vec3 vColour;
    for (int i = 0; i < viewports;i++) {
	    //getviewport
	    ResizeViewport(i, numPlayerScreens, width, height);
	
		  // Render
		  //make sure rendering a player screen that 
		  //1) exists
		  //2) is not dead
		  //3) is defined as a human
		  if (i < players.size() && !players[i]->isDead() && i < numPlayerScreens) {
			  UpdateHudInfo(players[i], projectionMatrix, viewMatrix, healthStr, armourStr, velocityStr, vColour, canLayTrap);
		  }
		  else {
			  UpdateHudInfoEmpty(players, i, projectionMatrix, viewMatrix, winningCam, overheadCam, healthStr, armourStr, velocityStr, vColour);
		  }
		
		  //render the game hud
		  glEnable(GL_CULL_FACE);
		  glCullFace(GL_BACK);
	//  glEnable(GL_FRAMEBUFFER_SRGB);
		  arena->Render(viewMatrix, projectionMatrix, lightSource);
	//	  glDisable(GL_FRAMEBUFFER_SRGB);

		  glDisable(GL_CULL_FACE);
		  skybox->Render(viewMatrix, projectionMatrix, lightSource);
		  gameHud.Render(healthStr, armourStr, velocityStr, &positions, vColour, canLayTrap);
	  }

	  glfwSwapBuffers(window);
	
    positions.clear();
    // For all players get input
    for (Player* p : players)
    {
    //get radar points for radar map
    if (!p->isDead())
	    positions.push_back(p->vehicle->getModelMatrix()[3]);
    //kill player if out of bounds
    if (p->vehicle->getModelMatrix()[3].y < -10)
	    p->vehicle->updateHealth(1000);

    AI* ai = dynamic_cast<AI*> (p);
    if (ai != nullptr && !ai->isDead())
    {
      ai->getInput(gameState);
	    //ai->driveTo(players[0]->vehicle->getModelMatrix()[3]);
	    if (abs(ai->vehicle->physXVehicle->computeForwardSpeed()) > 20)
		    ai->layTrap = true;
    }

    Human* human = dynamic_cast<Human*> (p);
    if (human != nullptr && !human->isDead()) {
	    human->getInput(window, pause);
	    if (human->pressedPause()) {
		    menuIndex = 0;
		    pauseIdentifier = human->identifier;
	    }
    }

    p->playerCam->followVehicle(p->vehicle);

    if (p->layTrap && p->ableToTrap)
    {
	    // create a new item at the appropriate location and add it to the items list
	    mat4 M = p->vehicle->getModelMatrix();
	    vec3 vPos = vec3(M[3]);
	    vec3 dis = vec3(0, 8, 0);

	    Item* item = new Item(p->trap);
	    mat4 m = mat4(1);
	    m[3] = vec4(vPos + dis, 1);
	    item->setModelMatrix(m);

	    itemsToAdd.push_back(make_pair(p, gameState->timer.getTicks() + 5000));

	    initItem(item);

	    p->layTrap = false;
	    p->ableToTrap = false;
    }
		
  }

    // CHECK GAMEOVER
    int aliveCount = 0;
    for (Player* p : players)
      aliveCount += p->isDead() ? 0 : 1;

	  //regenerate armour
	  if (frameCtr % 60*8 == 0)
	  {
		  for (Player* p : players) 
			  p->vehicle->regenArmour();
	  }
	  frameCtr >= 60*8 ? frameCtr = 1: frameCtr++;
    gameOver = aliveCount < 2;

    glfwPollEvents();
  }
}

void Game::initSkyBox(const string &pathname)
{
	string skyboxConfigFile = pathname +"skyboxFile.txt";
	ifstream f_stream(skyboxConfigFile);
	if (!f_stream.is_open()) {
		cout << "Error loading file " << skyboxConfigFile << endl;
		return;
	}
	//load the file
	string line;
	size_t found;
	vector<string> skyboxFiles;
	cout << "Loading file " << skyboxConfigFile << endl;
	while (getline(f_stream, line)) {
		//If there is a #, then there is a new celestial body to be added
		if (line.length() == 0) {
			continue;
		}
		if ((found = line.find("bump")) != string::npos) {
			//arenaBumpmap = line;
			continue;
		}
		skyboxFiles.push_back(pathname +line);
	}
	arenaTexFilename = skyboxFiles[3]; //ground 

  skybox = new GEO();
  skybox->setColour(vec3(0, 0, 0));
  if (!skybox->initMesh("cube.obj")) {
    cout << "Failed to initialize skybox mesh." << endl;
  }
  //scale cube large
  skybox->setScale(vec3(500.0));
  skybox->updateModelMatrix();
  if (!skybox->initSkybox(skyboxFiles)) {
    cout << "Failed to initialize skybox texture." << endl;
  }
  skybox->addShaders("shaders/skybox.vert", "shaders/skybox.frag");
}

void Game::initVehicle(Vehicle* v, int type)
{
  v->setScale(vec3(1.5));
 
  if (!v->initMesh("cube.obj")) {	//dead mesh
    cout << "Failed to initialize mesh." << endl;
  }
  v->addShaders("shaders/toon.vert", "shaders/toon.frag");


  if (!v->initBuffers()) {
    cout << "Could not initialize buffers for game object " << v->getAliveCarMesh() << endl;
  }

  physX.initVehicle(v, type);

  physXObjects.push_back(v);
}

//
void Game::initItem(Item* item)
{
  item->setScale(vec3(2));
  item->setColour(vec3(1, 1, 0));

  if (!item->initMesh("/ObjModels/bearTrap.obj")) {	//dead mesh
    cout << "Failed to initialize bear trap." << endl;
  }
  item->addShaders("shaders/toon.vert", "shaders/toon.frag");
  

  mat3 scaleM = mat3(1);
  scaleM[0][0] = item->getScale().x;
  scaleM[1][1] = item->getScale().y;
  scaleM[2][2] = item->getScale().z;

  for (int i = 0; i < item->getMesh().vertices.size(); ++i)
	  item->getMesh().vertices[i] = scaleM * item->getMesh().vertices[i];

  if (!item->initBuffers()) {
	  cout << "Could not initialize buffers for initialized item." << endl;
  }
  physX.initItem(item);
  item->setEnvironmentMap(skybox->getTexture());
  skybox->children.push_back(item);
}


//initialize arena
GEO* Game::initArena(const string &texfilename, const string &objfilename) {
	GEO *arena = new GEO();
	arena->setColour(vec3(1, 0, 0));

	if (!arena->initMesh(objfilename)) {
		cout << "Failed to init arena" << endl;
	}
	arena->setEnvironmentMap(skybox->getTexture());
	arena->setReflectance(0.1);
	if (!arena->initTexture("textures/ground.png", GL_TEXTURE_2D)) {
	//if (!arena->initTexture("textures/lightmaps/pisa.hdr", GL_TEXTURE_2D)) {	//NOTE: have to fix this for cube map; this doesn't work
//	if (!arena->initTexture("textures/lightmaps/pisa_cube_radiance.hdr", GL_TEXTURE_CUBE_MAP)) {	//NOTE: have to fix this for cube map; this doesn't work
		cout << "Failed to initialize arena ground texture." << endl;
	}

	//arena->addShaders("shaders/tex2D.vert", "shaders/tex2D.frag");
	arena->addShaders("shaders/hdr.vert", "shaders/hdr.frag");

	arena->setScale(vec3(30.f));
	arena->setPosition(vec3(0, 0, 0));
	arena->updateModelMatrix();

	physX.initArena(arena);
	return arena;
}

void Game::ResizeViewport(const int index, const int numPlayerScreens, const int width, const int height) {

	// Set up the game
	int vHeight = 0, vWidth = 0;	//viewport starting point for height
	int wSplit = 1, hSplit = 1;
	switch (numPlayerScreens) {
		//1 human
	case 1:
		break;
		//2 humans
	case 2:
		wSplit = 2;
		break;
		// >3 players
	default:
		wSplit = 2;
		hSplit = 2;
		//top screens for players 1 and 2
		if (index < 2) {
			vHeight = height / 2;
		}
		//bottom screens for players 3 and 4 = 0, so do nothing
	}

	//resixe viewport
	glViewport((index % 2)*float(width) / 2, vHeight, width / wSplit, height / hSplit);
}

//State when the game is paused (start button/esc pressed) in game
void Game::goToGamePausedState() {
	audio.ChangeMusicVolume(MIX_MAX_VOLUME / 4);	//volume decreased
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	vec3 pauseColour(0);	//colour for pause menu
	for (Player *p : players) {
		Human* human = dynamic_cast<Human*> (p);
		//get pause input for human that pressed pause
		if (human != nullptr && human->pressedPause()) {
			pauseColour = *human->getColour();	//set pause menu colour
			human->menuControls(window, pause, menuIndex, &audio);
			//index check
			if (menuIndex > 2)
				menuIndex = 0;
			if (menuIndex < 0)
				menuIndex = 2;
			if (human->MenuItemSelected()) {
				switch (menuIndex) {
				case 0:	//pressed "resume"
					human->menuItemPressed = false;
					audio.ChangeMusicVolume(MIX_MAX_VOLUME);
					break;
				case 1:
					restart = true;
					break;
				case 2:	//pressed "quit"
					glfwSetWindowShouldClose(window, true);
					break;
				default:	//pressed "resume"
					human->menuItemPressed = false;
					audio.ChangeMusicVolume(MIX_MAX_VOLUME);
					break;
				}
			}
		}
	}
	//resize viewport so renders fullscreen
	ResizeViewport(0, 1, width, height);
	pauseText.Render(GL_TRIANGLES, pauseColour);
	gameHud.RenderMenu(menuIndex, pauseColour);	//render menu

	glfwSwapBuffers(window);
	glfwPollEvents();
	//volume back to max volume
	audio.ChangeMusicVolume(MIX_MAX_VOLUME);
}

//updates information for the game hud if it is a player
void Game::UpdateHudInfo(Player * player, mat4 &projectionMatrix, mat4 &viewMatrix, string &healthStr, string &armourStr, string &velocityStr, vec3 &vColour, bool &canLayTrap) {
		projectionMatrix = player->playerCam->calculateProjectionMatrix();
		viewMatrix = player->playerCam->calculateViewMatrix();
		healthStr = player->vehicle->getHealthString();
		armourStr = player->vehicle->getArmourString();
		velocityStr = player->vehicle->getVelocityString();
		vColour = *player->getColour();
		canLayTrap = player->ableToTrap;
}

//updates hud if there is no player in current screen
void Game::UpdateHudInfoEmpty(const vector <Player*> players, const int &i, mat4 &projectionMatrix, mat4 &viewMatrix, Camera &winningCam, Camera &overheadCam, string &healthStr, string &armourStr, string &velocityStr, vec3 &vColour) {

	//otherwise, render either overhead camera or current win camera
		if (i == 0 || i == 3) {	//overhead cameras for bottom right or top left
								//overhead cam
			projectionMatrix = overheadCam.calculateProjectionMatrix();
			viewMatrix = overheadCam.calculateViewMatrix();
		}
		//overhead cam for player with highest health
		else {
			int health = 0, playerIndex = 0;
			for (int j = 0; j < players.size(); j++) {
				if (players[j]->vehicle->getHealth() > health) {
					health = players[j]->vehicle->getHealth();
					playerIndex = j;
				}
			}
			winningCam.followVehicle(players[playerIndex]->vehicle);
			projectionMatrix = winningCam.calculateProjectionMatrix();
			viewMatrix = winningCam.calculateViewMatrix();
		}
		healthStr = "000";
		armourStr = "000";
		velocityStr = "00";
		vColour = vec3(0);
}


