#include <ctileia.h>

TileIA::TileIA()
{
	RightOnTile = true;

	Compass = PathFinder();
	Dests = TileActorList(TileActorList::ORDER_BY_RANGE);
}

TileIA::TileIA(TileIA &model)
{
	RightOnTile = model.GetRightOnTile();

	Compass = PathFinder(*model.GetCompass());
	Dests = *model.GetDests();
}

bool TileIA::GenerateDests(List *pModel)
{
	int i;

	Dests.SetOriginNodeID(TileNodeID);

	if (!Dests.GetElementCount())
	{
		if ((pModel == NULL) || (pModel->GetElementCount() == 0)) return false;
		for (i = 0; i < pModel->GetElementCount(); i++)
			if (CheckDest(pModel->GetElement(i)))
				if (!Dests.AddElement(pModel->GetElement(i))) return false;
		return true;
	}

	return false;
}

void TileIA::Face(TileActor *pTileActor)
{
	Axe Round, TargetRound, TileNodeDistance, PositiveTND;

	Round.x = TileNodeID / PATHNODE_ID_INTERVAL;
	Round.y = TileNodeID - Round.x * PATHNODE_ID_INTERVAL;
	TargetRound.x = pTileActor->GetTileNodeID() / PATHNODE_ID_INTERVAL;
	TargetRound.y = pTileActor->GetTileNodeID() - TargetRound.x * PATHNODE_ID_INTERVAL;

	TileNodeDistance.x = PositiveTND.x = TargetRound.x - Round.x;
	TileNodeDistance.y = PositiveTND.y = TargetRound.y - Round.y;

	if (PositiveTND.x < 0) PositiveTND.x = -PositiveTND.x;
	if (PositiveTND.y < 0) PositiveTND.y = -PositiveTND.y;

	if (PositiveTND.x > PositiveTND.y)
	{
		if (TileNodeDistance.x > 0)
			angle = 0;
		else
			angle = PI;
	}
	else
	{
		if (TileNodeDistance.y > 0)
			angle = PI / 2;
		else
			angle = 3 * PI / 2;
	}

	GenerateDirection();
}

bool TileIA::FollowCompass(double Mover_magnitude)
{
	int i, x, y;

	for (i = 0; i < Compass.GetPaths()->GetElementCount(); i++)
	{
		x = PathNode::getPathNodeElement(i, Compass.GetPaths())->GetZone()->x;
		y = PathNode::getPathNodeElement(i, Compass.GetPaths())->GetZone()->y;
		if ((position.x == Map->GetMapData()->TileZone[x].west - Map->GetScreenData()->Camera.getPosition()->x) && (position.y == Map->GetMapData()->TileZone[y].north - Map->GetScreenData()->Camera.getPosition()->y))
		{
			RightOnTile = true;
			TileNodeID = Map->GetMapData()->TileNodeID[y][x];
			if (PathNode::getPathNodeElement(i, Compass.GetPaths())->GetAngle() >= 0)
			{
				magnitude = Mover_magnitude;
				angle = PathNode::getPathNodeElement(i, Compass.GetPaths())->GetAngle();
			}
			else
				magnitude = 0;
			TileCoord.x = x;
			TileCoord.y = y;
			return true;
		}
		else
			RightOnTile = false;
	}

	return false;
}

bool TileIA::StopMove()
{
	if (RightOnTile)
	{
		magnitude = 0;
		return true;
	}

	return false;
}

void TileIA::DisplayPaths(int x, int y, int layer)
{
	int i;

	for (i = 0; i < Compass.GetOpens()->GetElementCount(); i++)
		if (PathNode::getPathNodeElement(i, Compass.GetOpens())->GetNodeID() == Map->GetMapData()->TileNodeID[y][x])
			Map->GetMapData()->TileID[y][x][layer] = 9;

	for (i = 0; i < Compass.GetCloses()->GetElementCount(); i++)
		if (PathNode::getPathNodeElement(i, Compass.GetCloses())->GetNodeID() == Map->GetMapData()->TileNodeID[y][x])
			Map->GetMapData()->TileID[y][x][layer] = 10;

	for (i = 0; i < Compass.GetPaths()->GetElementCount(); i++)
	{
		if (PathNode::getPathNodeElement(i, Compass.GetPaths())->GetNodeID() == Map->GetMapData()->TileNodeID[y][x])
		{
			if (PathNode::getPathNodeElement(i, Compass.GetPaths())->GetAngle() == -1)
				Map->GetMapData()->TileID[y][x][layer] = 10;
			if (PathNode::getPathNodeElement(i, Compass.GetPaths())->GetAngle() == PI)
				Map->GetMapData()->TileID[y][x][layer] = 1;
			if (PathNode::getPathNodeElement(i, Compass.GetPaths())->GetAngle() == 0)
				Map->GetMapData()->TileID[y][x][layer] = 2;
			if (PathNode::getPathNodeElement(i, Compass.GetPaths())->GetAngle() == 3 * PI / 2)
				Map->GetMapData()->TileID[y][x][layer] = 3;
			if (PathNode::getPathNodeElement(i, Compass.GetPaths())->GetAngle() == PI / 2)
				Map->GetMapData()->TileID[y][x][layer] = 4;
			if (PathNode::getPathNodeElement(i, Compass.GetPaths())->GetAngle() == 5 * PI / 4)
				Map->GetMapData()->TileID[y][x][layer] = 5;
			if (PathNode::getPathNodeElement(i, Compass.GetPaths())->GetAngle() == 7 * PI / 4)
				Map->GetMapData()->TileID[y][x][layer] = 6;
			if (PathNode::getPathNodeElement(i, Compass.GetPaths())->GetAngle() == PI / 4)
				Map->GetMapData()->TileID[y][x][layer] = 7;
			if (PathNode::getPathNodeElement(i, Compass.GetPaths())->GetAngle() == 3 * PI / 4)
				Map->GetMapData()->TileID[y][x][layer] = 8;
		}
		if (!magnitude)
			Map->GetMapData()->TileID[y][x][layer] = 0;
	}
}
