#ifndef GAME_H
#define GAME_H


#include "../GEO/Player/AI.h"
#include "../GEO/player/Human.h"
#include "../Physics/PhysXMain.h"


class Game
{
private:
  vector<Player *> players;
  PhysXMain PhysX;

public:

  void start();


};

#endif
