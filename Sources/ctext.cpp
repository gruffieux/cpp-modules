#include <ctext.h>

extern Display *Screen;

Text::Text()
{
	BGColor = FGColor = 0;
	SetRect(&rect, 0, 0, 0, 0);
	Sprite = new Surface();
	Content = NULL;
	Displayer = Screen;
}

Text::Text(COLORREF Text_BGColor, COLORREF Text_FGColor)
{
	BGColor = Text_BGColor;
	FGColor = Text_FGColor;
	SetRect(&rect, 0, 0, 0, 0);
	Sprite = new Surface();
	Content = NULL;
	Displayer = Screen;
}

void Text::Update(int MaxLength, COLORREF Text_BGColor, COLORREF Text_FGColor, Str *Text_Content)
{
	rect = Displayer->LoadText(Sprite, NULL, *Text_Content, MaxLength, DT_CALCRECT | DT_WORDBREAK);
	Sprite->SetColorKey(RGB(255, 255, 255));

	BGColor = Text_BGColor;
	FGColor = Text_FGColor;
	Content = Text_Content;
}
