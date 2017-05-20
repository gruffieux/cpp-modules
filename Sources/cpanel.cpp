#include <cpanel.h>
#include <canimation.h>
#include <cdatafilelist.h>

extern Display *Screen;

Panel::Panel()
{
	int i;

	ColSpace = RowSpace = BorderSpace = 0;
	save = move = false;
	sizePolicy = RESIZE_AND_ADJUST;
	Thread = NULL;
	Popup = new Text();
	CloseButton = NULL;

	for (i = 0; i < TOTAL_BORDER; i++)
	{
		border[i] = NULL;
		borderCorner[i] = NULL;
	}

	InitGrid(0, 0);

	displayer = Screen;
	Popup->setDisplayer(displayer);
}

Panel::Panel(int Panel_ColCount, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, Text Panel_Popup, Str Object_Name) :
Item(NULL, NULL, NULL, NULL, Object_Name)
{
	int i;

	ColSpace = Panel_ColSpace;
	RowSpace = Panel_RowSpace;
	BorderSpace = Panel_BorderSpace;
	save = Panel_save;
	move = false;
	sizePolicy = RESIZE_AND_ADJUST;
	Thread = NULL;
	Popup = new Text(Panel_Popup.GetBGColor(), Panel_Popup.GetFGColor());
	CloseButton = NULL;

	for (i = 0; i < TOTAL_BORDER; i++)
	{
		border[i] = NULL;
		borderCorner[i] = NULL;
	}

	InitGrid(Panel_ColCount, Panel_RowCount);

	displayer = Screen;
	Popup->setDisplayer(displayer);
}

Panel::Panel(int Panel_ColCount, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, Text Panel_Popup, int Item_width, int Item_height, Str Object_Name) :
Item(NULL, NULL, Item_width, Item_height, Object_Name)
{
	int i;

	ColSpace = Panel_ColSpace;
	RowSpace = Panel_RowSpace;
	BorderSpace = Panel_BorderSpace;
	save = Panel_save;
	move = false;
	sizePolicy = FIXED;
	Thread = NULL;
	Popup = new Text(Panel_Popup.GetBGColor(), Panel_Popup.GetFGColor());
	CloseButton = NULL;

	for (i = 0; i < TOTAL_BORDER; i++)
	{
		border[i] = NULL;
		borderCorner[i] = NULL;
	}

	InitGrid(Panel_ColCount, Panel_RowCount);

	displayer = Screen;
	Popup->setDisplayer(displayer);
}

Panel::Panel(int Panel_ColCount, int Panel_RowCount, int Panel_ColSpace, int Panel_RowSpace, int Panel_BorderSpace, bool Panel_save, Text Panel_Popup, int Item_xpos, int Item_ypos, int Item_width, int Item_height, Str Object_Name) :
Item(Item_xpos, Item_ypos, Item_width, Item_height, Object_Name)
{
	int i;

	ColSpace = Panel_ColSpace;
	RowSpace = Panel_RowSpace;
	BorderSpace = Panel_BorderSpace;
	save = Panel_save;
	move = false;
	sizePolicy = FIXED;
	Thread = NULL;
	Popup = new Text(Panel_Popup.GetBGColor(), Panel_Popup.GetFGColor());
	CloseButton = NULL;
	
	for (i = 0; i < TOTAL_BORDER; i++)
	{
		border[i] = NULL;
		borderCorner[i] = NULL;
	}

	InitGrid(Panel_ColCount, Panel_RowCount);

	displayer = Screen;
	Popup->setDisplayer(displayer);
}

Panel::Panel(Panel &model) : Visual(model)
{
	int i, x, y;

	ColCount = model.GetColCount();
	RowCount = model.GetRowCount();
	ColSpace = model.GetColSpace();
	RowSpace = model.GetRowSpace();
	BorderSpace = model.GetBorderSpace();
	save = model.GetSave();
	sizePolicy = model.getSizePolicy();
	move = model.GetMove();
	CursorDistance = *model.GetCursorDistance();
	Thread = NULL;

	for (x = 0; x < ColCount; x++)
		for (y = 0; y < RowCount; y++)
			Grid[y][x] = *model.GetGrid(x, y);

	Popup = new Text(*model.GetPopup());
	CloseButton = model.GetCloseButton();
	
	for (i = 0; i < TOTAL_BORDER; i++)
	{
		border[i] = model.getBorder(i);
		borderCorner[i] = model.getBorderCorner(i);
	}

	for (i = 0; i < Forbidden1.GetElementCount(); i++)
		*GetForbidden1()->GetElement(i) = *model.GetForbidden1()->GetElement(i);
	
	for (i = 0; i < Forbidden2.GetElementCount(); i++)
		*GetForbidden2()->GetElement(i) = *model.GetForbidden2()->GetElement(i);

	displayer = model.getDisplayer();
	Popup->setDisplayer(displayer);
}

Panel::~Panel()
{
	int i;

	if (CloseButton)
		delete CloseButton;

	for (i = 0; i < TOTAL_BORDER; i++)
	{
		if (border[i])
			delete border[i];
		if (borderCorner[i])
			delete borderCorner[i];
	}

	for (i = 0; i < RowCount; i++)
		delete [] Grid[i];

	delete Popup;
}

void Panel::SetEdge()
{
	// On définit à quel bord le panel doit coller
	Edge.west =  (position.x == 0);
	Edge.east = (position.x + dimension.x == displayer->GetCurrentMode()->GetWidth());
	Edge.north = (position.y == 0);
	Edge.south =  (position.y + dimension.y == displayer->GetCurrentMode()->GetHeight());
}

void Panel::AlignCol(int x)
{
	int y;

	for (y = 0; y < RowCount; y++)
		Grid[y][x].AlignCol = true;
}

void Panel::AlignCols()
{
	int x, y;

	for (x = 0; x < ColCount; x++)
		for (y = 0; y < RowCount; y++)
			Grid[y][x].AlignCol = true;
}

void Panel::AlignColsOnRow(int y)
{
	int x;

	for (x = 0; x < ColCount; x++)
		Grid[y][x].AlignCol = true;
}

void Panel::AlignRow(int y)
{
	int x;

	for (x = 0; x < ColCount; x++)
		Grid[y][x].AlignRow = true;
}

void Panel::AlignRows()
{
	int x, y;

	for (x = 0; x < ColCount; x++)
		for (y = 0; y < RowCount; y++)
			Grid[y][x].AlignRow = true;
}

void Panel::AlignRowsOnCol(int x)
{
	int y;

	for (y = 0; y < RowCount; y++)
		Grid[y][x].AlignRow = true;
}

void Panel::AlignColContent(int x, DATA_HALIGN halign, DATA_VALIGN valign)
{
	int y;

	for (y = 0; y < RowCount; y++)
	{
		Grid[y][x].halign = halign;
		Grid[y][x].valign = valign;
	}
}

void Panel::AlignRowContent(int y, DATA_HALIGN halign, DATA_VALIGN valign)
{
	int x;

	for (x = 0; x < ColCount; x++)
	{
		Grid[y][x].halign = halign;
		Grid[y][x].valign = valign;
	}
}

bool Panel::show()
{
	if (!Visual::show())
		return false;

	cascadePlanBack();

	return true;
}

bool Panel::hide()
{
	if (!Visual::hide())
		return false;

	return true;
}

bool Panel::Refresh()
{
	Thread = NULL;

	GenerateShape();
	
	return true;
}

void Panel::WaitForRefresh()
{
	if (!Thread)
		Thread = CreateThread(NULL, 0, RefreshBySignal, this, CREATE_SUSPENDED, NULL);
}

void Panel::StartRefresh()
{
	if (Thread)
		ResumeThread(Thread);
}

void Panel::InitGrid(int Panel_ColCount, int Panel_RowCount)
{
	int i;

	ColCount = Panel_ColCount;
	RowCount = Panel_RowCount;

	Grid = new Cell*[RowCount];

	for (i = 0; i < RowCount; i++)
		Grid[i] = new Cell[ColCount];
}

void Panel::Init()
{
	int x, y;

	for (x = 0; x < ColCount; x++)
		for (y = 0; y < RowCount; y++)
			switch (Grid[y][x].data)
			{
			case TEXT:
				Grid[y][x].Content = new Text();
				GetTextData(x, y)->setDisplayer(displayer);
				break;
			case VISUAL:
				Grid[y][x].Content = new Visual();
				GetVisualData(x, y)->setDisplayer(displayer);
				break;
			default:
				Grid[y][x].Content = NULL;
				break;
			}

	if (CloseButton)
		AddForbidden1(CloseButton);
}

void Panel::InitBackground(int BmpId, Str SkinId, int width, int height, bool mosaic, int colorKey)
{
	setMosaic(mosaic);

	addAni(BmpId, SkinId, 1, width, height, colorKey, 0);
}

void Panel::InitBackground(Str BmpDirectory, Str BmpFileName, int width, int height, bool mosaic, int colorKey)
{
	setMosaic(mosaic);

	addAni(BmpDirectory, BmpFileName, 1, width, height, colorKey, 0);
}

void Panel::initBorders(int BmpId, Str SkinId, int width, int height, bool mosaic)
{
	int i;

	for (i = 0; i < TOTAL_BORDER; i++)
	{
		border[i] = new Visual();
		border[i]->setDisplayer(displayer);
		border[i]->setMosaic(mosaic);
		border[i]->addAni(BmpId, SkinId, 1, width, height, -1, 0);
		border[i]->rotateAni(i*90);
	}
}

void Panel::initBorders(Str BmpDirectory, Str BmpFileName, int width, int height, bool mosaic)
{
	int i;

	for (i = 0; i < TOTAL_BORDER; i++)
	{
		border[i] = new Visual();
		border[i]->setDisplayer(displayer);
		border[i]->setMosaic(mosaic);
		border[i]->addAni(BmpDirectory, BmpFileName, 1, width, height, -1, 0);
		border[i]->rotateAni(i*90);
	}
}

void Panel::initBorder(int index, int BmpId, Str SkinId, int width, int height, int colorKey, bool mosaic)
{
	border[index] = new Visual();
	border[index]->setDisplayer(displayer);
	border[index]->setMosaic(mosaic);
	border[index]->addAni(BmpId, SkinId, 1, width, height, colorKey, 0);
}

void Panel::initBorder(int index, Str BmpDirectory, Str BmpFileName, int width, int height, int colorKey, bool mosaic)
{
	border[index] = new Visual();
	border[index]->setDisplayer(displayer);
	border[index]->setMosaic(mosaic);
	border[index]->addAni(BmpDirectory, BmpFileName, 1, width, height, colorKey, 0);
}

void Panel::initBorderCorners(int BmpId, Str SkinId, int width, int height, int colorKey)
{
	int i;

	for (i = 0; i < TOTAL_BORDER; i++)
	{
		borderCorner[i] = new Visual();
		borderCorner[i]->setDisplayer(displayer);
		borderCorner[i]->addAni(BmpId, SkinId, TOTAL_BORDER, width, height, colorKey, 0);
	}
}

void Panel::initBorderCorners(Str BmpDirectory, Str BmpFileName, int width, int height, int colorKey)
{
	int i;

	for (i = 0; i < TOTAL_BORDER; i++)
	{
		borderCorner[i] = new Visual();
		borderCorner[i]->setDisplayer(displayer);
		borderCorner[i]->addAni(BmpDirectory, BmpFileName, TOTAL_BORDER, width, height, colorKey, 0);
	}
}

void Panel::InitCloseButton(int BmpId, Str SkinId, int width, int height)
{
	CloseButton = new Visual();

	CloseButton->setDisplayer(displayer);
	CloseButton->addAni(BmpId, SkinId, 1, width, height, RGB(255, 255, 255), 90);
	*CloseButton->getDimension() = Axe(width, height);
}

void Panel::InitCloseButton(Str BmpDirectory, Str BmpFileName, int width, int height)
{
	CloseButton = new Visual();

	CloseButton->setDisplayer(displayer);
	CloseButton->addAni(BmpDirectory, BmpFileName, 1, width, height, RGB(255, 255, 255), 90);
	*CloseButton->getDimension() = Axe(width, height);
}

void Panel::Initialize()
{
	int x, y;
	Text *pText;
	Visual *pVisual;

	for (x = 0; x < ColCount; x++)
		for (y = 0; y < RowCount; y++)
		{
			pText = GetTextData(x, y);
			if (pText)
			{
				displayer->LoadText(pText->GetSprite(), NULL, 0, RGB(255, 255, 255), RGB(255, 255, 255));
				pText->GetSprite()->SetColorKey(RGB(255, 255, 255));
			}
			pVisual = GetVisualData(x, y);
			if ((pVisual != NULL) && (pVisual->getAnimations()->ElementExist(0) == true))
			{
				if (!pVisual->getDimension()->x)
					pVisual->getDimension()->x = Animation::getAnimationElement(0, pVisual->getAnimations())->GetFrameDimension()->x;
				if (!pVisual->getDimension()->y)
					pVisual->getDimension()->y = Animation::getAnimationElement(0, pVisual->getAnimations())->GetFrameDimension()->y;
			}
		}
}

void Panel::GenerateCursorDistance(Item *pItem)
{
	CursorDistance.x = pItem->getPosition()->x - position.x;
	CursorDistance.y = pItem->getPosition()->y - position.y;
}

void Panel::restoreAni()
{
	int i;

	Visual::restoreAni();

	if (CloseButton)
		CloseButton->restoreAni();

	for (i = 0; i < TOTAL_BORDER; i++)
	{
		if (border[i])
			border[i]->restoreAni();
		if (borderCorner[i])
			borderCorner[i]->restoreAni();
	}
}

void Panel::destroyAni()
{
	int i;

	Visual::destroyAni();

	if (CloseButton)
		CloseButton->destroyAni();

	for (i = 0; i < TOTAL_BORDER; i++)
	{
		if (border[i])
			border[i]->destroyAni();
		if (borderCorner[i])
			borderCorner[i]->destroyAni();
	}
}

void Panel::DestroyText()
{
	int x, y;
	Text *pText;

	Popup->GetSprite()->Destroy();

	for (x = 0; x < ColCount; x++)
		for (y = 0; y < RowCount; y++)
		{
			pText = GetTextData(x, y);
			if (pText)
				pText->GetSprite()->Destroy();
		}
}

void Panel::StickToEdge()
{
	if (position.x < 0)
		Edge.west = 1;

	if (position.x + dimension.x > displayer->GetCurrentMode()->GetWidth())
		Edge.east = 1;

	if (position.y < 0)
		Edge.north = 1;

	if (position.y + dimension.y > displayer->GetCurrentMode()->GetHeight())
		Edge.south = 1;

	// On repositionne le panel sur le bord de droite
	if (Edge.east)
		if (position.x + dimension.x != displayer->GetCurrentMode()->GetWidth())
			position.x = displayer->GetCurrentMode()->GetWidth() - dimension.x;

	// On repositionne le panel sur le bord de gauche
	if (Edge.west)
		if (position.x != 0)
			position.x = 0;

	// On repositionne le panel sur le bord du bas
	if (Edge.south)
		if (position.y + dimension.y != displayer->GetCurrentMode()->GetHeight())
			position.y = displayer->GetCurrentMode()->GetHeight() - dimension.y;

	// On repositionne le panel sur le bord du haut
	if (Edge.north)
		if (position.y != 0)
			position.y = 0;
}

void Panel::GenerateShape()
{
	int i, x, y, *ColWidth, *RowHeight, ColLength, *RowLength;
	Axe NewDimension;
	Text *pText;
	Visual *pVisual;

	ColWidth = new int[ColCount];
	RowHeight = new int[RowCount];
	RowLength = new int[RowCount];

	for (x = 0; x < ColCount; x++)
		ColWidth[x] = 0;

	for (y = 0; y < RowCount; y++)
	{
		RowHeight[y] = 0;
		RowLength[y] = 0;
	}

	// On dimensionne les cellules
	for (x = 0; x < ColCount; x++)
		for (y = 0; y < RowCount; y++)
		{
			Grid[y][x].width = Grid[y][x].height = 0;
			switch (Grid[y][x].data)
			{
			case TEXT:
				pText = GetTextData(x, y);
				if (pText)
				{
					if (!Grid[y][x].width)
						Grid[y][x].width = pText->GetSprite()->GetWidth();
					if (!Grid[y][x].height)
						Grid[y][x].height = pText->GetSprite()->GetHeight();
				}
				break;
			default:
				pVisual = GetVisualData(x, y);
				if (pVisual)
				{
					if (!Grid[y][x].width)
						Grid[y][x].width = (int)pVisual->getDimension()->x;
					if (!Grid[y][x].height)
						Grid[y][x].height = (int)pVisual->getDimension()->y;
				}
				break;
			}
			if ((Grid[y][x].width > ColWidth[x]) && (Grid[y][x].ColSpan == 0))
				ColWidth[x] = Grid[y][x].width;
			if ((Grid[y][x].height > RowHeight[y]) && (Grid[y][x].RowSpan == 0))
				RowHeight[y] = Grid[y][x].height;
		}

	// On aligne les cellules et on calcul la longueur et largeur total
	for (x = 0; x < ColCount; x++)
	{
		ColLength = 0;
		for (y = 0; y < RowCount; y++)
		{
			if ((Grid[y][x].width < ColWidth[x]) && (Grid[y][x].AlignCol == true))
				Grid[y][x].width = ColWidth[x];
			if ((Grid[y][x].height < RowHeight[y]) && (Grid[y][x].AlignRow == true))
				Grid[y][x].height = RowHeight[y];
			ColLength += (Grid[y][x].height + RowSpace);
			RowLength[y] += (Grid[y][x].width + ColSpace);
			if (RowLength[y] > NewDimension.x)
				NewDimension.x = RowLength[y] + BorderSpace;
		}
		if (ColLength > NewDimension.y)
			NewDimension.y = ColLength + BorderSpace;
	}

	// On redimensionne le panel avec la longeur et largeur calculée
	switch (sizePolicy)
	{
	case RESIZE:
		dimension.x = NewDimension.x;
		dimension.y = NewDimension.y;
		break;
	case RESIZE_AND_ADJUST:
		if (animations.ElementExist(0))
		{
			dimension.x = (NewDimension.x / Animation::getAnimationElement(0, &animations)->GetSprite()->GetWidth() + 1) * Animation::getAnimationElement(0, &animations)->GetSprite()->GetWidth();
			dimension.y = (NewDimension.y / Animation::getAnimationElement(0, &animations)->GetSprite()->GetHeight() + 1) * Animation::getAnimationElement(0, &animations)->GetSprite()->GetHeight();
		}
		else
		{
			dimension.x = NewDimension.x;
			dimension.y = NewDimension.y;
		}
		break;
	}

	// On redimensionne les bordures
	for (i = 0; i < TOTAL_BORDER; i++)
	{
		if (borderCorner[i])
			*borderCorner[i]->getDimension() = Axe(BorderSpace, BorderSpace);
		if (border[i])
		{
			switch (i)
			{
			case 0:
				x = dimension.x;
				y = BorderSpace;
				break;
			case 1:
				x = BorderSpace;
				y = dimension.y;
				break;
			case 2:
				x = dimension.x;
				y = BorderSpace;
				break;
			case 3:
				x = BorderSpace;
				y = dimension.y;
				break;
			}
			*border[i]->getDimension() = Axe(x, y);
		}
	}

	// On span les cellules
	for (x = 0; x < ColCount; x++)
		for (y = 0; y < RowCount; y++)
		{
			// Colonnes
			if (Grid[y][x].ColSpan == -1)
			{
				Grid[y][x].width = ColWidth[x];
				for (i = x+1; i < ColCount; i++)
					Grid[y][x].width += ColWidth[i];
				i = Grid[y][x].width;
				Grid[y][x].width += dimension.x - i;
			}
			else
				if (Grid[y][x].ColSpan > 0)
				{
					Grid[y][x].width = ColWidth[x];
					for (i = 0; i < Grid[y][x].ColSpan; i++)
						Grid[y][x].width += ColWidth[i+x+1];
				}

			// Lignes
			if (Grid[y][x].RowSpan == -1)
			{
				Grid[y][x].height = RowHeight[y];
				for (i = y+1; i < RowCount; i++)
					Grid[y][x].height += RowHeight[i];
				i = Grid[y][x].height;
				Grid[y][x].height += dimension.y - i;
			}
			else
				if (Grid[y][x].RowSpan > 0)
				{
					Grid[y][x].height = RowHeight[y];
					for (i = 0; i < Grid[y][x].RowSpan; i++)
						Grid[y][x].height += RowHeight[i+y+1];
				}
		}

	delete [] ColWidth;
	delete [] RowHeight;
	delete [] RowLength;
}

bool Panel::GenerateForbidden2()
{
	int i;

	if (!planGenerator) return false;

	for (i = 0; i < planGenerator->GetElementCount(); i++)
		if ((collision(Item::getItemElement(i, planGenerator)) == true) && (Visual::getVisualElement(i, planGenerator)->getVisible() == true) && (getVisible() == true) && (Visual::getVisualElement(i, planGenerator)->getPlan() > getPlan()))
			Panel::getPanelElement(i, planGenerator)->AddForbidden2(this);
		else
			Panel::getPanelElement(i, planGenerator)->RemoveForbidden2(this);

	return true;
}

bool Panel::ClickOnGrid(int x, int y, bool waited, Cursor *cursor, bool press)
{
	Cursor::ClickState state;

	Item *pItem = dynamic_cast<Item*>(Grid[y][x].Content);

	if (press)
		state = cursor->buttonPressed();
	else
		state = cursor->buttonReleased();

	if (pItem && waited && state != Cursor::NO_CLICK && CanClick(cursor))
		return cursor->collision(pItem) && Grid[y][x].display;

	return false;
}

bool Panel::generate(bool waited, Cursor *cursor)
{
	// Evénement sur le bouton de fermeture
	if (waited && cursor->buttonPressed() == Cursor::LEFT_CLICK && CanClick(cursor))
		if (CloseButton && cursor->collision(CloseButton))
			hide();

	return true;
}

bool Panel::Run(Item *Cursor)
{
	int i, x, y, PopupsCount;
	Axe *PopupsCoord, InnerPos;
	Text *pText;
	Visual *pVisual;

	if (Thread)
		return false;

	PopupsCount = 0;
	PopupsCoord = new Axe[ColCount*RowCount];

	StickToEdge();

	DataPosition.x = position.x + BorderSpace;
	DataPosition.y = position.y + BorderSpace;

	// Affichage du fond
	if (animations.ElementExist(0))
		Visual::runAnimations();

	// On affiche les bordures
	for (i = 0; i < TOTAL_BORDER; i++)
	{
		if (borderCorner[i])
		{
			switch (i)
			{
			case 0:
				x = position.x - BorderSpace;
				y = position.y - BorderSpace;
				break;
			case 1:
				x = position.x + dimension.x;
				y = position.y - BorderSpace;
				break;
			case 2:
				x = position.x + dimension.x;
				y = position.y + dimension.y;
				break;
			case 3:
				x = position.x - BorderSpace;
				y = position.y + dimension.y;
				break;
			}
			borderCorner[i]->stickToPanel(Axe(x, y));
			borderCorner[i]->setVisibleZone();
			borderCorner[i]->runAni(0, i);
		}
		if (border[i])
		{
			switch (i)
			{
			case 0:
				if (borderCorner[i])
				{
					x = position.x;
					y = position.y - BorderSpace;
				}
				else
				{
					x = position.x;
					y = position.y;
				}
				break;
			case 1:
				if (borderCorner[i])
				{
					x = position.x + dimension.x;
					y = position.y;
				}
				else
				{
					x = position.x + dimension.x - BorderSpace;
					y = position.y;
				}
				break;
			case 2:
				if (borderCorner[i])
				{
					x = position.x;
					y = position.y + dimension.y;
				}
				else
				{
					x = position.x;
					y = position.y + dimension.y - BorderSpace;
				}
				break;
			case 3:
				if (borderCorner[i])
				{
					x = position.x - BorderSpace;
					y = position.y;
				}
				else
				{
					x = position.x;
					y = position.y;
				}
				break;
			}
			border[i]->stickToPanel(Axe(x, y));
			border[i]->runAnimations();
		}
	}

	// Positionnement des cellules et affichage de celles-ci
	for (x = 0; x < ColCount; x++)
		for (y = 0; y < RowCount; y++)
		{
			InnerPos = Axe();

			// On définit la position horizontale à l'intérieur de la cellule en fonction de l'alignement
			switch (Grid[y][x].halign)
			{
			case HALIGN_LEFT:
				InnerPos.x = 0;
				break;
			case HALIGN_RIGHT:
				InnerPos.x = Grid[y][x].width;
				break;
			case HALIGN_CENTER:
				pText = GetTextData(x, y);
				pVisual = GetVisualData(x, y);
				InnerPos.x = Grid[y][x].width / 2;
				if (pText)
					InnerPos.x -= pText->GetSprite()->GetWidth() / 2;
				if (pVisual)
					InnerPos.x -= pVisual->getDimension()->x / 2;
				break;
			}

			// On définit la position verticale à l'intérieur de la cellule en fonction de l'alignement
			switch (Grid[y][x].valign)
			{
			case VALIGN_TOP:
				InnerPos.y = 0;
				break;
			case VALIGN_BOTTOM:
				InnerPos.y = Grid[y][x].height;
				break;
			case VALIGN_CENTER:
				pText = GetTextData(x, y);
				pVisual = GetVisualData(x, y);
				InnerPos.y = Grid[y][x].height / 2;
				if (pText)
					InnerPos.y -= pText->GetSprite()->GetHeight() / 2;
				if (pVisual)
					InnerPos.y -= pVisual->getDimension()->y / 2;
				break;
			}

			// On positionne la cellule après la précédente
			if (x)
				Grid[y][x].posx = Grid[y][x-1].posx + Grid[y][x-1].width + ColSpace;
			if (y)
				Grid[y][x].posy = Grid[y-1][x].posy + Grid[y-1][x].height + RowSpace;

			// On affiche les données
			switch (Grid[y][x].data)
			{
			case TEXT:
				pText = GetTextData(x, y);
				if ((Grid[y][x].display == true) && (pText != NULL))
				{
					displayer->Blit((int)DataPosition.x + Grid[y][x].posx + InnerPos.x, (int)DataPosition.y + Grid[y][x].posy + InnerPos.y, pText->GetSprite(), NULL, pText->GetRect(), DDBLT_WAIT | DDBLT_KEYSRC);
					if (pText->GetContent())
					{
						displayer->BlitColor(pText->GetRect(), pText->GetSprite()->GetDDBltFX(), pText->GetBGColor(), DDBLT_COLORFILL | DDBLT_WAIT);
						displayer->ShowText(pText->GetContent()->Get(), pText->GetRect(), pText->GetBGColor(), pText->GetFGColor(), DT_WORDBREAK);
					}
				}
				break;
			default:
				pVisual = GetVisualData(x, y);
				if (pVisual)
				{
					pVisual->stickToPanel(Axe(DataPosition.x + Grid[y][x].posx + InnerPos.x, DataPosition.y + Grid[y][x].posy + InnerPos.y));
					if (Grid[y][x].display)
						pVisual->showOnPanel(Grid[y][x].PicFrame, Cursor);
				}
				break;
			}

			// On ajoute le popup si il y'en a un pour cette cellule
			if (Grid[y][x].popup)
			{
				PopupsCoord[PopupsCount].x = x;
				PopupsCoord[PopupsCount].y = y;
				PopupsCount++;
			}
		}

	// On affiche le bouton de fermeture
	if (CloseButton)
	{
		CloseButton->stickToPanel(Axe(position.x + dimension.x - CloseButton->getDimension()->x - BorderSpace, position.y + BorderSpace));
		CloseButton->runAnimations();
	}

	// On affiche le popup où le curseur passe
	for (i = 0; i < PopupsCount; i++)
	{
		x = PopupsCoord[i].x;
		y = PopupsCoord[i].y;
		if ((Cursor != NULL) && (Cursor->collision((int)DataPosition.x + Grid[y][x].posx, (int)DataPosition.x + Grid[y][x].posx + Grid[y][x].width, (int)DataPosition.y + Grid[y][x].posy, (int)DataPosition.y + Grid[y][x].posy + Grid[y][x].height) == true))
		{
			displayer->LoadText(Popup->GetSprite(), NULL, Grid[y][x].PopupText, Popup->GetBGColor(), Popup->GetFGColor());
			displayer->Blit(Cursor->getPosition()->x, Cursor->getPosition()->y - Popup->GetSprite()->GetHeight(), Popup->GetSprite(), NULL, NULL, NULL);
		}
	}

	delete [] PopupsCoord;

	return true;
}

bool Panel::showOnPanel(int frame, Item *cursor)
{
	Run(cursor);

	return true;
}

bool Panel::CanMove(Item *pItem)
{
	int i;

	for (i = 0; i < Forbidden1.GetElementCount(); i++)
		if (pItem->collision(Item::getItemElement(i, &Forbidden1)))
			return false;

	for (i = 0; i < Forbidden2.GetElementCount(); i++)
		if (pItem->collision(Item::getItemElement(i, &Forbidden2)))
			return false;

	return true;
}

bool Panel::CanClick(Item *pItem)
{
	int i;

	for (i = 0; i < Forbidden2.GetElementCount(); i++)
		if (pItem->collision(Item::getItemElement(i, &Forbidden2)))
			return false;

	return true;
}

///////////////////////
// Fonctions friends //
///////////////////////

Panel *Sticky;

bool LoadPanels(List *pList)
{
	int i, show;
	Panel *pPanel;
	DataFileList File = DataFileList("panels.dat");

	if (!File.Open(true, false, false))
	{
		File.SetFileName("panels_default.dat");
		if (!File.Open(true, false, false))
		{
			for (i = 0; i < pList->GetElementCount(); i++)
			{
				pPanel = Panel::getPanelElement(i, pList);
				if (pPanel) pPanel->Init();
			}
			return false;
		}
	}

	for (i = 0; i < pList->GetElementCount(); i++)
	{
		pPanel = Panel::getPanelElement(i, pList);
		if (!pPanel) return false;

		pPanel->Init();

		if (pPanel->GetSave())
		{
			pPanel->getPosition()->x = File.Read("position_x", false, *pPanel->getName(), false);
			pPanel->getPosition()->y = File.Read("position_y", false, *pPanel->getName(), false);
			pPanel->GetEdge()->west = File.Read("edge_west", false, *pPanel->getName(), false);
			pPanel->GetEdge()->east = File.Read("edge_east", false, *pPanel->getName(), false);
			pPanel->GetEdge()->north = File.Read("edge_north", false, *pPanel->getName(), false);
			pPanel->GetEdge()->south = File.Read("edge_south", false, *pPanel->getName(), false);
			pPanel->setPlan(File.Read("plan", false, *pPanel->getName(), false));
			show = File.Read("show", false, *pPanel->getName(), false);

			if (show == 1)
				pPanel->show();
			else
				pPanel->hide();
		}
	}

	File.Close();

	return true;
}

bool SavePanels(List *pList)
{
	int i;
	Panel *pPanel;
	DataFileList File = DataFileList("panels.dat");

	if (!File.Open(false, true, true))
		return false;

	for (i = 0; i < pList->GetElementCount(); i++)
	{
		pPanel = Panel::getPanelElement(i, pList);
		if (!pPanel) return false;

		if (pPanel->GetSave())
		{
			File.Write(pPanel->getPosition()->x, "position_x", false, *pPanel->getName(), false);
			File.Write(pPanel->getPosition()->y, "position_y", false, *pPanel->getName(), false);
			File.Write(pPanel->GetEdge()->west, "edge_west", false, *pPanel->getName(), false);
			File.Write(pPanel->GetEdge()->east, "edge_east", false, *pPanel->getName(), false);
			File.Write(pPanel->GetEdge()->north, "edge_north", false, *pPanel->getName(), false);
			File.Write(pPanel->GetEdge()->south, "edge_south", false, *pPanel->getName(), false);
			File.Write(pPanel->getPlan(), "plan", false, *pPanel->getName(), false);
			File.Write((int)pPanel->getVisible(), "show", false, *pPanel->getName(), false);
		}
	}

	File.Close();

	return true;
}

void MovePanel(Panel *pPanel, Cursor *cursor, List PanelPack, bool MoveBlocked)
{
	if (!MoveBlocked)
		if (cursor->buttonPressed() == Cursor::LEFT_CLICK && cursor->collision(pPanel))
			if (pPanel->GetMove())
				StopPanel(pPanel, cursor->getMover());
			else
				if (!Sticky)
					if (pPanel->CanMove(cursor))
					{
						pPanel->SetMove(true);
						pPanel->setPlan(1);
						pPanel->cascadePlanBack();
						Sticky = pPanel;
						pPanel->GenerateCursorDistance(cursor);
						cursor->getMover()->getPosition()->x -= pPanel->GetCursorDistance()->x;
						cursor->getMover()->getPosition()->y -= pPanel->GetCursorDistance()->y;
						*cursor->getMover()->getDimension() = *pPanel->getDimension();
					}
					else
						StopPanel(pPanel, cursor->getMover());

	if (pPanel->GetMove())
	{
		*pPanel->GetEdge() = CardinalPoint(0, 0, 0, 0);
		pPanel->getPosition()->x = cursor->getPosition()->x - pPanel->GetCursorDistance()->x;
		pPanel->getPosition()->y = cursor->getPosition()->y - pPanel->GetCursorDistance()->y;
	}
}

void StopPanel(Panel *pPanel, Mover *CursorMovement)
{
	if (pPanel->GetMove())
	{
		pPanel->SetMove(false);
		pPanel->SetEdge();
		Sticky = NULL;
		CursorMovement->getPosition()->x += pPanel->GetCursorDistance()->x;
		CursorMovement->getPosition()->y += pPanel->GetCursorDistance()->y;
		*CursorMovement->getDimension() = Axe(10, 10);
	}
}

void StickToCursor(Item *Cursor, Mover *CursorMovement)
{
	if (Sticky)
	{
		Cursor->getPosition()->x = CursorMovement->getPosition()->x + Sticky->GetCursorDistance()->x;
		Cursor->getPosition()->y = CursorMovement->getPosition()->y + Sticky->GetCursorDistance()->y;
	}
	else
	{
		Cursor->getPosition()->x = CursorMovement->getPosition()->x;
		Cursor->getPosition()->y = CursorMovement->getPosition()->y;
	}
}

DWORD WINAPI RefreshBySignal(LPVOID lParam)
{
	DWORD result = -1;
	Panel *pPanel;

	pPanel = reinterpret_cast<Panel*>(lParam);

	if (pPanel)
		result = pPanel->Refresh();

	ExitThread(result);
}
