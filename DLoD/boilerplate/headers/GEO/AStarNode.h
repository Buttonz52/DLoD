#ifndef ASTARNODE
#define ASTARNODE

#include "../Game/Utility.h"

class AStarNode
{
public:
  ~AStarNode();

  vec3 position;
  vector<AStarNode *> neighbours;

  double distanceTo;

  bool obstructed = false;
};

class OctTree
{

  vec3 centerPoint;
  double distx, disty, distz;

  vector<OctTree*> children;

  vector<AStarNode*> nodes;

public:

  OctTree(vector<AStarNode*> ps, vec3 cp, double x, double y, double z);
  ~OctTree();

  // Takes a centerpoint and a radius. Will treat the cursor as a cube
  vector<AStarNode *> getNodesForSphere(vec3, double);

};


#endif
