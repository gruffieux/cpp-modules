#ifndef CANIMATION
#define CANIMATION

#include <cclock.h>
#include <clist.h>
#include <csurface.h>
#include <cdisplay.h>
#include <cinteger.h>

class Animation : public Object
{
private:
	int BmpId, FrameCount, CurrentFrame, ColorKey, *FrameHit;
	RECT *Frame;
	Axe FrameDimension, OriginPoint[4];
	Surface Sprite;
	Clock Fluidity;
	List Skins;
	Display *Displayer;
public:
	Animation();
	Animation(int Animation_BmpId);
	Animation(Str FileName);
	Animation(Animation &model);
	~Animation();
	void SetCurrentFrame(int Animation_CurrentFrame) {CurrentFrame = Animation_CurrentFrame;}
	void SetColorKey(int Animation_ColorKey);
	void SetColorKey();
	void SetFrame(int n, int x, int y);
	void SetOriginPoint(int index, int x, int y) {OriginPoint[index] = Axe(x, y);}
	void setDisplayer(Display *Animation_Displayer) {Displayer = Animation_Displayer;}
	bool MapSkin(Integer *Skin) {return Skins.AddElement(Skin);}
	void RotateFrame(int angle);
	void DiffuseFrame(RECT *pRECT, double fps);
	void PaintFrame(RECT *pRECT, int index, bool mosaic);
	int GetSquaredAngle(int angle);
	Axe * GetOriginPoint();
	int GetBmpId(Str SkinId);
	int GetBmpId() {return BmpId;}
	int GetFrameCount() {return FrameCount;}
	int GetCurrentFrame() {return CurrentFrame;}
	int GetColorKey() {return ColorKey;}
	int GetFrameHit(int index) {return FrameHit[index];}
	RECT * GetFrame(int index) {return &Frame[index];}
	Axe * GetFrameDimension() {return &FrameDimension;}
	Axe * GetOriginPoint(int index) {return &OriginPoint[index];}
	Surface * GetSprite() {return &Sprite;}
	Clock * GetFluidity() {return &Fluidity;}
	List * GetSkins() {return &Skins;}
	Display * getDisplayer() {return Displayer;}
	static Animation * getAnimationElement(int index, List *pList) {return dynamic_cast<Animation*>(pList->GetElement(index));}
};

#endif
