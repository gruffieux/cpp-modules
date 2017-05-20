#include <math.h>
#include <citem.h>

Item::Item()
{
	removed = false;
}

Item::Item(int Item_xpos, int Item_ypos, int Item_width, int Item_height, Str Object_Name) : Object(Object_Name)
{
	position.x = Item_xpos;
	position.y = Item_ypos;
	dimension.x = Item_width;
	dimension.y = Item_height;
	removed = false;
}

Item::Item(Item &model) : Object(model)
{
	position = *model.getPosition();
	dimension = *model.getDimension();
	removed = model.getRemoved();
}

void Item::centerPosition(Axe max)
{
	position.x = max.x / 2 - dimension.x / 2;
	position.y = max.y / 2 - dimension.y / 2;
}

void Item::centerPosition(int x, int y, int width, int height)
{
	position.x = x + (width / 2) - (dimension.x / 2);
	position.y = y + (height / 2) - (dimension.y / 2);
}

void Item::centerPosition(Item *item)
{
	centerPosition(item->getPosition()->x, item->getPosition()->y, item->getDimension()->x, item->getDimension()->y);
}

bool Item::collision(int left, int right, int up, int down, int dimDivider)
{
	if (position.x > right)
		return false;

	if (position.x + dimension.x / dimDivider < left)
		return false;

	if (position.y > down)
		return false;

	if (position.y + dimension.y / dimDivider < up)
		return false;

	return true;
}

bool Item::collision(Item *pItem, int dimDivider)
{
	if (position.x > pItem->getPosition()->x + pItem->getDimension()->x / dimDivider)
		return false;

	if (position.x + dimension.x < pItem->getPosition()->x)
		return false;

	if (position.y > pItem->getPosition()->y + pItem->getDimension()->y / dimDivider)
		return false;

	if (position.y + dimension.y < pItem->getPosition()->y)
		return false;

	return true;
}

void Item::remove()
{
	position.x = -dimension.x;
	position.y = -dimension.y;
	removed = true;
}

void Item::rescale(int percentX, int percentY)
{
	double x, y;

	x = (double)dimension.x * percentX / 100 + 0.5;
	y = (double)dimension.y * percentY / 100 + 0.5;

	dimension.x = (int)x;
	dimension.y = (int)y;
}
