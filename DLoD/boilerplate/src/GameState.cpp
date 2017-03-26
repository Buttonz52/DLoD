#include "Game\GameState.h"
#include "GEO\AStarNode.h"



GameState::GameState()
{
  vector<AStarNode*> starNodes;

  for (int i = -10; i < 10; ++i)
  {
    for (int j = -10; j < 10; ++j)
    {
      if (abs(i) < 2 || abs(j) < 2)
        continue;


      AStarNode* node = new AStarNode();
      node->position = vec3(i * 15, 0, j * 15);
      starNodes.push_back(node);
    }
    
  } 

  nodes = new OctTree(starNodes, vec3(0, 0, 0), 50.0, 50.0, 50.0);

  for (AStarNode* n : starNodes)
  {
    vector<AStarNode*> neighbours;
    nodes->getNodesForSphere(neighbours, n->position, n->neighbourRadius);
    for (AStarNode* m : neighbours)
    {
      vec3 dist = n->position - m->position;
      double l2 = length(dist);

      if (m != n && length(dist * vec3(0, 1, 0)) < 0.75 * l2)
        n->neighbours.push_back(m);
    }
  }

}


GameState::~GameState()
{
}

bool GameState::GameFinished()
{
	return false;
}

bool GameState::CheckWinCondition()
{
	return false;
}
