#ifndef CELEVATOR
#define CELEVATOR

#include <ccursor.h>

class Elevator : public Visual
{
private:
	bool vertical, sliding, OppInitPosX, OppInitPosY;
	Axe StopPosition;
	CardinalPoint StageLimit;
	Visual Cage;
public:
	Elevator();
	Elevator(bool Elevator_vertical, bool Elevator_OppInitPosX, bool Elevator_OppInitPosY, int Item_width, int Item_height, Str Object_Name);
	Elevator(Elevator &model);
	~Elevator() {}
	void SetSliding(bool Elevator_sliding) {sliding = Elevator_sliding;}
	void restoreAni();
	void destroyAni();
	void Initialize(int posx, int posy, int width, int height);
	void stickToPanel(Axe cellPosition);
	bool Slide(Cursor *cursor);
	bool showOnPanel(int frame, Item *Cursor);
	void runAnimations();
	int GetStage(int MaxResult);
	bool GetSliding() {return sliding;}
	bool GetOppInitPosX() {return OppInitPosX;}
	bool GetOppInitPosY() {return OppInitPosY;}
	Axe * GetStopPosition() {return &StopPosition;}
	CardinalPoint * GetStageLimit() {return &StageLimit;}
	Visual * GetCage() {return &Cage;}
	static Elevator * getElevatorElement(int index, List *pList) {return dynamic_cast<Elevator*>(pList->GetElement(index));}
};

#endif