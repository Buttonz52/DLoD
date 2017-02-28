#ifndef AI_H
#define AI_H
#include "Player.h"
#include "..\AStarNode.h"

class AI :
	public Player
{
  AStarNode* closestNode;

public:
	AI();
	~AI();
	int difficultyState;

	int DetermineBehaviour();
  void getInput();
  void driveTo(vec3);
};
#endif