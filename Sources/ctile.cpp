#include <ctile.h>

Tile::Tile()
{
	TileID = 0;
	Left = NULL;
	Right = NULL;
	Up = NULL;
	Down = NULL;
}

Tile::Tile(int Tile_TileID)
{
	TileID = Tile_TileID;
	Left = NULL;
	Right = NULL;
	Up = NULL;
	Down = NULL;
}

Tile::Tile(Tile &model)
{
	TileID = model.GetTileID();
	Left = model.GetLeft();
	Right = model.GetRight();
	Up = model.GetUp();
	Down = model.GetDown();
}

Tile::~Tile()
{
}
