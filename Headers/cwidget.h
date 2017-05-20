#ifndef CWIDGET
#define CWIDGET

#include <cvisual.h>

class Widget : public Visual
{
private:
	bool hasFocus;
public:
	Widget();
	~Widget();
	virtual void focus();
	virtual void initBackground(int bmpId, Str skinId, int width, int height, bool mosaic, int colorKey);
	bool isFocused() {return hasFocus;}
	virtual void unfocus();
};

#endif
