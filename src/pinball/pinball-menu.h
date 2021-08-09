
#ifndef DOSBOX_PINBALL_MENU_H
#define DOSBOX_PINBALL_MENU_H

#include <types.h>
#include <keyboard.h>
#include "../libs/pngpp/png.hpp"

struct Table {
	const char *fullname;
	const char *shortName;

	int plaqueGraphicOffset;
	png::image<png::index_pixel> tableGraphic;
};

struct PressKey {
	bool active;
	KBD_KEYS key;
	float time;
	bool flip;
};

class PinballMenu {
public:
	const int NumTables = 8;
	const float MaxKeyRepeatDelay = 0.2f;

	inline PinballMenu() :
		_displayMenu(false),
		_activeTable(0),
		_keyRepeatDelay(0),
		_activateTable(-1)
	{ 
		_pressKey.active = false;
		_pressKey.flip = false;
		_pressKey.key = KBD_f1;
		_pressKey.time = 0;
	}

	bool load();
	void setActive(bool active);
	bool isActive() const { return _displayMenu; }

	void update(float frameTime);
	void render(Bit8u *frameBuffer, int width, int height);

private:
	Table _tables[8];
	png::image<png::index_pixel> _plaqueGraphic;

	void handleControls(float frameTime);
	bool _displayMenu;

	int _activeTable;
	float _keyRepeatDelay;
	int _activateTable;
	PressKey _pressKey;
};

#endif
