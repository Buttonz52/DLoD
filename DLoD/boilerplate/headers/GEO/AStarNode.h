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

  OctTree(vector<AStarNode*> ps, const vec3 &cp, const double &x, const double &y, const double &z);
  ~OctTree();

  // Takes a centerpoint and a radius. Will treat the cursor as a cube
  vector<AStarNode *> getNodesForSphere(const vec3 &, const double &);

};


#endif
