#include "Game\GameState.h"
#include "GEO\AStarNode.h"


GameState::GameState(vector<vec3> vertices, vector<vec3> itemSpawnLocations)
{
  nodes = new OctTree();

  for (vec3 v : vertices)
  {
    vector<AStarNode*> neighbours;
    nodes->getNodesForSphere(neighbours, v, 0.5);

    if (!neighbours.empty())
      continue;

    AStarNode* n = new AStarNode();
    n->position = v;

    nodes->getNodesForSphere(neighbours, v, n->neighbourRadius);
    for (AStarNode* m : neighbours)
    {
      vec3 dist = n->position - m->position;
      double l2 = length(dist);

      if (m != n && length(dist * vec3(0, 1, 0)) < 0.75 * l2)
      {
        n->neighbours.push_back(m);
        m->neighbours.push_back(n);
      }
    }
    nodes->addNode(n);
  }


  vector<int> itemTypes = { HealthPack, ArmourPack };
  for (vec3 pos : itemSpawnLocations)
    itemSpawners.push_back(new ItemSpawner(itemTypes, pos));
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
