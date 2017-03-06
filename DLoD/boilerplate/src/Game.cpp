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
  delete skybox;
  physX.cleanupPhysics(true);

  //problems with this
  //for (Player * p: players) {
	 // if (typeid(*p) == typeid(AI))
	 // {
		//  if (p->isDead()){
		//	audio.PlaySfx(winSFX);
		//	break;
		// }
		//  //ai->getInput();
	 // }
	 // else if (typeid(*p) == typeid(Human)){
		//  if (p->isDead()) {
		//	  audio.PlaySfx(loseSFX);
		//	  break;
		//  }
		//}
  //}

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
}

void Game::gameLoop()
{
  // Add items to the scene
  vector<pair<pair<Item*, Player*>, int>>::iterator itr = itemsToAdd.begin();
  while (itr != itemsToAdd.end()) {
    if (itr->second < timer.getTicks()) {
      //initItem(itr->first->first);
      itr->first.second->ableToTrap = true;
      itr = itemsToAdd.erase(itr);
      break;
    }
    else {
      ++itr;
    }
  }


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
      AI* ai = dynamic_cast<AI*> (p);
      if (ai != nullptr && !ai->isDead())
      {
        ai->driveTo(players[0]->vehicle->getModelMatrix()[3]);
        //ai->getInput();
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

        // Get the rotation of the object
        physx::PxVec3 axis = physx::PxVec3(0, 1, 0);
        physx::PxReal angle = 0;
        p->vehicle->physXVehicle->getRigidDynamicActor()->getGlobalPose().q.toRadiansAndUnitAxis(angle, axis);

        int fix = (axis.y < 0) ? 1 : -1;
        angle *= fix;
        if (angle < 0)
          angle += M_PI * 2;

        // Calculation the normalized orientated displacement
        mat3 rotation(1);
        rotation[0][0] = cos(-angle);
        rotation[0][2] = sin(-angle);
        rotation[2][0] = -sin(-angle);
        rotation[2][2] = cos(-angle);
        vec3 dis = rotation * -vec3(0, 0, 8);

        Item* item = new Item(DamageTrap);
        mat4 m = mat4(1);
        m[3] = vec4(vPos + dis, 1);
        item->setModelMatrix(m);

        itemsToAdd.push_back(make_pair(make_pair(item, p), timer.getTicks() + 1500));

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

