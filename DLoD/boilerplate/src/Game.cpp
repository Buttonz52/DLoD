#include "Game/Game.h"


Game::Game(GLFWwindow* w)
{
  window = w;
  physX.init();

  initSkyBox();
  //skybox->children.push_back(initGroundPlane());

  Human* human = new Human();
  human->vehicle = new Vehicle();
  initVehicle(human->vehicle);


  AI* ai = new AI();
  ai->vehicle = new Vehicle();
  ai->vehicle->setPosition(vec3(30, 0, 30));

  players.push_back(human);
  players.push_back(ai);
}


void Game::start()
{
  // clear screen to a dark grey colour;
  //clearScreen();

  // Step Physx
  physX.stepPhysics(true, physXObjects);

  // For all players get input
  for (Player* p : players)
  {
    AI* ai = dynamic_cast<AI*> (p);
    if (ai != nullptr)
      ai->getInput();

    Human* human = dynamic_cast<Human*> (p);
    if (human != nullptr)
      human->getInput();
  }


  //mat4 projectionMatrix = players[0]->playerCam->calculateProjectionMatrix();
  

  // Render
  //skybox.Render();

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


