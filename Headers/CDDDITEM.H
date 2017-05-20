#ifndef CDDDITEM
#define CDDDITEM

#include <dddengine.h>

class DDDItem
{
private:
	DDDAxe Position, Dimension;
public:
	DDDItem();
	DDDItem(float DDDItem_xpos, float DDDItem_ypos, float DDDItem_zpos, float DDDItem_width, float DDDItem_height, float DDDItem_depth);
	void SetPosition(float x, float y, float z);
	void SetDimension(float x, float y, float z);
	DDDAxe * GetPosition() {return &Position;}
	DDDAxe * GetDimension() {return &Dimension;}
};

#endif