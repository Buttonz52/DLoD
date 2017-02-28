#include "GEO\AStarNode.h"

OctTree::OctTree(vector<AStarNode *> aStarNodes, vec3 cp, double x, double y, double z)
{

  centerPoint = cp;
  distx = x;
  disty = y;
  distz = z;

  // if there are more than 10 points split
  if (aStarNodes.size() > 10) {

    vector<AStarNode*> lll, llg, lgl, lgg, gll, glg, ggl, ggg;

    for (int i = 0; i < aStarNodes.size(); ++i) {
      AStarNode* node = aStarNodes[i];

      if (node->position.x < centerPoint.x && node->position.y < centerPoint.y && node->position.z < centerPoint.z) {
        lll.push_back(node);
      }
      else if (node->position.x < centerPoint.x && node->position.y < centerPoint.y && node->position.z > centerPoint.z) {
        llg.push_back(node);
      }
      else if (node->position.x < centerPoint.x && node->position.y > centerPoint.y && node->position.z < centerPoint.z) {
        lgl.push_back(node);
      }
      else if (node->position.x < centerPoint.x && node->position.y > centerPoint.y && node->position.z > centerPoint.z) {
        lgg.push_back(node);
      }
      else if (node->position.x > centerPoint.x && node->position.y < centerPoint.y && node->position.z < centerPoint.z) {
        gll.push_back(node);
      }
      else if (node->position.x > centerPoint.x && node->position.y < centerPoint.y && node->position.z > centerPoint.z) {
        glg.push_back(node);
      }
      else if (node->position.x > centerPoint.x && node->position.y > centerPoint.y && node->position.z < centerPoint.z) {
        ggl.push_back(node);
      }
      else if (node->position.x > centerPoint.x && node->position.y > centerPoint.y && node->position.z > centerPoint.z) {
        ggg.push_back(node);
      }
      else {
        nodes.push_back(node);
      }
    }

    children.push_back(new OctTree(lll, cp + vec3(-x / 2, -y / 2, -z / 2), x / 2, y / 2, z / 2));
    children.push_back(new OctTree(llg, cp + vec3(-x / 2, -y / 2, z / 2), x / 2, y / 2, z / 2));
    children.push_back(new OctTree(lgl, cp + vec3(-x / 2, y / 2, -z / 2), x / 2, y / 2, z / 2));
    children.push_back(new OctTree(lgg, cp + vec3(-x / 2, y / 2, z / 2), x / 2, y / 2, z / 2));
    children.push_back(new OctTree(gll, cp + vec3(x / 2, -y / 2, -z / 2), x / 2, y / 2, z / 2));
    children.push_back(new OctTree(glg, cp + vec3(x / 2, -y / 2, z / 2), x / 2, y / 2, z / 2));
    children.push_back(new OctTree(ggl, cp + vec3(x / 2, y / 2, -z / 2), x / 2, y / 2, z / 2));
    children.push_back(new OctTree(ggg, cp + vec3(x / 2, y / 2, z / 2), x / 2, y / 2, z / 2));

  }
  else {
    nodes = aStarNodes;
  }

}

OctTree::~OctTree()
{
}

vector<AStarNode*> OctTree::getNodesForSphere(vec3 cp, double r)
{

  vector<AStarNode *> nodesInArea;
  if (abs(cp.x - centerPoint.x) < distx + r && abs(cp.y - centerPoint.y) < disty + r && abs(cp.z - centerPoint.z) < distz + r)
  {
    for (AStarNode* node : nodes) {
      if (!node->obstructed && length(node->position - cp) <= r)
        nodesInArea.push_back(node);
    }

    for (OctTree* child : children)
    {
      vector<AStarNode*> childNodes = child->getNodesForSphere(cp, r);
      copy(childNodes.begin(), childNodes.end(), back_inserter(nodesInArea));
    }
  }

  return nodesInArea;

}

// This is a special delete method dont use it
AStarNode::~AStarNode()
{
  // delete itself from the neighbours list of its neighbours 
  for (AStarNode * n : neighbours)
  {
    n->neighbours.pop_back();
  }
}
