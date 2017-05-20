#include <ctextfield.h>

TextField::TextField()
{
	textColor = RGB(0, 0, 0);
	bkgColor = RGB(255, 255, 255);
	max = -1;
	valid = true;
	listener = NULL;
}

TextField::TextField(int textColor, int bkgColor, KeyboardListener *listener, int max)
{
	this->textColor = textColor;
	this->bkgColor = bkgColor;
	this->max = max;
	valid = true;
	this->listener = listener;
}

TextField::~TextField()
{
}

void TextField::destroyAni()
{
	Visual::destroyAni();

	surface.Destroy();
}

void TextField::focus()
{
	if (!valid)
	{
		valid = true;
		updateText("", RGB(0, 0, 0));
	}

	if (listener)
		listener->setInputString(name);

	Widget::focus();
}

void TextField::restoreAni()
{
	Visual::restoreAni();

	updateContent();
}

void TextField::readInput()
{
	listener->readInputKey(max);
}

void TextField::runAnimations()
{
	Visual::runAnimations();
}

void TextField::setInvalid(Str text, int color)
{
	valid = false;

	updateText(text, color);
	unfocus();
}

bool TextField::showOnPanel(int frame, Item *Cursor)
{
	runAnimations();

	if (listener && listener->isListeningInput() && isFocused())
	{
		name = listener->getInputString();
		updateContent();
	}

	if (name.GetLength())
		displayer->Blit(position.x, position.y, &surface, NULL, NULL, DDBLT_WAIT | DDBLT_KEYSRC);

	return true;
}

void TextField::unfocus()
{
	Widget::unfocus();
}

void TextField::updateContent()
{
	if (displayer->LoadText(&surface, NULL, name, (COLORREF)bkgColor, (COLORREF)textColor))
		surface.SetColorKey(bkgColor);
}

void TextField::updateText(Str text, int textColor, int bkgColor)
{
	if (textColor != -1)
		this->textColor = textColor;

	if (bkgColor != -1)
		this->bkgColor = bkgColor;

	setName(text);

	updateContent();
}
