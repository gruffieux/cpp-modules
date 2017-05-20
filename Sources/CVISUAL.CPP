#include <cvisual.h>
#include <canimation.h>

extern Display *Screen;

Visual::Visual() : Item()
{
	visible = mosaic = false;
	currentAni = visibleZone.left = visibleZone.right = visibleZone.top = visibleZone.bottom = 0;
	plan = 2;
	planGenerator = NULL;
	displayer = Screen;
}

Visual::Visual(Visual &model) : Item(model)
{
	currentAni = model.getCurrentAni();
	plan = model.getPlan();
	visible = model.getVisible();
	mosaic = model.getMosaic();

	visibleZone = *model.getVisibleZone();

	animations = List(*model.getAnimations());
	planGenerator = model.getPlanGenerator();
	displayer = model.getDisplayer();
}

void Visual::addAni(int bmpId, Str skinId, int nFrame, int frameWidth, int frameHeight, int colorKey, int refAngle)
{
	animations.AddElement(new Animation(bmpId));

	displayer->LoadBitmap(bmpId, Animation::getAnimationElement(animations.GetElementCount()-1, &animations)->GetSprite(), 0, refAngle);

	if (nFrame)
		Animation::getAnimationElement(animations.GetElementCount()-1, &animations)->SetFrame(nFrame, frameWidth, frameHeight);

	Animation::getAnimationElement(animations.GetElementCount()-1, &animations)->setDisplayer(displayer);
	Animation::getAnimationElement(animations.GetElementCount()-1, &animations)->MapSkin(new Integer(bmpId, skinId));
	Animation::getAnimationElement(animations.GetElementCount()-1, &animations)->SetColorKey(colorKey);
}

void Visual::addAni(Str directory, Str fileName, int nFrame, int frameWidth, int frameHeight, int colorKey, int refAngle)
{
	skinDirOrId = directory;

	animations.AddElement(new Animation(fileName));

	displayer->LoadBitmap(getAniFilePath(fileName), Animation::getAnimationElement(animations.GetElementCount()-1, &animations)->GetSprite(), 0, refAngle);

	if (nFrame)
		Animation::getAnimationElement(animations.GetElementCount()-1, &animations)->SetFrame(nFrame, frameWidth, frameHeight);

	Animation::getAnimationElement(animations.GetElementCount()-1, &animations)->setDisplayer(displayer);
	Animation::getAnimationElement(animations.GetElementCount()-1, &animations)->SetColorKey(colorKey);
}

void Visual::addAni(int index, int bmpId, Str skinId, int nFrame, int frameWidth, int frameHeight, int colorKey, int refAngle)
{
	animations.AddElement(index, new Animation(bmpId));

	displayer->LoadBitmap(bmpId, Animation::getAnimationElement(index, &animations)->GetSprite(), 0, refAngle);

	if (nFrame)
		Animation::getAnimationElement(index, &animations)->SetFrame(nFrame, frameWidth, frameHeight);

	Animation::getAnimationElement(index, &animations)->setDisplayer(displayer);
	Animation::getAnimationElement(index, &animations)->MapSkin(new Integer(bmpId, skinId));
	Animation::getAnimationElement(index, &animations)->SetColorKey(colorKey);
}

void Visual::addAni(int index, Str directory, Str fileName, int nFrame, int frameWidth, int frameHeight, int colorKey, int refAngle)
{
	skinDirOrId = directory;

	animations.AddElement(index, new Animation(fileName));

	displayer->LoadBitmap(getAniFilePath(fileName), Animation::getAnimationElement(index, &animations)->GetSprite(), 0, refAngle);

	if (nFrame)
		Animation::getAnimationElement(index, &animations)->SetFrame(nFrame, frameWidth, frameHeight);

	Animation::getAnimationElement(index, &animations)->setDisplayer(displayer);
	Animation::getAnimationElement(index, &animations)->SetColorKey(colorKey);
}

void Visual::addDynColor(int index, DynColor *color)
{
	int i;

	for (i = 0; i < animations.GetElementCount(); i++)
	{
		if (Animation::getAnimationElement(i, &animations)->GetBmpId())
			Animation::getAnimationElement(i, &animations)->GetSprite()->GetDynColors()->AddElement(index, color);
	}
}

void Visual::cascadePlanBack()
{
	int i;

	// On met l'élément devant et on cascade les autres derrière
	if (planGenerator)
		for (i = 0; i < planGenerator->GetElementCount(); i++)
			if ((Visual::getVisualElement(i, planGenerator) != this) && (Visual::getVisualElement(i, planGenerator)->getPlan() == plan))
				Visual::getVisualElement(i, planGenerator)->nextPlan(true); // Effet recursif!
}

void Visual::cascadePlanFront()
{
	int i;

	// On met l'élément derrière et on cascade les autres devant
	if (planGenerator)
		for (i = 0; i < planGenerator->GetElementCount(); i++)
			if ((Visual::getVisualElement(i, planGenerator) != this) && (Visual::getVisualElement(i, planGenerator)->getPlan() == plan))
				Visual::getVisualElement(i, planGenerator)->previousPlan(true); // Effet recursif!
}

void Visual::cloneAni(Visual *pVisual)
{
	int i, j;

	for (i = 0; i < pVisual->getAnimations()->GetElementCount(); i++)
	{
		if (Animation::getAnimationElement(i, pVisual->getAnimations())->GetBmpId())
			addAni(Animation::getAnimationElement(i, pVisual->getAnimations())->GetBmpId(pVisual->getSkinDirOrId()), pVisual->getSkinDirOrId(), Animation::getAnimationElement(i, pVisual->getAnimations())->GetFrameCount(), Animation::getAnimationElement(i, pVisual->getAnimations())->GetFrameDimension()->x, Animation::getAnimationElement(i, pVisual->getAnimations())->GetFrameDimension()->y, Animation::getAnimationElement(i, pVisual->getAnimations())->GetColorKey(), Animation::getAnimationElement(i, pVisual->getAnimations())->GetSprite()->GetRefAngle());
		else
			addAni(pVisual->getSkinDirOrId(), *Animation::getAnimationElement(i, pVisual->getAnimations())->getName(), Animation::getAnimationElement(i, pVisual->getAnimations())->GetFrameCount(), Animation::getAnimationElement(i, pVisual->getAnimations())->GetFrameDimension()->x, Animation::getAnimationElement(i, pVisual->getAnimations())->GetFrameDimension()->y, Animation::getAnimationElement(i, pVisual->getAnimations())->GetColorKey(), Animation::getAnimationElement(i, pVisual->getAnimations())->GetSprite()->GetRefAngle());
		Animation::getAnimationElement(i, pVisual->getAnimations())->setDisplayer(displayer);
		for (j = 0; j < 4; j++)
			Animation::getAnimationElement(animations.GetElementCount()-1, &animations)->SetOriginPoint(j, Animation::getAnimationElement(i, pVisual->getAnimations())->GetOriginPoint(j)->x, Animation::getAnimationElement(i, pVisual->getAnimations())->GetOriginPoint(j)->y);
	}
}

void Visual::destroyAni()
{
	int i;

	for (i = 0; i < animations.GetElementCount(); i++)
		if (animations.ElementExist(i))
			Animation::getAnimationElement(i, &animations)->GetSprite()->Destroy();
}

Str Visual::getAniFilePath(Str fileName)
{
	if (skinDirOrId.GetLength() > 0)
		return skinDirOrId + (Str)"/" + fileName;
	else
		return fileName;
}

int Visual::getBmpFrameCount(char *filename, int frameSize)
{
	BITMAP bm;
	HBITMAP hbm;

	hbm = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (GetObject(hbm, sizeof(bm), &bm))
	{
		DeleteObject(hbm);
		if (Integer(bm.bmHeight).IsDivisable(frameSize))
			return bm.bmHeight / frameSize;
		else
			return -1;
	}

	return 0;
}

bool Visual::hide()
{
	if (!visible)
		return false;

	visible = false;

	return true;
}

void Visual::initDynColors(int size)
{
	int i;

	// On supprime les couleurs uniquement dans la première Animation car c'est les mêmes dans toute
	for (i = 0; i < animations.GetElementCount(); i++)
	{
		if (i == 0)
			Animation::getAnimationElement(0, &animations)->GetSprite()->GetDynColors()->RemoveAllElement(0, true);
		Animation::getAnimationElement(i, &animations)->GetSprite()->GetDynColors()->Initialize(size);
	}
}

void Visual::nextPlan(bool cascade)
{
	if ((planGenerator != NULL) && (plan < planGenerator->GetElementCount()))
	{
		setPlan(plan + 1);
		if (cascade)
			cascadePlanBack();
	}
}

void Visual::previousPlan(bool cascade)
{
	if (plan > 1)
	{
		setPlan(plan - 1);
		if (cascade)
			cascadePlanFront();
	}
}

void Visual::restoreAni()
{
	int i;

	for (i = 0; i < animations.GetElementCount(); i++)
		if (animations.ElementExist(i))
		{
			if (Animation::getAnimationElement(i, &animations)->GetBmpId())
				displayer->LoadBitmap(Animation::getAnimationElement(i, &animations)->GetBmpId(skinDirOrId), Animation::getAnimationElement(i, &animations)->GetSprite(), Animation::getAnimationElement(i, &animations)->GetSprite()->GetCurrentAngle(), Animation::getAnimationElement(i, &animations)->GetSprite()->GetRefAngle());
			else
				displayer->LoadBitmap(getAniFilePath(*animations.GetElement(i)->getName()), Animation::getAnimationElement(i, &animations)->GetSprite(), Animation::getAnimationElement(i, &animations)->GetSprite()->GetCurrentAngle(), Animation::getAnimationElement(i, &animations)->GetSprite()->GetRefAngle());
			Animation::getAnimationElement(i, &animations)->SetColorKey();
		}
}

void Visual::rotateAni(int angle)
{
	int i, DestAngle;

	DestAngle = Animation::getAnimationElement(0, &animations)->GetSquaredAngle(angle);

	if (Animation::getAnimationElement(0, &animations)->GetSprite()->GetCurrentAngle() != DestAngle)
	{
		for (i = 0; i < animations.GetElementCount(); i++)
		{
			DestAngle = Animation::getAnimationElement(i, &animations)->GetSquaredAngle(angle);
			if (Animation::getAnimationElement(i, &animations)->GetFrameCount())
				Animation::getAnimationElement(i, &animations)->RotateFrame(DestAngle);
			Animation::getAnimationElement(i, &animations)->GetSprite()->Destroy();
			if (Animation::getAnimationElement(i, &animations)->GetBmpId())
				displayer->LoadBitmap(Animation::getAnimationElement(i, &animations)->GetBmpId(skinDirOrId), Animation::getAnimationElement(i, &animations)->GetSprite(), DestAngle, Animation::getAnimationElement(i, &animations)->GetSprite()->GetRefAngle());
			else
				displayer->LoadBitmap(getAniFilePath(*animations.GetElement(i)->getName()), Animation::getAnimationElement(i, &animations)->GetSprite(), DestAngle, Animation::getAnimationElement(i, &animations)->GetSprite()->GetRefAngle());
			Animation::getAnimationElement(i, &animations)->SetColorKey();
		}
	}
}

void Visual::replaceAniColors()
{
	int i;

	for (i = 0; i < animations.GetElementCount(); i++)
		displayer->ReplaceBitmapColors(Animation::getAnimationElement(i, &animations)->GetBmpId(skinDirOrId), Animation::getAnimationElement(i, &animations)->GetSprite());
}

bool Visual::runAni(int index, double fps)
{
	if (!animations.ElementExist(index))
		return false;

	Animation::getAnimationElement(index, &animations)->DiffuseFrame(&visibleZone, fps);
	currentAni = index;

	return true;
}

bool Visual::runAni(int index, int frame)
{
	if (!animations.ElementExist(index))
		return false;

	Animation::getAnimationElement(index, &animations)->PaintFrame(&visibleZone, frame, mosaic);
	currentAni = index;

	return true;
}

void Visual::runAnimations()
{
	setVisibleZone();

	runAni(0, 0);
}

void Visual::setPlan(short plan)
{
	this->plan = plan;

	if (planGenerator)
		planGenerator->OrderMe();
}

void Visual::setVisibleZone(int left, int right, int up, int down)
{
	if (left)
		visibleZone.left = left;
	else
		visibleZone.left = position.x;

	if (right)
		visibleZone.right = right;
	else
		visibleZone.right = position.x + dimension.x;

	if (up)
		visibleZone.top = up;
	else
		visibleZone.top = position.y;

	if (down)
		visibleZone.bottom = down;
	else
		visibleZone.bottom = position.y + dimension.y;
}

bool Visual::show()
{
	if (visible)
		return false;

	visible = true;
	setPlan(1);

	return true;
}

bool Visual::showOnPanel(int frame, Item *cursor)
{
	setVisibleZone();

	return runAni(0, frame);
}

void Visual::updateAni(Str skinDirOrId)
{
	int i;

	this->skinDirOrId = skinDirOrId;

	// On update par la ressource en relation avec la skin ou par le chemin
	for (i = 0; i < animations.GetElementCount(); i++)
	{
		Animation::getAnimationElement(i, &animations)->GetSprite()->Destroy();
		if (Animation::getAnimationElement(i, &animations)->GetBmpId())
			displayer->LoadBitmap(Animation::getAnimationElement(i, &animations)->GetBmpId(skinDirOrId), Animation::getAnimationElement(i, &animations)->GetSprite(), Animation::getAnimationElement(i, &animations)->GetSprite()->GetCurrentAngle(), Animation::getAnimationElement(i, &animations)->GetSprite()->GetRefAngle());
		else
			displayer->LoadBitmap(getAniFilePath(*animations.GetElement(i)->getName()), Animation::getAnimationElement(i, &animations)->GetSprite(), Animation::getAnimationElement(i, &animations)->GetSprite()->GetCurrentAngle(), Animation::getAnimationElement(i, &animations)->GetSprite()->GetRefAngle());
		Animation::getAnimationElement(i, &animations)->SetColorKey();
	}
}
