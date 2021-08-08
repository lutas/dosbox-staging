#include "graphics-helper.h"
#include <SDL2/SDL_stdinc.h>
#include <render.h>

void GraphicsHelper::clear(Bit8u *frameBuffer, int width, int height, Bit8u color)
{
	SDL_memset(frameBuffer, color, (size_t)width * (size_t)height * sizeof(Bit8u));
}

void GraphicsHelper::drawImage(Bit8u *frameBuffer,
                               int frameBufferWidth,
                               int frameBufferHeight,
                               const IndexedPNG& img,
                               int x,
                               int y)
{
	const int lineBytes = img.get_width() * sizeof(Bit8u);
	const int frameBufferLineSize = frameBufferWidth * sizeof(Bit8u);

	for (size_t line = 0; line < img.get_height(); ++line) {
		int imgOffset = line * lineBytes;
		Bit8u *frameBufferOffset =
		        frameBuffer +
		        (((size_t)frameBufferLineSize * (y + line)) + x);

		const std::vector<png::index_pixel> row = img.get_row(line);
		std::vector<png::index_pixel>::const_iterator iter;
		for (iter = row.begin(); iter != row.end(); ++iter) {

			memcpy(frameBufferOffset, (void*)(&*iter), 1);
			frameBufferOffset++;
		}
	}
}

void GraphicsHelper::drawPartialImage(Bit8u *frameBuffer,
                               int frameBufferWidth,
                               int frameBufferHeight,
                               const IndexedPNG &img,
                               int x,
                               int y, 
							   RECT imgSrc)
{
	const int lineBytes = img.get_width() * sizeof(Bit8u);
	const int frameBufferLineSize = frameBufferWidth * sizeof(Bit8u);

	for (size_t line = imgSrc.y; line < img.get_height(); ++line) {
		int imgOffset = line * lineBytes;
		Bit8u *frameBufferOffset =
		        frameBuffer +
		        (((size_t)frameBufferLineSize * (y + line)) + x);

		const std::vector<png::index_pixel> row = img.get_row(line);
		std::vector<png::index_pixel>::const_iterator iter;
		for (iter = row.begin(); iter != row.end(); ++iter) {
			memcpy(frameBufferOffset, (void *)(&*iter), 1);
			frameBufferOffset++;
		}
	}
}

void GraphicsHelper::setPalette(const IndexedPNG &img)
{
	const auto v = img.get_palette();

	for (int i = 0; i < v.size(); ++i) {
		RENDER_SetPal(i, v[i].red, v[i].green, v[i].blue);
	}
}