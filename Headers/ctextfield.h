#ifndef CTEXTFIELD
#define CTEXTFIELD

#include <cwidget.h>
#include <ckeyboardlistener.h>

class TextField : public Widget
{
private:
	int textColor, bkgColor, max;
	bool valid;
	Surface surface;
	KeyboardListener *listener;
public:
	TextField();
	TextField(int textColor, int bkgColor, KeyboardListener *listener, int max=-1);
	~TextField();
	void destroyAni();
	void focus();
	bool isValid() {return valid;}
	void restoreAni();
	void readInput();
	void runAnimations();
	void setInvalid(Str text, int color=RGB(255, 0, 0));
	void setListener(KeyboardListener *listener) {this->listener = listener;}
	void setTextColor(int textColor) {this->textColor = textColor;}
	bool showOnPanel(int frame, Item *cursor);
	void unfocus();
	void updateContent();
	void updateText(Str text, int textColor=-1, int bkgColor=-1);
};

#endif
