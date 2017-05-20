#include <cworldmap.h>

WorldMap::WorldMap(int WM_WorldWidth, int WM_WorldHeight, int WM_MapWidth, int WM_MapHeight, int Item_posx, int Item_posy, int Item_width, int Item_height) :
Item(Item_posx, Item_posy, Item_width, Item_height, "Carte"), List(ORDER_BY_WMTYPE)
{
	WorldSize.x = WM_WorldWidth;
	WorldSize.y = WM_WorldHeight;
	MapSize.x = WM_MapWidth;
	MapSize.y = WM_MapHeight;
}

WorldMap::WorldMap(WorldMap &model) : List(model)
{
	WorldSize = *model.GetWorldSize();
	MapSize = *model.GetMapSize();
	PicsMap = model.GetPicsMap();
}

void WorldMap::AddPicMap(int BmpId)
{
	if (PicsMap.GetLength()) PicsMap.Cat(",");
	PicsMap.Cat(BmpId);
}

bool WorldMap::AddItemMap(ItemMap *pItemMap)
{
	int i;
	Str PicMap;

	if (!pItemMap) return false;

	// On ajoute l'élément en copiant l'animation pour ce type
	for (i = 0; i < ElementCount; i++)
		if (GetItemMapElement(i)->GetType() == pItemMap->GetType())
		{
			if (!AddElement(pItemMap)) return false;
			pItemMap->cloneAni(GetItemMapElement(i));
			return true;
		}

	// C'est le premier élément de ce type on l'ajoute avec son animation
	if (i == ElementCount)
	{
		if (!AddElement(pItemMap)) return false;
		PicMap = ExtractString(PicsMap, ",", pItemMap->GetType());
		if (!PicMap.GetLength()) return false;
		pItemMap->addAni(PicMap.Get(10), "", 1, pItemMap->getDimension()->x, pItemMap->getDimension()->y, -1, 0);
		return true;
	}

	return false;
}

bool WorldMap::SetItemOnMap(ItemMap *pItemMap, int WorldPosX, int WorldPosY, int CoordX, int CoordY, int ZoneX, int ZoneY, bool center)
{
	if (!pItemMap) return false;

	pItemMap->GetWorldPosition()->x = WorldPosX;
	pItemMap->GetWorldPosition()->y = WorldPosY;
	pItemMap->GetCoord()->x = CoordX;
	pItemMap->GetCoord()->y = CoordY;
	pItemMap->GetZoneID()->x = ZoneX;
	pItemMap->GetZoneID()->y = ZoneY;

	pItemMap->GetPositionOnMap()->x = WorldPosX * MapSize.x / WorldSize.x;
	pItemMap->GetPositionOnMap()->y = WorldPosY * MapSize.y / WorldSize.y;

	pItemMap->getPosition()->x = ZoneX * MapSize.x + (int)pItemMap->GetPositionOnMap()->x + position.x;
	pItemMap->getPosition()->y = ZoneY * MapSize.y + (int)pItemMap->GetPositionOnMap()->y + position.y;

	if (center)
	{
		pItemMap->getPosition()->x -= pItemMap->getDimension()->x / 2;
		pItemMap->getPosition()->y -= pItemMap->getDimension()->y / 2;
	}

	return true;
}

bool WorldMap::MoveItemOnMap(ItemMap *pItemMap, int WorldPosX, int WorldPosY, int CameraPosX, int CameraPosY, int ZoneX, int ZoneY, bool center)
{
	if (!pItemMap) return false;

	pItemMap->GetWorldPosition()->x = WorldPosX;
	pItemMap->GetWorldPosition()->y = WorldPosY;
	pItemMap->GetZoneID()->x = ZoneX;
	pItemMap->GetZoneID()->y = ZoneY;

	pItemMap->GetPositionOnMap()->x = (WorldPosX + CameraPosX) * MapSize.x / WorldSize.x;
	pItemMap->GetPositionOnMap()->y = (WorldPosY + CameraPosY) * MapSize.y / WorldSize.y;

	pItemMap->getPosition()->x = ZoneX * MapSize.x + (int)pItemMap->GetPositionOnMap()->x + position.x;
	pItemMap->getPosition()->y = ZoneY * MapSize.y + (int)pItemMap->GetPositionOnMap()->y + position.y;

	if (center)
	{
		pItemMap->getPosition()->x -= pItemMap->getDimension()->x / 2;
		pItemMap->getPosition()->y -= pItemMap->getDimension()->y / 2;
	}

	return true;
}

void WorldMap::ShowMap()
{
	int i;

	runAnimations();

	for (i = 0; i < ElementCount; i++)
		GetItemMapElement(i)->runAnimations();
}

int WorldMap::OrderBy(int NewOrder, Container *pContainer)
{
	int i, result;

	result = List::OrderBy(NewOrder, pContainer);

	if (result == ORDER_UNKNOWN)
	{
		switch (NewOrder)
		{
		case ORDER_BY_WMTYPE:
			for (i = 0; i < ElementCount; i++)
				pList[i].Content->getSortData()->num = GetItemMapElement(i)->GetType();
			result = OrderElements(ORDER_NUM);
			if (!result) return result;
			break;
		default:
			return ORDER_UNKNOWN;
		}
	}
	else
		return result;

	FinalizeOrder(NewOrder);

	return result;
}

bool WorldMap::RemoveAllElement(int type)
{
	int i, StartIndex, total;

	StartIndex = total = 0;

	// On assume que la liste est triée par type
	for (i = 0; i < ElementCount; i++)
		if (GetItemMapElement(i)->GetType() == type)
		{
			if (!StartIndex) StartIndex = i;
			total++;
		}

	if (total) return RemoveElement(StartIndex, total, true);

	return true;
}

void WorldMap::restoreAni()
{
	int i;

	Visual::restoreAni();
	for (i = 0; i < ElementCount; i++) GetItemMapElement(i)->restoreAni();
}

void WorldMap::destroyAni()
{
	int i;

	Visual::destroyAni();
	for (i = 0; i < ElementCount; i++) GetItemMapElement(i)->destroyAni();
}
