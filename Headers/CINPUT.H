#ifndef CINPUT
#define CINPUT

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

class Input
{
private:
	LPDIRECTINPUT8 lpDI;
	LPDIRECTINPUTDEVICE8 lpDIDevice;
	HWND dxWnd;
	HINSTANCE hInst;
public:
	Input() {}
	Input(HWND Input_dxWnd, HINSTANCE Input_hInst);
	~Input() {}
	void SetDxWnd(HWND Input_dxWnd) {dxWnd = Input_dxWnd;}
	void SetInstance(HINSTANCE instance) {hInst = instance;}
	void Destroy();
	void CreateDevice(THIS_ REFGUID Guid, THIS_ LPCDIDATAFORMAT DataFormat, int msg);	//Crée un dispositif pour un périphérique d'entrée donné
	LPDIRECTINPUTDEVICE8 GetDevice() {return lpDIDevice;}								//Obtient un dispositif
};

#endif