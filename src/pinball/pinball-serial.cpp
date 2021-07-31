#include "pinball-serial.h"

PinballSerial::PinballSerial() :
	_comport(NULL),
	_connected(false)
{}

PinballSerial::~PinballSerial()
{
	disconnect();
}

bool PinballSerial::connect(const char *port)
{
	disconnect();

	if (!SERIAL_open(port, &_comport)) {
		char errorbuffer[256];
		SERIAL_getErrorString(errorbuffer, sizeof(errorbuffer));

		return false;
	}
	if (!SERIAL_setCommParameters(_comport, 9600, 0, 1, 8)) {
		return false;
	}
	_connected = true;
	return true;
}

void PinballSerial::disconnect()
{
	if (_comport) {
		SERIAL_close(_comport);
		_comport = NULL;
		_connected = false;
	}
}

bool PinballSerial::isConnected() const
{
	return _connected;
}

void PinballSerial::sendDMBuffer(const PinballDM &dm)
{
	if (!_connected) {
		return;
	}
	const int numChars = dm.getNumCharacters();

	for (int i = 0; i < numChars; ++i) {
		uint16_t val = dm.getCharacterForTransport(i);
		SERIAL_senddata(_comport, (void*)val, sizeof(val));
	}
}