#include "GEO\player\AI.h"

struct sortClass
{
  vec3 destination;

  bool operator()(AStarNode* a, AStarNode* b)
  {
    double dista = a->distanceTo + length(destination - a->position);
    double distb = b->distanceTo + length(destination - b->position);

    return dista > distb;
  }
};

bool sortFunc(AStarNode* a, AStarNode* b)
{
  return a->distanceTo > b->distanceTo;
}

bool vecContains(vector<AStarNode*> vec, AStarNode* node)
{
  return find(vec.begin(), vec.end(), node) != vec.end();
}



AI::AI()
{
}


AI::~AI()
{
}

int AI::DetermineBehaviour()
{
	return 0;
}

void AI::getInput()
{

}

/*
This function does not work dont use it yet
wont currently support multithreading
*/
vec3 AI::pathTo(const vec3 &dest)
{

  // Get all the AStar Nodes and set there distance to value to be big

  // These values need to be grabed from somewhere
  OctTree* AStarTree;
  double radius = 0;

  // Find all the closest AStarNodes to the destination
  AStarNode* start = new AStarNode();
  start->position = vec3(vehicle->getModelMatrix()[3]);
  //start->neighbours = AStarTree->getNodesForSphere(dest, radius);

  // Remove this at some point
  assert(!start->neighbours.empty());

  // Add itself as a neighbour to its neighbours
  for (AStarNode* n : start->neighbours)
    n->neighbours.push_back(start);


  // Find all the closest AStarNodes to the destination
  AStarNode* destination = new AStarNode();
  destination->position = dest;
  //destination->neighbours = AStarTree->getNodesForSphere(dest, radius);
  
  // Remove this at some point
  assert(!destination->neighbours.empty());

  // Add itself as a neighbour to its neighbours
  for (AStarNode* n : destination->neighbours)
    n->neighbours.push_back(destination);

  vector<AStarNode*> investigatedNodes;
  vector<AStarNode*> neighbours; // neighbours to the investigatedNodes
  investigatedNodes.push_back(closestNode);

  AStarNode* node = closestNode;

  //
  while (!vecContains(investigatedNodes, destination))
  {
    for (AStarNode* n : node->neighbours)
    {
      // calculate a new distanceTo 
      n->distanceTo = min(n->distanceTo, node->distanceTo + length(n->position - node->position));

      if (!n->obstructed && !vecContains(investigatedNodes, n) && !vecContains(neighbours, destination))
        neighbours.push_back(n);
    }

    // Sort the neighbours 
    sortClass obj;
    obj.destination = dest;
    sort(neighbours.begin(), neighbours.end(), obj);

    if (!neighbours.empty())
    {
      node = neighbours.back();
      neighbours.pop_back();

      investigatedNodes.push_back(node);
    }
    else {
      return start->position;
    }
  }

  // Now need to find the actual path
  vector<AStarNode*> path;
  node = destination;

  while (!vecContains(path, start))
  {
    path.push_back(node);

    vector<AStarNode*> temp;
    for (AStarNode* n : node->neighbours)
    {
      if (vecContains(investigatedNodes, n))
        temp.push_back(n);
    }

    // sort the temp
    sort(temp.begin(), temp.end(), sortFunc);

    node = temp.back();
    temp.pop_back();
  }

  // delete the destination node that we created
  delete destination;
  delete start;

  return path.back()->position;
} 


/*
Method to drive the AI to the destination point
No actual pathfinding happens here
*/
void AI::driveTo(const vec3 &destination)
{
  // get the position of the vehicle
  mat4 M = vehicle->getModelMatrix();
  vec3 pos = vec3(M[3]);

  vec3 displacement = destination - pos;

  // Get the rotation of the object
  physx::PxVec3 axis = physx::PxVec3(0, 1, 0);
  physx::PxReal angle = 0;
  vehicle->physXVehicle->getRigidDynamicActor()->getGlobalPose().q.toRadiansAndUnitAxis(angle, axis);

  int fix = (axis.y < 0) ? 1 : -1;
  angle *= fix;
  if (angle < 0)
    angle += M_PI * 2;

  // Calculation the normalized orientated displacement
  mat3 rotation(1);
  rotation[0][0] = cos(-angle);
  rotation[0][2] = sin(-angle);
  rotation[2][0] = -sin(-angle);
  rotation[2][2] = cos(-angle);
  vec3 oD = rotation * displacement;
  oD = normalize(oD);



  /* If you want to do some fine tuning of the AI change 
   * the code below.
   */

  if (oD.z > 0)
  {
    vehicle->accelerate(5);

    double turn = min(max(-oD.x / oD.z, -0.6), 0.6);
    turn = ((vehicle->physXVehicle->computeForwardSpeed() > 0)) ? turn : -turn;

    vehicle->turn(turn);
  }
  else
  {
    vehicle->decelerate(5);
    
    double turn = min(max(-oD.x / oD.z, -0.7), 0.7);
    turn = ((vehicle->physXVehicle->computeForwardSpeed() < 0)) ? turn : -turn;

    vehicle->turn(turn);
  }

  if (length(displacement) < 5)
  {
    vehicle->brake(10000.0);
    cout << "breaking" << endl;
  }

}
