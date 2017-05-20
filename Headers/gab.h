#ifndef GAB
#define GAB

#define PI 3.14159265359 //acos(-1) calcul de la valeur de PI exacte mais est foireux sur Win2000

struct Axe
{
	int x, y;
	Axe() {x = y = 0;}
	Axe(int Axe_x, int Axe_y)
	{
		x = Axe_x;
		y = Axe_y;
	}
};

struct Axe2
{
	double x, y;
	Axe2() {x = y = 0;}
};

struct CardinalPoint
{
	int west, east, north, south;
	CardinalPoint() {west = east = north = south = 0;}
	CardinalPoint(int CP_west, int CP_east, int CP_north, int CP_south)
	{
		west = CP_west;
		east = CP_east;
		north = CP_north;
		south = CP_south;
	}
};

#endif
