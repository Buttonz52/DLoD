#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "GEO\player\Player.h"
#include "GEO\item\ItemSpawner.h"
#include "GEO\item\Item.h"
#include "GEO\AStarNode.h"


class GameState
{
public:
  
  vector<Player*> players;
  vector<Item*> items;
  vector<ItemSpawner*> itemSpawners;

  OctTree* nodes;

  Timer timer;

	GameState(vector<vec3> vertices, vector<vec3> itemSpawnLocations);
	~GameState();

	bool GameFinished();
	bool CheckWinCondition();
};
#endif
