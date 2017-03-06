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

  // Clean up and Display the win screen
  delete skybox;
  physX.cleanupPhysics(true);

  //glfwSetKeyCallback(window, GameKeyCallback);

  audio.PlaySfx(winSFX);
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

  //InitializeGameText(&fontTex, to_string(players[0]->vehicle->getHealth()), vec3(0.5,0.9,0));
  char textCharBuffer[3];
  InitializeGameText(&fontTex, players[0]->vehicle->toString(), vec3(0.5, 0.9, 0), vec3(0.5,0,0));

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

	UpdateGameText(&fontTex, players[0]->vehicle->toString());
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

    // CHECK GAMEOVER
    int aliveCount = 0;
    for (Player* p : players)
      aliveCount += p->isDead() ? 0 : 1;

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

  v->wheelFileName = "mediumCarTire.obj";
  v->setFilename("ObjModels/mediumCarBody.obj");	//alive mesh
  if (!v->initMesh("cube.obj")) {	//dead mesh
    cout << "Failed to initialize mesh." << endl;
  }
  v->addShaders("shaders/toon.vert", "shaders/toon.frag");


  if (!v->initBuffers()) {
    cout << "Could not initialize buffers for game object " << v->getFilename() << endl;
  }

  physX.initVehicle(v);

  v->updateWheelPosition();			//init the 4 wheels per car

  physXObjects.push_back(v);
}

void Game::InitializeGameText(ScreenOverlay *fontTex, const string &text, const vec3 &position, const vec3 &colour) {
	int kerning = 15.f;
	fontTex->isFontTex = 1;
	fontTex->setColour(colour);
	if (!fontTex->initTexture("fonts/grim12x12.png", GL_TEXTURE_2D)) {
		cout << "Failed to init fonts." << endl;
	}
	fontTex->InitializeShaders("shaders/screenOverlay.vert", "shaders/screenOverlay.frag");
	vector<vec3> verts;
	vector<vec2> uvs;

	int index = 0;
	for (char c : text) {
		GenerateTextUVs(uvs, c);

		verts.push_back(vec3(0 + float(index)/kerning, 0, 0));
		verts.push_back(vec3(0.05 + float(index) / kerning, 0, 0));
		verts.push_back(vec3(0 + float(index) / kerning, 0.1, 0));
		verts.push_back(vec3(0.05 + float(index) / kerning, 0, 0));
		verts.push_back(vec3(0 + float(index) / kerning, 0.1, 0));
		verts.push_back(vec3(0.05 + float(index) / kerning, 0.1, 0));
		index++;
	}

	//if (!fontTex->GenerateSquareVertices(0.1, 0.1, vec3(0))) {
	if (!fontTex->GenerateVertices(&verts, vec3(1, 0, 0), &uvs)) {
		cout << "Failed to initialize font overlay." << endl;
	}

	fontTex->setPosition(vec3(0.5f, 0.9f, 0));
}
void Game::UpdateGameText(ScreenOverlay *fontTex, const string &text) {
	vector<vec2> uvs;
	for (char c : text) {
		GenerateTextUVs(uvs, c);
	}
	fontTex->UpdateBuffers(&uvs);
}

void Game::GenerateTextUVs(vector <vec2> &uvs,const char &ch) {
	//for letters 
	char c = ch;
	c = toupper(c); //make lowercase
	float asciiLocation = int(c) - int('A');
	float horizontalLoc;
	//is a letter ( >= A)
	if (asciiLocation >= 0) {
		horizontalLoc = float((int(c) - int('A') + 1) % 16);
	}
	//is the row above (numbers)
	else {
		horizontalLoc = float((16 - abs((int(c) - int('A') + 1))) % 16);
	}

	float verticalLoc = asciiLocation;
	if (verticalLoc >= 15) {
		verticalLoc = 5;
	}
	else if (verticalLoc < -1) {
		verticalLoc = 3;
	}
	else {
		verticalLoc = 4;
	}

	uvs.push_back(vec2((horizontalLoc) / 16.f, (verticalLoc + 1.f) / 16.f));	//bottom left
	uvs.push_back(vec2((horizontalLoc + 1.f) / 16.f, (verticalLoc + 1.f) / 16.f));		//bottom right
	uvs.push_back(vec2((horizontalLoc) / 16.f, (verticalLoc) / 16.f));			//top left
	uvs.push_back(vec2((horizontalLoc + 1.f) / 16.f, (verticalLoc + 1.f) / 16.f));
	uvs.push_back(vec2((horizontalLoc) / 16.f, (verticalLoc) / 16.f));
	uvs.push_back(vec2((horizontalLoc + 1.f) / 16.f, (verticalLoc) / 16.f));
}

