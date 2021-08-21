
#include "pinball-dm.h"
#include <cassert>

PinballDM::PinballDM() 
{
}

PinballDM::~PinballDM()
{
}

#define IS_ON(xx, yy, cc) \
	(*(frameBuffer + (yy * 320) + xx + (cc * 16)) == 0xFF)

void PinballDM::updateData(Bit8u *frameBuffer)
{
	// save out file
	// write 1 line per frame
	// we need 20 lines, so 3 updates per sec

	 for (int ci = 0; ci < NUM_CHARACTERS; ++ci) {
		 PinballCharacter &ch = _characters[ci];

		 int y = 3;

		 ch.row1 = (IS_ON(6, y, ci) << 1) | (IS_ON(12, y, ci));
		 y += 2;
		 ch.row2 = (IS_ON(2, y, ci) << 4) | (IS_ON(4, y, ci) << 3) |
		           (IS_ON(8, y, ci) << 2) | (IS_ON(12, y, ci) << 1) |
		           (IS_ON(14, y, ci));

		 y += 7;
		 ch.row3 = (IS_ON(6, y, ci) << 1) | (IS_ON(12, y, ci));

		 y += 7;
		 ch.row4 = (IS_ON(2, y, ci) << 4) | (IS_ON(4, y, ci) << 3) |
		           (IS_ON(8, y, ci) << 2) | (IS_ON(12, y, ci) << 1) |
		           (IS_ON(14, y, ci));

		 y += 2;
		 ch.row5 = (IS_ON(6, y, ci) << 1) | (IS_ON(12, y, ci));
	 }
}

uint16_t PinballDM::bitMaskChar(const PinballCharacter &ch) const
{
	uint16_t val = 0;
	// row 1
	val |= ch.row1;
	val |= (ch.row2 << 2);
	val |= (ch.row3 << 7);
	val |= (ch.row4 << 9);
	val |= (ch.row5 << 14);

	return val;
}

uint16_t PinballDM::getCharacterForTransport(int characterIndex) const
{
	assert(characterIndex >= 0 && characterIndex < NUM_CHARACTERS);

	return bitMaskChar(_characters[characterIndex]);
}

void PinballDM::dumpData(const char* filename) const
{
	FILE *fp = fopen(filename, "wb");

	// row1
	for (int c = 0; c < NUM_CHARACTERS; ++c) {
		fprintf(fp, (_characters[c].row1 & 0x02) ? " - " : "   ");
		fprintf(fp, (_characters[c].row1 & 0x01) ? "-  " : "   ");
		fprintf(fp, " ");
	}
	fprintf(fp, "\n");
	// row2
	for (int c = 0; c < NUM_CHARACTERS; ++c) {
		fprintf(fp, (_characters[c].row2 & 0x10) ? "|" : " ");
		fprintf(fp, (_characters[c].row2 & 0x08) ? "\\" : " ");
		fprintf(fp, (_characters[c].row2 & 0x04) ? "|" : " ");
		fprintf(fp, (_characters[c].row2 & 0x02) ? "/" : " ");
		fprintf(fp, (_characters[c].row2 & 0x01) ? "|" : " ");
		fprintf(fp, "  ");
	}
	fprintf(fp, "\n");
	// row 3
	for (int c = 0; c < NUM_CHARACTERS; ++c) {
		fprintf(fp, (_characters[c].row3 & 0x02) ? " - " : "   ");
		fprintf(fp, (_characters[c].row3 & 0x01) ? "-  " : "   ");
		fprintf(fp, " ");
	}
	fprintf(fp, "\n");
	// row4
	for (int c = 0; c < NUM_CHARACTERS; ++c) {
		fprintf(fp, (_characters[c].row4 & 0x10) ? "|" : " ");
		fprintf(fp, (_characters[c].row4 & 0x08) ? "/" : " ");
		fprintf(fp, (_characters[c].row4 & 0x04) ? "|" : " ");
		fprintf(fp, (_characters[c].row4 & 0x02) ? "\\" : " ");
		fprintf(fp, (_characters[c].row4 & 0x01) ? "|" : " ");
		fprintf(fp, "  ");
	}
	fprintf(fp, "\n");
	// row 5
	for (int c = 0; c < NUM_CHARACTERS; ++c) {
		fprintf(fp, (_characters[c].row5 & 0x02) ? " - " : "   ");
		fprintf(fp, (_characters[c].row5 & 0x01) ? "-  " : "   ");
		fprintf(fp, " ");
	}
	fprintf(fp, "\n");

	fclose(fp);
}
