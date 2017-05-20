#include <cscene.h>

Scene::Scene()
{
	number = 0;
}

Scene::Scene(int number)
{
	this->number = number;
}

Scene::~Scene()
{
}

void Scene::destroy()
{
}

void Scene::doMouseEvents(Cursor *cursor)
{
}

void Scene::init()
{
}

void Scene::initBackground(int bmpId, Str skinId, int frameWidth, int frameHeight, bool mosaic)
{
	setMosaic(mosaic);

	addAni(bmpId, skinId, 1, frameWidth, frameHeight, -1, 0);
}

void Scene::play()
{
}

void Scene::runAnimations()
{
	setVisibleZone(0, displayer->GetCurrentMode()->GetWidth(), 0, displayer->GetCurrentMode()->GetHeight());

	runAni(0, 0);
}
