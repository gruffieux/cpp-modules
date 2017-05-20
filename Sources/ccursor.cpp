#include <ccursor.h>

Cursor::Cursor()
{
	clickState = prevState = NO_CLICK;
	mover = new Mover();
}

Cursor::~Cursor()
{
	delete mover;
}

Cursor::ClickState Cursor::buttonPressed()
{
	if (clickState != NO_CLICK)
		return clickState;

	return NO_CLICK;
}

Cursor::ClickState Cursor::buttonReleased()
{
	Cursor::ClickState result;

	if (clickState == NO_CLICK && prevState != NO_CLICK)
	{
		result = prevState;
		prevState = NO_CLICK;
		return result;
	}

	return NO_CLICK;
}

void Cursor::setClickState(BYTE *data)
{
	prevState = clickState;

	if (data[0])
		clickState = LEFT_CLICK;
	else if (data[1])
		clickState = RIGHT_CLICK;
	else if (data[2])
		clickState = MIDDLE_CLICK;
	else if (data[3])
		clickState = OTHER_CLICK;
	else
		clickState = NO_CLICK;
}

void Cursor::stickToMover()
{
	position.x = mover->getPosition()->x;
	position.y = mover->getPosition()->y;
}
