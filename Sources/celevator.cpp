#include <celevator.h>

Elevator::Elevator()
{
	vertical = true;
	sliding = OppInitPosX = OppInitPosY = false;
}

Elevator::Elevator(bool Elevator_vertical, bool Elevator_OppInitPosX, bool Elevator_OppInitPosY, int Item_width, int Item_height, Str Object_Name) :
Item(NULL, NULL, Item_width, Item_height, Object_Name)
{
	vertical = Elevator_vertical;
	sliding = false;
	OppInitPosX = Elevator_OppInitPosX;
	OppInitPosY = Elevator_OppInitPosY;
}

Elevator::Elevator(Elevator &model) : Item(model)
{
	sliding = model.GetSliding();
	OppInitPosX = model.GetOppInitPosX();
	OppInitPosY = model.GetOppInitPosY();
	StopPosition = *model.GetStopPosition();
	StageLimit = *model.GetStageLimit();
}

void Elevator::restoreAni()
{
	Visual::restoreAni();
	Cage.restoreAni();
}

void Elevator::destroyAni()
{
	Visual::destroyAni();
	Cage.destroyAni();
}

void Elevator::Initialize(int posx, int posy, int width, int height)
{
	position.x = posx;
	position.y = posy;
	dimension.x = width;
	dimension.y = height;

	if (vertical)
	{
		Cage.getDimension()->x = width;
		Cage.getDimension()->y = width;
	}
	else
	{
		Cage.getDimension()->x = height;
		Cage.getDimension()->y = height;
	}

	if (OppInitPosX)
		Cage.getPosition()->x = posx + dimension.x - Cage.getDimension()->x;
	else
		Cage.getPosition()->x = posx;

	if (OppInitPosY)
		Cage.getPosition()->y = posy + dimension.y - Cage.getDimension()->y;
	else
		Cage.getPosition()->y = posy;
}

void Elevator::stickToPanel(Axe cellPosition)
{
	Visual::stickToPanel(cellPosition);

	if (!Cage.getPosition()->x)
		if (OppInitPosX)
			Cage.getPosition()->x = cellPosition.x + dimension.x - Cage.getDimension()->x;
		else
			Cage.getPosition()->x = cellPosition.x;

	if (!Cage.getPosition()->y)
		if (OppInitPosY)
			Cage.getPosition()->y = cellPosition.y + dimension.y - Cage.getDimension()->y;
		else
			Cage.getPosition()->y = cellPosition.y;

	if (sliding)
	{
		StopPosition.x = Cage.getPosition()->x - cellPosition.x;
		StopPosition.y = Cage.getPosition()->y - cellPosition.y;
	}
	else
	{
		Cage.getPosition()->x = cellPosition.x + StopPosition.x;
		Cage.getPosition()->y = cellPosition.y + StopPosition.y;
	}
}

bool Elevator::Slide(Cursor *cursor)
{
	if (cursor->buttonPressed() == Cursor::LEFT_CLICK)
	{
		if (cursor->collision(this))
			sliding = true;
	}
	else
		sliding = false;

	if (sliding)
	{
		if (vertical)
		{
			Cage.getPosition()->x = position.x;
			Cage.getPosition()->y = cursor->getPosition()->y;
		}
		else
		{
			Cage.getPosition()->x = cursor->getPosition()->x;
			Cage.getPosition()->y = position.y;
		}
	}
	
	return sliding;
}

bool Elevator::showOnPanel(int frame, Item *cursor)
{
	Visual::showOnPanel(0, cursor);

	Cage.setVisibleZone();

	return Cage.runAni(0, frame);
}

void Elevator::runAnimations()
{
	Visual::runAnimations();
	Cage.runAnimations();
}

int Elevator::GetStage(int MaxResult)
{
	int MinPosition, MaxPosition;
	float stage;

	if (vertical)
	{
		MinPosition = position.y;
		MaxPosition = dimension.y;
		stage = (float)(Cage.getPosition()->y - MinPosition) / MaxPosition * MaxResult;
		StageLimit.west = Cage.getPosition()->x;
		StageLimit.east = Cage.getPosition()->x + Cage.getDimension()->x;
		StageLimit.north = MinPosition;
		StageLimit.south = MinPosition + MaxPosition;
	}
	else
	{
		MinPosition = position.x;
		MaxPosition = dimension.x;
		stage = (float)(Cage.getPosition()->x - MinPosition) / MaxPosition * MaxResult;
		StageLimit.west = MinPosition;
		StageLimit.east = MinPosition + MaxPosition;
		StageLimit.north = Cage.getPosition()->y;
		StageLimit.south = Cage.getPosition()->y + Cage.getDimension()->y;
	}
	
	stage += 0.5f;

	if (stage < 0)
		stage = 0;

	return (int)stage;
}
