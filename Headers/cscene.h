#ifndef CSCENE
#define CSCENE

#include <ccursor.h>

class Scene : public Visual
{
protected:
	int number;
public:
	Scene();
	Scene(int number);
	~Scene();
	virtual void destroy();
	virtual void doMouseEvents(Cursor *cursor);
	int getNumber() {return number;}
	virtual void init();
	void initBackground(int bmpId, Str skinId, int frameWidth, int frameHeight, bool mosaic);
	virtual void play();
	void runAnimations();
};

#endif
