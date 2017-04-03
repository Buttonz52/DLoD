#include "GEO\AStarNode.h"


OctTree::OctTree()
{
  centerPoint = vec3(0, 0, 0);
  distx = 128000;
  disty = 128000;
  distz = 128000;
}

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
  for (AStarNode* n : nodes)
    delete n;

  for (OctTree* c : children)
    delete c;
}

void OctTree::addNode(AStarNode * node)
{
  if (children.empty()) {
    nodes.push_back(node);

    if (nodes.size() > 10)
      split();
  }
  else
  {
    if (node->position.x < centerPoint.x && node->position.y < centerPoint.y && node->position.z < centerPoint.z) {
      children[0]->addNode(node);
    }
    else if (node->position.x < centerPoint.x && node->position.y < centerPoint.y && node->position.z > centerPoint.z) {
      children[1]->addNode(node);
    }
    else if (node->position.x < centerPoint.x && node->position.y > centerPoint.y && node->position.z < centerPoint.z) {
      children[2]->addNode(node);
    }
    else if (node->position.x < centerPoint.x && node->position.y > centerPoint.y && node->position.z > centerPoint.z) {
      children[3]->addNode(node);
    }
    else if (node->position.x > centerPoint.x && node->position.y < centerPoint.y && node->position.z < centerPoint.z) {
      children[4]->addNode(node);
    }
    else if (node->position.x > centerPoint.x && node->position.y < centerPoint.y && node->position.z > centerPoint.z) {
      children[5]->addNode(node);
    }
    else if (node->position.x > centerPoint.x && node->position.y > centerPoint.y && node->position.z < centerPoint.z) {
      children[6]->addNode(node);
    }
    else if (node->position.x > centerPoint.x && node->position.y > centerPoint.y && node->position.z > centerPoint.z) {
      children[7]->addNode(node);
    }
    else {
      nodes.push_back(node);
    }
  }
}

void OctTree::split()
{
  vector<AStarNode*> lll, llg, lgl, lgg, gll, glg, ggl, ggg, newNodes;

  for (int i = 0; i < nodes.size(); ++i) {
    AStarNode* node = nodes[i];

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
      newNodes.push_back(node);
    }
  }

  nodes = newNodes;

  children.push_back(new OctTree(lll, centerPoint + vec3(-distx / 2, -disty / 2, -distz / 2), distx / 2, disty / 2, distz / 2));
  children.push_back(new OctTree(llg, centerPoint + vec3(-distx / 2, -disty / 2,  distz / 2), distx / 2, disty / 2, distz / 2));
  children.push_back(new OctTree(lgl, centerPoint + vec3(-distx / 2,  disty / 2, -distz / 2), distx / 2, disty / 2, distz / 2));
  children.push_back(new OctTree(lgg, centerPoint + vec3(-distx / 2,  disty / 2,  distz / 2), distx / 2, disty / 2, distz / 2));
  children.push_back(new OctTree(gll, centerPoint + vec3( distx / 2, -disty / 2, -distz / 2), distx / 2, disty / 2, distz / 2));
  children.push_back(new OctTree(glg, centerPoint + vec3( distx / 2, -disty / 2,  distz / 2), distx / 2, disty / 2, distz / 2));
  children.push_back(new OctTree(ggl, centerPoint + vec3( distx / 2,  disty / 2, -distz / 2), distx / 2, disty / 2, distz / 2));
  children.push_back(new OctTree(ggg, centerPoint + vec3( distx / 2,  disty / 2,  distz / 2), distx / 2, disty / 2, distz / 2));
}

int OctTree::numberOfNodes()
{
  int n = nodes.size();
  for (OctTree* child : children)
    n += child->numberOfNodes();

  return n;
}


void OctTree::getNodesForSphere(vector<AStarNode*> &nodesInArea, vec3 cp, double r)
{
  if (abs(cp.x - centerPoint.x) < distx + r && abs(cp.y - centerPoint.y) < disty + r && abs(cp.z - centerPoint.z) < distz + r)
  {
    for (AStarNode* node : nodes) {
      if (!node->obstructed && length(node->position - cp) <= r)
        nodesInArea.push_back(node);
    }

    for (OctTree* child : children)
    {
      child->getNodesForSphere(nodesInArea, cp, r);
    }
  }
}

void OctTree::getNodesForArc(vector<AStarNode*>& nodesInArea, vec3 cp, vec3 ray)
{
  vector<AStarNode *> nodes;
  getNodesForSphere(nodes, cp, length(ray));

  for (AStarNode* node : nodes)
  {
    if (dot((node->position - cp), ray) > 1.0 / sqrt(2.0))
      nodesInArea.push_back(node);
  }
}

void OctTree::resetNodes()
{
  // just needs to be a big number
  for (AStarNode* n : nodes)
    n->distanceTo = 50000;

  for (OctTree* child : children)
    child->resetNodes();
}


AStarNode::~AStarNode()
{
  // delete itself from the neighbours list of its neighbours 
  for (AStarNode* n : neighbours)
  {
    for (int i = 0; i < n->neighbours.size(); ++i)
    {
      if (n->neighbours[i] == this)
        n->neighbours.erase(n->neighbours.begin() + i);
    }

  }
}
