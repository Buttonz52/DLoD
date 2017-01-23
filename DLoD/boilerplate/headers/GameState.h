#ifndef GAMESTATE_H
#define GAMESTATE_H
class GameState
{
public:
	GameState();
	~GameState();

	bool GameFinished();
	bool CheckWinCondition();
};
#endif
