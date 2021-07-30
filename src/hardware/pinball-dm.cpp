
#include "pinball-dm.h"
#include <vcruntime_string.h>


PinballDM::PinballDM() 
{
}

#define IS_ON(xx, yy, cc) \
	(*(frameBuffer + (yy * 320) + xx + (cc * 16)) == 0xFF)

void PinballDM::updateData(Bit8u *frameBuffer)
{
	// save out file
	// write 1 line per frame
	// we need 20 lines, so 3 updates per sec
	 const int pixelInsetY = 3; //ignore top pixels since they're always black
	 
	 const int pixelsPerCharacter = 16; 

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

		 //memset(&ch, 0xFFFF, 4);
	 }
}

void PinballDM::dumpToConsole() const
{
	FILE *fp = fopen("c:\\dev\\tmp\\dmd-new1.txt", "wb");

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

void save()
{
	//if (frameCheck == 0) {
	//	fp = fopen(files[activeFile], "wb");
	//	activeFile = 1 - activeFile;
	//}
	//if (fp) {
	//	int y = frameCheck % 20;
	//	for (int x = 0; x < 320; ++x) {
	//		Bit8u *dmdStart = vga.draw.linear_base + (320 * 3);
	//		uint8_t val = *(dmdStart + (y * 320) + x);

	//		fprintf(val == 0xFF ? On : Off, sizeof(char), 1, fp);
	//	}
	//	fprintf(endl, sizeof(char), 1, fp);
	//}

	//++frameCheck;
	//if (frameCheck == 20) {
	//	frameCheck = 0;
	//	fclose(fp);
	//}

	//if (fp) {
	//	int y = frameCheck % 20;
	//	for (int x = 0; x < 320; ++x) {
	//		Bit8u *dmdStart = vga.draw.linear_base + (320 * 3);
	//		uint8_t val = *(dmdStart + (y * 320) + x);

	//		fprintf(val == 0xFF ? On : Off, sizeof(char), 1, fp);
	//	}
	//	fprintf(endl, sizeof(char), 1, fp);
	//}

	//++frameCheck;
	//if (frameCheck == 20) {
	//	frameCheck = 0;
	//	fclose(fp);
	//}
}