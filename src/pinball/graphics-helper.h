#ifndef DOSBOX_GRAPHICS_HELPER_H
#define DOSBOX_GRAPHICS_HELPER_H

#include <types.h>
#include "../libs/pngpp/png.hpp"

typedef png::image<png::index_pixel> IndexedPNG;

struct RECT {
	int x;
	int y;
	int width;
	int height;
};

static class GraphicsHelper {
public:
	static void clear(Bit8u *frameBuffer, int width, int height, Bit8u color);

	static void setPalette(const IndexedPNG &png);
	static void drawImage(Bit8u *frameBuffer, int frameBufferWidth, int frameBufferHeight, const IndexedPNG& img, int x, int y);

	static void drawPartialImage(Bit8u *frameBuffer,
	                             int frameBufferWidth,
	                             int frameBufferHeight,
	                             const IndexedPNG &img,
	                             int x,
	                             int y,
	                             RECT imgSrc);
};


#endif