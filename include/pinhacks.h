/*
 * Copyleft:
 * This file was made to be used with DOSBOX, and so
 * is under GNU general public license
 */

#ifndef __DOSBOX_PINHACK_H
#define __DOSBOX_PINHACK_H
#define PINHACKVERSION 3 

extern struct scrollhack {
	bool enabled, trigger;
	struct {
		int min, max;
	} triggerwidth, triggerheight;
	struct {
		int height, width;
	} expand;
	struct {
		struct {
			bool enabled, trigger;
		} psychopinball, pinballdreams;
	} specifichack;
	const char *doublewidth;
	struct {
		bool on;
		const char *port;
	} dotmatrix;
	struct {
		bool on;
		const char *port;
	} buttonlights;
} pinhack;
  
#endif /* __DOSBOX_PINHACK_H */