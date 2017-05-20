#include <cinput.h>
#include <cmsgreporter.h>
#include <dxerror.h>

Input::Input(HWND Input_dxWnd, HINSTANCE Input_hInst)
{
	dxWnd = Input_dxWnd;
	hInst = Input_hInst;
	lpDI = NULL;
	lpDIDevice = NULL;
}

void Input::Destroy()
{
	if (lpDIDevice)
	{
		lpDIDevice->Unacquire();
		lpDIDevice->Release();
		lpDIDevice = NULL;
	}

	if (lpDI)
	{
		lpDI->Release();
		lpDI = NULL;
	}
}

void Input::CreateDevice(THIS_ REFGUID Guid, THIS_ LPCDIDATAFORMAT DataFormat, int msg)
{
	//Crée l'objet DirectInput
	ReportDxError(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&lpDI, NULL), DIRECTINPUT, ERR_DI_DIRECTINPUTCREATE);

	//Crée le dispositif
	ReportDxError(lpDI->CreateDevice(Guid, &lpDIDevice, NULL), DIRECTINPUT, ERR_DI_CREATEDEVICE);

	//Définit le format de donnée pour le dispositif
	ReportDxError(lpDIDevice->SetDataFormat(DataFormat), DIRECTINPUT, ERR_DI_SETDATAFORMAT);

	//Définit le comportement du dispositif
	ReportDxError(lpDIDevice->SetCooperativeLevel(dxWnd, msg), DIRECTINPUT, ERR_DI_SETCOOPERATIVELEVEL);

	lpDIDevice->Acquire();
}
