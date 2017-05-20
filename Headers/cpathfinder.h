#ifndef CPATHFINDER
#define CPATHFINDER

#include <cpathnode.h>
#include <ctileengine.h>

class PathFinder
{
private:
	bool CheckObstacle;
	PathNode Start, Target;				//Point de départ/arrivée
	List Opens, Closes, Paths, Adjs;	//Points à vérifier/à ne pas vérifier/formant le chemin final/Adjacents
	HANDLE Thread;
	TileEngine *Map;
	bool ProcessSearching(int NodeID, bool ByCycle);
public:
	PathFinder();
	PathFinder(TileEngine *PF_Map);
	PathFinder(PathFinder &model);
	~PathFinder() {}
	void SetCheckObstacle(bool PF_CheckObstacle) {CheckObstacle = PF_CheckObstacle;}
	void SetMap(TileEngine *PF_Map) {Map = PF_Map;}
	void StartSearch(int PointA, int PointB);
	void RunThread() {if (!Thread) Thread = CreateThread(NULL, 0, RunPathFinder, this, NULL, NULL);}
	short Search();
	void SearchByCycle(int round);
	void BuildPath();
	DWORD GetThreadResult();
	bool GetCheckObstacle() {return CheckObstacle;}
	TileEngine * GetMap() {return Map;}
	PathNode * GetStart() {return &Start;}
	PathNode * GetTarget() {return &Target;}
	List * GetOpens() {return &Opens;}
	List * GetCloses() {return &Closes;}
	List * GetPaths() {return &Paths;}
	List * GetAdjs() {return &Adjs;}
	friend DWORD WINAPI RunPathFinder(LPVOID lParam);
};

#endif
