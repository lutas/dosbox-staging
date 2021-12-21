#include "graphics-helper.h"
#include <SDL2/SDL_stdinc.h>
#include <render.h>

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

void GraphicsHelper::clear(FrameBuffer &frameBuffer, Bit8u color)
{
	SDL_memset(frameBuffer.buffer, color, (size_t)frameBuffer.width * (size_t)frameBuffer.height * sizeof(Bit8u));
}

void GraphicsHelper::drawImage(FrameBuffer &frameBuffer,
                               const IndexedPNG& img,
                               int x,
                               int y)
{
	const int lineBytes = img.get_width() * sizeof(Bit8u);
	const int frameBufferLineSize = frameBuffer.width * sizeof(Bit8u);

	for (size_t line = 0; line < img.get_height(); ++line) {
		int imgOffset = line * lineBytes;
		Bit8u *frameBufferOffset =
		        frameBuffer.buffer +
		        (((size_t)frameBufferLineSize * (y + line)) + x);

		const std::vector<png::index_pixel> row = img.get_row(line);
		std::vector<png::index_pixel>::const_iterator iter;
		for (iter = row.begin(); iter != row.end(); ++iter) {

			memcpy(frameBufferOffset, (void*)(&*iter), 1);
			frameBufferOffset++;
		}
	}
}

void GraphicsHelper::drawPartialImage(
							   FrameBuffer &frameBuffer,
                               const IndexedPNG &img,
                               int x,
                               int y, 
							   RECT imgSrc)
{
	const int lineBytes = img.get_width() * sizeof(Bit8u);
	const int frameBufferLineSize = frameBuffer.width * sizeof(Bit8u);

	for (size_t line = 0; line < imgSrc.height; ++line) {
		Bit8u *frameBufferOffset =
		        frameBuffer.buffer +
		        (((size_t)frameBufferLineSize * (y + line)) + x);

		const std::vector<png::index_pixel> row = img.get_row(line + imgSrc.y);
		
		std::vector<png::index_pixel>::const_iterator iter = row.begin();
		std::advance(iter, imgSrc.x);
		std::vector<png::index_pixel>::const_iterator iterEnd = iter;
		int width = min(imgSrc.width, frameBuffer.width - x);
	
		std::advance(iterEnd, width);
		int xOffset = 0;
		for (iter; iter != iterEnd; ++iter) {
			if (*iter != 43) {
				memcpy(frameBufferOffset + xOffset,
				       (void *)(&*iter), 1);
			}
			++xOffset;
		}
	}
}

void GraphicsHelper::drawPartialImageDither(
											FrameBuffer &frameBuffer,
                                            const IndexedPNG &img,
                                            int x,
                                            int y,
                                            RECT imgSrc)
{
	const int lineBytes = img.get_width() * sizeof(Bit8u);
	const int frameBufferLineSize = frameBuffer.width * sizeof(Bit8u);

	for (size_t line = 0; line < imgSrc.height; ++line) {
		Bit8u *frameBufferOffset =
		        frameBuffer.buffer +
		        (((size_t)frameBufferLineSize * (y + line)) + x);

		const std::vector<png::index_pixel> row = img.get_row(line +
		                                                      imgSrc.y);

		std::vector<png::index_pixel>::const_iterator iter = row.begin();
		std::advance(iter, imgSrc.x);
		std::vector<png::index_pixel>::const_iterator iterEnd = iter;
		int width = min(imgSrc.width, frameBuffer.width - x);

		int step = (int)ceil(((line / (float)imgSrc.height)) * 8.0f);

		std::advance(iterEnd, width);
		int xOffset = 0;
		for (iter; iter != iterEnd; ++iter) {
			//if ((line % 2) == 0 && ((xOffset - 20) * (xOffset % 2) > 0)) {
			if (step > 0 && xOffset % step == 0) {
				// dither
			} else {
				memcpy(frameBufferOffset + xOffset,
				       (void *)(&*iter), 1);
			}
			++xOffset;
		}
	}
}

void GraphicsHelper::setPalette(const IndexedPNG &img)
{
	const auto v = img.get_palette();

	for (int i = 0; i < v.size(); ++i) {
		RENDER_SetPal(i, v[i].red, v[i].green, v[i].blue);
	}

	RENDER_SetPal(255, 0, 0, 0);
}