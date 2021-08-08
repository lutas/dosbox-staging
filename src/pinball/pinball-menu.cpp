#include "pinball-menu.h"
#include "graphics-helper.h"
#include <keyboard.h>

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

	_tables[2].fullname = "Ignition";
	_tables[2].shortName = "IGN";
	_tables[2].tableGraphic.read(
	        "C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	_tables[2].plaqueGraphicOffset = 48;

	_tables[3].fullname = "Ignition";
	_tables[3].shortName = "IGN";
	_tables[3].tableGraphic.read(
	        "C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	_tables[3].plaqueGraphicOffset = 72;

	_tables[4].fullname = "Ignition";
	_tables[4].shortName = "IGN";
	_tables[4].tableGraphic.read(
	        "C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	_tables[4].plaqueGraphicOffset = 96;

	_tables[5].fullname = "Ignition";
	_tables[5].shortName = "IGN";
	_tables[5].tableGraphic.read(
	        "C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	_tables[5].plaqueGraphicOffset = 120;

	_tables[6].fullname = "Ignition";
	_tables[6].shortName = "IGN";
	_tables[6].tableGraphic.read(
	        "C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	_tables[6].plaqueGraphicOffset = 144;

	_tables[7].fullname = "Ignition";
	_tables[7].shortName = "IGN";
	_tables[7].tableGraphic.read(
	        "C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	_tables[7].plaqueGraphicOffset = 168;

	return _plaqueGraphic.get_width();
}

static float tmpinc = 1;

void PinballMenu::update(float frameTime)
{
	handleControls();

	// temp hack for display
	tmpinc -= frameTime;
	if (tmpinc < 0) {
		tmpinc = 1 + tmpinc;
		_activeTable++;
		if (_activeTable >= NumTables) {
			_activeTable = 0;
		}
	}
}

void PinballMenu::handleControls()
{
	// check keyboard controls
	bool keyLeftPressed = false; // left CTRL 
	bool keyRightPressed = false; // right CTRL
	bool keyStartPressed = false;

	if (keyLeftPressed) {
		_activeTable--;
		if (_activeTable < 0) {
			_activeTable = NumTables - 1;
		}
	}
	if (keyRightPressed) {
		_activeTable++;
		if (_activeTable >= NumTables) {
			_activeTable = 0;
		}
	}
	if (keyStartPressed) {
		startActiveTable();
	}
}

void PinballMenu::startActiveTable()
{
	const KBD_KEYS TableKeys[] = {KBD_f1, KBD_f2, KBD_f3, KBD_f4,
	                         KBD_f5, KBD_f6, KBD_f7, KBD_f8};

	_displayMenu = false;

	// press appropriate button for the table (F1 - F8)
	KEYBOARD_AddKey(TableKeys[_activeTable], true);
	
	// notify DM?

}

void PinballMenu::render(Bit8u* frameBuffer, int width, int height)
{
	if (!_displayMenu) {
		return;
	}

	GraphicsHelper::setPalette(_plaqueGraphic);
	GraphicsHelper::clear(frameBuffer, width, height, 0x0);

	//GraphicsHelper::drawImage(frameBuffer, width, height, _plaqueGraphic, 100, 0);

	RECT tableGraphic;
	tableGraphic.x = 0;
	tableGraphic.y = _tables[_activeTable].plaqueGraphicOffset;
	tableGraphic.width = 119;
	tableGraphic.height = 24;
	GraphicsHelper::drawPartialImage(frameBuffer, width, height,
	                                 _plaqueGraphic, 100, 80, tableGraphic);

}