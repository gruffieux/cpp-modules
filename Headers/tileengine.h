#ifndef TILEENGINE
#define TILEENGINE

#define MAX_TILES_X 1000
#define MAX_TILES_Y 1000
#define MAX_TILES MAX_TILES_X * MAX_TILES_Y
#define MAX_LAYERS 5
#define TILE_SIZE 32
#define PATHNODE_ID_INTERVAL 1000 //Doit être de du même nombre que MAX_TILES_X et MAX_TILES_Y! (cf Map())

#include <cmover.h>

struct Screen
{
	Axe dimension;				//Résolution de l'écran
	Axe VisibleTileCount;		//Nombre de briques visibles
	CardinalPoint MaxCamera;	//Positions maximum de la caméra
	Mover Camera;				//Caméra de scrolling
};

struct Map
{
	int TileID[MAX_TILES_Y][MAX_TILES_X][MAX_LAYERS];	//Contient les id de chaque briques
	int TileIDCount[MAX_LAYERS];						//Nombre de briques différentes
	int TileNodeID[MAX_TILES_Y][MAX_TILES_X];			//ID unique de la tile
	short obstacle[MAX_TILES_Y][MAX_TILES_X];			//Pour si la tile est un obstacle
	Axe TileCount;										//Nombre de briques en tout
	CardinalPoint TileZone[MAX_TILES];					//Coordonnées des briques
	Map()
	{
		int i, j, x, y;

		for (i = 0; i < MAX_LAYERS; i++)
		{
			j = TileIDCount[i] = 0;
			for (x = 0; x < MAX_TILES_X; x++)
				for (y = 0; y < MAX_TILES_Y; y++)
				{
					TileID[y][x][i] = 0;
					TileNodeID[y][x] = j;
					j++;
				}
		}
		for (x = 0; x < MAX_TILES_X; x++)
			for (y = 0; y < MAX_TILES_Y; y++)
				obstacle[y][x] = 0;
	}
};

#endif