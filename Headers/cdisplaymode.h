#ifndef CDISPLAYMODE
#define CDISPLAYMODE

#include <clist.h>

class DisplayMode : public Object
{
private:
	int width, height, bit;
	short windowed;
public:
	DisplayMode();
	DisplayMode(int DM_width, int DM_height, int DM_bit, short DM_windowed=1);
	DisplayMode(Str Object_Name);
	void SetWidth(int DM_width) {width = DM_width;}
	void SetHeight(int DM_height) {height = DM_height;}
	void SetBit(int DM_bit) {bit = DM_bit;}
	void SetWindowed(short DM_windowed) {windowed = DM_windowed;}
	void GenerateName() {name = BuildString("%d x %d x %d", width, height, bit);}
	void Switch(int NewWidth=-1, int NewHeight=-1, int NewBit=-1);
	bool IsUndefined() {return width == 0 && height == 0;}
	bool IsInvalid() {return ((width <= 0) | (height <= 0));}
	int GetWidth() {return width;}
	int GetHeight() {return height;}
	int GetBit() {return bit;}
	short GetWindowed() {return windowed;}
	static DisplayMode * getDisplayModeElement(int index, List *pList) {return dynamic_cast<DisplayMode*>(pList->GetElement(index));}
};

#endif
