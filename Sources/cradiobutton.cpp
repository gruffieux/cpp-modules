#include <cradiobutton.h>

RadioButton::RadioButton(Str labelText, int textColor, int textBackColor)
{
	checked = false;

	if (labelText.GetLength())
	{
		this->textColor = textColor;
		this->textBackColor = textBackColor;
		setName(labelText);
		label = new Surface();
		updateLabel();
	}
	else
		label = NULL;
}

RadioButton::~RadioButton()
{
	if (label)
		delete label;
}

void RadioButton::destroyAni()
{
	Visual::destroyAni();

	if (label)
		label->Destroy();
}

void RadioButton::focus()
{
	checked = true;

	Widget::focus();
}

void RadioButton::initBackground(int bmpId, Str skinId, int width, int height, bool mosaic, int colorKey)
{
	setMosaic(mosaic);

	addAni(bmpId, skinId, 2, width, height, colorKey, 90);
}

void RadioButton::restoreAni()
{
	Visual::restoreAni();

	updateLabel();
}

void RadioButton::runAnimations()
{
	setVisibleZone();

	if (!checked)
		runAni(0, 0);
	else
		runAni(0, 1);

	if (label)
		displayer->Blit(position.x + dimension.x + 5, position.y, label, NULL, NULL, DDBLT_WAIT | DDBLT_KEYSRC);
}

bool RadioButton::showOnPanel(int frame, Item *Cursor)
{
	runAnimations();

	return true;
}

void RadioButton::unfocus()
{
	checked = false;

	Widget::unfocus();
}

void RadioButton::updateLabel()
{
	if (label && displayer->LoadText(label, NULL, name, (COLORREF)textBackColor, (COLORREF)textColor))
		label->SetColorKey(textBackColor);
}
