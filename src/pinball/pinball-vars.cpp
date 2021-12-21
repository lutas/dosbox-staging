#include "pinball-vars.h"

#include "mem.h"

#if _DEBUG
extern void GFX_SetTitle(const char* title);
#endif

namespace MemOffset {

// used cheat engine to get offsets for these variables 
// (remember to get offset by taking CheatEngine address from MemBase value)

	// tables 5-8 have a different offset
	const int ExtendedTablesOffset = 142876 - 143595;

	const int Score = 138930;
	const int Lives = 143595;
    const int BallX = 143595 - 2523;
	const int BallY = 143595 - 2519;

	const int InPlay = Lives - 2527;
}

PinballVars::PinballVars(PinballSerial &pinballLights)
        : 
	_pPinballLights(&pinballLights),
	_activeGameState(GameState::Startup),
	_activeTable(0),
	_tableMemoryOffset(0)
{
}

int PinballVars::getActiveTable() const
{
	return _activeTable;
}

void PinballVars::setActiveTable(int table)
{
	_activeTable = table;
	if (_activeTable >= 4 && _activeTable < 8) {
		_tableMemoryOffset = MemOffset::ExtendedTablesOffset;
	} else {
		_tableMemoryOffset = 0;
	}
}

void PinballVars::setGameState(const GameState &state)
{
	if (state != _activeGameState) {
		_activeGameState = state;

		// notify something
		switch (state) {
		case GameState::BallInPlay: {
			_pPinballLights->setLight(LightId::LeftBumper, true);
			_pPinballLights->setLight(LightId::RightBumper, true);
			_pPinballLights->setLight(LightId::Space, false);
			_pPinballLights->setLight(LightId::Start, false);
			_pPinballLights->setLight(LightId::Escape, true);
			_pPinballLights->setLight(LightId::Launch, false);
		} 
		break;
		case GameState::LaunchBall: {
			_pPinballLights->setLight(LightId::LeftBumper, false);
			_pPinballLights->setLight(LightId::RightBumper, false);
			_pPinballLights->setLight(LightId::Space, false);
			_pPinballLights->setLight(LightId::Start, false);
			_pPinballLights->setLight(LightId::Escape, true);
			_pPinballLights->setLight(LightId::Launch, true);
		} break;
		case GameState::LifeLost: {
			_pPinballLights->setLight(LightId::LeftBumper, false);
			_pPinballLights->setLight(LightId::RightBumper, false);
			_pPinballLights->setLight(LightId::Space, false);
			_pPinballLights->setLight(LightId::Start, false);
			_pPinballLights->setLight(LightId::Escape, false);
			_pPinballLights->setLight(LightId::Launch, false);
		} break;
		case GameState::Menu: {
			_pPinballLights->setLight(LightId::LeftBumper, true);
			_pPinballLights->setLight(LightId::RightBumper, true);
			_pPinballLights->setLight(LightId::Space, true);
			_pPinballLights->setLight(LightId::Start, false);
			_pPinballLights->setLight(LightId::Escape, false);
			_pPinballLights->setLight(LightId::Launch, false);

			_hiscore.LoadHiscoreFiles(
			        "..\\DELUXE\\HISCORES.PD1",
			        "..\\DELUXE\\HISCORES.PD2");
		} break;
		case GameState::Startup: {
			_pPinballLights->setLight(LightId::LeftBumper, false);
			_pPinballLights->setLight(LightId::RightBumper, false);
			_pPinballLights->setLight(LightId::Space, true);
			_pPinballLights->setLight(LightId::Start, false);
			_pPinballLights->setLight(LightId::Escape, false);
			_pPinballLights->setLight(LightId::Launch, false);
		} break;
		case GameState::PressStart: {
			_pPinballLights->setLight(LightId::LeftBumper, false);
			_pPinballLights->setLight(LightId::RightBumper, false);
			_pPinballLights->setLight(LightId::Space, false);
			_pPinballLights->setLight(LightId::Start, true);
			_pPinballLights->setLight(LightId::Escape, true);
			_pPinballLights->setLight(LightId::Launch, false);
		} break;
		}


#if _DEBUG
		switch (_activeGameState) {
		case GameState::BallInPlay: GFX_SetTitle("Ball in play"); break;
		case GameState::LaunchBall: GFX_SetTitle("Launch ball"); break;
		case GameState::LifeLost: GFX_SetTitle("Life lost"); break;
		case GameState::Menu: GFX_SetTitle("Menu"); break;
		case GameState::Startup: GFX_SetTitle("Startup"); break;
		case GameState::PressStart: GFX_SetTitle("Press Start"); break;
		}
#endif
	}
}

uint8_t PinballVars::getPlayerLives() const
{
	uint8_t *_pPlayerLives = (uint8_t *)(MemBase +
	                                     _tableMemoryOffset +
										MemOffset::Lives);
	return *_pPlayerLives;
}

uint16_t PinballVars::getBallX() const
{
	uint16_t *_pBallX = (uint16_t *)(MemBase + 
									_tableMemoryOffset +
									MemOffset::BallX);
	return *_pBallX;
}

uint16_t PinballVars::getBallY() const
{
	uint16_t *_pBallY = (uint16_t *)(MemBase + 
									_tableMemoryOffset +
									MemOffset::BallY);
	return *_pBallY;
}

long PinballVars::getScore() const
{
	uint8_t *_pScoreStart = (uint8_t *)(MemBase + 
										_tableMemoryOffset + 
										MemOffset::Score);

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

bool PinballVars::inPlay() const
{
	uint8_t *pPlay = (uint8_t *)(MemBase + _tableMemoryOffset + MemOffset::InPlay);
	return (*pPlay) > 0;
}

PinballVars::GameState PinballVars::deduceState() const
{
	uint8_t lives = getPlayerLives();

	// deduce in game state
	if (lives == 0 || lives > 10) {
		return GameState::PressStart;
	}

	// we have lives, we must be playing
	const uint16_t y = getBallY();
	if (y > 65000) {
		return GameState::PressStart;
	}

	// if it's offscreen, we've lost a life
	if (y > 64) {
		return GameState::LifeLost;
	}

	const uint16_t x = getBallX();

	// if ball is in tube, we're waiting for launch
	if (x > 35 && x < 39 && y > 54) {
		if (!inPlay()) {
			return GameState::PressStart;
		}
		return GameState::LaunchBall;
	}
	// otherwise the ball is active
	return GameState::BallInPlay;
}

PinballHiscore::Table PinballVars::getHiscoreTable(int table)
{
	return _hiscore.getTable(table);
}

void PinballVars::update(float frameTime)
{
	if (_activeGameState == GameState::Startup || _activeGameState == GameState::Menu) {
		return;
	}

	setGameState(deduceState());

}
