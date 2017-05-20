#include <ctileengine.h>
#include <canimation.h>

TileEngine::TileEngine()
{
	int i;

	TileData = Visual();
	*TileData.getDimension() = Axe(TILE_SIZE, TILE_SIZE);

	for (i = 0; i < MAX_LAYERS; i++)
		Tiles[i] = NULL;

	MapFile = DataFileList("", DataFileList::NO_INDEX);
}

TileEngine::TileEngine(int width, int height, double Camera_angle, double Camera_acc, double Camera_dec)
{
	int i;

	ScreenData.dimension.x = width;
	ScreenData.dimension.y = height;
	ScreenData.Camera = Mover(Camera_angle, Camera_acc, Camera_dec);

	TileData = Visual();
	*TileData.getDimension() = Axe(TILE_SIZE, TILE_SIZE);
	
	for (i = 0; i < MAX_LAYERS; i++)
		Tiles[i] = NULL;

	MapFile = DataFileList("", DataFileList::NO_INDEX);
}

TileEngine::TileEngine(TileEngine &model)
{
	int i;

	ScreenData = *model.GetScreenData();
	MapData = *model.GetMapData();
	TileData = *model.GetTileData();
	
	for (i = 0; i < MAX_LAYERS; i++)
		Tiles[i] = NULL;

	MapFile = *model.GetMapFile();
}

void TileEngine::InitTiles(int layer, void (CALLBACK *CallBackProc)())
{
	int x, y;

	if (!Tiles[layer])
	{
		Tiles[layer] = new Tile**[MapData.TileCount.x];
		for (x = 0; x < MapData.TileCount.x; x++)
		{
			Tiles[layer][x] = new Tile*[MapData.TileCount.y];
			for (y = 0; y < MapData.TileCount.y; y++)
			{
				Tiles[layer][x][y] = new Tile(MapData.TileID[y][x][layer]);
				if (CallBackProc) CallBackProc();
			}
		}

		for (x = 0; x < MapData.TileCount.x; x++)
			for (y = 0; y < MapData.TileCount.y; y++)
			{
				if (x)
					Tiles[layer][x][y]->SetLeft(Tiles[layer][x-1][y]);

				if (x < MapData.TileCount.x - 1)
					Tiles[layer][x][y]->SetRight(Tiles[layer][x+1][y]);
				
				if (y)
					Tiles[layer][x][y]->SetUp(Tiles[layer][x][y-1]);

				if (y < MapData.TileCount.y - 1)
					Tiles[layer][x][y]->SetDown(Tiles[layer][x][y+1]);

				if (CallBackProc) CallBackProc();
			}
	}
}

void TileEngine::UpdateTiles(int layer, bool in, void (CALLBACK *CallBackProc)())
{
	int x, y;

	for (x = 0; x < MapData.TileCount.x; x++)
		for (y = 0; y < MapData.TileCount.y; y++)
		{
			if (in)
				Tiles[layer][x][y]->SetTileID(MapData.TileID[y][x][layer]);
			else
				MapData.TileID[y][x][layer] = Tiles[layer][x][y]->GetTileID();
			if (CallBackProc) CallBackProc();
		}
}

void TileEngine::DestroyTiles(int layer, void (CALLBACK *CallBackProc)())
{
	int x, y;

	if (Tiles[layer])
	{
		for (x = 0; x < MapData.TileCount.x; x++)
		{
			for (y = 0; y < MapData.TileCount.y; y++)
			{
				MapData.TileID[y][x][layer] = 0;
				delete Tiles[layer][x][y];
				Tiles[layer][x][y] = NULL;
				if (CallBackProc) CallBackProc();
			}
			delete [] Tiles[layer][x];
			Tiles[layer][x] = NULL;
		}
		delete [] Tiles[layer];
		Tiles[layer] = NULL;
	}
}

void TileEngine::InitMap(int TilesX, int TilesY, int TileIDCount, int layer, void (CALLBACK *CallBackProc)())
{
	int x, y;

	MapData.TileCount.x = TilesX;
	MapData.TileCount.y = TilesY;
	MapData.TileIDCount[layer] = TileIDCount;

	for (x = 0; x < MapData.TileCount.x; x++)
		for (y = 0; y < MapData.TileCount.y; y++)
		{
			MapData.TileID[y][x][layer] = 0;
			InitTilesZone(x, y);
			if (CallBackProc) CallBackProc();
		}

	UpdateMapAni(layer, "");
	InitScreenData();
}

void TileEngine::InitScreenData()
{
	ScreenData.VisibleTileCount.x = ScreenData.dimension.x / TileData.getDimension()->x;
	ScreenData.VisibleTileCount.y = ScreenData.dimension.y / TileData.getDimension()->y;
	ScreenData.MaxCamera.east = (MapData.TileCount.x - ScreenData.VisibleTileCount.x) * TileData.getDimension()->x;
	ScreenData.MaxCamera.south = (MapData.TileCount.y - ScreenData.VisibleTileCount.y) * TileData.getDimension()->y;
}

void TileEngine::InitTilesZone(int x, int y)
{
	MapData.TileZone[x].west = x * TileData.getDimension()->x;
	MapData.TileZone[x].east = x * TileData.getDimension()->x + TileData.getDimension()->x;
	MapData.TileZone[y].north = y * TileData.getDimension()->y;
	MapData.TileZone[y].south = y * TileData.getDimension()->y + TileData.getDimension()->y;
}

void TileEngine::UpdateMapAni(int layer, Str BmpFileName)
{
	if (BmpFileName.GetLength())
	{
		if (TileData.getAnimations()->ElementExist(layer))
			delete Animation::getAnimationElement(layer, TileData.getAnimations());
		TileData.addAni(layer, "", BmpFileName, 0, 0, 0, RGB(255, 255, 255), 0);
	}
	
	if (TileData.getAnimations()->ElementExist(layer))
	{
		MapData.TileIDCount[layer] = Animation::getAnimationElement(layer, TileData.getAnimations())->GetSprite()->GetHeight() / TILE_SIZE;
		Animation::getAnimationElement(layer, TileData.getAnimations())->SetFrame(MapData.TileIDCount[layer], TileData.getDimension()->x, TileData.getDimension()->y);
	}
}

void TileEngine::EmptyClipBoard(int layer)
{
	int x, y;

	for (x = 0; x < MAX_TILES_X; x++)
		for (y = 0; y < MAX_TILES_Y; y++)
			TilesCopy[y][x][layer] = 0;
}

void TileEngine::CopyToClipBoard(int layer)
{
	int x, y;

	EmptyClipBoard(layer);

	for (x = 0; x < MapData.TileCount.x; x++)
		for (y = 0; y < MapData.TileCount.y; y++)
			TilesCopy[y][x][layer] = MapData.TileID[y][x][layer];
}

void TileEngine::CopyToClipBoard(int **data, int layer)
{
	int x, y;

	EmptyClipBoard(layer);

	for (x = 0; x < MapData.TileCount.x; x++)
		for (y = 0; y < MapData.TileCount.y; y++)
			TilesCopy[y][x][layer] = data[x][y];
}

void TileEngine::PasteClipBoard(int LayerSrc, int LayerDest)
{
	int x, y;

	InitTiles(LayerDest);

	for (x = 0; x < MapData.TileCount.x; x++)
		for (y = 0; y < MapData.TileCount.y; y++)
		{
			MapData.TileID[y][x][LayerDest] = TilesCopy[y][x][LayerSrc];
			Tiles[LayerDest][x][y]->SetTileID(TilesCopy[y][x][LayerSrc]);
		}
}

bool TileEngine::LoadMap(int layer, void (CALLBACK *CallBackProc)())
{
	int x, y;

	if (!MapFile.Open(true, false, false))
		return false;

	// Les données de la map sont récupérées dans un fichier .dat
	MapData.TileCount.x = MapFile.Read();			// On récupère la dimension x
	MapData.TileCount.y = MapFile.Read();			// On récupère la dimension y
	MapData.TileIDCount[layer] = MapFile.Read();	// On récupère le nombre d'id

	// On récupère l'id et la zone des tiles
	for (x = 0; x < MapData.TileCount.x; x++)
		for (y = 0; y < MapData.TileCount.y; y++)
		{
			MapData.TileID[y][x][layer] = MapFile.Read();
			InitTilesZone(x, y);
			if (CallBackProc) CallBackProc();
		}

	MapFile.Close();

	UpdateMapAni(layer, "");
	InitScreenData();

	return true;
}

bool TileEngine::SaveMap(int layer, void (CALLBACK *CallBackProc)())
{
	int x, y;

	if (!MapFile.Open(false, true, true))
		return false;

	MapFile.Write(MapData.TileCount.x);
	MapFile.Write(MapData.TileCount.y);
	MapFile.Write(MapData.TileIDCount[layer]);

	for (x = 0; x < MapData.TileCount.x; x++)
		for (y = 0; y < MapData.TileCount.y; y++)
		{
			MapFile.Write(MapData.TileID[y][x][layer]);
			if (CallBackProc) CallBackProc();
		}

	MapFile.Close();

	return true;
}

void TileEngine::RenderMap(int layer)
{
	int i, x, y;
	Axe Start, End;

	TileData.getVisibleZone()->left = TileData.getVisibleZone()->top = 0;
	TileData.getVisibleZone()->right = TileData.getDimension()->x;
	TileData.getVisibleZone()->bottom = TileData.getDimension()->y;

	Start.x = ScreenData.Camera.getPosition()->x >> 5;
	Start.y = ScreenData.Camera.getPosition()->y >> 5;
	End.x = Start.x + ScreenData.VisibleTileCount.x;
	End.y = Start.y + ScreenData.VisibleTileCount.y;

	x = ScreenData.Camera.getPosition()->x & 0x0000001F;
	y = ScreenData.Camera.getPosition()->y & 0x0000001F;

	//Si la coordonnée x de la caméra est divisible par 32
	if (!x)
		End.x--;
	//Sinon déplace rcDest à gauche pour placer la première colonne de briques
	else
	{
		TileData.getVisibleZone()->left -= x;
		TileData.getVisibleZone()->right -= x;
	}

	//Si la coordonnée y de la caméra est divisible par 32
	if (!y)
		End.y--;
	//Sinon déplace rcDest en haut pour placer la première ligne de briques
	else
	{
		TileData.getVisibleZone()->top -= y;
		TileData.getVisibleZone()->bottom -= y;
	}

	//Vérifie qu'on excéde pas la limite
	if (End.x > MapData.TileCount.x)
		End.x = MapData.TileCount.x;
	if (End.y > MapData.TileCount.y)
		End.y = MapData.TileCount.y;

	for (x = Start.x; x <= End.x; x++)
	{
		for (y = Start.y; y <= End.y; y++)
		{
			i = MapData.TileID[y][x][layer];
			//Affiche uniquement une brique ayant l'id supérieur à 0
			if (i > 0)
				TileData.runAni(layer, i);
			//Avance rcDest
			TileData.getVisibleZone()->top += TileData.getDimension()->y;
			TileData.getVisibleZone()->bottom += TileData.getDimension()->y;
		}
		//Réinitialise rcDest en haut de la prochaine colonne
		TileData.getVisibleZone()->left += TileData.getDimension()->x;
		TileData.getVisibleZone()->right += TileData.getDimension()->x;
		TileData.getVisibleZone()->top -= ((End.y - Start.y + 1) << 5);
		TileData.getVisibleZone()->bottom -= ((End.y - Start.y + 1) << 5);
	}
}

void TileEngine::Scroll(Mover *pMover, CardinalPoint *MaxDistance, bool stop, bool rebound)
{
	if (pMover)
	{
		pMover->GenerateDirection();
		ScreenData.Camera.SetAngle(pMover->GetAngle());
		ScreenData.Camera.SetAcceleration(pMover->GetAcceleration());
		ScreenData.Camera.SetDeceleration(pMover->GetDeceleration());
		ScreenData.Camera.SetMagnitude(pMover->GetMagnitude());
		*ScreenData.Camera.GetDirection() = CardinalPoint(pMover->GetDirection()->west, pMover->GetDirection()->east, pMover->GetDirection()->north, pMover->GetDirection()->south);
		*ScreenData.Camera.GetBlockedDirection() = CardinalPoint(pMover->GetBlockedDirection()->west, pMover->GetBlockedDirection()->east, pMover->GetBlockedDirection()->north, pMover->GetBlockedDirection()->south);
		if (((pMover->GetDirection()->west == 1) && (pMover->GetBlockedDirection()->west == 0) && ((ScreenData.Camera.getPosition()->x == ScreenData.MaxCamera.west) | (pMover->getPosition()->x > ScreenData.dimension.x / 2))) |
			((pMover->GetDirection()->east == 1) && (pMover->GetBlockedDirection()->east == 0) && ((ScreenData.Camera.getPosition()->x == ScreenData.MaxCamera.east) | (pMover->getPosition()->x + pMover->getDimension()->x < ScreenData.dimension.x / 2))) |
			((pMover->GetDirection()->north == 1) && (pMover->GetBlockedDirection()->north == 0) && ((ScreenData.Camera.getPosition()->y == ScreenData.MaxCamera.north) | (pMover->getPosition()->y > ScreenData.dimension.y / 2))) |
			((pMover->GetDirection()->south == 1) && (pMover->GetBlockedDirection()->south == 0) && ((ScreenData.Camera.getPosition()->y == ScreenData.MaxCamera.south) | (pMover->getPosition()->y + pMover->getDimension()->y < ScreenData.dimension.y / 2))))
		{
			if (MaxDistance)
			{
				if ((pMover->getPosition()->x <= MaxDistance->west) && (ScreenData.Camera.getPosition()->x == ScreenData.MaxCamera.west))
					stop = rebound = false;
				if ((pMover->getPosition()->x + pMover->getDimension()->x >= MaxDistance->east) && (ScreenData.Camera.getPosition()->x == ScreenData.MaxCamera.east))
					stop = rebound = false;
				if ((pMover->getPosition()->y <= MaxDistance->north) && (ScreenData.Camera.getPosition()->y == ScreenData.MaxCamera.north))
					stop = rebound = false;
				if ((pMover->getPosition()->y + pMover->getDimension()->y >= MaxDistance->south) && (ScreenData.Camera.getPosition()->y == ScreenData.MaxCamera.south))
					stop = rebound = false;
			}
			pMover->Move(MaxDistance, stop, rebound);
		}
		else
		{
			*pMover->GetCurrentObstacle() = CardinalPoint();
			*pMover->GetBlockedDirection() = CardinalPoint();
			ScreenData.Camera.Move(&ScreenData.MaxCamera, stop, rebound);
		}
	}
	else
		ScreenData.Camera.Move(&ScreenData.MaxCamera, stop, rebound);
}
