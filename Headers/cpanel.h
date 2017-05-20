#ifndef CPANEL
#define CPANEL

#include <ccursor.h>
#include <ctext.h>
#include <panel.h>

class Panel : public Visual
{
public:
	enum SizePolicy
	{
		FIXED,
		RESIZE,
		RESIZE_AND_ADJUST
	};

	Panel();
	Panel(int Panel_ColCount, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, Text Panel_Popup, Str Object_Name);
	Panel(int Panel_ColCount, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, Text Panel_Popup, int Item_width, int Item_height, Str Object_Name);
	Panel(int Panel_ColCount, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, Text Panel_Popup, int Item_xpos, int Item_ypos, int Item_width, int Item_height, Str Object_Name);
	Panel(Panel &model);
	~Panel();
	void setSizePolicy(SizePolicy sizePolicy) {this->sizePolicy = sizePolicy;}
	void SetMove(bool Panel_move) {move = Panel_move;}
	void SetEdge();
	void AlignCol(int x);
	void AlignCols();
	void AlignColsOnRow(int y);
	void AlignRow(int y);
	void AlignRows();
	void AlignRowsOnCol(int x);
	void AlignColContent(int x, DATA_HALIGN halign=HALIGN_LEFT, DATA_VALIGN valign=VALIGN_TOP);
	void AlignRowContent(int y, DATA_HALIGN halign=HALIGN_LEFT, DATA_VALIGN valign=VALIGN_TOP);
	SizePolicy getSizePolicy() {return sizePolicy;}
	bool show();
	bool hide();
	virtual bool Refresh();
	void WaitForRefresh();
	void StartRefresh();
	virtual void Init();
	void InitBackground(int BmpId, Str SkinId, int width, int height, bool mosaic=false, int colorKey=-1);
	void InitBackground(Str BmpDirectory, Str BmpFileName, int width, int height, bool mosaic=false, int colorKey=-1);
	void initBorders(int BmpId, Str SkinId, int width, int height, bool mosaic=false);
	void initBorders(Str BmpDirectory, Str BmpFileName, int width, int height, bool mosaic=false);
	void initBorder(int index, int BmpId, Str SkinId, int width, int height, int colorKey, bool mosaic=false);
	void initBorder(int index, Str BmpDirectory, Str BmpFileName, int width, int height, int colorKey, bool mosaic=false);
	void initBorderCorners(int BmpId, Str SkinId, int width, int height, int colorKey);
	void initBorderCorners(Str BmpDirectory, Str BmpFileName, int width, int height, int colorKey);
	void InitCloseButton(int BmpId, Str SkinId, int width, int height);
	void InitCloseButton(Str BmpDirectory, Str BmpFileName, int width, int height);
	virtual void Initialize();
	void AddForbidden1(Item *pItem) {Forbidden1.AddElement(pItem);}
	virtual void AddForbidden2(Item *pItem) {Forbidden2.AddElement(pItem, true);}
	virtual void RemoveForbidden2(Item *pItem) {Forbidden2.RemoveElement(pItem);}
	void GenerateCursorDistance(Item *pItem);
	void restoreAni();
	void destroyAni();
	virtual void DestroyText();
	void StickToEdge();
	bool GenerateForbidden2();
	bool ClickOnGrid(int x, int y, bool waited, Cursor *cursor, bool press=true);
	virtual bool generate(bool waited=false, Cursor *cursor=NULL);
	virtual bool Run(Item *Cursor=NULL);
	bool showOnPanel(int frame, Item *cursor);
	virtual bool CanMove(Item *pItem);
	virtual bool CanClick(Item *pItem);
	int GetColCount() {return ColCount;}
	int GetRowCount() {return RowCount;}
	int GetColSpace() {return ColSpace;}
	int GetRowSpace() {return RowSpace;}
	int GetBorderSpace() {return BorderSpace;}
	bool GetSave() {return save;}
	bool GetMove() {return move;}
	Axe* GetCursorDistance() {return &CursorDistance;}
	CardinalPoint* GetEdge() {return &Edge;}
	Cell* GetGrid(int x, int y) {return &Grid[y][x];}
	Text* GetPopup() {return Popup;}
	Visual* GetCloseButton() {return CloseButton;}
	Visual* getBorder(int index) {return border[index];}
	Visual* getBorderCorner(int index) {return borderCorner[index];}
	Text* GetTextData(int x, int y) {return dynamic_cast<Text*>(Grid[y][x].Content);}
	Visual* GetVisualData(int x, int y) {return dynamic_cast<Visual*>(Grid[y][x].Content);}
	List* GetForbidden1() {return &Forbidden1;}
	List* GetForbidden2() {return &Forbidden2;}
	friend bool LoadPanels(List *pList);
	friend bool SavePanels(List *pList);
	friend void MovePanel(Panel *pPanel, Cursor *cursor, List PanelPack, bool MoveBlocked);
	friend void StopPanel(Panel *pPanel, Mover *CursorMovement);
	friend void StickToCursor(Item *Cursor, Mover *CursorMovement);
	friend DWORD WINAPI RefreshBySignal(LPVOID lParam);
	static Panel* getPanelElement(int index, List *pList) {return dynamic_cast<Panel*>(pList->GetElement(index));}
protected:
	int ColCount, RowCount, BorderSpace;
	Cell **Grid;
private:
	static const int TOTAL_BORDER = 4;
	int ColSpace, RowSpace;
	bool save, move;
	SizePolicy sizePolicy;
	Axe DataPosition, CursorDistance;
	CardinalPoint Edge;
	HANDLE Thread;
	Text *Popup;
	Visual *border[TOTAL_BORDER], *borderCorner[TOTAL_BORDER], *CloseButton;
	List Forbidden1, Forbidden2;
	void InitGrid(int Panel_ColCount, int Panel_RowCount);
	void GenerateShape();
};

#endif