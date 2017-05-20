#ifndef CTEXT
#define CTEXT

#include <cobject.h>
#include <cdisplay.h>

class Text : public Object
{
private:
	COLORREF BGColor, FGColor;
	Display *Displayer;
protected:
	RECT rect;
	Surface *Sprite;
	Str *Content;
public:
	Text();
	Text(COLORREF Text_BGColor, COLORREF Text_FGColor);
	~Text() {delete Sprite;}
	void SetContent(Str *Text_Content) {Content = Text_Content;}
	void setDisplayer(Display *Text_Displayer) {Displayer = Text_Displayer;}
	void Update(int MaxLength, COLORREF Text_BGColor, COLORREF Text_FGColor, Str *Text_Content);
	COLORREF GetBGColor() {return BGColor;}
	COLORREF GetFGColor() {return FGColor;}
	RECT * GetRect() {return &rect;}
	Surface * GetSprite() {return Sprite;}
	Str * GetContent() {return Content;}
	Display * getDisplayer() {return Displayer;}
};

#endif