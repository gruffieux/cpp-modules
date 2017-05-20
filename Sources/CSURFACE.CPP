#include <csurface.h>
#include <cdyncolor.h>
#include <cmsgreporter.h>
#include <dxerror.h>
#include <tchar.h>

Surface::Surface()
{
	CurrentAngle = RefAngle = ddsd.dwWidth = ddsd.dwHeight = 0;

	ZeroMemory(&DDBltFX, sizeof(DDBltFX));
	DDBltFX.dwSize = sizeof(DDBLTFX);

	lpDDS = NULL;
}

Surface::Surface(Surface &model)
{
	CurrentAngle = model.GetCurrentAngle();
	RefAngle = model.GetRefAngle();
	ddsd.dwWidth = model.GetWidth();
	ddsd.dwHeight = model.GetHeight();
	DDBltFX = *model.GetDDBltFX();
	DynColors = *model.GetDynColors();
	lpDDS = model.GetDDSurface();
}

void Surface::Create(DDSURFACEDESC Surface_ddsd, LPDIRECTDRAW lpDD)
{
	ddsd = Surface_ddsd;

	ReportDxError(lpDD->CreateSurface(&ddsd, &lpDDS, NULL), DIRECTDRAW, ERR_DD_CREATESURFACE);
}

bool Surface::Destroy()
{
	HRESULT hr;

	if (lpDDS)
	{
		hr = lpDDS->Release();
		if (hr == DD_OK)
		{
			lpDDS = NULL;
			return true;
		}
		else
			ReportDxError(hr, DIRECTDRAW, ERR_DD_DESTROYSURFACE);
	}

	return false;
}

void Surface::DrawBitmap(HBITMAP hbmSrc, int dx, int dy, int angle, int Surface_RefAngle)
{
	HDC hdcImage;
	HDC hdc;
	HBITMAP hbmDest;
	HRESULT hr;

	CurrentAngle = angle;
	RefAngle = Surface_RefAngle;

	hr = lpDDS->GetDC(&hdc);

	if (hr == DD_OK)
	{
		hdcImage = CreateCompatibleDC(NULL);
		hbmDest = GetRotatedBitmap(hdc, hbmSrc, CurrentAngle);
		if (hbmDest != NULL)
		{
			SelectObject(hdcImage, hbmDest);
			BitBlt(hdc, 0, 0, dx, dy, hdcImage, 0, 0, SRCCOPY);
			DeleteObject(hbmDest);
			if (!DeleteDC(hdcImage))
				ReportDxError(S_FALSE, DIRECTDRAW, ERR_DD_DELDEVICECONTEXT);
		}
		else
			ReportDxError(S_FALSE, DIRECTDRAW, ERR_DD_GETROTATEDBITMAP);
		ReportDxError(lpDDS->ReleaseDC(hdc), DIRECTDRAW, ERR_DD_DELDEVICECONTEXT);
	}
	else
		ReportDxError(hr, DIRECTDRAW, ERR_DD_GETDEVICECONTEXT);
}

bool Surface::DrawText(HFONT hFont, LPCSTR strText, DWORD dwOriginX, DWORD dwOriginY, COLORREF crBackground, COLORREF crForeground)
{
	HDC hdc = NULL;
	HRESULT hr;

	if (!Str((char*)strText).GetLength())
		return false;

	hr = lpDDS->GetDC(&hdc);

	if (hr == DD_OK)
	{
		SetBkColor(hdc, crBackground);
		SetTextColor(hdc, crForeground);
		if (hFont)
			SelectObject(hdc, hFont);
		TextOut(hdc, dwOriginX, dwOriginY, strText, (int)_tcslen(strText));
		lpDDS->ReleaseDC(hdc);
	}
	else
		ReportDxError(hr, DIRECTDRAW, ERR_DD_GETDEVICECONTEXT);

	return true;
}

void Surface::SetColorKey(unsigned long color)
{
	DDCOLORKEY ddck;

	ddck.dwColorSpaceLowValue = color;
	ddck.dwColorSpaceHighValue = color;

	lpDDS->SetColorKey(DDCKEY_SRCBLT, &ddck);
}

//***************************************************************************** 
// GetBmpSize : récupère la taille d'un bitmap. 
// entrée : hBmp : bitmap dont on souhaite récupérer la taille. 
// retour : taille du bitmap spécifié. 
//***************************************************************************** 
SIZE Surface::GetBmpSize(HBITMAP hBmp)
{
	// récupération des informations sur le bitmap
	BITMAP bmpInfo;
	GetObject(hBmp, sizeof(bmpInfo), &bmpInfo);

	// taille
	SIZE size;
	size.cx = bmpInfo.bmWidth;
	size.cy = bmpInfo.bmHeight;

	return size;
}

//***************************************************************************** 
// GetBmpData : récupère les données (pixels) d'un bitmap. 
// entrée : hdc : DC à utiliser pour les appels aux fonctions API. 
// hBmp : bitmap source. 
// retour : données du bitmap (à libérer par VirtualFree) ou NULL en cas 
// d'erreur. 
//----------------------------------------------------------------------------- 
// Remarques : - les données sont récupéres en 32 bits par pixels (4octets) 
// afin d'éviter d'avoir à rajouter du padding pour aligner les 
// lignes sur 4 octets. 
// - les bitmaps top-down ne sont pas gérés. 
// - les pixels sont stockées par ligne à partir du bas du bitmap 
// (bottum-up). 
//***************************************************************************** 
LPVOID Surface::GetBmpData(HDC hdc, HBITMAP hBmp)
{
	// taille du bitmap, si bitmap top-down on renvoi NULL
	SIZE sizeBmp = GetBmpSize(hBmp);
	if(sizeBmp.cy < 0)
		return NULL;

	// allocation mémoire (on va récupérer en 32 bits par pixel)
	DWORD dwSize = 4*sizeBmp.cx*sizeBmp.cy;
	LPVOID lpMem = VirtualAlloc(NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	if(lpMem == NULL)
		return NULL;

	// initialisation structure BITMAPINFO
	BITMAPINFO bi;
	ZeroMemory(&bi, sizeof(BITMAPINFO));
	bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biPlanes		= 1;
	bi.bmiHeader.biBitCount		= 32;
	bi.bmiHeader.biWidth		= sizeBmp.cx;
	bi.bmiHeader.biHeight		= sizeBmp.cy;
	bi.bmiHeader.biCompression	= BI_RGB;

	// récupération bits
	int nResult = GetDIBits(hdc, hBmp, 0, sizeBmp.cy, lpMem, &bi, DIB_RGB_COLORS);

	DeleteObject(hBmp);

	// si tout c'est bien passé
	if(nResult != 0)
		return lpMem;

	// erreur, libération de la mémoire
	VirtualFree(lpMem, 0, MEM_RELEASE);
	return NULL;
}

//***************************************************************************** 
// RotateBmp: effectue une rotation d'un bitmap. 
// entrée	: hdc : DC à utiliser pour les appels aux fonctions API. 
// hBmpSrc	: bitmap source. 
// nAngle	: angle de rotation, en degré, dans le sens trigo (inverse horaire).
//			  Seules les valeurs 0, 90, 180 et 270 sont valides. 
// retour	: bitmap avec la rotation demandée ou NULL en cas d'erreur. 
//***************************************************************************** 
HBITMAP Surface::GetRotatedBitmap(HDC hdc, HBITMAP hBmpSrc, int nAngle)
{ 
	int xSrc, ySrc;

	// nAngle doit être 0, 90, 180 ou 270 
	if(nAngle != 0 && nAngle != 90 && nAngle != 180 && nAngle != 270) 
		return NULL; 

	// taille et données du bitmap source 
	SIZE sizeSrc = GetBmpSize(hBmpSrc); 
	LPVOID lpDataSrc = GetBmpData(hdc, hBmpSrc); 
	if(lpDataSrc == NULL) 
		return NULL; 

	// taille du bitmap de destination 
	SIZE sizeDst = sizeSrc; 
	if(nAngle == 90 || nAngle == 270) 
	{ 
		sizeDst.cx = sizeSrc.cy; 
		sizeDst.cy = sizeSrc.cx; 
	} 

	// allocation mémoire pour le bitmap de destination (32 bits par pixel) 
	DWORD dwSize = 4*sizeDst.cx*sizeDst.cy; 
	LPVOID lpDataDst = VirtualAlloc(NULL, dwSize, MEM_COMMIT, PAGE_READWRITE); 
	if(lpDataDst == NULL) 
	{ 
		VirtualFree(lpDataSrc, 0, MEM_RELEASE); 
		return NULL; 
	} 

	// affectation des bits 
	for(xSrc = 0; xSrc < sizeSrc.cx; xSrc++)
		for(ySrc = 0; ySrc < sizeSrc.cy; ySrc++) 
		{ 
			// pixel de destination : 
			int xDst, yDst; 
			switch(nAngle) 
			{
			case 0:
				xDst = xSrc;
				yDst = ySrc;
				break;
			case 90:
				xDst = sizeSrc.cy-ySrc-1;
				yDst = xSrc;
				break; 
			case 180:
				xDst = sizeSrc.cx-xSrc-1;
				yDst = sizeSrc.cy-ySrc-1;
				break; 
			case 270:
				xDst = ySrc;
				yDst = sizeSrc.cx-xSrc-1;
				break; 
			} 

			// affectation du pixels (32 bits = 4 octets = 1 DWORD)
			LPDWORD lpSrc = (LPDWORD)lpDataSrc+sizeSrc.cx*ySrc+xSrc;
			LPDWORD lpDst = (LPDWORD)lpDataDst+sizeDst.cx*yDst+xDst;
			*lpDst = *lpSrc;
		}

	// initialisation structure BITMAPINFO pour le bitmap de destination 
	BITMAPINFO bi; 
	ZeroMemory(&bi, sizeof(BITMAPINFO)); 
	bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER); 
	bi.bmiHeader.biPlanes		= 1; 
	bi.bmiHeader.biBitCount		= 32; 
	bi.bmiHeader.biWidth		= sizeDst.cx; 
	bi.bmiHeader.biHeight		= sizeDst.cy; 
	bi.bmiHeader.biCompression	= BI_RGB; 

	// création du bitmap de destination 
	HBITMAP hBmpDst = CreateDIBitmap(hdc, &bi.bmiHeader, CBM_INIT, lpDataDst, &bi, DIB_RGB_COLORS);

	// libération mémoire 
	VirtualFree(lpDataSrc, 0, MEM_RELEASE); 
	VirtualFree(lpDataDst, 0, MEM_RELEASE); 

	// retour 
	return hBmpDst;
} 

HBITMAP Surface::ReplaceColor(HBITMAP hBmp, HDC hBmpDC)
{
	COLORREF cOldColor, cNewColor;

    HBITMAP RetBmp=NULL;

    if (!hBmp) return NULL;
    
	HDC BufferDC=CreateCompatibleDC(NULL);	// DC for Source Bitmap

	if (!BufferDC) return NULL;

	HBITMAP hTmpBitmap = (HBITMAP) NULL;
    
	if (hBmpDC)
        if (hBmp == (HBITMAP)GetCurrentObject(hBmpDC, OBJ_BITMAP))
		{
			hTmpBitmap = CreateBitmap(1, 1, 1, 1, NULL);
			SelectObject(hBmpDC, hTmpBitmap);
		}
	    
    HGDIOBJ PreviousBufferObject=SelectObject(BufferDC,hBmp);
	
	// here BufferDC contains the bitmap
	HDC DirectDC=CreateCompatibleDC(NULL); // DC for working

    if (DirectDC)
    {
		// Get bitmap size
		BITMAP bm;
		GetObject(hBmp, sizeof(bm), &bm);
				
		// create a BITMAPINFO with minimal initilisation 
		// for the CreateDIBSection
		BITMAPINFO RGB32BitsBITMAPINFO; 
		ZeroMemory(&RGB32BitsBITMAPINFO,sizeof(BITMAPINFO));
		RGB32BitsBITMAPINFO.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		RGB32BitsBITMAPINFO.bmiHeader.biWidth=bm.bmWidth;
		RGB32BitsBITMAPINFO.bmiHeader.biHeight=bm.bmHeight;
		RGB32BitsBITMAPINFO.bmiHeader.biPlanes=1;
		RGB32BitsBITMAPINFO.bmiHeader.biBitCount=32;

		// pointer used for direct Bitmap pixels access
		UINT * ptPixels;	

		HBITMAP DirectBitmap = CreateDIBSection(DirectDC,
			(BITMAPINFO *)&RGB32BitsBITMAPINFO,
			DIB_RGB_COLORS,
			(void **)&ptPixels,
			NULL, 0);

		if (DirectBitmap)
		{
			// here DirectBitmap!=NULL so ptPixels!=NULL no need to test
			HGDIOBJ PreviousObject=SelectObject(DirectDC, DirectBitmap);
			BitBlt(DirectDC, 0, 0, bm.bmWidth, bm.bmHeight, BufferDC, 0, 0, SRCCOPY);

			// here the DirectDC contains the bitmap

			// After all the inits we can do the job : Replace Color
			for (int i = 0; i < DynColors.GetElementCount(); i++)
			{
				// Convert COLORREF to RGB (Invert RED and BLUE)
				cOldColor = COLORREF2RGB(DynColor::getDynColorElement(i, &DynColors)->GetOldColor());
				cNewColor = COLORREF2RGB(DynColor::getDynColorElement(i, &DynColors)->GetNewColor());
				for (int j = ((bm.bmWidth*bm.bmHeight)-1); j >= 0; j--)
					if (ptPixels[j] == cOldColor)
						ptPixels[j] = cNewColor;
			}

			// little clean up

			// Don't delete the result of SelectObject because it's 
			// our modified bitmap (DirectBitmap)
			SelectObject(DirectDC,PreviousObject);
					
			// finish
			RetBmp=DirectBitmap;
		}

		// clean up
		DeleteDC(DirectDC);
	}
    
	if (hTmpBitmap)
    {
		SelectObject(hBmpDC, hBmp);
		DeleteObject(hTmpBitmap);
    }

    SelectObject(BufferDC,PreviousBufferObject);

    // BufferDC is now useless
    DeleteDC(BufferDC);

    return RetBmp;
}
