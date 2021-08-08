
#ifndef DOSBOX_PINBALL_MENU_H
#define DOSBOX_PINBALL_MENU_H

#include <types.h>
#include "../libs/pngpp/png.hpp"

struct Table {
	const char *fullname;
	const char *shortName;

	int plaqueGraphicOffset;
	png::image<png::index_pixel> tableGraphic;
};

class PinballMenu {
public:
	const int NumTables = 8;

	inline PinballMenu() :
		_displayMenu(false),
		_activeTable(0)
	{ 
	}

	bool load();
	inline void setActive(bool active) { _displayMenu = active; }
	bool isActive() const { return _displayMenu; }

	void update(float frameTime);
	void render(Bit8u *frameBuffer, int width, int height);

private:
	Table _tables[8];
	png::image<png::index_pixel> _plaqueGraphic;

	void handleControls();
	void startActiveTable();
	bool _displayMenu;

	int _activeTable;
};

#endif
