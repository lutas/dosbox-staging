
#ifndef DOSBOX_PINBALL_VARS_H
#define DOSBOX_PINBALL_VARS_H

#include <types.h>
#include "pinball-serial.h"
#include "pinball-hiscore.h"

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

	enum LightId {
		LeftBumper = 0,
		RightBumper = 1,
		Space = 2,
		Start = 3,
		Escape = 4,
		Launch = 5
	};

	PinballVars(PinballSerial& pinballLights);

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

	PinballHiscore::Table getHiscoreTable(int table);

	void update(float frameTime);

private:
	GameState deduceState() const;

	GameState _activeGameState;

	PinballSerial *_pPinballLights;
	PinballHiscore _hiscore;

	int _activeTable;
	int _tableMemoryOffset;

};

#endif
