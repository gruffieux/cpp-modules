#include <canimation.h>

extern Display *Screen;

Animation::Animation()
{
	FrameCount = 1;
	BmpId = CurrentFrame = 0;
	ColorKey = -1;
	FrameHit = new int[FrameCount];
	Frame = new RECT[FrameCount];
	Displayer = Screen;
}

Animation::Animation(int Animation_BmpId)
{
	FrameCount = 1;
	CurrentFrame = 0;
	ColorKey = -1;
	FrameHit = new int[FrameCount];
	Frame = new RECT[FrameCount];
	BmpId = Animation_BmpId;
	Displayer = Screen;
}

Animation::Animation(Str FileName) : Object(FileName)
{
	FrameCount = 1;
	BmpId = CurrentFrame = 0;
	ColorKey = -1;
	FrameHit = new int[FrameCount];
	Frame = new RECT[FrameCount];
	Displayer = Screen;
}

Animation::~Animation()
{
	delete [] FrameHit;
	delete [] Frame;
}

Animation::Animation(Animation &model)
{
	int i;

	BmpId = model.GetBmpId();
	FrameCount = model.GetFrameCount();
	CurrentFrame = model.GetCurrentFrame();
	ColorKey = model.GetColorKey();
	for (i = 0; i < FrameCount; i++)
		Frame[i] = *model.GetFrame(i);
	FrameDimension = *model.GetFrameDimension();
	for (i = 0; i < 4; i++)
		OriginPoint[i] = *model.GetOriginPoint(i);
	Sprite = Surface(*model.GetSprite());
	Fluidity = Clock(*model.GetFluidity());
	for (i = 0; i < model.GetSkins()->GetElementCount(); i++)
		Skins.AddElement(Integer::getIntegerElement(i, model.GetSkins()));
	Displayer = model.getDisplayer();
}

void Animation::SetColorKey(int Animation_ColorKey)
{
	ColorKey = Animation_ColorKey;
	SetColorKey();
}

void Animation::SetColorKey()
{
	if (ColorKey >= 0)
		Sprite.SetColorKey(ColorKey);
}

void Animation::SetFrame(int n, int x, int y)
{
	int i;

	FrameCount = n;
	FrameHit = new int[n];
	Frame = new RECT[n];

	for (i = 0; i < FrameCount; i++)
	{
		Frame[i].left = 0;
		Frame[i].top = i * y;
		Frame[i].right = x;
		Frame[i].bottom = i * y + y;
		FrameHit[i] = 0;
	}

	FrameDimension.x = x;
	FrameDimension.y = y;
}

void Animation::RotateFrame(int angle)
{
	int i, x, y, DiffAngle;

	DiffAngle = Sprite.GetCurrentAngle() - angle;

	while (DiffAngle < 0) DiffAngle += 360;

	switch (DiffAngle)
	{
	case 0:
		x = FrameDimension.x;
		y = FrameDimension.y;
		break;
	case 90:
		x = FrameDimension.y;
		y = FrameDimension.x;
		break;
	case 180:
		x = FrameDimension.x;
		y = FrameDimension.y;
		break;
	case 270:
		x = FrameDimension.y;
		y = FrameDimension.x;
		break;
	}

	switch (angle)
	{
	case 0:
		for (i = 0; i < FrameCount; i++)
		{
			Frame[i].left = 0;
			Frame[i].top = i * y;
			Frame[i].right = x;
			Frame[i].bottom = i * y + y;
		}
		break;
	case 90:
		for (i = 0; i < FrameCount; i++)
		{
			Frame[i].left = i * x;
			Frame[i].top = 0;
			Frame[i].right = i * x + x;
			Frame[i].bottom = y;
		}
		break;
	case 180:
		for (i = 0; i < FrameCount; i++)
		{
			Frame[i].left = 0;
			Frame[i].top = (FrameCount-i) * y - y;
			Frame[i].right = x;
			Frame[i].bottom = (FrameCount-i) * y;
		}
		break;
	case 270:
		for (i = 0; i < FrameCount; i++)
		{
			Frame[i].left = (FrameCount-i) * x - x;
			Frame[i].top = 0;
			Frame[i].right = (FrameCount-i) * x;
			Frame[i].bottom = y;
		}
		break;
	}

	FrameDimension.x = x;
	FrameDimension.y = y;
}

void Animation::DiffuseFrame(RECT *pRECT, double fps)
{
	int msg;

	if (ColorKey == -1)
		msg = DDBLT_WAIT;
	else
		msg = DDBLT_WAIT | DDBLT_KEYSRC;

	if (Fluidity.getCounter() < FrameCount)
	{
		if (Fluidity.getCounter() != CurrentFrame)
		{
			FrameHit[CurrentFrame] = 0;
			CurrentFrame = Fluidity.getCounter();
		}
		if (FrameCount > 1)
			FrameHit[CurrentFrame]++;
		Displayer->Blit(pRECT, &Sprite, &Frame[CurrentFrame], msg);
	}

	Fluidity.goCounter(fps, 0, FrameCount, false);
}

void Animation::PaintFrame(RECT *pRECT, int index, bool mosaic)
{
	int x, y, msg;

	if (ColorKey == -1)
		msg = DDBLT_WAIT;
	else
		msg = DDBLT_WAIT | DDBLT_KEYSRC;

	if (mosaic)
	{
		for (x = pRECT->left; x < pRECT->right; x += Sprite.GetWidth())
			for (y = pRECT->top; y < pRECT->bottom; y += Sprite.GetHeight())
				Displayer->Blit(x, y, &Sprite, &Frame[index], NULL, msg);
	}
	else
		Displayer->Blit(pRECT, &Sprite, &Frame[index], msg);
}

int Animation::GetSquaredAngle(int angle)
{
	int SquaredAngle = angle + (360-Sprite.GetRefAngle());

	while (SquaredAngle >= 360) SquaredAngle -= 360;

	while (SquaredAngle < 0) SquaredAngle += 360;

	return SquaredAngle;
}

Axe * Animation::GetOriginPoint()
{
	switch (Sprite.GetCurrentAngle())
	{
	case 0:
		return &OriginPoint[0];
	case 90:
		return &OriginPoint[1];
	case 180:
		return &OriginPoint[2];
	case 270:
		return &OriginPoint[3];
	default:
		return &OriginPoint[0];
	}
}

int Animation::GetBmpId(Str SkinId)
{
	int i;

	i = Skins.SearchElementByName(SkinId, true);
	
	if (i == -1)
		i = 0;

	return Integer::getIntegerElement(i, &Skins)->GetValue();
}
