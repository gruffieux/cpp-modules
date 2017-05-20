#include <cmover.h>
#include <math.h>

Mover::Mover(double Mover_angle, double Mover_acc, double Mover_dec)
{
	angle = Mover_angle;
	acceleration = Mover_acc;
	deceleration = Mover_dec;
	magnitude = 0;
}

Mover::Mover(Mover &model)
{
	angle = model.GetAngle();
	acceleration = model.GetAcceleration();
	deceleration = model.GetDeceleration();
	magnitude = model.GetMagnitude();
	Direction = *model.GetDirection();
	BlockedDirection = *model.GetBlockedDirection();
	CurrentObstacle = *model.GetCurrentObstacle();
}

void Mover::GenerateMouseAngle(int x, int y)
{
	if (x)
	{
		if (x > 0)
		{
			if (y)
			{
				if (y > 0)
					angle = PI / 4;
				else
					angle = 7 * PI / 4;
			}
			else
				angle = 0;
		}
		else
			if (y)
			{
				if (y > 0)
					angle = 3 * PI / 4;
				else
					angle = 5 * PI / 4;
			}
			else
				angle = PI;
	}
	else
		if (y)
			if (y > 0)
				angle = PI / 2;
			else
				angle = 3 * PI / 2;

	GenerateDirection();
}

void Mover::IncreaseAngle(double min, double max, int total)
{
	//2Pi équivaut à 360° en angle radian
	angle += 2 * PI / total;
	if (angle >= max)
		angle = min;
}

void Mover::DecreaseAngle(double min, double max, int total)
{
	angle -= 2 * PI / total;
	if (angle <= min)
		angle = max;
}

void Mover::IncreaseMagnitude(int max)
{
	if (magnitude < max)
		magnitude += acceleration;
	else
		magnitude = max;
}

void Mover::DecreaseMagnitude(int max)
{
	if (magnitude > max)
		magnitude -= deceleration;
	else
		magnitude = max;
}

void Mover::GenerateDirection()
{
	if (angle == 0)
	{
		Direction.west = 0;
		Direction.east = 1;
		Direction.north = 0;
		Direction.south = 0;
	}

	if ((angle > 0) && (angle < PI / 2))
	{
		Direction.west = 0;
		Direction.east = 1;
		Direction.north = 0;
		Direction.south = 1;
	}

	if (angle == PI / 2)
	{
		Direction.west = 0;
		Direction.east = 0;
		Direction.north = 0;
		Direction.south = 1;
	}

	if ((angle > PI / 2) && (angle < PI))
	{
		Direction.west = 1;
		Direction.east = 0;
		Direction.north = 0;
		Direction.south = 1;
	}

	if (angle == PI)
	{
		Direction.west = 1;
		Direction.east = 0;
		Direction.north = 0;
		Direction.south = 0;
	}

	if ((angle > PI) && (angle < 3 * PI / 2))
	{
		Direction.west = 1;
		Direction.east = 0;
		Direction.north = 1;
		Direction.south = 0;
	}

	if (angle == 3 * PI / 2)
	{
		Direction.west = 0;
		Direction.east = 0;
		Direction.north = 1;
		Direction.south = 0;
	}

	if ((angle > 3 * PI / 2) && (angle < 2 * PI))
	{
		Direction.west = 0;
		Direction.east = 1;
		Direction.north = 1;
		Direction.south = 0;
	}
}

CardinalPoint * Mover::GenerateCollision(CardinalPoint *Obstacle, CardinalPoint ObstacleGroup[], int ObstacleCount, bool stop, bool rebound)
{
	int i;

	if ((position.x + dimension.x + (int)(magnitude * cos(angle)) > Obstacle->west) && (position.x + (int)(magnitude * cos(angle)) < Obstacle->east) && (position.y + dimension.y + (int)(magnitude * sin(angle)) > Obstacle->north) && (position.y + (int)(magnitude * sin(angle)) < Obstacle->south))
	{
		if ((Direction.west == 1) && (position.x >= Obstacle->east - magnitude))
		{
			//Lorsque les obstacles sont côte à côte, ils ne peuvent pas être percuté dans les coins.
			for (i = 0; i < ObstacleCount; i++)
				if ((ObstacleGroup[i].north == Obstacle->north) && (ObstacleGroup[i].west == Obstacle->west + Obstacle->east - Obstacle->west))
					break;
			if (Obstacle->east != ObstacleGroup[i].west)
			{
				if (stop)
				{
					position.x -= position.x - Obstacle->east;
					BlockedDirection.west = 1;
				}
				if (rebound)
					ReverseAngle(true, false, false, false);
			}
		}
		if ((Direction.east == 1) && (position.x + dimension.x <= Obstacle->west + magnitude))
		{
			for (i = 0; i < ObstacleCount; i++)
				if ((ObstacleGroup[i].north == Obstacle->north) && (ObstacleGroup[i].west == Obstacle->west - (Obstacle->east - Obstacle->west)))
					break;
			if (Obstacle->west != ObstacleGroup[i].east)
			{
				if (stop)
				{
					position.x += Obstacle->west - (position.x + dimension.x);
					BlockedDirection.east = 1;
				}
				if (rebound)
					ReverseAngle(false, true, false, false);
			}
		}
		if ((Direction.north == 1) && (position.y >= Obstacle->south - magnitude))
		{
			for (i = 0; i < ObstacleCount; i++)
				if ((ObstacleGroup[i].west == Obstacle->west) && (ObstacleGroup[i].north == Obstacle->north + Obstacle->south - Obstacle->north))
					break;
			if (Obstacle->south != ObstacleGroup[i].north)
			{
				if (stop)
				{
					position.y -= position.y - Obstacle->south;
					BlockedDirection.north = 1;
				}
				if (rebound)
					ReverseAngle(false, false, true, false);
			}
		}
		if ((Direction.south == 1) && (position.y + dimension.y <= Obstacle->north + magnitude))
		{
			for (i = 0; i < ObstacleCount; i++)
				if ((ObstacleGroup[i].west == Obstacle->west) && (ObstacleGroup[i].north == Obstacle->north - (Obstacle->south - Obstacle->north)))
					break;
			if (Obstacle->north != ObstacleGroup[i].south)
			{
				if (stop)
				{
					position.y += Obstacle->north - (position.y + dimension.y);
					BlockedDirection.south = 1;
				}
				if (rebound)
					ReverseAngle(false, false, false, true);
			}
		}
		CurrentObstacle = CardinalPoint(Obstacle->west, Obstacle->east, Obstacle->north, Obstacle->south);
		return Obstacle;
	}

	return NULL;
}

void Mover::GenerateCollision(int x, int y, CardinalPoint *Obstacle, bool stop)
{
	if ((Direction.west == 1) && (position.x + x < Obstacle->west))
	{
		CurrentObstacle.west = Obstacle->west;
		if (stop)
		{
			position.x = Obstacle->west;
			BlockedDirection.west = 1;
		}
	}

	if ((Direction.east == 1) && (position.x + dimension.x + x > Obstacle->east))
	{
		CurrentObstacle.east = Obstacle->east;
		if (stop)
		{
			position.x = Obstacle->east - dimension.x;
			BlockedDirection.east = 1;
		}
	}

	if ((Direction.north == 1) && (position.y + y < Obstacle->north))
	{
		CurrentObstacle.north = Obstacle->north;
		if (stop)
		{
			position.y = Obstacle->north;
			BlockedDirection.north = 1;
		}
	}

	if ((Direction.south == 1) && (position.y + dimension.y + y > Obstacle->south))
	{
		CurrentObstacle.south = Obstacle->south;
		if (stop)
		{
			position.y = Obstacle->south - dimension.y;
			BlockedDirection.south = 1;
		}
	}
}

void Mover::Move(CardinalPoint *MaxDistance, bool stop, bool rebound)
{
	GenerateDirection();

	if (MaxDistance)
	{
		if ((Direction.west == 1) && (BlockedDirection.west == 0))
			if ((position.x + (int)(magnitude * cos(angle)) > MaxDistance->west) | ((stop == false) && (rebound == false)))
				position.x += (int)(magnitude * cos(angle));
			else
			{
				if (stop)
					position.x -= position.x - MaxDistance->west;
				if (rebound)
					ReverseAngle(true, false, false, false);
			}
		if ((Direction.east == 1) && (BlockedDirection.east == 0))
			if ((position.x + dimension.x + (int)(magnitude * cos(angle)) < MaxDistance->east) | ((stop == false) && (rebound == false)))
				position.x += (int)(magnitude * cos(angle));
			else
			{
				if (stop)
					position.x += MaxDistance->east - (position.x + dimension.x);
				if (rebound)
					ReverseAngle(false, true, false, false);
			}
		if ((Direction.north == 1) && (BlockedDirection.north == 0))
			if ((position.y + (int)(magnitude * sin(angle)) > MaxDistance->north) | ((stop == false) && (rebound == false)))
				position.y += (int)(magnitude * sin(angle));
			else
			{
				if (stop)
					position.y -= position.y - MaxDistance->north;
				if (rebound)
					ReverseAngle(false, false, true, false);
			}
		if ((Direction.south == 1) && (BlockedDirection.south == 0))
			if ((position.y + dimension.y + (int)(magnitude * sin(angle)) < MaxDistance->south) | ((stop == false) && (rebound == false)))
				position.y += (int)(magnitude * sin(angle));
			else
			{
				if (stop)
					position.y += MaxDistance->south - (position.y + dimension.y);
				if (rebound)
					ReverseAngle(false, false, false, true);
			}
	}
	else
	{
		if (((Direction.west == 1) && (BlockedDirection.west == 0)) | ((Direction.east == 1) && (BlockedDirection.east == 0)))
			position.x += (int)(magnitude * cos(angle));
		if (((Direction.north == 1) && (BlockedDirection.north == 0)) | ((Direction.south == 1) && (BlockedDirection.south == 0)))
			position.y += (int)(magnitude * sin(angle));
	}

	if ((CurrentObstacle.west == 0) && (CurrentObstacle.east == 0) && (CurrentObstacle.north == 0) && (CurrentObstacle.south == 0))
		BlockedDirection = CardinalPoint();
	CurrentObstacle = CardinalPoint();
}

void Mover::Move(int x, int y, CardinalPoint *MaxDistance, bool stop)
{
	if (MaxDistance)
	{
		if ((Direction.west == 1) && (BlockedDirection.west == 0))
			if (position.x + x > MaxDistance->west)
				position.x += x;
			else
			{
				if (stop)
					position.x = MaxDistance->west;
			}
		if ((Direction.east == 1) && (BlockedDirection.east == 0))
			if (position.x + dimension.x + x < MaxDistance->east)
				position.x += x;
			else
			{
				if (stop)
					position.x = MaxDistance->east - dimension.x;
			}
		if ((Direction.north == 1) && (BlockedDirection.north == 0))
			if (position.y + y > MaxDistance->north)
				position.y += y;
			else
			{
				if (stop)
					position.y = MaxDistance->north;
			}
		if ((Direction.south == 1) && (BlockedDirection.south == 0))
			if (position.y + dimension.y + y < MaxDistance->south)
				position.y += y;
			else
			{
				if (stop)
					position.y = MaxDistance->south - dimension.y;
			}
	}
	else
	{
		if (((Direction.west == 1) && (BlockedDirection.west == 0)) | ((Direction.east == 1) && (BlockedDirection.east == 0)))
			position.x += x;
		if (((Direction.north == 1) && (BlockedDirection.north == 0)) | ((Direction.south == 1) && (BlockedDirection.south == 0)))
			position.y += y;
	}

	if (!CurrentObstacle.west)
		BlockedDirection.west = 0;

	if (!CurrentObstacle.east)
		BlockedDirection.east = 0;

	if (!CurrentObstacle.north)
		BlockedDirection.north = 0;

	if (!CurrentObstacle.south)
		BlockedDirection.south = 0;

	CurrentObstacle = CardinalPoint();
}

bool Mover::StopMove()
{
	magnitude = 0;

	return true;
}

void Mover::ReverseAngle(bool left, bool right, bool up, bool down)
{
	if (left)
		if (Direction.north)
			angle = 2 * (3 * PI / 2) - angle;
		else
			if (Direction.south)
				angle = 2 * (PI / 2) - angle;
			else
				angle = 0;

	if (right)
		if (Direction.north)
			angle = 2 * (3 * PI / 2) - angle;
		else
			if (Direction.south)
				angle = 2 * (PI / 2) - angle;
			else
				angle = PI;

	if (up)
		if (Direction.west)
			angle = 2 * PI - angle;
		else
			if (Direction.east)
				angle = 2 * PI - angle;
			else
				angle = PI / 2;

	if (down)
		if (Direction.west)
			angle = 2 * PI - angle;
		else
			if (Direction.east)
				angle = 2 * PI - angle;
			else
				angle = 3 * PI / 2;
}
