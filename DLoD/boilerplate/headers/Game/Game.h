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

public:

  Game(GLFWwindow*);

  void start();

  void initSkyBox();

  void initVehicle(Vehicle * v);


};

#endif
