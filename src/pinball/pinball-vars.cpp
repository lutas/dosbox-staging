#include "pinball-vars.h"

#include "mem.h"
#include <types.h>
#include <keyboard.h>

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

PinballVars::PinballVars(PinballSerial& pinballDotMatrix, PinballSerial &pinballLights)
        : 
	_pPinballDotMatrix(&pinballDotMatrix),
	_pPinballLights(&pinballLights),
	_activeGameState(GameState::Startup),
	_activeTable(0),
	_tableMemoryOffset(0),
    _quitGameActive(QuitState::Playing)
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

		_prevGameState = _activeGameState;
		_activeGameState = state;

		// notify something
		switch (state) {
		case GameState::BallInPlay: {
			_pPinballLights->setLight(LightId::LeftBumper, true);
			_pPinballLights->setLight(LightId::RightBumper, true);
			_pPinballLights->setLight(LightId::Space, false);
			_pPinballLights->setLight(LightId::Start, false);
			_pPinballLights->setLight(LightId::Escape, false);
			_pPinballLights->setLight(LightId::Launch, false);
		} 
		break;
		case GameState::LaunchBall: {
			_pPinballLights->setLight(LightId::LeftBumper, false);
			_pPinballLights->setLight(LightId::RightBumper, false);
			_pPinballLights->setLight(LightId::Space, false);
			_pPinballLights->setLight(LightId::Start, false);
			_pPinballLights->setLight(LightId::Escape, false);
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
			        "../DELUXE/HISCORES.PD1",
			        "../DELUXE/HISCORES.PD2");
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
			_pPinballLights->setLight(LightId::Escape, false);
			_pPinballLights->setLight(LightId::Launch, false);
		} break;
		case GameState::Paused: {
			_pPinballLights->setLight(LightId::LeftBumper, false);
			_pPinballLights->setLight(LightId::RightBumper, false);
			_pPinballLights->setLight(LightId::Space, true);
			_pPinballLights->setLight(LightId::Start, false);
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
		case GameState::Paused: GFX_SetTitle("Paused"); break;
		}
#endif
	}
}

bool PinballVars::isPlayingOnATable() const
{
	if (_activeTable < 0) {
		return false;
	}

	switch (_activeGameState) {
		case GameState::PressStart:
		case GameState::LaunchBall:
		case GameState::BallInPlay:
		case GameState::LifeLost:
		case GameState::Paused: {
			return true;
		}

		default: {
			return false;
		}
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
	if (_activeGameState == GameState::Startup ||
	    _activeGameState == GameState::Menu) {
		return;
	}

	_quitGameTimer -= frameTime;
	if (_quitGameTimer < 0) {
		_quitGameTimer = 0;
	}

	setGameState(deduceState());

	// monitor for quit game being pressed
	switch (_quitGameActive) {
	case QuitState::QuitPressed:
		if (_quitGameTimer <= 0) {
			// n = 49, y = 21
			if (KEYBOARD_IsKeyPressed(1, false)) {
				// if escape pressed again, then send cancel quit (N key)
				KEYBOARD_ClrBuffer();
				KEYBOARD_AddKey(KBD_n, true);

				_quitGameActive = QuitState::QuitCancelled;
				_quitGameTimer = 0.2f;
			} else if (KEYBOARD_IsKeyPressed(54, false) || KEYBOARD_IsKeyPressed(57, false)) {
				// if shift or space, then send confirm quit (Y key)
				KEYBOARD_ClrBuffer();
				KEYBOARD_AddKey(KBD_y, true);

				_quitGameActive = QuitState::QuitConfirmed;
				_quitGameTimer = 0.2f;
			}
		} else {
			KEYBOARD_AddKey(KBD_esc, false);

		}
		break;

	case QuitState::Playing:
		if (KEYBOARD_IsKeyPressed(1, false)) {

			KEYBOARD_ClrBuffer();
			KEYBOARD_AddKey(KBD_esc, true);

			_quitGameActive = QuitState::QuitPressed;
			setGameState(GameState::Paused);
			_quitGameTimer = 0.2f;			
		}
		break;

	case QuitState::QuitCancelled:
		_quitGameTimer -= frameTime;
		if (_quitGameTimer <= 0) {
			KEYBOARD_AddKey(KBD_n, false);
			
			_quitGameActive = QuitState::Playing;
			_quitGameTimer = 0.2f;

			// reset to state before paused
			setGameState(_prevGameState);
		}
		break;

	case QuitState::QuitConfirmed:
		_quitGameTimer -= frameTime;
		if (_quitGameTimer <= 0) {
			KEYBOARD_AddKey(KBD_y, false);

			_quitGameActive = QuitState::Playing;
			_quitGameTimer = 0.2f;

			setGameState(GameState::Menu);
		}
		break;
	}
}
