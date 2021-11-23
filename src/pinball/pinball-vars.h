
#ifndef DOSBOX_PINBALL_VARS_H
#define DOSBOX_PINBALL_VARS_H

#include <types.h>

class PinballVars {
public:
	enum GameState {
		Startup,
		Menu,
		PressStart,
		LaunchBall,
		BallInPlay,
		LifeLost
	};

	PinballVars();

	uint8_t getPlayerLives() const;
	uint16_t getBallX() const;
	uint16_t getBallY() const;
	long getScore() const;
	bool inPlay() const;

	// -1 = no table
	int getActiveTable() const;
	void setActiveTable(int table);

	GameState getGameState() const { return _activeGameState; }
	void setGameState(const GameState &state);

	void update(float frameTime);

private:
	GameState deduceState() const;

	GameState _activeGameState;

	int _activeTable;
	int _tableMemoryOffset;

};

#endif
