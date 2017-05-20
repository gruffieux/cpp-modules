#ifndef CWORLDMAP
#define CWORLDMAP

#include <citemmap.h>

class WorldMap : public Visual, public List
{
private:
	Axe WorldSize, MapSize;
	Str PicsMap;
public:
	static const int ORDER_BY_WMTYPE = 10;
	WorldMap() : List(ORDER_BY_WMTYPE) {}
	WorldMap(int WM_WorldWidth, int WM_WorldHeight, int WM_MapWidth, int WM_MapHeight, int Item_posx, int Item_posy, int Item_width, int Item_height);
	WorldMap(WorldMap &model);
	~WorldMap() {List::RemoveAllElement(0, true);}
	void AddPicMap(int BmpId);
	void AddPicMap(Str FileName);
	bool AddItemMap(ItemMap *pItemMap);
	bool SetItemOnMap(ItemMap *pItemMap, int WorldPosX, int WorldPosY, int CoordX, int CoordY, int ZoneX, int ZoneY, bool center);
	bool MoveItemOnMap(ItemMap *pItemMap, int WorldPosX, int WorldPosY, int CameraPosX, int CameraPosY, int ZoneX, int ZoneY, bool center);
	void ShowMap();
	int OrderBy(int NewOrder, Container *pContainer=NULL);
	bool RemoveAllElement(int type);
	void restoreAni();
	void destroyAni();
	Axe * GetWorldSize() {return &WorldSize;}
	Axe * GetMapSize() {return &MapSize;}
	Str GetPicsMap() {return PicsMap;}
	ItemMap * GetItemMapElement(int index) {return dynamic_cast<ItemMap*>(pList[index].Content);}
};

#endif