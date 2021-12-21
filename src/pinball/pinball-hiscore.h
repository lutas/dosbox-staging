#ifndef DOSBOX_PINBALL_HISCORE_H
#define DOSBOX_PINBALL_HISCORE_H

static const char *hex_lookup = "0123456789ABCDEF";
const char tableNames[][4] = {"IGN", "STW", "BBX", "NTM",
                              "NPT", "SFR", "RRW", "STT"};

class PinballHiscore {
public:
	PinballHiscore();
	virtual ~PinballHiscore();

	struct Entry {
		char name[4];
		long score;
	};

	struct Table {
		char name[4];
		Entry scores[4];
	};

	void LoadHiscoreFiles(const char *file1, const char *file2);

	inline Table getTable(int table) { return _tables[table]; }

private:
	void LoadHiscoreFile(const char *filename, int offset);

	static Entry LoadEntryData(const char *data);

	Table _tables[8];
};

#endif