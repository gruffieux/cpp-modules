#ifndef CTILEACTORLIST
#define CTILEACTORLIST

#include <clist.h>
#include <ctileactor.h>

class TileActorList : public List
{
private:
	int OriginNodeID;
public:
	static const int ORDER_BY_RANGE = 10;
	TileActorList() {OriginNodeID = -1;}
	TileActorList(int List_Order) : List(List_Order) {OriginNodeID = -1;}
	TileActorList(TileActorList &model) {OriginNodeID = model.GetOriginNodeID();}
	~TileActorList() {}
	void SetOriginNodeID(int TileActorList_OriginNodeID) {OriginNodeID = TileActorList_OriginNodeID;}
	int OrderBy(int NewOrder, Container *pContainer=NULL);
	int GetOriginNodeID() {return OriginNodeID;}
	TileActor * GetTileActorElement(int index) {return dynamic_cast<TileActor*>(pList[index].Content);}
};

#endif