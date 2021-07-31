#ifndef DOSBOX_PINBALL_SERIAL_H
#define DOSBOX_PINBALL_SERIAL_H

#include "../hardware/serialport/libserial.h"
#include "pinball-dm.h"

class PinballSerial {
public:
	PinballSerial();
	virtual ~PinballSerial();

	bool connect(const char* port);
	bool isConnected() const;

	void sendDMBuffer(const PinballDM &dm);

private:
	void disconnect();

	COMPORT _comport;
	bool _connected;
};

#endif