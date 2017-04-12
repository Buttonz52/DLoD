#include "Game/Game.h"

Game::Game(GLFWwindow *w, Audio audio, const string &skyboxFilepath, const string &arenaFilepath, const string &starObjFilename, const string &arenaMapFile, const vector<int> *humanVehicleChoice, const int numPlayers, const vector<vec3> spawnPoints, const vector<vec3> itemSpawnPoints)
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

  physX.init(4);

  srand(time(NULL));

  initSkyBox(skyboxFilepath);

  //create the starnode
  vector<vec3> astarVertices;
  GEO aStarGEO;
  aStarGEO.initMesh(starObjFilename);	//load vertices
  for (vec3 v : aStarGEO.getMesh().vertices)
  {
    v *= 30;
    astarVertices.push_back(v);
  }

  gameState = new GameState(astarVertices, itemSpawnPoints);

  arena = initArena(arenaTexFilename, arenaFilepath);
  arenaMap = arenaMapFile;

  for (int i = 0; i < numPlayers; i++) {
	  Human* human = new Human(i, &audio);
	  human->setNumCams(5);
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

  pauseText.setScale(vec3(4.f));
  pauseText.InitializeGameText("PAUSE", vec3(-0.4, 0.5, 0), vec3(1, 0.5, 0.3), 30);

  switchCamText.setScale(vec3(0.5f));
  switchCamText.InitializeGameText("Press <TAB> or <BACK> to change between cameras", vec3(-0.4, -0.8, 0), vec3(1, 1, 1), 30);
  
  // Enter the game Loop
  gameState->timer.start();
  for (ItemSpawner* s : gameState->itemSpawners)
    s->timer.start();
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
  Sleep(1000);
	//	audio.PlaySfx(winSFX);
	//	audio.PlaySfx(loseSFX);

  bool pause = true;
  Human *h = dynamic_cast<Human*> (players[0]);

  vector <string> loserVectorNames;
  vector<vec3> loserVectorColours;
  vector<clock_t> ToDs;
  
  //Adds players in rank based on when they died

  for (int i = 0; i < players.size(); i++) {
	  std::stringstream fmt;
	  Human* human = dynamic_cast<Human*> (players[i]);
	  //Player 
	  if (human != nullptr)
		  fmt << "Player " << i + 1;
	  //Otherwise, is an AI
	  else
		  fmt << "AI " << i -(numPlayerScreens-1);
	 
	  //If not dead, then automatically goes to front of list
	  if (!players[i]->isDead()) {
		  loserVectorNames.insert(loserVectorNames.begin(), fmt.str());	//first place
		  loserVectorColours.insert(loserVectorColours.begin(), *players[i]->getColour());
		  ToDs.insert(ToDs.begin(),0);
	  }
	  //Else, iterate over list and find proper location
	  else {
		  bool inList = false;
		  for (int j = 0; j < ToDs.size(); j++) {
			  if (!inList) {
				  //died earlier than person
				  if (players[i]->getTimeOfDeath() > ToDs[j] && ToDs[j] !=0) {
					  loserVectorNames.insert(loserVectorNames.begin() + j, fmt.str());	
					  loserVectorColours.insert(loserVectorColours.begin() + j, *players[i]->getColour());
					  ToDs.insert(ToDs.begin() + j, players[i]->getTimeOfDeath());
					  inList = true;
				  }
			  }
		  }
		  //If not in list, then push to back of list (last place)
		  if (!inList)
		  {
			  loserVectorNames.push_back( fmt.str());
			  loserVectorColours.push_back(*players[i]->getColour());
			  ToDs.push_back(players[i]->getTimeOfDeath());
		  }
	  }
  }
  gameHud.InitializeEndGame(loserVectorNames, loserVectorColours);
  pauseText.InitializeGameText("END GAME", vec3(-0.57, 0.5, 0), vec3(1, 0.5, 0.3), 30);

  while (!restart && !glfwWindowShouldClose(window))
  {
	  h->pausePressed = true;
	  for (Player* p : players)
	  {
		  Human* human = dynamic_cast<Human*> (p);
		  if (human != nullptr) {
			  goToEndGameState();
		  }
	  }

	  glfwPollEvents();
  }
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
    {
      skybox->removeChild(g);
      delete g;
    }

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
	
		  // Render
		  //make sure rendering a player screen that 
		  //1) exists
		  //2) is not dead
		  //3) is defined as a human
		  if (i < players.size() && !players[i]->isDead() && i < numPlayerScreens) {
			  UpdateHudInfo(players[i], projectionMatrix, viewMatrix, healthStr, armourStr, velocityStr, vColour, canLayTrap);
		  }
		  //player is dead
		  else {
			  int camIndex = 0;
			  Human* human = dynamic_cast<Human*> (players[i]);
			  if (human != nullptr) {
				  camIndex = human->camIndex;
			  }
			  UpdateHudInfoEmpty(players, i, projectionMatrix, viewMatrix, winningCam, overheadCam, healthStr, armourStr, velocityStr, vColour, camIndex);
		  }
	
		  //render normally
		  //resize the viewport
		  glEnable(GL_CULL_FACE);
		  glCullFace(GL_BACK);

		  arena->Render(viewMatrix, projectionMatrix, lightSource);

		  glDisable(GL_CULL_FACE);
		  skybox->Render(viewMatrix, projectionMatrix, lightSource);
		  if (players[i]->isDead())
			 switchCamText.Render(GL_TRIANGLES, vColour);
		  vector<vec3> radarColours;
		  for (int i = 0; i < players.size(); i++)
			  radarColours.push_back(*players[i]->getColour());
		  gameHud.Render(healthStr, armourStr, velocityStr, &positions, vColour, &radarColours, canLayTrap);
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
	    p->vehicle->updateHealth(10000);

    AI* ai = dynamic_cast<AI*> (p);
    if (ai != nullptr && !ai->isDead())
    {
      ai->getInput(gameState);
	    if (abs(ai->vehicle->physXVehicle->computeForwardSpeed()) > 20)
		    ai->layTrap = true;
    }

    Human* human = dynamic_cast<Human*> (p);
    if (human != nullptr) {
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

	    Item* item = new Item(p->trap, nullptr);
	    mat4 m = mat4(1);
	    m[3] = vec4(vPos + dis, 1);
	    item->setModelMatrix(m);

	    itemsToAdd.push_back(make_pair(p, gameState->timer.getTicks() + 5000));

	    initItem(item);

	    p->layTrap = false;
	    p->ableToTrap = false;
    }
  }

    for (ItemSpawner* spawner : gameState->itemSpawners)
    {
      if (spawner->timer.getTicks() > spawner->spawnTime && spawner->item == nullptr) {
        spawner->spawnItem(skybox);
        physX.initItem(spawner->item);
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
  skybox->setScale(vec3(1500.0));
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
    cout << "Could not initialize buffers for game object car mesh"<< endl;
  }

  physX.initVehicle(v, type);

  physXObjects.push_back(v);
}

//
void Game::initItem(Item* item)
{
  item->setScale(vec3(2));

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
	arena->setReflectance(0.5);
	arena->setExposure(0.7);
	if (!arena->initTexture("textures/gold.png", GL_TEXTURE_2D)) {
	//if (!arena->initTexture("textures/ground.png", GL_TEXTURE_2D)) {

		cout << "Failed to initialize arena ground texture." << endl;
	}

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

void Game::goToEndGameState() {
	//audio.ChangeMusicVolume(MIX_MAX_VOLUME / 4);	//volume decreased
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	vec3 pauseColour(0);	//colour for pause menu
	for (Player *p : players) {
		Human* human = dynamic_cast<Human*> (p);
		//get pause input for human that pressed pause
		if (human != nullptr && human->pressedPause()) {
			pauseColour = *human->getColour();	//set pause menu colour
			human->menuControls(window, pause, menuIndex);
			//index check
			if (menuIndex > 1)
				menuIndex = 0;
			if (menuIndex < 0)
				menuIndex = 1;
			if (human->MenuItemSelected()) {
				switch (menuIndex) {
				case 0:
					restart = true;
					break;
				case 1:	//pressed "quit"
					glfwSetWindowShouldClose(window, true);
					break;
				}
			}
		}
	}
	//resize viewport so renders fullscreen
	ResizeViewport(0, 1, width, height);
	pauseText.Render(GL_TRIANGLES, pauseColour);
	gameHud.RenderEndGame(menuIndex, pauseColour);	//render menu

	glfwSwapBuffers(window);
	glfwPollEvents();
	//volume back to max volume
	//audio.ChangeMusicVolume(MIX_MAX_VOLUME);

}
//State when the game is paused (start button/esc pressed) in game
void Game::goToGamePausedState() {
//	audio.ChangeMusicVolume(MIX_MAX_VOLUME / 4);	//volume decreased
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	vec3 pauseColour(0);	//colour for pause menu
	for (Player *p : players) {
		Human* human = dynamic_cast<Human*> (p);
		//get pause input for human that pressed pause
		if (human != nullptr && human->pressedPause()) {
			pauseColour = *human->getColour();	//set pause menu colour
			human->menuControls(window, pause, menuIndex);
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
	//audio.ChangeMusicVolume(MIX_MAX_VOLUME);
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
void Game::UpdateHudInfoEmpty(const vector <Player*> players, const int &i, mat4 &projectionMatrix, mat4 &viewMatrix, Camera &winningCam, Camera &overheadCam, string &healthStr, string &armourStr, string &velocityStr, vec3 &vColour, const int &camIndex) {
	//overhead view
	if (camIndex ==0) {
		projectionMatrix = overheadCam.calculateProjectionMatrix();
		viewMatrix = overheadCam.calculateViewMatrix();

	}
	//camera of winning vehicle
	else if (camIndex == 1) {
		//overhead cam for player with highest health
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

	//player cams
	else {
		projectionMatrix = players[camIndex-2]->playerCam->calculateProjectionMatrix();
		viewMatrix = players[camIndex-2]->playerCam->calculateViewMatrix();
	}
		healthStr = "000";
		armourStr = "000";
		velocityStr = "00";
		vColour = vec3(0);
}


