#include "pinball-vars.h"

#include "mem.h"

namespace MemOffset {

// used cheat engine to get offsets for these variables 
// (remember to get offset by taking CheatEngine address from MemBase value)

	const int Lives = 143595;
	const int BallX = 143595 - 2523;
    const int BallY = 143595 - 2523 + 4;
    const int Score = 138930;

}


PinballVars::PinballVars() : _activeGameState(GameState::Startup)
{
}

void PinballVars::setGameState(const GameState &state)
{
	if (state != _activeGameState) {
		_activeGameState = state;

		// notify something
	}
}

uint8_t PinballVars::getPlayerLives() const
{
	uint8_t *_pPlayerLives = (uint8_t *)(MemBase + MemOffset::Lives);
	return *_pPlayerLives;
}

uint16_t PinballVars::getBallX() const
{
	uint16_t *_pBallX = (uint16_t *)(MemBase + MemOffset::BallX);
	return *_pBallX;
}

uint16_t PinballVars::getBallY() const
{
	uint16_t *_pBallY = (uint16_t *)(MemBase + MemOffset::BallY);
	return *_pBallY;
}

long PinballVars::getScore() const
{
	uint8_t *_pScoreStart = (uint8_t *)(MemBase + MemOffset::Score);

	// score is 12 segments. Segments are stored in a 6 byte array (2 integers per byte)
	// eg.
	// Score = 000018580001
	// Hex = [0x00] [0x00] [0x18] [0x58] [0x00] [0x01]
	// need to extract the left and right digit from each hex pair
	long score = 0;
	long multiplier = 1;
	for (int i = 0; i < 6; ++i) {
		uint8_t val = *(_pScoreStart + i);

		long rhs = (val & 0x0F);
		score += rhs * multiplier;

		multiplier *= 10;

		long lhs = ((val & 0xF0) >> 4);
		score += lhs * multiplier;

		multiplier *= 10;
	}

	return score;
}

PinballVars::GameState PinballVars::deduceState() const
{
	uint8_t lives = getPlayerLives();

	// deduce in game state
	if (lives == 0 || lives > 10) {
		return GameState::TableLoad;
	}

	// we have lives, we must be playing
	const uint16_t x = getBallX();
	const uint16_t y = getBallY();

	long score = getScore();

	// if ball is in tube, we're waiting for launch
	if (x > 36 && x < 39 && y > 56) {
		return GameState::LaunchBall;
	}

	// if it's offscreen, we've lost a life
	if (y > 70) {
		return GameState::LifeLost;
	}

	// otherwise the ball is active
	return GameState::BallInPlay;
}


void PinballVars::update(float frameTime)
{
	if (_activeGameState == GameState::Startup || _activeGameState == GameState::Menu) {
		return;
	}

	setGameState(deduceState());
}
