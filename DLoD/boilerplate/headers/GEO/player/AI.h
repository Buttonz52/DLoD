#ifndef AI_H
#define AI_H
#include "Player.h"
#include "GEO\AStarNode.h"
#include "Game\GameState.h"


enum Behaviour
{
  retreating = 0, 
  attacking = 1,
  pickup = 2,
  patrolling = 3,
};

class AI :
	public Player
{

  Behaviour behaviour;
  Behaviour oldBehaviour = attacking;
  vec3 oldTarget;

public:
	AI(int);
	~AI();


  void determineBehaviour(GameState* state);
  vec3 determineTarget(GameState* state);
  void getInput(GameState* state);

  // Given a destination will return a point to drive to
  vec3 pathTo(GameState* state, vec3);
  void driveTo(vec3);
};
#endif