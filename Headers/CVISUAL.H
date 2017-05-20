#ifndef CVISUAL
#define CVISUAL

#include <cdisplay.h>
#include <citem.h>
#include <cdyncolor.h>

class Visual : virtual public Item
{
private:
	bool visible, mosaic;
	RECT visibleZone;
protected:
	int currentAni;
	short plan;
	Str skinDirOrId;
	List animations, *planGenerator;
	Display *displayer;
public:
	Visual();
	Visual(Visual &model);
	~Visual() {animations.RemoveAllElement(0, true);}
	void addAni(int bmpId, Str skinId, int nFrame, int frameWidth, int frameHeight, int colorKey, int refAngle);
	void addAni(Str directory, Str fileName, int nFrame, int frameWidth, int frameHeight, int colorKey, int refAngle);
	void addAni(int index, int bmpId, Str skinId, int nFrame, int frameWidth, int frameHeight, int colorKey, int refAngle);
	void addAni(int index, Str directory, Str fileName, int nFrame, int frameWidth, int frameHeight, int colorKey, int refAngle);
	void addDynColor(int index, DynColor *color);
	void cascadePlanBack();
	void cascadePlanFront();
	void cloneAni(Visual *pVisual);
	virtual void destroyAni();
	Str getAniFilePath(Str fileName);
	List * getAnimations() {return &animations;}
	static int getBmpFrameCount(char *filename, int frameSize);
	int getCurrentAni() {return currentAni;}
	Display * getDisplayer() {return displayer;}
	bool getMosaic() {return mosaic;}
	short getPlan() {return plan;}
	List * getPlanGenerator() {return planGenerator;}
	Str getSkinDirOrId() {return skinDirOrId;}
	bool getVisible() {return visible;}
	RECT * getVisibleZone() {return &visibleZone;}
	static Visual * getVisualElement(int index, List *pList) {return dynamic_cast<Visual*>(pList->GetElement(index));}
	virtual bool hide();
	virtual void initAni() {;}
	void initDynColors(int size);
	void nextPlan(bool cascade);
	void previousPlan(bool cascade);
	void replaceAniColors();
	virtual void restoreAni();
	void rotateAni(int angle);
	bool runAni(int index, double fps);
	bool runAni(int index, int frame);
	virtual void runAnimations();
	void setDisplayer(Display *displayer) {this->displayer = displayer;}
	void setMosaic(bool mosaic) {this->mosaic = mosaic;}
	void setPlan(short plan);
	void setPlanGenerator(List *planGenerator) {this->planGenerator = planGenerator;}
	void setSkinDirOrId(Str skinDirOrId) {this->skinDirOrId = skinDirOrId;}
	void setVisibleZone(int left=0, int right=0, int up=0, int down=0);
	virtual bool show();
	virtual bool showOnPanel(int frame, Item *cursor);
	virtual void stickToPanel(Axe cellPosition) {position = cellPosition;}
	void updateAni(Str skinDirOrId);
};

#endif
