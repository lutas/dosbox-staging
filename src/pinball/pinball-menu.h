
#ifndef DOSBOX_PINBALL_MENU_H
#define DOSBOX_PINBALL_MENU_H

#include <types.h>
#include <keyboard.h>

#include "pinball-vars.h"
#include "../libs/pngpp/png.hpp"
#include "graphics-helper.h"

struct Table {
	const char *fullname;
	const char *shortName;

	int plaqueGraphicOffset;
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

	inline PinballMenu(PinballVars& vars) :
		pPinballVars(&vars),
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

protected:
	void getCharOffset(char c, int &x, int &y);
	void renderText(FrameBuffer &frameBuffer, const char *szText, int x, int y);
	void renderOutline(FrameBuffer &frameBuffer, RECT outline);

	void renderTableSelection(FrameBuffer &frameBuffer);
	void renderHiscores(FrameBuffer &frameBuffer);

private:
	Table _tables[8];
	png::image<png::index_pixel> _menuGraphic;

	PinballVars *pPinballVars;

	void handleMenuControls(float frameTime);

	bool _displayMenu;

	int _activeTable;
	float _keyRepeatDelay;
	int _activateTable;
	PressKey _pressKey;
};

#endif
