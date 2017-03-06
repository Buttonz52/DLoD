#include "Game/Game.h"

GEO* initGroundPlane()
{
  GEO* plane = new GEO();
  plane->setFilename("plane.obj");
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


Game::Game(GLFWwindow* w, Audio audio)
{
  window = w;
  physX.init();

  initSkyBox();
  //skybox->children.push_back(initArena());
  arena = initArena();
  //skybox->children.push_back(initGroundPlane());

  Human* human = new Human(0);
  human->vehicle = new Vehicle();
  human->vehicle->setPosition(vec3(0, 20, -50));
  initVehicle(human->vehicle);
  skybox->children.push_back(human->vehicle);


  AI* ai = new AI(1);
  ai->vehicle = new Vehicle();
  ai->vehicle->setPosition(vec3(0, 20, 50));
  ai->vehicle->setColour(vec3(0, 1, 0));
  initVehicle(ai->vehicle);
  skybox->children.push_back(ai->vehicle);

  /*
  Item* item = new Item(DamageTrap);
  mat4 m = mat4(1);
  m[3] = vec4(vec3(0,5,30), 1);
  item->setModelMatrix(m);
  initItem(item); */


  players.push_back(human);
  players.push_back(ai);

  //skybox->children.push_back(arena);
}


void Game::start()
{
  // Set up the game
	

  // start the game loop
  timer.start();
  gameLoop();

  // Clean up and Display the win screen

  ScreenOverlay endGameText;
	if (players[1]->isDead())
	{
		endGameText.InitializeGameText("WIN!", vec3(-0.1, 0, 0), vec3(0, 1, 0), 20);
		endGameText.Render(GL_TRIANGLES);
		glfwSwapBuffers(window);

		audio.PlaySfx(winSFX);
	}
	else if (players[0]->isDead()){
		endGameText.InitializeGameText("LOSE!", vec3(-0.1, 0, 0), vec3(0, 0, 1), 20);
		endGameText.Render(GL_TRIANGLES);
		glfwSwapBuffers(window);

		audio.PlaySfx(loseSFX);
	}

  bool pause = true;

  while (pause && !glfwWindowShouldClose(window))
  {
	  for (Player* p : players)
	  {
		  Human* human = dynamic_cast<Human*> (p);
		  if (human != nullptr)
			  human->getGameOverInput(window,pause);
	  }

	  glfwPollEvents();
  }
  delete skybox;
  physX.cleanupPhysics(true);
}

void Game::gameLoop()
{

  //InitializeGameText(&fontTex, to_string(players[0]->vehicle->getHealth()), vec3(0.5,0.9,0));
  healthTitle.InitializeGameText("Health:", vec3(0, 0.9, 0), vec3(1, 0, 0), 20);
  armourTitle.InitializeGameText("Armour:", vec3(-1, 0.9, 0), vec3(0, 0, 1), 20);

  healthTex.InitializeGameText(players[0]->vehicle->getHealthString(), vec3(0.5, 0.9, 0), vec3(1,0,0), 20);
  armourTex.InitializeGameText(players[0]->vehicle->getArmourString(), vec3(-0.5, 0.9, 0), vec3(0, 0, 1), 20);

  int frameCtr = 0;
 /* glEnable(GL_CULL_FACE);

  glCullFace(GL_FRONT);*/
  while (!glfwWindowShouldClose(window) && !gameOver)
  {
    // Add items to the scene
    vector<pair<Player*, int>>::iterator itr = itemsToAdd.begin();
    while (itr != itemsToAdd.end()) {
      if (itr->second < timer.getTicks()) {
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
    }
    physX.deletedGeos.clear();

    // clear screen to a dark grey colour;
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Step Physx
    physX.stepPhysics(true, physXObjects);

    // Render
    mat4 projectionMatrix = players[0]->playerCam->calculateProjectionMatrix();
    mat4 viewMatrix = players[0]->playerCam->calculateViewMatrix();

	healthTex.UpdateGameText(players[0]->vehicle->getHealthString());
	armourTex.UpdateGameText(players[0]->vehicle->getArmourString());
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	arena->Render(viewMatrix, projectionMatrix, lightSource);
	glDisable(GL_CULL_FACE);
    skybox->Render(viewMatrix, projectionMatrix, lightSource);
	  healthTitle.Render(GL_TRIANGLES);
	  armourTitle.Render(GL_TRIANGLES);
	  healthTex.Render(GL_TRIANGLES);
	  armourTex.Render(GL_TRIANGLES);

    glfwSwapBuffers(window);

    // For all players get input
    for (Player* p : players)
    {
		if (p->vehicle->getModelMatrix()[3].y < -10)
			p->vehicle->updateHealth(1000);

      AI* ai = dynamic_cast<AI*> (p);
      if (ai != nullptr && !ai->isDead())
      {
        ai->driveTo(players[0]->vehicle->getModelMatrix()[3]);
        if (abs(ai->vehicle->physXVehicle->computeForwardSpeed()) > 20)
          ai->layTrap = true;
      }

      Human* human = dynamic_cast<Human*> (p);
      if (human != nullptr)
        human->getInput(window);

      p->playerCam->followVehicle(p->vehicle);
      
      if (p->layTrap && p->ableToTrap)
      {
        // create a new item at the appropriate location and add it to the items list
        mat4 M = p->vehicle->getModelMatrix();
        vec3 vPos = vec3(M[3]);
        vec3 dis = vec3(0, 8, 0);

        Item* item = new Item(DamageTrap);
        mat4 m = mat4(1);
        m[3] = vec4(vPos + dis, 1);
        item->setModelMatrix(m);

        itemsToAdd.push_back(make_pair(p, timer.getTicks() + 5000));

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

void Game::initSkyBox()
{
  vector<string> skyboxFiles = {
    "textures/ame_ash/ashcanyon_rt.tga",
    "textures/ame_ash/ashcanyon_lf.tga",
    "textures/ame_ash/ashcanyon_up.tga",
    "textures/ame_ash/ashcanyon_dn.tga",
    "textures/ame_ash/ashcanyon_bk.tga",
    "textures/ame_ash/ashcanyon_ft.tga",
  };

  skybox = new GEO();
  skybox->setFilename("cube.obj");
  if (!skybox->initMesh("cube.obj")) {
    cout << "Failed to initialize mesh." << endl;
  }
  //scale cube large
  skybox->setScale(vec3(500.0));
  skybox->updateModelMatrix();
  if (!skybox->initSkybox(skyboxFiles)) {
    cout << "Failed to initialize skybox." << endl;
  }
  skybox->addShaders("shaders/skybox.vert", "shaders/skybox.frag");
}

void Game::initVehicle(Vehicle* v)
{
  v->setScale(vec3(1.5));
 
  v->setFilename("ObjModels/mediumCarBody.obj");	//alive mesh
  if (!v->initMesh("cube.obj")) {	//dead mesh
    cout << "Failed to initialize mesh." << endl;
  }
  v->addShaders("shaders/toon.vert", "shaders/toon.frag");


  if (!v->initBuffers()) {
    cout << "Could not initialize buffers for game object " << v->getFilename() << endl;
  }

  physX.initVehicle(v);

  physXObjects.push_back(v);
}


//
void Game::initItem(Item* item)
{
  item->setFilename("cub.obj");
  item->setScale(vec3(2));
  if (!item->initMesh("/ObjModels/bearTrap.obj")) {	//dead mesh
    cout << "Failed to initialize mesh." << endl;
  }
  item->addShaders("shaders/toon.vert", "shaders/toon.frag");
  
  item->setColour(vec3(1, 1, 0));

  mat3 scaleM = mat3(1);
  scaleM[0][0] = item->getScale().x;
  scaleM[1][1] = item->getScale().y;
  scaleM[2][2] = item->getScale().z;

  for (int i = 0; i < item->getMesh().vertices.size(); ++i)
	  item->getMesh().vertices[i] = scaleM * item->getMesh().vertices[i];

  if (!item->initBuffers()) {
    cout << "Could not initialize buffers for game object " << item->getFilename() << endl;
  }

  physX.initItem(item);
  skybox->children.push_back(item);
}


//initialize arena
GEO* Game::initArena() {
	GEO *arena = new GEO();
	arena->setFilename("ObjModels/bpArena2.obj");
	if (!arena->initMesh("ObjModels/bpArena2.obj")) {
		cout << "Failed to init arena" << endl;
	}
	arena->setColour(vec3(1, 0, 0));
	if (!arena->initTexture("textures/ground.png", GL_TEXTURE_2D)) {
		cout << "Failed to initialize plane." << endl;
	}
	arena->addShaders("shaders/tex2D.vert", "shaders/tex2D.frag");
	//arena->addShaders("shaders/phong.vert", "shaders/phong.frag");

	if (!arena->initBuffers()) {
		cout << "Could not initialize buffers for game object " << arena->getFilename() << endl;
	}
	arena->setScale(vec3(100.f));
	arena->setPosition(vec3(0, 0, 0));
	arena->updateModelMatrix();

	physX.initArena(arena);
	//physXObjects.push_back(arena);
	return arena;
}

