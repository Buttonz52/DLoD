#ifndef AI_H
#define AI_H
#include "Player.h"
#include "GEO\AStarNode.h"

class AI :
	public Player
{
public:
	AI(int);
	~AI();
	int difficultyState;

  vector<AStarNode*> AStarNodes;
  OctTree* nodeTree;

	int DetermineBehaviour();
  void getInput();

  // Given a destination will return a point to drive to
  vec3 pathTo(vec3);
  void driveTo(vec3);
};
#endif