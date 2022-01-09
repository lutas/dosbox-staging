#include "pinball-serial.h"

namespace {

	extern uint8_t SerialFlag = 0x08;
	extern uint8_t LevelDetailsMode = 0x01;
    extern uint8_t DotMatrixBufferMode = 0x02;
} // namespace

PinballSerial::PinballSerial()
        :
	_comport(NULL),
	_connected(false)
{}

PinballSerial::~PinballSerial()
{
	disconnect();
}

bool PinballSerial::connect(const char *port)
{
	if (_connected) 
		disconnect();

	if (!SERIAL_open(port, &_comport)) {
		char errorbuffer[256];
		SERIAL_getErrorString(errorbuffer, sizeof(errorbuffer));

		return false;
	}
	if (!SERIAL_setCommParameters(_comport, 115200, 'n', 1, 8)) {
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
	}
	_connected = false;
}

bool PinballSerial::isConnected() const
{
	return _connected;
}

void PinballSerial::sendLevelData(uint8_t activeTable, uint8_t gameState, long score)
{
	if (!_connected) {
		return;
	}

	SERIAL_senddata(_comport, (void *)&SerialFlag, sizeof(uint8_t));
	SERIAL_senddata(_comport, (void *)&SerialFlag, sizeof(uint8_t));
	SERIAL_senddata(_comport, (void *)&SerialFlag, sizeof(uint8_t));
	SERIAL_senddata(_comport, (void *)&SerialFlag, sizeof(uint8_t));

	SERIAL_senddata(_comport, (void *)&LevelDetailsMode, sizeof(uint8_t));

	SERIAL_senddata(_comport, (void *)&activeTable, sizeof(uint8_t));
	SERIAL_senddata(_comport, (void *)&gameState, sizeof(uint8_t));
	//SERIAL_senddata(_comport, (void *)&score, sizeof(long));


}

void PinballSerial::sendDMBuffer(const PinballDM &dm)
{
	if (!_connected) {
		return;
	}
	const int numChars = dm.getNumCharacters();

	SERIAL_senddata(_comport, (void *)&SerialFlag, sizeof(uint8_t));
	SERIAL_senddata(_comport, (void *)&SerialFlag, sizeof(uint8_t));
	SERIAL_senddata(_comport, (void *)&SerialFlag, sizeof(uint8_t));
	SERIAL_senddata(_comport, (void *)&SerialFlag, sizeof(uint8_t));

	SERIAL_senddata(_comport, (void*)&DotMatrixBufferMode, sizeof(uint8_t));

	for (int i = 0; i < numChars; ++i) {
		uint16_t val = dm.getCharacterForTransport(i);
		SERIAL_senddata(_comport, (void*)&val, sizeof(val));
	}
}

void PinballSerial::setLight(uint16_t light, bool on)
{
	if (!_connected) {
		return;
	}

	if (!on) {
		light += 6;
	}

	SERIAL_senddata(_comport, (void *)&light, sizeof(light));

}