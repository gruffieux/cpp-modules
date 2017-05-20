#include <cddditem.h>

DDDItem::DDDItem()
{
	Position.x = Position.y = Position.z = 0;
	Dimension.x = Dimension.y = Dimension.z = 0;
}

DDDItem::DDDItem(float DDDItem_xpos, float DDDItem_ypos, float DDDItem_zpos, float DDDItem_width, float DDDItem_height, float DDDItem_depth)
{
	Position.x = DDDItem_xpos;
	Position.y = DDDItem_ypos;
	Position.z = DDDItem_zpos;
	Dimension.x = DDDItem_width;
	Dimension.y = DDDItem_height;
	Dimension.z = DDDItem_depth;
}

void DDDItem::SetPosition(float x, float y, float z)
{
	Position.x = x;
	Position.y = y;
	Position.z = z;
}

void DDDItem::SetDimension(float x, float y, float z)
{
	Dimension.x = x;
	Dimension.y = y;
	Dimension.z = z;
}
