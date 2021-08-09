#include "pinball-menu.h"
#include "graphics-helper.h"

namespace {
	const KBD_KEYS TableKeys[] = {KBD_f1, KBD_f2, KBD_f3, KBD_f4,
								  KBD_f5, KBD_f6, KBD_f7, KBD_f8};
}

bool PinballMenu::load()
{
	_plaqueGraphic.read("C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	
	
	_tables[0].fullname = "Ignition";
	_tables[0].shortName = "IGN";
	_tables[0].tableGraphic.read("C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	_tables[0].plaqueGraphicOffset = 0;

	_tables[1].fullname = "Steel Wheel";
	_tables[1].shortName = "STW";
	_tables[1].tableGraphic.read(
	        "C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	_tables[1].plaqueGraphicOffset = 24;

	_tables[2].fullname = "Beat Table";
	_tables[2].shortName = "BBX";
	_tables[2].tableGraphic.read(
	        "C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	_tables[2].plaqueGraphicOffset = 48;

	_tables[3].fullname = "Nightmare";
	_tables[3].shortName = "NTM";
	_tables[3].tableGraphic.read(
	        "C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	_tables[3].plaqueGraphicOffset = 72;

	_tables[4].fullname = "Neptune";
	_tables[4].shortName = "NPT";
	_tables[4].tableGraphic.read(
	        "C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	_tables[4].plaqueGraphicOffset = 96;

	_tables[5].fullname = "Safari";
	_tables[5].shortName = "SFR";
	_tables[5].tableGraphic.read(
	        "C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	_tables[5].plaqueGraphicOffset = 120;

	_tables[6].fullname = "Revenge";
	_tables[6].shortName = "RRW";
	_tables[6].tableGraphic.read(
	        "C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	_tables[6].plaqueGraphicOffset = 144;

	_tables[7].fullname = "Stall Turn";
	_tables[7].shortName = "STT";
	_tables[7].tableGraphic.read(
	        "C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	_tables[7].plaqueGraphicOffset = 168;

	return _plaqueGraphic.get_width();
}

void PinballMenu::setActive(bool active)
{
	_displayMenu = active;
	if (!active) {
		_activateTable = -1;
		_keyRepeatDelay = 0;
		_pressKey.active = false;
	}
}

void PinballMenu::update(float frameTime)
{
	if (_activateTable != -1) {
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
			_pressKey.time = MaxKeyRepeatDelay - _pressKey.time;
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
	} else if (KEYBOARD_IsKeyPressed(29, false)) {
		// left ctrl
		_keyRepeatDelay = MaxKeyRepeatDelay;

		_activeTable--;
		if (_activeTable < 0) {
			_activeTable = NumTables - 1;
		}
	}
	if (KEYBOARD_IsKeyPressed(54, false)) {
		_activateTable = _activeTable;

		KEYBOARD_AddKey(TableKeys[_activateTable], true);
	}
}

void PinballMenu::render(Bit8u* frameBuffer, int width, int height)
{
	if (!_displayMenu) {
		return;
	}

	GraphicsHelper::setPalette(_plaqueGraphic);
	GraphicsHelper::clear(frameBuffer, width, height, 0xff);

	RECT tableGraphic;
	tableGraphic.x = 0;
	tableGraphic.y = _tables[_activeTable].plaqueGraphicOffset;
	tableGraphic.width = 119;
	tableGraphic.height = 24;
	GraphicsHelper::drawPartialImage(frameBuffer, width, height,
	                                 _plaqueGraphic, 100, 80, tableGraphic);

}