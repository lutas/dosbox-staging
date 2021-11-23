#include "pinball-vars.h"

#include "mem.h"

PinballVars::PinballVars() : _activeGameState(PinballState::Startup)
{
}

void PinballVars::setGameState(const PinballState &state)
{
	if (state != _activeGameState) {
		_activeGameState = state;

		// notify something
	}
}

uint8_t PinballVars::getPlayerLives() const
{
	uint8_t *_pPlayerLives = (uint8_t *)(MemBase + 143595);
	return *_pPlayerLives;
}

uint16_t PinballVars::getBallX() const
{
	uint16_t *_pBallX = (uint16_t *)(MemBase + 143595 - 2523);
	return *_pBallX;
}

uint16_t PinballVars::getBallY() const
{
	uint16_t *_pBallY = (uint16_t *)(MemBase + 143595 - 2523 + 4);
	return *_pBallY;
}

PinballState PinballVars::deduceState() const
{
	uint8_t lives = getPlayerLives();

	// deduce in game state
	if (lives == 0 || lives > 10) {
		return PinballState::TableLoad;
	}

	// we have lives, we must be playing
	const uint16_t x = getBallX();
	const uint16_t y = getBallY();

	// if ball is in tube, we're waiting for launch
	if (x > 36 && x < 39 && y > 56) {
		return PinballState::LaunchBall;
	}

	// if it's offscreen, we've lost a life
	if (y > 70) {
		return PinballState::LifeLost;
	}

	// otherwise the ball is active
	return PinballState::BallInPlay;
}


void PinballVars::update(float frameTime)
{
	if (_activeGameState == PinballState::Startup || _activeGameState == PinballState::Menu) {
		return;
	}

	setGameState(deduceState());
}
