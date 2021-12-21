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

	RECT(){};
	RECT(int x, int y, int w, int h)
		: x(x), y(y), width(w), height(h){};
};

struct FrameBuffer {
	Bit8u *buffer;
	int width;
	int height;
};

class GraphicsHelper {
public:
	static void clear(FrameBuffer &frameBuffer, Bit8u color);

	static void setPalette(const IndexedPNG &png);
	static void drawImage(FrameBuffer& frameBuffer, const IndexedPNG& img, int x, int y);

	static void drawPartialImage(FrameBuffer &frameBuffer,
	                             const IndexedPNG &img,
	                             int x,
	                             int y,
	                             RECT imgSrc);

	static void drawPartialImageDither(FrameBuffer& frameBuffer,
	                             const IndexedPNG &img,
	                             int x,
	                             int y,
	                             RECT imgSrc);
};

#endif