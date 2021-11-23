
#ifndef DOSBOX_PINBALL_VARS_H
#define DOSBOX_PINBALL_VARS_H

#include <types.h>

class PinballVars {
public:
	enum GameState {
		Startup,
		Menu,
		TableLoad,
		LaunchBall,
		BallInPlay,
		LifeLost
	};

	PinballVars();

	uint8_t getPlayerLives() const;
	uint16_t getBallX() const;
	uint16_t getBallY() const;
	
	GameState getGameState() const { return _activeGameState; }
	void setGameState(const GameState &state);

	void update(float frameTime);

private:
	GameState deduceState() const;

	GameState _activeGameState;
};

#endif
