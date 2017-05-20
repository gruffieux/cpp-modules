#include <csceneslinker.h>

ScenesLinker::ScenesLinker()
{
	scenesIter.setList(&scenes);
}

ScenesLinker::~ScenesLinker()
{
}

void ScenesLinker::appendScene(Scene *scene)
{
	scenes.append(scene);
}

void ScenesLinker::begin()
{
	Scene *scene;

	scene = scenesIter.first();

	scene->init();
}

Scene* ScenesLinker::currentScene()
{
	if (scenesIter.hasCurrent())
		return scenesIter.currentData();

	return NULL;
}

void ScenesLinker::destroyAll()
{
	Scene *scene;
	LinkedList<Scene*>::Iterator iterator(&scenes);

	scene = iterator.first();

	while (scene)
	{
		scene->destroy();
		delete scene;
		scene = iterator.next();
	}

	scenes.clear();
}

Scene* ScenesLinker::findScene(int number)
{
	Scene *scene;
	LinkedList<Scene*>::Iterator iterator(&scenes);

	scene = iterator.first();

	while (scene)
	{
		if (scene->getNumber() == number)
			return scene;
		scene = iterator.next();
	}

	return NULL;
}

bool ScenesLinker::goToNextScene()
{
	Scene* scene;

	if (scenesIter.hasNext())
	{
		scene = scenesIter.next();
		scene->init();
		return true;
	}

	return false;
}

bool ScenesLinker::goToPreviousScene()
{
	Scene* scene;

	if (scenesIter.hasPrevious())
	{
		scene = scenesIter.previous();
		scene->init();
		return true;
	}

	return false;
}

bool ScenesLinker::goToScene(int number)
{
	Scene *scene;

	scene = scenesIter.first();

	while (scene)
	{
		if (scene->getNumber() == number)
		{
			scene->init();
			return true;
		}
		scene = scenesIter.next();
	}

	return false;
}

bool ScenesLinker::insertScene(Scene *scene)
{
	return scenesIter.insert(scene, currentScene());
}

bool ScenesLinker::removeScene(Scene *scene)
{
	return scenesIter.remove(scene, true);
}
