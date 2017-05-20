#ifndef CCONFIGFILE
#define CCONFIGFILE

#include <cdatafilelist.h>
#include <cdisplaymode.h>

class ConfigFile : public DataFileList
{
private:
	int KeyTotal;
	Str ModeFilters;
	List KeyMap;
public:
	ConfigFile();
	~ConfigFile();
	void SetModeFilters(Str CF_ModeFilters) {ModeFilters = CF_ModeFilters;}
	bool Open(bool ReadOnly, bool CreateIfNotExist, bool erase);
	bool Close();
	bool WriteGraphic(DisplayMode *pDisplayMode);
	bool WriteKeys();
	bool WriteSound(BOOL PlayMusic, BOOL PlaySounds);
	bool WriteModeFilters();
	void ReadGraphic(DisplayMode *pDisplayMode);
	void ReadKeys();
	void ReadModeFilters();
	bool PlayMusic() {return Read("MUSIC_ON") == 1;}
	bool PlaySounds() {return Read("SOUNDS_ON") == 1;}
	bool IsModeValid(int width, int height);
	Str GetModeFilters() {return ModeFilters;}
	List * GetKeyMap() {return &KeyMap;}
};

#endif
