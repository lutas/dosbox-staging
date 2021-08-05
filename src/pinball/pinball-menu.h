
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
	inline PinballMenu() :
		_active(true)
	{ 
	}

	bool load();
	inline void setActive(bool active) { _active = active; }

	void update(int frameTime);
	void render(Bit8u *frameBuffer, int width, int height);

private:
	Table _tables[8];
	png::image<png::index_pixel> _plaqueGraphic;
	bool _active;
};

#endif
