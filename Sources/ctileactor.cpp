#include <ctileactor.h>

TileActor::TileActor()
{
	int i;

	TileNodeID = -1;
	TileID = LivingTileCount = 0;
	for (i = 0; i < MAX_LIVING_TILE_ID; i++)
		LivingTileID[i] = 0;
	TileCoord = Axe(-1, -1);
	Map = NULL;
}

TileActor::TileActor(TileActor &model) : Item(model)
{
	int i;

	TileNodeID = model.GetTileNodeID();
	TileID = model.GetTileID();
	for (i = 0; i < MAX_LIVING_TILE_ID; i++)
		LivingTileID[i] = model.GetLivingTileID(i);
	LivingTileCount = model.GetLivingTileCount();
	MapPosition = *model.GetMapPosition();
	TileCoord = *model.GetTileCoord();
	Map = model.GetMap();
}

void TileActor::SetMapPosition()
{
	MapPosition.x = position.x + Map->GetScreenData()->Camera.getPosition()->x;
	MapPosition.y = position.y + Map->GetScreenData()->Camera.getPosition()->y;
}

void TileActor::SetMapPosition(int x, int y)
{
	MapPosition.x = Map->GetMapData()->TileZone[x].west;
	MapPosition.y = Map->GetMapData()->TileZone[y].north;
}

void TileActor::SetMapPositionByCoord(int x, int y, bool center)
{
	TileNodeID = Map->GetMapData()->TileNodeID[y][x];
	TileCoord.x = x;
	TileCoord.y = y;
	SetMapPosition(x, y);

	if (center)
		CenterMapPosition();
	else
		GenerateMapPosition();
}

void TileActor::CenterMapPosition()
{
	MapPosition.x += (Map->GetTileData()->getDimension()->x / 2) - (dimension.x / 2);
	MapPosition.y += (Map->GetTileData()->getDimension()->y / 2) - (dimension.y / 2);
}

void TileActor::GenerateMapPosition()
{
	position.x = MapPosition.x - Map->GetScreenData()->Camera.getPosition()->x;
	position.y = MapPosition.y - Map->GetScreenData()->Camera.getPosition()->y;
}

void TileActor::GenerateTileCoord(int x, int y)
{
	// Attention ce type de détection ne marche pas bien lorsque il y a mouvement avec plusieurs directions!
	if (collision(Map->GetMapData()->TileZone[x].west - Map->GetScreenData()->Camera.getPosition()->x,
		Map->GetMapData()->TileZone[x].east - Map->GetScreenData()->Camera.getPosition()->x,
		Map->GetMapData()->TileZone[y].north - Map->GetScreenData()->Camera.getPosition()->y,
		Map->GetMapData()->TileZone[y].south - Map->GetScreenData()->Camera.getPosition()->y, 2))
		TileCoord = Axe(x, y);
}

Axe TileActor::GenerateZonePosition(int ZoneTile[], int ZoneTileCount, Axe UsedPosition[], int DispatchCount, int TotalTile, int layer, bool SpecifiedZone, bool ForbiddenObstacle1, bool ForbiddenObstacle2)
{
	int i, j, x, y, TileID, RandomTileID, RandomLivingPos, *LivingPosCount;
	Axe **LivingPosition, ZonePosition;

	LivingPosCount = new int[ZoneTileCount];
	LivingPosition = new Axe*[ZoneTileCount];
	for (i = 0; i < ZoneTileCount; i++)
		LivingPosition[i] = new Axe[TotalTile];

	RandomTileID = RandomLivingPos = 0;
	ZonePosition.x = ZonePosition.y = -1;

	//Définit les positions possibles de l'acteur dans cette zone
	if (ZoneTileCount)
		for (i = 0; i < ZoneTileCount; i++)
		{
			LivingPosCount[i] = 0;
			for (x = 0; x < Map->GetMapData()->TileCount.x; x++)
				for (y = 0; y < Map->GetMapData()->TileCount.y; y++)
				{
					if (SpecifiedZone)
						TileID = Map->GetMapData()->TileNodeID[y][x];
					else
						TileID = Map->GetMapData()->TileID[y][x][layer];
					if ((TileID == ZoneTile[i]) && (ZoneTile[i] > 0))
						if ((ForbiddenObstacle1 == false) | (Map->GetMapData()->obstacle[y][x] != 1))
							if ((ForbiddenObstacle2 == false) | (Map->GetMapData()->obstacle[y][x] != 2))
							{
								for (j = 0; j < DispatchCount; j++)
									if ((x == UsedPosition[j].x) && (y == UsedPosition[j].y))
										break;
								if (j == DispatchCount)
								{
									LivingPosition[i][LivingPosCount[i]].x = x;
									LivingPosition[i][LivingPosCount[i]].y = y;
									LivingPosCount[i]++;
								}
							}
				}
		}

	if (SpecifiedZone)
	{
		if (ZoneTileCount)
			RandomTileID = ZoneTile[Clock::random(0, ZoneTileCount - 1)];
	}
	else
		if (LivingTileCount)
			RandomTileID = LivingTileID[Clock::random(0, LivingTileCount - 1)];

	for (i = 0; i < ZoneTileCount; i++)
	{
		if ((ZoneTile[i] == RandomTileID) && (LivingPosCount[i] > 0))
		{
			RandomLivingPos = Clock::random(0, LivingPosCount[i] - 1);
			ZonePosition.x = LivingPosition[i][RandomLivingPos].x;
			ZonePosition.y = LivingPosition[i][RandomLivingPos].y;
			break;
		}
	}

	for (i = 0; i < ZoneTileCount; i++)
		delete [] LivingPosition[i];
	delete [] LivingPosition;
	delete [] LivingPosCount;

	return ZonePosition;
}

void TileActor::GenerateTileNodeID(int x, int y)
{
	CardinalPoint TileEdge;

	TileEdge.west = Map->GetMapData()->TileZone[x].west + 1 - (int)Map->GetScreenData()->Camera.getPosition()->x;
	TileEdge.east = Map->GetMapData()->TileZone[x].east - 1 - (int)Map->GetScreenData()->Camera.getPosition()->x;
	TileEdge.north = Map->GetMapData()->TileZone[y].north + 1 - (int)Map->GetScreenData()->Camera.getPosition()->y;
	TileEdge.south = Map->GetMapData()->TileZone[y].south - 1 - (int)Map->GetScreenData()->Camera.getPosition()->y;
	
	if (collision(TileEdge.west, TileEdge.east, TileEdge.north, TileEdge.south))
		TileNodeID = Map->GetMapData()->TileNodeID[y][x];
}

void TileActor::AddLivingTile(int TileID)
{
	LivingTileID[LivingTileCount] = TileID;

	LivingTileCount++;
}

int TileActor::CheckLivingTile(int TileCount, int TileID[])
{
	int i, j;

	if (LivingTileCount)
	{
		for (i = 0; i < LivingTileCount; i++)
			for (j = 0; j < TileCount; j++)
				if ((LivingTileID[i] > 0) && (LivingTileID[i] == TileID[j]))
					return 1; //L'acteur doit figurer uniquement sur un ou plusieurs terrains (TileID[])
	}
	else
		return 2; //L'acteur vit n'importe où

	return 0; //L'acteur ne peut pas figurer sur ce terrain
}

bool TileActor::CheckLivingTile(int TileID)
{
	int i;

	if (LivingTileCount)
		for (i = 0; i < LivingTileCount; i++)
			if ((LivingTileID[i] > 0) && (LivingTileID[i] == TileID))
				return true; //L'acteur peut figurer sur ce terrain

	return false; //L'acteur ne peut pas figurer sur ce terrain
}

void TileActor::GenerateAdjNode()
{
	AdjNode[0].id = TileNodeID - 1;							//Node haut
	AdjNode[1].id = TileNodeID - 1 - PATHNODE_ID_INTERVAL;	//Node haut-gauche
	AdjNode[2].id = TileNodeID - PATHNODE_ID_INTERVAL;		//Node gauche
	AdjNode[3].id = TileNodeID - PATHNODE_ID_INTERVAL + 1;	//Node gauche-bas
	AdjNode[4].id = TileNodeID + 1;							//Node bas
	AdjNode[5].id = TileNodeID + 1 + PATHNODE_ID_INTERVAL;	//Node bas-droite
	AdjNode[6].id = TileNodeID + PATHNODE_ID_INTERVAL;		//Node droite
	AdjNode[7].id = TileNodeID + PATHNODE_ID_INTERVAL - 1;	//Node droite-haut
}

void TileActor::CheckAdjNode(int x, int y)
{
	int i, n;

	// On vérifie si les nodes sont des obstalces sur la carte
	for (i = 0; i < 8; i++)
		if (AdjNode[i].id == Map->GetMapData()->TileNodeID[y][x])
			if (Map->GetMapData()->obstacle[y][x] == 1)
			{
				AdjNode[i].obstacle = true;
				if (AdjNode[i].ActorID != -1) AdjNode[i].ActorID = -1;
			}
			else
				AdjNode[i].obstacle = false;

	n = 0;

	// On vérifie si les nodes sont hors de l'écran
	if (TileNodeID == Map->GetMapData()->TileNodeID[y][x])
	{
		if (x == 0)
			AdjNode[1].forbidden = AdjNode[2].forbidden = AdjNode[3].forbidden = true;
		else
			n++;
		if (x == 79)
			AdjNode[5].forbidden = AdjNode[6].forbidden = AdjNode[7].forbidden = true;
		else
			n++;
		if (y == 0)
			AdjNode[0].forbidden = AdjNode[1].forbidden = AdjNode[7].forbidden = true;
		else
			n++;
		if (y == 79)
			AdjNode[3].forbidden = AdjNode[4].forbidden = AdjNode[5].forbidden = true;
		else
			n++;
		if (n >= 4)
		{
			for (i = 0; i < 8; i++)
				AdjNode[i].forbidden = false;
		}
		else
			for (i = 0; i < 8; i++)
				if (AdjNode[i].forbidden)
					if (AdjNode[i].ActorID != -1) AdjNode[i].ActorID = -1;
	}
}

bool TileActor::FillAdjNode(ActorNode *pActorNode, int AdjNode_ActorID)
{
	int i;

	// On fait cibler l'acteur sur la node
	for (i = 0; i < 8; i++)
		if (AdjNode[i].id == pActorNode->id)
		{
			AdjNode[i].ActorID = AdjNode_ActorID;
			return true;
		}

	return false;
}

void TileActor::FreeAdjNode(int AdjNode_ActorID)
{
	int i;

	// On libère la node occupée par l'acteur
	for (i = 0; i < 8; i++)
		if (AdjNode[i].ActorID == AdjNode_ActorID)
			AdjNode[i].ActorID = -1;
}

void TileActor::FreeAllAdjNodes()
{
	int i;

	for (i = 0; i < 8; i++)
		AdjNode[i].ActorID = -1;
}

void TileActor::FocusWithCamera(int AdjustPosX, int AdjustPosY)
{
	*Map->GetScreenData()->Camera.getPosition() = Axe(position.x - AdjustPosX, position.y - AdjustPosY);

	if (Map->GetScreenData()->Camera.getPosition()->x < Map->GetScreenData()->MaxCamera.west)
		Map->GetScreenData()->Camera.getPosition()->x = Map->GetScreenData()->MaxCamera.west;
	if (Map->GetScreenData()->Camera.getPosition()->x > Map->GetScreenData()->MaxCamera.east)
		Map->GetScreenData()->Camera.getPosition()->x = Map->GetScreenData()->MaxCamera.east;
	if (Map->GetScreenData()->Camera.getPosition()->y < Map->GetScreenData()->MaxCamera.north)
		Map->GetScreenData()->Camera.getPosition()->y = Map->GetScreenData()->MaxCamera.north;
	if (Map->GetScreenData()->Camera.getPosition()->y > Map->GetScreenData()->MaxCamera.south)
		Map->GetScreenData()->Camera.getPosition()->y = Map->GetScreenData()->MaxCamera.south;

	GenerateMapPosition();
}

bool TileActor::IsInZone(Axe Zone)
{
	int x, y;

	x = TileCoord.x;
	y = TileCoord.y;

	if ((x >= Zone.x) && (x < Zone.x + Map->GetScreenData()->VisibleTileCount.x + 1) && (y >= Zone.y) && (y < Zone.y + Map->GetScreenData()->VisibleTileCount.y + 1))
		if (TileNodeID == Map->GetMapData()->TileNodeID[y][x])
			return true;

	return false;
}

int TileActor::GetRange(int NodeID)
{
	Start.SetNodeID(TileNodeID);
	Start.GenerateHScore(NodeID, PATHNODE_ID_INTERVAL);
	return Start.GetHScore();
}

bool TileActor::GetAdjacent(TileActor *pTileActor)
{
	if (TileNodeID == pTileActor->GetTileNodeID()) return true;
	if (TileNodeID == pTileActor->GetTileNodeID() + 1) return true;
	if (TileNodeID == pTileActor->GetTileNodeID() - 1) return true;
	if (TileNodeID == pTileActor->GetTileNodeID() + PATHNODE_ID_INTERVAL) return true;
	if (TileNodeID == pTileActor->GetTileNodeID() - PATHNODE_ID_INTERVAL) return true;
	if (TileNodeID == pTileActor->GetTileNodeID() + (PATHNODE_ID_INTERVAL + 1)) return true;
	if (TileNodeID == pTileActor->GetTileNodeID() - (PATHNODE_ID_INTERVAL + 1)) return true;
	if (TileNodeID == pTileActor->GetTileNodeID() + (PATHNODE_ID_INTERVAL - 1)) return true;
	if (TileNodeID == pTileActor->GetTileNodeID() - (PATHNODE_ID_INTERVAL - 1)) return true;

	return false;
}

ActorNode * TileActor::GetFreeAdjNodeID()
{
	int i;

	// On cherche une node libre
	for (i = 0; i < 8; i++)
		if ((AdjNode[i].ActorID == -1) && (AdjNode[i].obstacle == false) && (AdjNode[i].forbidden == false))
			return &AdjNode[i];

	return NULL;
}

ActorNode * TileActor::GetFilledAdjNodeID(int AdjNode_ActorID)
{
	int i;

	// On cherche une node occupée par l'acteur
	for (i = 0; i < 8; i++)
		if (AdjNode[i].ActorID == AdjNode_ActorID)
			return &AdjNode[i];

	return NULL;
}
