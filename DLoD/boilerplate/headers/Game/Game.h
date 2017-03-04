#ifndef GAME_H
#define GAME_H


#include "../GEO/Player/AI.h"
#include "../GEO/player/Human.h"
#include "../Physics/PhysXMain.h"


class Game
{
private:
  GEO* skybox;
  vector<Player*> players;
  vector<GEO*> physXObjects;
  PhysXMain physX;
  GLFWwindow *window;

  vec3 lightSource = vec3(0.f, 100.f, 0.f);

  bool gameOver = false;

  void initSkyBox();
  void initVehicle(Vehicle * v);

  void gameLoop();

public:

  Game(GLFWwindow*);

  void start();
};

#endif
