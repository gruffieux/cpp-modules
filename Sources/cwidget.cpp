#include <cwidget.h>

Widget::Widget()
{
	hasFocus = false;
}

Widget::~Widget()
{
}

void Widget::focus()
{
	hasFocus = true;
}

void Widget::initBackground(int bmpId, Str skinId, int width, int height, bool mosaic, int colorKey)
{
	setMosaic(mosaic);

	addAni(bmpId, skinId, 1, width, height, colorKey, 90);
}

void Widget::unfocus()
{
	hasFocus = false;
}
