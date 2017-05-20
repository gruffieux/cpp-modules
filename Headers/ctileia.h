#ifndef CTILEIA
#define CTILEIA

#include <cmover.h>
#include <cpathfinder.h>
#include <ctileactorlist.h>

class TileIA : virtual public TileActor, virtual public Mover
{
private:
	bool RightOnTile;
protected:
	PathFinder Compass;
	TileActorList Dests;
public:
	TileIA();
	TileIA(TileIA &model);
	~TileIA() {}
	bool GenerateDests(List *pModel);
	virtual bool CheckDest(Object *pObject) {return pObject != this;}
	void Face(TileActor *pTileActor);
	bool FollowCompass(double Mover_magnitude); //Suit un chemin sur la map
	bool StopMove();
	void DisplayPaths(int x, int y, int layer);
	bool GetRightOnTile() {return RightOnTile;}
	PathFinder* GetCompass() {return &Compass;}
	TileActorList* GetDests() {return &Dests;}
	static TileIA* getTileIAElement(int index, List *pList) {return dynamic_cast<TileIA*>(pList->GetElement(index));}
};

#endif