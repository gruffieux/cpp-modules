#ifndef CRADIOBUTTON
#define CRADIOBUTTON

#include <cwidget.h>

class RadioButton : public Widget
{
public:
	RadioButton(Str labelText="", int textColor=0, int textBackColor=RGB(255,255,255));
	~RadioButton();
	void destroyAni();
	void focus();
	bool getChecked() {return checked;}
	Surface* getLabel() {return label;}
	void initBackground(int bmpId, Str skinId, int width, int height, bool mosaic, int colorKey);
	void restoreAni();
	void runAnimations();
	bool showOnPanel(int frame, Item *cursor);
	void unfocus();
	void updateLabel();
private:
	int textColor, textBackColor;
	bool checked;
	Surface *label;
};

#endif
