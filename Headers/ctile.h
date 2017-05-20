#ifndef CTILE
#define CTILE

#include <clist.h>

class Tile : public Object
{
private:
	int TileID;
	Tile *Left, *Right, *Up, *Down;
public:
	Tile();
	Tile(int Tile_TileID);
	Tile(Tile &model);
	~Tile();
	void SetTileID(int Tile_TileID) {TileID = Tile_TileID;}
	void SetLeft(Tile *Tile_Left) {Left = Tile_Left;}
	void SetRight(Tile *Tile_Right) {Right = Tile_Right;}
	void SetUp(Tile *Tile_Up) {Up = Tile_Up;}
	void SetDown(Tile *Tile_Down) {Down = Tile_Down;}
	int GetTileID() {return TileID;}
	Tile * GetLeft() {return Left;}
	Tile * GetRight() {return Right;}
	Tile * GetUp() {return Up;}
	Tile * GetDown() {return Down;}
	static Tile * getTileElement(int index, List *pList) {return dynamic_cast<Tile*>(pList->GetElement(index));}
};

#endif