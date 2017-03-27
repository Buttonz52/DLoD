#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GEO\player\Player.h"
#include "GEO\item\Item.h"
#include "GEO\AStarNode.h"


class GameState
{
public:
  
  vector<Player*> players;
  vector<Item*> items;

  OctTree* nodes;

  Timer timer;

	GameState(vector<vec3> vertices);
	~GameState();

	bool GameFinished();
	bool CheckWinCondition();
};
#endif
