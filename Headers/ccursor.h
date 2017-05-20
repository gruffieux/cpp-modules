#ifndef CCURSOR
#define CCURSOR

#include <cvisual.h>
#include <cmover.h>

class Cursor : public Visual
{
public:
	enum ClickState
	{
		NO_CLICK,
		LEFT_CLICK,
		RIGHT_CLICK,
		MIDDLE_CLICK,
		OTHER_CLICK
	};
	Cursor();
	~Cursor();
	ClickState buttonPressed();
	ClickState buttonReleased(); // TODO: ne marche pas toujours
	ClickState getClickState() {return clickState;}
	Mover* getMover() {return mover;}
	void setClickState(ClickState state) {clickState = state;}
	void setClickState(BYTE *data);
	void stickToMover();
private:
	ClickState clickState, prevState;
	Mover *mover;
};

#endif
