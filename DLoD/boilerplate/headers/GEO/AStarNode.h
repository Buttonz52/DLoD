#ifndef ASTARNODE
#define ASTARNODE

#include "Game/Utility.h"

class AStarNode
{
public:
  ~AStarNode();

  vec3 position;
  vector<AStarNode *> neighbours;

  double distanceTo;

  bool obstructed = false;

  double neighbourRadius = 9.0;

  double dist;
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

  // Takes a vector to fill a centerpoint and a radius. Will treat the cp and radius as a cube
  void getNodesForSphere(vector<AStarNode *> &nodesInArea, vec3 cp, double r);

  void resetNodes();

};


#endif
