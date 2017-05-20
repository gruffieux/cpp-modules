#include <cdisplaymode.h>

DisplayMode::DisplayMode()
{
	width = height = bit = 0;
	windowed = 0;
}

DisplayMode::DisplayMode(int DM_width, int DM_height, int DM_bit, short DM_windowed)
{
	width = DM_width;
	height = DM_height;
	bit = DM_bit;
	windowed = DM_windowed;
	GenerateName();
}

DisplayMode::DisplayMode(Str Object_Name) : Object(Object_Name)
{
	Str Token;

	Object_Name.Trim();

	Token = ExtractString(Object_Name, "x", 0);
	if (Token.GetLength())
		width = Token.Get(10);

	Token = ExtractString(Object_Name, "x", 1);
	if (Token.GetLength())
		height = Token.Get(10);

	Token = ExtractString(Object_Name, "x", 2);
	if (Token.GetLength())
		bit = Token.Get(10);
}

void DisplayMode::Switch(int NewWidth, int NewHeight, int NewBit)
{
	if (NewWidth != -1) width = NewWidth;
	if (NewHeight != -1) height = NewHeight;
	if (NewBit != -1) bit = NewBit;
	windowed = !windowed;
	GenerateName();
}
