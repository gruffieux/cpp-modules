#ifndef CITEMMAP
#define CITEMMAP

#include <cvisual.h>

class ItemMap : public Visual
{
private:
	int type;
	Axe WorldPosition, ZoneID, Coord;
	Axe2 PositionOnMap;
public:
	ItemMap() {type = 0;}
	ItemMap(int ItemMap_type, int Item_width, int Item_height);
	ItemMap(ItemMap &model);
	~ItemMap() {}
	void SetType(int ItemMap_type) {type = ItemMap_type;}
	int GetType() {return type;}
	Axe * GetWorldPosition() {return &WorldPosition;}
	Axe * GetZoneID() {return &ZoneID;}
	Axe * GetCoord() {return &Coord;}
	Axe2 * GetPositionOnMap() {return &PositionOnMap;}
};

#endif