#ifndef CITEM
#define CITEM

#include <gab.h>
#include <clist.h>

class Item : public Object
{
private:
protected:
	bool removed;
	Axe position, dimension;
public:
	Item();
	Item(int xpos, int ypos, int width, int height, Str name);
	Item(Item &model);
	~Item() {}
	void centerPosition(Axe max);
	void centerPosition(int x, int y, int width, int height);
	void centerPosition(Item *item);
	bool collision(int left, int right, int up, int down, int dimDivider=1);
	bool collision(Item *pItem, int dimDivider=1);
	Axe * getDimension() {return &dimension;}
	static Item * getItemElement(int index, List *pList) {return dynamic_cast<Item*>(pList->GetElement(index));}
	Axe * getPosition() {return &position;}
	bool getRemoved() {return removed;}
	void remove();
	void rescale(int percentX, int percentY);
	void setRemoved(bool removed) {this->removed = removed;}
};

#endif
