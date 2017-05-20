#ifndef CKEYBOARDLISTENER
#define CKEYBOARDLISTENER

#include <clist.h>
#include <cinput.h>
#include <cclock.h>

class KeyboardListener
{
public:
	static const int MAX_KEY = 256;
	KeyboardListener();
	~KeyboardListener();
	void captureKeys();
	Str getInputString() {return inputString;}
	bool isListeningInput() {return listenInput;}
	int keyPressed();
	bool keyPressed(int dik, double latency=-1);
	bool keyPressed(int index, List *pList, double latency=-1);
	bool keyPressed(Str description, List *pList, double latency=-1);
	bool keyPressedOnce(int dik);
	bool keyPressedOnce(int index, List *pList);
	bool keyPressedOnce(Str description, List *pList);
	bool keyIsPressing(int dik);
	bool keyIsPressing(int index, List *pList);
	bool keyIsPressing(Str description, List *pList);
	int keyReleased(int dik);
	bool keyReleased(int index, List *pList);
	bool keyReleased(Str description, List *pList);
	void readInputKey(int max=-1);
	void setInputString(Str inputString) {this->inputString = inputString;}
	bool startCaptureKeys(Input *pInput);
	bool startListenInput();
	bool stopListenInput();
private:
	int state[MAX_KEY], prevState[MAX_KEY], capturedKey;
	bool listenInput, waitFlow;
	char key[MAX_KEY];
	Str inputString;
	Clock clock[MAX_KEY], flowWaiter;
	void updateInputString(int dik, int max, bool upperCase);
};

#endif
