#ifndef CTILEENGINE
#define CTILEENGINE

#include <cvisual.h>
#include <ctile.h>
#include <cdatafilelist.h>
#include <tileengine.h>

class TileEngine
{
private:
	int TilesCopy[MAX_TILES_Y][MAX_TILES_X][MAX_LAYERS];
	Screen ScreenData;
	Map MapData;
	Tile ***Tiles[MAX_LAYERS];
	Visual TileData;
	DataFileList MapFile;
public:
	TileEngine();
	TileEngine(int width, int height, double Camera_angle, double Camera_acc, double Camera_dec);
	TileEngine(TileEngine &model);
	~TileEngine() {}
	void InitScreenData();
	void InitTilesZone(int x, int y);
	void InitTiles(int layer, void (CALLBACK *CallBackProc)()=NULL);
	void UpdateTiles(int layer, bool in, void (CALLBACK *CallBackProc)()=NULL);
	void DestroyTiles(int layer, void (CALLBACK *CallBackProc)()=NULL);
	void InitMap(int TilesX, int TilesY, int TileIDCount, int layer, void (CALLBACK *CallBackProc)()=NULL);
	void UpdateMapAni(int layer, Str BmpFileName);
	void EmptyClipBoard(int layer);
	void CopyToClipBoard(int layer);
	void CopyToClipBoard(int **data, int layer);
	void PasteClipBoard(int LayerSrc, int LayerDest);
	bool LoadMap(int layer, void (CALLBACK *CallBackProc)()=NULL);
	bool SaveMap(int layer, void (CALLBACK *CallBackProc)()=NULL);
	void RenderMap(int layer);
	void Scroll(Mover *pMover, CardinalPoint *MaxDistance, bool stop, bool rebound);
	bool TilesAreEmpty(int layer) {return Tiles[layer] == NULL;}
	Screen * GetScreenData() {return &ScreenData;}
	Map * GetMapData() {return &MapData;}
	Tile * GetTile(int x, int y, int layer) {return Tiles[layer][x][y];}
	Visual * GetTileData() {return &TileData;}
	DataFileList * GetMapFile() {return &MapFile;}
};

#endif