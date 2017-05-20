#ifndef CTILEACTOR
#define CTILEACTOR

#include <cpathnode.h>
#include <ctileengine.h>

#define MAX_LIVING_TILE_ID 5

struct ActorNode
{
	int id, ActorID;
	bool obstacle, forbidden;
	ActorNode()
	{
		id = ActorID = -1;
		obstacle = forbidden = false;
	}
};

class TileActor : virtual public Item
{
private:
	int TileID, LivingTileCount;
	Axe MapPosition;
protected:
	int TileNodeID, LivingTileID[MAX_LIVING_TILE_ID];
	Axe TileCoord;
	PathNode Start;
	ActorNode AdjNode[8];
	TileEngine *Map;
public:
	TileActor();
	TileActor(TileActor &model);
	~TileActor() {}
	void SetTileNodeID(int TileAcor_TileNodeID) {TileNodeID = TileAcor_TileNodeID;}
	void SetMap(TileEngine *TileActor_Map) {Map = TileActor_Map;}
	void SetMapPosition();																				//Définit la position sur la map d'après la position courante
	void SetMapPosition(int x, int y);																	//Définit la position sur la map d'après une tile
	void SetMapPositionByCoord(bool center) {SetMapPositionByCoord(TileCoord.x, TileCoord.y, center);}	//Définit la position sur la map d'après les coordonnées de la tile en cours
	void SetMapPositionByCoord(int x, int y, bool center);												//Définit la position sur la map d'après les coordonnées d'une tile
	void CenterMapPosition();																			//Place l'élément au centre d'une tile de la map
	void GenerateMapPosition();																			//Place l'élément sur la map en tenant compte de sa position courante
	void GenerateTileCoord(int x, int y);																//Génère les coordonnées de la tile où se trouve l'acteur (à éviter pour un héritier de Mover qui bouge librement)
	Axe GenerateZonePosition(int ZoneTile[], int ZoneTileCount, Axe UsedPosition[], int DispatchCount, int TotalTile, int layer, bool SpecifiedZone, bool ForbiddenObstacle1, bool ForbiddenObstacle2);
	void GenerateTileNodeID(int x, int y);
	void GenerateAdjNode();
	void AddLivingTile(int TileID);
	int CheckLivingTile(int TileCount, int TileID[]);
	bool CheckLivingTile(int TileID);
	void CheckAdjNode(int x, int y);
	bool FillAdjNode(ActorNode *pActorNode, int AdjNode_ActorID);
	void FreeAdjNode(int AdjNode_ActorID);
	void FreeAllAdjNodes();
	void FocusWithCamera(int AdjustPosX, int AdjustPosY);
	bool IsInZone(Axe Zone);
	int GetRange(int NodeID);
	bool GetAdjacent(TileActor *pTileActor);
	ActorNode * GetFreeAdjNodeID();
	ActorNode * GetFilledAdjNodeID(int AdjNode_ActorID);
	int GetTileNodeID() {return TileNodeID;}
	TileEngine * GetMap() {return Map;}
	int GetTileID() {return TileID;}
	int GetLivingTileID(int index) {return LivingTileID[index];}
	int GetLivingTileCount() {return LivingTileCount;}
	Axe * GetMapPosition() {return &MapPosition;}
	Axe * GetTileCoord() {return &TileCoord;}
	PathNode * GetStart() {return &Start;}
	ActorNode * GetAdjNode(int index) {return &AdjNode[index];}
	static TileActor * getTileActorElement(int index, List *pList) {return dynamic_cast<TileActor*>(pList->GetElement(index));}
};

#endif