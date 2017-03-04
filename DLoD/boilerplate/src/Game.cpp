#include "Game/Game.h"



void Game::start()
{

  // For all players get input
  for (Player* p : players)
  {
    AI* ai = dynamic_cast<AI*> (p);
    if (ai != nullptr)
      ai->getInput();

    Human* human = dynamic_cast<Human*> (p);
    //if (human != nullptr)
      //human->getInput();
  }

  // Step Physx
  

  // Render

}