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

  double neighbourRadius = 50.0;
};

class OctTree
{
  vec3 centerPoint;
  double distx, disty, distz;

  vector<OctTree*> children;

  vector<AStarNode*> nodes;

public:

  OctTree();
  OctTree(vector<AStarNode*> ps, vec3 cp, double x, double y, double z);
  ~OctTree();

  void addNode(AStarNode* node);
  void split();

  int numberOfNodes();

  // Takes a vector to fill a centerpoint and a radius. Will treat the cp and radius as a cube
  void getNodesForSphere(vector<AStarNode *> &nodesInArea, vec3 cp, double r);
  void getNodesForArc(vector<AStarNode *> &nodesInArea, vec3 cp, vec3 ray);

  void resetNodes();

};


#endif
