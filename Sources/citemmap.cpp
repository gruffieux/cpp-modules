#include <citemmap.h>

ItemMap::ItemMap(int ItemMap_type, int Item_width, int Item_height)
{
	type = ItemMap_type;
	dimension = Axe(Item_width, Item_height);
}

ItemMap::ItemMap(ItemMap &model) : Visual(model)
{
	type = model.GetType();
	WorldPosition = *model.GetWorldPosition();
	Coord = *model.GetCoord();
	ZoneID = *model.GetZoneID();
	PositionOnMap = *model.GetPositionOnMap();
	dimension = *model.getDimension();
}
