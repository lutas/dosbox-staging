#include "pinball-menu.h"
#include "graphics-helper.h"

bool PinballMenu::load()
{
	_plaqueGraphic.read("C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	
	
	_tables[0].fullname = "Ignition";
	_tables[0].shortName = "IGN";
	_tables[0].tableGraphic.read("C:\\dev\\pinball\\pinball-score\\img\\plaques-indexed.png");
	_tables[0].plaqueGraphicOffset = 0;

	return _plaqueGraphic.get_width();
}

void PinballMenu::update(int frameTime)
{
	if (!_active) {
		return;
	}
}

void PinballMenu::render(Bit8u* frameBuffer, int width, int height)
{
	if (!_active) {
		return;
	}

	GraphicsHelper::setPalette(_plaqueGraphic);
	GraphicsHelper::clear(frameBuffer, width, height, 0x0);

	GraphicsHelper::drawImage(frameBuffer, width, height, _plaqueGraphic, 100, 0);
}