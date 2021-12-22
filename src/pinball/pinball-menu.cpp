#include "pinball-menu.h"
#include <stdio.h>
#include <stdlib.h>

namespace {
	const KBD_KEYS TableKeys[] = {KBD_f1, KBD_f2, KBD_f3, KBD_f4,
                              KBD_f5, KBD_f6, KBD_f7, KBD_f8};

	const int CharPxSize = 8;
}

bool PinballMenu::load()
{
	_menuGraphic.read("img/menu.png");
	

	_tables[0].fullname = "Ignition";
	_tables[0].shortName = "IGN";
	_tables[0].plaqueGraphicOffset = 0;

	_tables[1].fullname = "Steel Wheel";
	_tables[1].shortName = "STW";
	_tables[1].plaqueGraphicOffset = 24;

	_tables[2].fullname = "Beat Table";
	_tables[2].shortName = "BBX";
	_tables[2].plaqueGraphicOffset = 48;

	_tables[3].fullname = "Nightmare";
	_tables[3].shortName = "NTM";
	_tables[3].plaqueGraphicOffset = 72;

	_tables[4].fullname = "Neptune";
	_tables[4].shortName = "NPT";
	_tables[4].plaqueGraphicOffset = 96;

	_tables[5].fullname = "Safari";
	_tables[5].shortName = "SFR";
	_tables[5].plaqueGraphicOffset = 120;

	_tables[6].fullname = "Revenge";
	_tables[6].shortName = "RRW";
	_tables[6].plaqueGraphicOffset = 144;

	_tables[7].fullname = "Stall Turn";
	_tables[7].shortName = "STT";
	_tables[7].plaqueGraphicOffset = 168;

	return _menuGraphic.get_width();
}

void PinballMenu::setActive(bool active)
{
	_displayMenu = active;
	if (!active) {
		_activateTable = -1;
		_keyRepeatDelay = 0;
		_pressKey.active = false;
		pPinballVars->setGameState(PinballVars::GameState::PressStart);
		KEYBOARD_ClrBuffer();
	} else {
		pPinballVars->setActiveTable(-1);

		pPinballVars->setGameState(PinballVars::GameState::Menu);
		KEYBOARD_ClrBuffer();
	}
}
void PinballMenu::update(float frameTime)
{
	if (_activateTable != -1) {
		pPinballVars->setActiveTable(_activateTable);

		KEYBOARD_AddKey(TableKeys[_activateTable], false);
		_activateTable = -1;
		_pressKey.active = true;
		return;
	}
	if (_displayMenu) {
		handleControls(frameTime);
	}

	if (_pressKey.active) {
		_pressKey.time -= frameTime;
		if (_pressKey.time <= 0) {
			_pressKey.time = 0.5f - _pressKey.time;
			_pressKey.flip = !_pressKey.flip;

			KEYBOARD_AddKey(_pressKey.key, _pressKey.flip);
		}
	}
}

void PinballMenu::handleControls(float frameTime)
{
	if (_keyRepeatDelay > 0) {
		_keyRepeatDelay -= frameTime;
		if (_keyRepeatDelay < 0) {
			_keyRepeatDelay = 0;
		} else {
			return;
		}
	}

	// check keyboard controls
	if (KEYBOARD_IsKeyPressed(29, true)) {
		// right ctrl
		_keyRepeatDelay = MaxKeyRepeatDelay;

		_activeTable++;
		if (_activeTable >= NumTables) {
			_activeTable = 0;
		}
		//KEYBOARD_ClrBuffer();
	} else if (KEYBOARD_IsKeyPressed(29, false)) {
		// left ctrl
		_keyRepeatDelay = MaxKeyRepeatDelay;

		_activeTable--;
		if (_activeTable < 0) {
			_activeTable = NumTables - 1;
		}
		//KEYBOARD_ClrBuffer();
	}
	#if _DEBUG
	if (KEYBOARD_IsKeyPressed(54, false)) {
#else
	if (KEYBOARD_IsKeyPressed(57, false)) {
	#endif
		// SPACE
		KEYBOARD_ClrBuffer();
		_activateTable = _activeTable;

		KEYBOARD_AddKey(TableKeys[_activateTable], true);
	}
}

void PinballMenu::getCharOffset(char c, int &x, int &y)
{
	const int LetterRowStart = 192;

	c = toupper(c);
	uint8_t asc = (uint8_t)c;

	// letter
	if (asc >= 65 && asc <= 80) {
		y = LetterRowStart;
		x = (asc - 65) * CharPxSize;
	} else if (asc >= 81 && asc <= 90) {
		y = LetterRowStart + CharPxSize;
		x = (asc - 81) * CharPxSize;
	} else if (asc >= 48 && asc <= 57) {
		y = LetterRowStart + CharPxSize + CharPxSize;
		x = (asc - 48) * CharPxSize;
	} else {
		y = LetterRowStart + CharPxSize;
		x = 104;
	}
}

void PinballMenu::renderText(FrameBuffer& fb,
        const char *szText, int x, int y)
{	
	int numChars = strlen(szText);
	RECT glyph;
	glyph.width = 8;
	glyph.height = 8;
	int offset = 0;
	for (int c = 0; c < numChars; ++c) {
		getCharOffset(szText[c], glyph.x, glyph.y);

		GraphicsHelper::drawPartialImage(fb,
		                                 _menuGraphic, x + offset, y,
		                                 glyph);

		offset += CharPxSize;
	}
}

void PinballMenu::renderOutline(FrameBuffer& fb, RECT outline)
{
	assert(outline.width % 8 == 0);
	assert(outline.height % 8 == 0);

	const int TablePieceRow = 216;

	const int PX = 8; //pixel size of each segment

	const int TL = 0x0;
	const int TR = 0x8;
	const int BL = 0x10;
	const int BR = 0x18;
	const int L = 0x20; // going down left hand side
	const int R = 0x28; // going down right hand side
	const int T = 0x30; // going across top
	const int B = 0x38; // going across bottom

	RECT r;
	r.y = TablePieceRow;
	r.width = PX;
	r.height = PX;

	int rhs = outline.x + outline.width;
	int bottom = outline.y + outline.height;

	// horizontal
	r.x = T;
	for (int x = outline.x; x < rhs; x += 8) {
		r.x = T;
		GraphicsHelper::drawPartialImage(fb, _menuGraphic, x,
		                                 outline.y - PX, r);
		r.x = B;
		GraphicsHelper::drawPartialImage(fb, _menuGraphic, x,
		                                 bottom, r);
	}

	// vertical
	for (int y = outline.y; y < bottom; y += 8) {
		r.x = L;
		GraphicsHelper::drawPartialImage(fb, _menuGraphic, outline.x - PX,
		                                 y, r);

		r.x = R;
		GraphicsHelper::drawPartialImage(fb, _menuGraphic,
		                                 rhs, y, r);
	}

	// corners
	r.x = TL;
	GraphicsHelper::drawPartialImage(fb, _menuGraphic, outline.x - PX,
	                                 outline.y - PX, r);
	r.x = TR;
	GraphicsHelper::drawPartialImage(fb, _menuGraphic, rhs, outline.y - PX, r);
	r.x = BL;
	GraphicsHelper::drawPartialImage(fb, _menuGraphic, outline.x - PX, bottom, r);
	r.x = BR;
	GraphicsHelper::drawPartialImage(fb, _menuGraphic, rhs, bottom, r);

}

void PinballMenu::renderTableSelection(FrameBuffer &fb)
{
	RECT plaqueOffset;
	plaqueOffset.x = 0;
	plaqueOffset.y = _tables[_activeTable].plaqueGraphicOffset;
	plaqueOffset.width = 119;
	plaqueOffset.height = 24;
	GraphicsHelper::drawPartialImage(fb, _menuGraphic, 100, 24, plaqueOffset);

	const int SPACING = 20;
	int prevTable = _activeTable - 1;
	if (prevTable < 0) {
		prevTable = NumTables - 1;
	}
	plaqueOffset.x = SPACING + SPACING;
	plaqueOffset.y = _tables[prevTable].plaqueGraphicOffset;
	plaqueOffset.width = 119 - SPACING - SPACING;
	plaqueOffset.height = 24;

	GraphicsHelper::drawPartialImage(fb, _menuGraphic, 0, 10, plaqueOffset);

	int nextTable = _activeTable + 1;
	if (nextTable >= NumTables) {
		nextTable = 0;
	}
	plaqueOffset.x = 0;
	plaqueOffset.y = _tables[nextTable].plaqueGraphicOffset;
	plaqueOffset.width = 119 - SPACING;
	plaqueOffset.height = 24;

	GraphicsHelper::drawPartialImage(fb, _menuGraphic, 100 + 119 + SPACING,
	                                 10, plaqueOffset);

	renderOutline(fb, RECT(100 - 4, 24 - 4, 120 + 8, 24 + 8));
}

void PinballMenu::renderHiscores(FrameBuffer &fb)
{
	PinballHiscore::Table hi = pPinballVars->getHiscoreTable(_activeTable);

	renderText(fb, "HISCORES", 160 - 32, 75);

	for (int score = 0; score < 4; ++score) {
		renderText(fb, hi.scores[score].name, 82, 100 + (score * 15));
		char scoreBuf[13] = {0};
		snprintf(scoreBuf, sizeof(scoreBuf), "%d", hi.scores[score].score);
		int rightAlignPx = (12 - strlen(scoreBuf)) * CharPxSize;
		renderText(fb, scoreBuf, 132 + rightAlignPx, 100 + (score * 15));
	}

	renderOutline(fb, RECT(100 - 32, 70, 120 + 56, 112));
}

void PinballMenu::render(Bit8u* frameBuffer, int width, int height)
{
	if (!_displayMenu) {
		return;
	}

	FrameBuffer fb;
	fb.buffer = frameBuffer;
	fb.width = width;
	fb.height = height;

	if (_activateTable == -1) {
		GraphicsHelper::setPalette(_menuGraphic);
		GraphicsHelper::clear(fb, 0xff);
	}

	renderTableSelection(fb);
	renderHiscores(fb);
}
