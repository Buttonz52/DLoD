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


Game::Game(GLFWwindow* w)
{
  window = w;
  physX.init();

  initSkyBox();
  skybox->children.push_back(initGroundPlane());

  Human* human = new Human();
  human->vehicle = new Vehicle();
  initVehicle(human->vehicle);
  skybox->children.push_back(human->vehicle);


  AI* ai = new AI();
  ai->vehicle = new Vehicle();
  ai->vehicle->setPosition(vec3(30, 0, 30));
  initVehicle(ai->vehicle);
  skybox->children.push_back(ai->vehicle);

  players.push_back(human);
  players.push_back(ai);
}


void Game::start()
{
  // Set up the game


  // start the game loop
  gameLoop();


  // Display the win screen

  delete skybox;
  physX.cleanupPhysics(true);

}


void Game::gameLoop()
{
  InitializeGameOverlay(&logo, &fontTex);

  while (!glfwWindowShouldClose(window) || gameOver)
  {
    // clear screen to a dark grey colour;
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Step Physx
    physX.stepPhysics(true, physXObjects);

    // Render
    mat4 projectionMatrix = players[0]->playerCam->calculateProjectionMatrix();
    mat4 viewMatrix = players[0]->playerCam->calculateViewMatrix();



    skybox->Render(viewMatrix, projectionMatrix, lightSource);
	fontTex.Render(GL_TRIANGLES);


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
    }

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
  v->setScale(vec3(0.8));

  v->setFilename("teapot.obj");	//alive mesh
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

void Game::InitializeGameOverlay(ScreenOverlay *logo, ScreenOverlay *fontTex) {
	//if (!logo->initTexture("textures/DLoDLogo.png", GL_TEXTURE_2D)) {
	//	cout << "Failed to init texture." << endl;
	//}

	//logo->InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	//if (!logo->GenerateSquareVertices(0.1, 0.1, vec3(0))) {
	//	cout << "Failed to initialize screen overlay." << endl;
	//}
	//logo->setPosition(vec3(0.9f, 0.9f, 0));



	if (!fontTex->initTexture("fonts/grim12x12.png", GL_TEXTURE_2D)) {
		cout << "Failed to init fonts." << endl;
	}
	fontTex->InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");

	vector<vec3> verts = {
		vec3(0,0,0),
		vec3(0.05,0,0),
		vec3(0,0.1,0),
		vec3(0.05,0,0),
		vec3(0,0.1,0),
		vec3(0.05,0.1,0)


	};

	vector<vec2> uvs = {
		vec2(0.5,0.25),
		vec2(0.5625,0.25),
		vec2(0.5,0.3125),
		vec2(0.5625,0.25),
		vec2(0.5,0.3125),
		vec2(0.5625,0.3125)
	};

	//if (!fontTex->GenerateSquareVertices(0.1, 0.1, vec3(0))) {
	if (!fontTex->GenerateVertices(&verts, vec3(1, 0, 0), &uvs)) {
		cout << "Failed to initialize font overlay." << endl;
	}

	fontTex->setPosition(vec3(0.5f, 0.9f, 0));
}

