#include <ckeyboardlistener.h>
#include <cmsgreporter.h>
#include <ckey.h>
#include <dxerror.h>

KeyboardListener::KeyboardListener()
{
	int i;

	capturedKey = 0;
	listenInput = waitFlow = false;

	for (i = 0; i < MAX_KEY; i++)
	{
		key[i] = 0;
		state[i] = prevState[i] = 0;
	}
}

KeyboardListener::~KeyboardListener()
{
}

void KeyboardListener::captureKeys()
{
	int i, counter;

	counter = 0;

	// On définit quelle touches sont pressées
	for (i = 0; i < MAX_KEY; i++)
		if (key[i])
		{
			capturedKey = i;
			prevState[i] = state[i];
			state[i] = 1;
			counter++;
		}
		else
			state[i] = 0;

	// Si plusieurs touches sont pressée à la fois, on prend la dernière
	if (counter > 1)
		for (i = 0; i < MAX_KEY; i++)
			if (state[i] == 1 && prevState[i] == 0)
			{
				capturedKey = i;
				break;
			}
}

int KeyboardListener::keyPressed()
{
	int i;

	for (i = 0; i < MAX_KEY; i++)
		if (key[i] && state[capturedKey])
			return i;

	return 0;
}

bool KeyboardListener::keyPressed(int dik, double latency)
{
	if (key[dik] && state[dik] && capturedKey == dik)
	{
		if (latency != -1)
		{
			clock[dik].goCounter(latency, 0, 1, false);
			return !clock[dik].isGoing();
		}
		else
			return true;
	}
	else
		clock[dik].stopCounter();

	return false;
}

bool KeyboardListener::keyPressed(int index, List *pList, double latency)
{
	int dik;

	dik = Key::getKeyElement(index, pList)->getDik();

	if (key[dik])
	{
		if (latency != -1)
		{
			clock[dik].goCounter(latency, 0, 1, false);
			return !clock[dik].isGoing();
		}
		else
			return true;
	}
	else
		clock[dik].stopCounter();

	return false;
}

bool KeyboardListener::keyPressed(Str description, List *pList, double latency)
{
	int index;

	index = pList->SearchElementByName(description, true);

	return index != -1 && keyPressed(index, pList, latency);
}

bool KeyboardListener::keyPressedOnce(int dik)
{
	return !keyIsPressing(dik) && keyPressed(dik);
}

bool KeyboardListener::keyPressedOnce(int index, List *pList)
{
	return !keyIsPressing(index, pList) && keyPressed(index, pList);
}

bool KeyboardListener::keyPressedOnce(Str description, List *pList)
{
	int index;

	index = pList->SearchElementByName(description, true);

	return index != -1 && keyPressedOnce(index, pList);
}

bool KeyboardListener::keyIsPressing(int dik)
{
	return state[dik] == 1;
}

bool KeyboardListener::keyIsPressing(int index, List *pList)
{
	int dik = Key::getKeyElement(index, pList)->getDik(); 
	
	return state[dik] == 1;
}

bool KeyboardListener::keyIsPressing(Str description, List *pList)
{
	int index;

	index = pList->SearchElementByName(description, true);

	return index != -1 && keyIsPressing(index, pList);
}

int KeyboardListener::keyReleased(int dik)
{
	if (!key[dik] && prevState[dik] && capturedKey == dik)
	{
		prevState[dik] = 0;
		return dik;
	}

	return 0;
}

bool KeyboardListener::keyReleased(int index, List *pList)
{
	int dik;

	dik = Key::getKeyElement(index, pList)->getDik();

	if (prevState[dik])
	{
		prevState[dik] = 0;
		return !key[dik] && capturedKey == dik;
	}

	return false;
}

bool KeyboardListener::keyReleased(Str description, List *pList)
{
	int index;

	index = pList->SearchElementByName(description, true);

	return index != -1 && keyReleased(index, pList);
}

void KeyboardListener::readInputKey(int max)
{
	bool upperCase;

	// On lit et traite le texte entré
	if (listenInput)
	{
		if (keyPressed(capturedKey))
		{
			upperCase = (state[DIK_LSHIFT] == 1) | (state[DIK_RSHIFT] == 1);
			if (prevState[capturedKey] == 0)
			{
				waitFlow = true;
				flowWaiter.setCounter(0);
				updateInputString(capturedKey, max, upperCase);
			}
			else
				if (flowWaiter.getCounter() > 1)
				{
					waitFlow = false;
					updateInputString(capturedKey, max, upperCase);
				}
			if (waitFlow)
				flowWaiter.goCounter(2, 0, 3, false);
		}
	}
}

bool KeyboardListener::startCaptureKeys(Input *pInput)
{
	HRESULT hr;

	hr = pInput->GetDevice()->GetDeviceState(sizeof(key), (LPVOID)&key);

	if (hr != DI_OK)
	{
		ReportDxError(hr, DIRECTINPUT, ERR_DI_GETDEVICESTATE);
		return false;
	}

	return true;
}

bool KeyboardListener::startListenInput()
{
	if (listenInput)
		return false;

	listenInput = true;
	inputString = "";

	return true;
}

bool KeyboardListener::stopListenInput()
{
	if (!listenInput)
		return false;

	listenInput = false;

	return true;
}

void KeyboardListener::updateInputString(int dik, int max, bool upperCase)
{
	switch (dik)
	{
	case DIK_BACK:
		if (inputString.GetLength())
			inputString.removeAt(inputString.GetLength()-1);
		break;
	default:
		if (max == -1 || inputString.GetLength() < max)
			inputString.Cat(Key(dik).character(upperCase));
		break;
	}
}
