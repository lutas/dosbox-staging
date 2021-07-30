#ifndef DOSBOX_PINBALL_DM_H
#define DOSBOX_PINBALL_DM_H

#include <cstdio>
#include <types.h>

//const char *files[] = {"c:\\dev\\tmp\\dmd1.txt", "c:\\dev\\tmp\\dmd2.txt"};
/*
* This reads the VGA framebuffer for Pinball Dreams 
* to get the top dotmatrix screen data so that it can be 
* externalised
* 
* need to get 1 pixel for each of the segments
* 16x segments per character
* 20x characters in total
                 - -
                |\|/|
                 - -
                |/|\|
                 - -
 */

struct PinballCharacter {
	uint8_t row2 : 5;
	uint8_t row4 : 5;
	uint8_t row1 : 2;
	uint8_t row3 : 2;
	uint8_t row5 : 2;
}; //2 bytes

#define NUM_CHARACTERS 20

class PinballDM {
public:

	PinballDM();

	void updateData(Bit8u* frameBuffer);

	void save(const char *szFilename);
	void dumpToConsole() const;

private:
	PinballCharacter _characters[NUM_CHARACTERS];
};


#endif