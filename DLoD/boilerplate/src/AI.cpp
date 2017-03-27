#include "GEO\player\AI.h"

struct sortClass
{
  vec3 destination;

  bool operator()(AStarNode* a, AStarNode* b)
  {
    double dista = a->distanceTo + 5.0 * length(destination - a->position);
    double distb = b->distanceTo + 5.0 * length(destination - b->position);

    //a->dist = dista;
    //b->dist = distb;

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



AI::AI(int i) : Player(i)
{
  //
  //for (int i = -10; i < 10; ++i)
  //{
  //  for (int j = -10; j < 10; ++j)
  //  {

  //    AStarNode* node = new AStarNode();
  //    node->position = vec3(i * 5, 0, j * 5);
  //    AStarNodes.push_back(node);
  //  }
  //  
  //} 

  //nodeTree = new OctTree(AStarNodes, vec3(0, 0, 0), 50.0, 50.0, 50.0);

  //for (AStarNode* n : AStarNodes)
  //{
  //  vector<AStarNode*> neighbours;
  //  nodeTree->getNodesForSphere(neighbours, n->position, n->neighbourRadius);
  //  for (AStarNode* m : neighbours)
  //  {
  //    if (m != n)
  //      n->neighbours.push_back(m);
  //  }
  //}
}


AI::~AI()
{
}

void AI::determineBehaviour(GameState* state)
{

}

vec3 AI::determineTarget(GameState * state)
{
  return vec3();
}

void AI::getInput(GameState* state)
{
  // determine a target
  Player* nearestPlayer = nullptr;
  Item* nearestPickUp = nullptr;

  behaviour = patrolling;

  double distNP = 999999999;
  for (Player* p : state->players)
  {
    if (p == this || p->isDead())
      continue;

    if (nearestPlayer != nullptr)
    {
      vec3 dist = vec3(p->vehicle->getModelMatrix()[3] - vehicle->getModelMatrix()[3]);
      double l2 = length(dist);

      if (length(dist * vec3(0, 1, 0)) > 0.75 * l2)
        continue;

      nearestPlayer = (distNP < l2) ? nearestPlayer : p;
      distNP = (distNP < l2) ? distNP : l2;
    }
    else
    {
      nearestPlayer = p;
      distNP = length(vec3(nearestPlayer->vehicle->getModelMatrix()[3] - vehicle->getModelMatrix()[3]));
    }
  }

  double distNPU = 999999999;
  for (Item* item : state->items)
  {
    if (item->isTrap)
      continue;

    if (nearestPickUp != nullptr)
    {
      vec3 dist = vec3(item->getModelMatrix()[3] - vehicle->getModelMatrix()[3]);
      double l2 = length(dist);

      if (length(dist * vec3(0, 1, 0)) > 0.75 * l2)
        continue;

      nearestPickUp = (distNPU < l2) ? nearestPickUp : item;
      distNPU = (distNPU < l2) ? distNPU : l2;
    }
    else
    {
      nearestPickUp = item;
      distNPU = length(vec3(nearestPickUp->getModelMatrix()[3] - vehicle->getModelMatrix()[3]));
    }
  }

  if (min(distNP, distNPU) < 100.0)
    behaviour = (distNP < distNPU) ? attacking : pickup;

  double health = vehicle->getHealth();
  double armour = vehicle->getArmour();

  if (armour / vehicle->getInitialArmour() < 0.25)
  {
    behaviour = retreating;
  }


  vec3 target;
  vec3 cp;
  vec3 ray;
  vector<AStarNode*> nodes;
  bool safe = false;


  switch (behaviour)
  {
  case retreating:
    if (nearestPlayer == nullptr)
      break;
      
    cp = vec3(vehicle->getModelMatrix()[3]);
    ray = cp - vec3(nearestPlayer->vehicle->getModelMatrix()[3]);
    state->nodes->getNodesForArc(nodes, cp, ray);

    if (nodes.size() == 0)
    {
      state->nodes->getNodesForSphere(nodes, cp, 20);
    }

    if (nodes.size() != 0)
    {
      safe = true;
      int i = state->timer.getTicks();
      target = nodes[i % nodes.size()]->position;
    }
    break;

  case attacking:
    if (nearestPlayer != nullptr)
    {
      target = vec3(nearestPlayer->vehicle->getModelMatrix()[3]);
      safe = true;
    }
    break;

  case pickup:
    if (nearestPickUp != nullptr)
    {
      target = vec3(nearestPickUp->getModelMatrix()[3]);
      safe = true;
    }
    break;

  case patrolling:
    cp = vec3(vehicle->getModelMatrix()[3]);
    state->nodes->getNodesForSphere(nodes, cp, 20);

    if (nodes.size() != 0)
    {
      safe = true;
      int i = state->timer.getTicks();
      target = nodes[i % nodes.size()]->position;
    }
    break;

  default:
    break;
  }

  if (safe)
    driveTo(pathTo(state, target));
}

/*
This function does not work dont use it yet
wont currently support multithreading
*/
vec3 AI::pathTo(GameState* state, vec3 dest)
{
  // Get all the AStar Nodes and set their distance to value to be big
  state->nodes->resetNodes();

  // Find all the closest AStarNodes to the destination
  AStarNode* start = new AStarNode();
  start->position = vec3(vehicle->getModelMatrix()[3]);
  state->nodes->getNodesForSphere(start->neighbours, start->position, start->neighbourRadius);

  AStarNode* destination = new AStarNode();
  destination->position = dest;
  state->nodes->getNodesForSphere(destination->neighbours, destination->position, destination->neighbourRadius);

  start->distanceTo = 0;
  destination->distanceTo = 50000;

  // Add start as a neighbour to its neighbours
  for (AStarNode* n : start->neighbours)
    n->neighbours.push_back(start);

  // Add destination as a neighbour to its neighbours
  for (AStarNode* n : destination->neighbours)
    n->neighbours.push_back(destination);

  
  if (length(start->position - destination->position) < start->neighbourRadius)
  {
    start->neighbours.push_back(destination);
    destination->neighbours.push_back(start);
  }

  if (destination->neighbours.size() == 0 || start->neighbours.size() == 0)
  {
    delete start;
    delete destination;
    return dest;
  }

  vector<AStarNode*> investigatedNodes;
  vector<AStarNode*> neighbours; // neighbours to the investigatedNodes
  investigatedNodes.push_back(start);

  AStarNode* node = start;

  //
  while (!vecContains(investigatedNodes, destination))
  {
    for (AStarNode* n : node->neighbours)
    {
      // calculate a new distanceTo 
      n->distanceTo = min(n->distanceTo, node->distanceTo + length(n->position - node->position));

      if (!n->obstructed && !vecContains(investigatedNodes, n) && !vecContains(neighbours, n))
        neighbours.push_back(n);
    }


    if (!neighbours.empty())
    {
      // Sort the neighbours 
      sortClass obj;
      obj.destination = dest;
      sort(neighbours.begin(), neighbours.end(), obj);

      node = neighbours.back();
      neighbours.pop_back();

      investigatedNodes.push_back(node);
    }
    else {
      delete start;
      delete destination;
      return vec3(0,0,0);
    }
  }

  // Now need to find the actual path
  vector<AStarNode*> path;
  node = destination;

  while (node != start)
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
  }

  vec3 nextPos = path.back()->position;

  // delete the destination node that we created
  delete destination;
  delete start;

  return nextPos;
} 


/*
Method to drive the AI to the destination point
No actual pathfinding happens here
*/
void AI::driveTo(vec3 destination)
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
    vehicle->accelerate(1);

    double turn = min(max(-oD.x / oD.z, -1), 1);
    turn = ((vehicle->physXVehicle->computeForwardSpeed() > 0)) ? turn : -turn;

    vehicle->turn(turn);
  }
  else
  {
    vehicle->decelerate(1);
    
    double turn = (oD.x < 0) ? -1.0 : 1.0;
    turn = ((vehicle->physXVehicle->computeForwardSpeed() < 0)) ? turn : -turn;

    vehicle->turn(turn * M_PI);
  }
}

