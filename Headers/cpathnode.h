#ifndef CPATHNODE
#define CPATHNODE

#include <clist.h>
#include <gab.h>

class PathNode : public Object
{
private:
	int NodeID, ParentID, FScore, GScore, HScore;
	double angle;
	Axe Zone;
public:
	PathNode();
	PathNode(PathNode &model);
	~PathNode() {}
	void SetNodeID(int PathNode_NodeID) {NodeID = PathNode_NodeID;}
	void SetParentID(int PathNode_ParentID) {ParentID = PathNode_ParentID;}
	void SetFScore(int PathNode_FScore) {FScore = PathNode_FScore;}
	void SetGScore(int PathNode_GScore) {GScore = PathNode_GScore;}
	void SetHScore(int PathNode_HScore) {HScore = PathNode_HScore;}
	void SetAngle(double PathNode_angle) {angle = PathNode_angle;}
	bool compare(Object *pObject);
	int GenerateGScore(int ParentID, int ParentGScore, int IDInterval);	//Définit la distance qu'il y a entre un point et un autre point.
	void GenerateHScore(int TargetID, int IDInterval);					//Définit la distance à parcourir horizontalement et verticalement pour atteindre la cible. (Méthode Manhattan)
	void GenerateFScore() {FScore = GScore + HScore;}					//Définit le score total du point.
	void GenerateAngle(int ParentID, int IDInterval);					//Définit l'angle du point par rapport au point parent.
	void GenerateTileCoord(int IDInterval);								//Définit sur quel tuile le point se trouve sur la map.
	int GetNodeID() {return NodeID;}
	int GetParentID() {return ParentID;}
	int GetFScore() {return FScore;}
	int GetGScore() {return GScore;}
	int GetHScore() {return HScore;}
	double GetAngle() {return angle;}
	Axe * GetZone() {return &Zone;}
	static PathNode * getPathNodeElement(int index, List *pList) {return dynamic_cast<PathNode*>(pList->GetElement(index));}
};

#endif