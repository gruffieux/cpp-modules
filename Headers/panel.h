enum DATA_TYPE
{
	TEXT,
	VISUAL,
	VISUAL_INHERIT
};

enum DATA_HALIGN
{
	HALIGN_LEFT,
	HALIGN_RIGHT,
	HALIGN_CENTER
};

enum DATA_VALIGN
{
	VALIGN_TOP,
	VALIGN_BOTTOM,
	VALIGN_CENTER
};

struct Cell
{
	int posx, posy, width, height, PicFrame, ColSpan, RowSpan;
	bool display, mosaic, popup, AlignCol, AlignRow;
	DATA_TYPE data;
	DATA_HALIGN halign;
	DATA_VALIGN valign;
	Str PopupText;
	Object *Content;
	
	Cell()
	{
		posx = posy = width = height = PicFrame = ColSpan = RowSpan = 0;
		display = true;
		mosaic = popup = AlignCol = AlignRow = false;
		data = TEXT;
		halign = HALIGN_LEFT;
		valign = VALIGN_TOP;
		Content = NULL;
	}
};
