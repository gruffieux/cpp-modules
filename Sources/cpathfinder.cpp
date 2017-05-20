#include <cpathfinder.h>

PathFinder::PathFinder()
{
	CheckObstacle = false;
	Thread = NULL;
	Map = NULL;
}

PathFinder::PathFinder(TileEngine *PF_Map)
{
	CheckObstacle = false;
	Thread = NULL;
	Map = PF_Map;
}

PathFinder::PathFinder(PathFinder &model)
{
	CheckObstacle = model.GetCheckObstacle();
	Start = *model.GetStart();
	Target = *model.GetTarget();
	Opens = *model.GetOpens();
	Closes = *model.GetCloses();
	Paths = *model.GetPaths();
	Thread = NULL;
	Map = model.GetMap();
}

void PathFinder::StartSearch(int PointA, int PointB)
{
	Start = PathNode();
	Target = PathNode();

	Opens.RemoveAllElement(0, true);
	Closes.RemoveAllElement(0, true);
	Paths.RemoveAllElement(0, true);

	Start.SetNodeID(PointA);
	Target.SetNodeID(PointB);

	Closes.AddElement(new PathNode(Start));
	PathNode::getPathNodeElement(0, &Closes)->GenerateHScore(Target.GetNodeID(), PATHNODE_ID_INTERVAL);
}

bool PathFinder::ProcessSearching(int NodeID, bool ByCycle)
{
	int i, x, y;
	PathNode *OpenNode, *AdjNode;

	// On créé le nouveau point adjacent en cours
	AdjNode = new PathNode();

	// On initialise le point en cours
	AdjNode->SetNodeID(NodeID);
	AdjNode->GenerateTileCoord(PATHNODE_ID_INTERVAL);
	x = AdjNode->GetZone()->x;
	y = AdjNode->GetZone()->y;

	// On vérifie si le point est un obstacle.
	if (CheckObstacle)
	{
		if ((Map->GetMapData()->obstacle[y][x] == 1) | (Map->GetMapData()->TileID[y][x][0] == 0))
			return false;
		if (Map->GetMapData()->obstacle[y+1][x])
			if ((Start.GetNodeID() == Map->GetMapData()->TileNodeID[y+1][x+1]) | (Start.GetNodeID() == Map->GetMapData()->TileNodeID[y+1][x-1]))
				return false;
		if (Map->GetMapData()->obstacle[y-1][x])
			if ((Start.GetNodeID() == Map->GetMapData()->TileNodeID[y-1][x+1]) | (Start.GetNodeID() == Map->GetMapData()->TileNodeID[y-1][x-1]))
				return false;
		if (Map->GetMapData()->obstacle[y][x+1])
			if ((Start.GetNodeID() == Map->GetMapData()->TileNodeID[y+1][x+1]) | (Start.GetNodeID() == Map->GetMapData()->TileNodeID[y-1][x+1]))
				return false;
		if (Map->GetMapData()->obstacle[y][x-1])
			if ((Start.GetNodeID() == Map->GetMapData()->TileNodeID[y+1][x-1]) | (Start.GetNodeID() == Map->GetMapData()->TileNodeID[y-1][x-1]))
				return false;
	}

	// On vérifie si le point se trouve dans la CloseList.
	i = Closes.SearchElement(AdjNode, true);
	if (i != -1) return false;

	// On vérifie si le point se trouve dans l'OpenList.
	i = Opens.SearchElement(AdjNode, true);
	if (i != -1)
	{
		// On vérifie si le chemin par ce point serait plus court.
		if ((ByCycle == false) && (PathNode::getPathNodeElement(i, &Opens)->GenerateGScore(Start.GetNodeID(), Start.GetGScore(), PATHNODE_ID_INTERVAL) < PathNode::getPathNodeElement(i, &Opens)->GetGScore()))
		{
			PathNode::getPathNodeElement(i, &Opens)->SetParentID(Start.GetNodeID());
			PathNode::getPathNodeElement(i, &Opens)->SetGScore(PathNode::getPathNodeElement(i, &Opens)->GenerateGScore(Start.GetNodeID(), Start.GetGScore(), PATHNODE_ID_INTERVAL));
			PathNode::getPathNodeElement(i, &Opens)->GenerateFScore();
			PathNode::getPathNodeElement(i, &Opens)->GenerateAngle(Start.GetNodeID(), PATHNODE_ID_INTERVAL);
		}
		*AdjNode = PathNode(*PathNode::getPathNodeElement(i, &Opens));
		Adjs.AddElement(AdjNode);
		return false;
	}

	// On initialise toutes les valeur du point et on l'ajoute dans l'OpenList.
	OpenNode = new PathNode();
	OpenNode->SetNodeID(AdjNode->GetNodeID());
	OpenNode->SetParentID(Start.GetNodeID());
	OpenNode->SetGScore(OpenNode->GenerateGScore(Start.GetNodeID(), Start.GetGScore(), PATHNODE_ID_INTERVAL));
	if (!ByCycle)
	{
		OpenNode->GenerateHScore(Target.GetNodeID(), PATHNODE_ID_INTERVAL);
		OpenNode->GenerateFScore();
		OpenNode->GenerateAngle(Start.GetNodeID(), PATHNODE_ID_INTERVAL);
	}
	else
		OpenNode->GenerateTileCoord(PATHNODE_ID_INTERVAL);
	Opens.AddElement(OpenNode);

	// On réinitialise le point en cours et on l'ajoute dans les points adjacents
	*AdjNode = PathNode(*OpenNode);
	Adjs.AddElement(AdjNode);

	return true;
}

/*
Retours:
 1 = Cible trouvée
 0 = Cible pas trouvée
-1 = Cible inateignable
*/

short PathFinder::Search()
{
	int i, j, BestFScore;
	PathNode *BestNode = NULL;

	BestFScore = 0;

	if (Start.GetNodeID() == Target.GetNodeID())
		return 1; // La cible a été trouvée

	Adjs.RemoveAllElement(0, true);

	//Traite les 8 points adjaçants au point Start.
	for (i = 0; i < 3; i++)
		for (j = PATHNODE_ID_INTERVAL - 1; j < PATHNODE_ID_INTERVAL + 2; j++)
			ProcessSearching(Start.GetNodeID() - j + i * PATHNODE_ID_INTERVAL, false);

	//Cherche dans les points adjaçants celui ayant le plus faible FScore (donc étant le plus proche de la cible).
	for (i = 0; i < Adjs.GetElementCount(); i++)
		if ((PathNode::getPathNodeElement(i, &Adjs)->GetFScore() > 0) && ((PathNode::getPathNodeElement(i, &Adjs)->GetFScore() < BestFScore) | (BestFScore == 0)))
		{
			BestNode = PathNode::getPathNodeElement(i, &Adjs);
			BestFScore = PathNode::getPathNodeElement(i, &Adjs)->GetFScore();
		}

	//Traite le point trouvé
	if (BestNode)
	{
		i = Opens.SearchElement(BestNode, true);
		if (i != -1)
		{
			BestNode = new PathNode(*PathNode::getPathNodeElement(i, &Opens));
			Start = PathNode(*BestNode); //Définit le nouveau point de départ.
			Closes.AddElement(BestNode);
			Opens.RemoveElement(i, 1, true);
		}
	}
	else
		return -1; //La cible est inateignable.

	return 0;
}

void PathFinder::SearchByCycle(int round)
{
	int i, j, k;
	PathNode *pPathNode;

	k = 0;
	Adjs.RemoveAllElement(0, true);

	//Faire tant que la node en cours n'est pas la dernière du cycle
	do
	{
		//Traite les 8 points adjaçants au point Start.
		for (i = 0; i < 3; i++)
			for (j = PATHNODE_ID_INTERVAL - 1; j < PATHNODE_ID_INTERVAL + 2; j++)
				ProcessSearching(Start.GetNodeID() - j + i * PATHNODE_ID_INTERVAL, true);
		//Traite le point en cours dans le cycle
		i = Opens.SearchElement(PathNode::getPathNodeElement(k, &Adjs), true);
		if (i != -1)
		{
			pPathNode = new PathNode(*PathNode::getPathNodeElement(i, &Opens));
			Start = PathNode(*pPathNode); //Définit le nouveau point de départ.
			Closes.AddElement(pPathNode);
			Opens.RemoveElement(i, 1, true);
		}
		k++;
	}
	while ((round > 1) && (k < Opens.GetElementCount()));
}

void PathFinder::BuildPath()
{
	int i, j, k;
	PathNode *pPathNode;

	i = k = Closes.GetElementCount() - 1;
	j = PathNode::getPathNodeElement(i, &Closes)->GetNodeID();

	while (i >= 0)
	{
		if (PathNode::getPathNodeElement(i, &Closes)->GetNodeID() == j)
		{
			pPathNode = new PathNode(*PathNode::getPathNodeElement(i, &Closes));
			j = pPathNode->GetParentID();
			pPathNode->SetAngle(PathNode::getPathNodeElement(k, &Closes)->GetAngle());
			pPathNode->GenerateTileCoord(PATHNODE_ID_INTERVAL);
			Paths.AddElement(pPathNode);
			k = i;
		}
		i--;
	}

	PathNode::getPathNodeElement(0, &Paths)->SetAngle(-1);
}

DWORD PathFinder::GetThreadResult()
{
	DWORD result;

	if (Thread == NULL) return -2;

	if ((GetExitCodeThread(Thread, &result) == TRUE) && (result != STILL_ACTIVE))
	{
		CloseHandle(Thread);
		Thread = NULL;
		return result;
	}

	return 0;
}

DWORD WINAPI RunPathFinder(LPVOID lParam)
{
	DWORD result = -1;
	PathFinder *pPathFinder;

	pPathFinder = reinterpret_cast<PathFinder*>(lParam);

	if (pPathFinder)
		do
			result = pPathFinder->Search();
		while (result == 0);

	ExitThread(result);
}
