#ifndef AI_H
#define AI_H
#include "Player.h"
class AI :
	public Player
{
public:
	AI();
	~AI();
	int difficultyState;

	int DetermineBehaviour();
};
#endif