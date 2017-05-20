#ifndef CSCENESLINKER
#define CSCENESLINKER

#include <cscene.h>
#include <clinkedlist.h>

class ScenesLinker
{
private:
	LinkedList<Scene*> scenes;
	LinkedList<Scene*>::Iterator scenesIter;
public:
	ScenesLinker();
	~ScenesLinker();
	void appendScene(Scene *scene);
	void begin();
	Scene* currentScene();
	void destroyAll();
	Scene* findScene(int number);
	LinkedList<Scene*>* getScenes() {return &scenes;}
	bool goToNextScene();
	bool goToPreviousScene();
	bool goToScene(int number);
	bool insertScene(Scene *scene);
	bool removeScene(Scene *scene);
};

#endif
