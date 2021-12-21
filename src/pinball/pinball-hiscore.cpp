#include "pinball-hiscore.h"

#include <fstream>
#include <iostream>

PinballHiscore::PinballHiscore() 
{}

PinballHiscore::~PinballHiscore()
{}


PinballHiscore::Entry PinballHiscore::LoadEntryData(const char *data)
{
	Entry entry;

	strcpy(entry.name, data);
	entry.name[3] = '\0';

	// take hex from each value and add to score
	char values[] = "000000000000";
	for (int i = 0; i < 6; ++i) {
		char val = *(data + 3 + i);

		values[i * 2] = hex_lookup[(val >> 4) % 16];
		values[i * 2 + 1] = hex_lookup[(val & 0x0F) % 16];
	}

	entry.score = atoi(values);

	return entry;
}

void PinballHiscore::LoadHiscoreFile(const char *filename, int offset)
{
	std::ifstream file(filename);

	const int bytesPerEntry = 9;
	for (int tableIndex = 0; tableIndex < 4; ++tableIndex) {
		for (int entryIndex = 0; entryIndex < 4; ++entryIndex) {
			char entryBuffer[bytesPerEntry + 1] = {};
			file.read(entryBuffer, bytesPerEntry);

			_tables[tableIndex + offset].scores[entryIndex] = LoadEntryData(
			        entryBuffer);
		}
	}
}

void PinballHiscore::LoadHiscoreFiles(const char *file1, const char *file2)
{
	LoadHiscoreFile(file1, 0);
	LoadHiscoreFile(file2, 4);
}
