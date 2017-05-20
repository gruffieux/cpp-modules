#include <cdisplay.h>
#include <cmsgreporter.h>
#include <dxerror.h>
#include <tchar.h>

Display::Display()
{
	FlipCounter = fps = 0;
	dxWnd = NULL;
	ResInst = NULL;
	lpDDClipper = NULL;
	lpDDSBack = NULL;
	lpDDSPrimary = NULL;
	lpDD = NULL;
}

Display::Display(HWND Display_dxWnd, HINSTANCE Display_ResInst)
{
	FlipCounter = fps = 0;
	dxWnd = Display_dxWnd;
	ResInst = Display_ResInst;
	lpDDClipper = NULL;
	lpDDSBack = NULL;
	lpDDSPrimary = NULL;
	lpDD = NULL;
}

Display::Display(HWND Display_dxWnd, DWORD Display_width, DWORD Display_height, DWORD Display_bit, bool Display_windowed, HINSTANCE Display_ResInst)
{
	FlipCounter = fps = 0;
	dxWnd = Display_dxWnd;
	ResInst = Display_ResInst;
	CurrentMode = DisplayMode(Display_width, Display_height, Display_bit, Display_windowed);
	lpDDClipper = NULL;
	lpDDSBack = NULL;
	lpDDSPrimary = NULL;
	lpDD = NULL;
}

void Display::Destroy()
{
	if (lpDDClipper)
	{
		lpDDClipper->Release();
		lpDDClipper = NULL;
	}

	if (lpDDSBack)
	{
		lpDDSBack->Release();
		lpDDSBack = NULL;
	}

	if (lpDDSPrimary)
	{
		lpDDSPrimary->Release();
		lpDDSPrimary = NULL;
	}

	if (lpDD)
	{
		lpDD->Release();
		lpDD = NULL;
	}
}

void Display::SetClipper()
{
	LPRGNDATA lpClipList = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + sizeof(RECT));
	RECT rcBoundary = {0, 0, CurrentMode.GetWidth(), CurrentMode.GetHeight()};

	//Crée le clipper
	ReportDxError(lpDD->CreateClipper(0, &lpDDClipper, NULL), DIRECTDRAW, ERR_DD_CREATECLIPPER);

	//Crée et règle la liste de clip
	memcpy(lpClipList->Buffer, &rcBoundary, sizeof(RECT));
	lpClipList->rdh.dwSize = sizeof(RGNDATAHEADER);
	lpClipList->rdh.iType = RDH_RECTANGLES;
	lpClipList->rdh.nCount = 1;
	lpClipList->rdh.nRgnSize = sizeof(RECT);
	lpClipList->rdh.rcBound = rcBoundary;
	lpDDClipper->SetClipList(lpClipList, 0);

	//Attache le clipper à la surface arrière
	ReportDxError(lpDDSBack->SetClipper(lpDDClipper), DIRECTDRAW, ERR_DD_SETCLIPPER);
	
	free(lpClipList); //Libère la mémoire allouée
}

void Display::SetFullScreen()
{
	DDSURFACEDESC ddsd;

	CurrentMode.SetWindowed(0);

	//Crée l'objet DirectDraw
	ReportDxError(DirectDrawCreate(NULL, &lpDD, NULL), DIRECTDRAW, ERR_DD_DIRECTDRAWCREATE);

	//Définit le comportement de l'affichage
	ReportDxError(lpDD->SetCooperativeLevel(dxWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE), DIRECTDRAW, ERR_DD_SETCOOPERATIVELEVEL);

	//Règle le mode vidéo
	ReportDxError(lpDD->SetDisplayMode(CurrentMode.GetWidth(), CurrentMode.GetHeight(), CurrentMode.GetBit()), DIRECTDRAW, ERR_DD_SETDISPLAYMODE);

	//Crée la surface primaire
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;
	ReportDxError(lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL), DIRECTDRAW, ERR_DD_CREATESURFACE);
	
	//Attache la surface arrière à la surface primaire
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
	ReportDxError(lpDDSPrimary->GetAttachedSurface(&ddsd.ddsCaps, &lpDDSBack), DIRECTDRAW, ERR_DD_GETATTACHEDSURFACE);

	SetClipper();
}

bool Display::SetWindowedScreen()
{
	DDSURFACEDESC ddsd;

	if (CurrentMode.IsInvalid()) return false;

	CurrentMode.SetWindowed(1);

	//Crée l'objet DirectDraw
	ReportDxError(DirectDrawCreate(NULL, &lpDD, NULL), DIRECTDRAW, ERR_DD_DIRECTDRAWCREATE);

	//Définit le comportement de l'affichage
	ReportDxError(lpDD->SetCooperativeLevel(dxWnd, DDSCL_NORMAL), DIRECTDRAW, ERR_DD_SETCOOPERATIVELEVEL);

	//Crée la surface primaire
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	ReportDxError(lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL), DIRECTDRAW, ERR_DD_CREATESURFACE);

	//Crée la surface arrière à part
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = CurrentMode.GetWidth();
	ddsd.dwHeight = CurrentMode.GetHeight();
	ReportDxError(lpDD->CreateSurface(&ddsd, &lpDDSBack, NULL), DIRECTDRAW, ERR_DD_CREATESURFACE);

	SetClipper();

	return true;
}

void Display::SetDisplayMode()
{
	if (CurrentMode.GetWindowed())
		SetWindowedScreen();
	else
		SetFullScreen();
}

void Display::EnumDisplayMode(DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK lpEnumModesCallback)
{
	ReportDxError(DirectDrawCreate(NULL, &lpDD, NULL), DIRECTDRAW, ERR_DD_DIRECTDRAWCREATE);
	ReportDxError(lpDD->SetCooperativeLevel(dxWnd, DDSCL_NORMAL), DIRECTDRAW, ERR_DD_SETCOOPERATIVELEVEL);
	ReportDxError(lpDD->EnumDisplayModes(dwFlags, lpDDSurfaceDesc, lpContext, lpEnumModesCallback), DIRECTDRAW, ERR_DD_ENUMDISPLAYMODES);
}

Axe Display::GetRotatedBmpDimension(int angle, BITMAP bm)
{
	int x, y;

	switch (angle)
	{
	case 0:
		x = bm.bmWidth;
		y = bm.bmHeight;
		break;
	case 90:
		x = bm.bmHeight;
		y = bm.bmWidth;
		break;
	case 180:
		x = bm.bmWidth;
		y = bm.bmHeight;
		break;
	case 270:
		x = bm.bmHeight;
		y = bm.bmWidth;
		break;
	}

	return Axe(x, y);
}

void Display::LoadBitmap(char *filename, Surface *pSurface, int angle, int BmpRefAngle)
{
	Axe BmpDim;
	BITMAP bm;
	HBITMAP hbm;

	hbm = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); //Charge une image bitmap du disque
	GetObject(hbm, sizeof(bm), &bm); //Obtient la taille du bitmap
	BmpDim = GetRotatedBmpDimension(angle, bm);

	CreateOffScreenSurface(pSurface, BmpDim.x, BmpDim.y);

	pSurface->DrawBitmap(hbm, BmpDim.x, BmpDim.y, angle, BmpRefAngle);

	DeleteObject(hbm);
}

void Display::LoadBitmap(int BmpId, Surface *pSurface, int angle, int BmpRefAngle)
{
	Axe BmpDim;
	BITMAP bm;
	HBITMAP hbm;

	hbm = (HBITMAP)LoadImage(ResInst, MAKEINTRESOURCE(BmpId), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION); //Charge une image bitmap du disque
	GetObject(hbm, sizeof(bm), &bm); //Obtient la taille du bitmap
	BmpDim = GetRotatedBmpDimension(angle, bm);

	CreateOffScreenSurface(pSurface, BmpDim.x, BmpDim.y);
	pSurface->DrawBitmap(hbm, BmpDim.x, BmpDim.y, angle, BmpRefAngle);

	DeleteObject(hbm);

	ReplaceBitmapColors(BmpId, pSurface);
}

bool Display::ReplaceBitmapColors(int BmpId, Surface *pSurface)
{
	Axe BmpDim;
	BITMAP bm;
	HBITMAP hbm, hbm2;

	if (!pSurface->GetDynColors()->GetElementCount())
		return false;

	hbm = (HBITMAP)LoadImage(ResInst, MAKEINTRESOURCE(BmpId), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION); //Charge une image bitmap du disque
	GetObject(hbm, sizeof(bm), &bm); //Obtient la taille du bitmap
	BmpDim = GetRotatedBmpDimension(pSurface->GetCurrentAngle(), bm);

	hbm2 = pSurface->ReplaceColor(hbm, NULL);
	DeleteObject(hbm);

	pSurface->DrawBitmap(hbm2, BmpDim.x, BmpDim.y, pSurface->GetCurrentAngle(), pSurface->GetRefAngle());
	DeleteObject(hbm2);

	return true;
}

bool Display::LoadText(Surface *pSurface, HFONT hFont, char *strText, COLORREF crBackground, COLORREF crForeground)
{
	HDC hdc = NULL;
	SIZE sizeText;

	hdc = GetDC(NULL);

	if (hFont)
		SelectObject(hdc, hFont);

	GetTextExtentPoint32(hdc, strText, (int)_tcslen(strText), &sizeText);
	ReleaseDC(NULL, hdc);

	if (!sizeText.cx)
	{
		CreateOffScreenSurface(pSurface, 1, 1);
		return false;
	}

	CreateOffScreenSurface(pSurface, sizeText.cx, sizeText.cy);

	return pSurface->DrawText(hFont, strText, 0, 0, crBackground, crForeground);
}

RECT Display::LoadText(Surface *pSurface, HFONT hFont, char *strText, int MaxLength, int format)
{
	HDC hdc = NULL;
	SIZE sizeText;
	RECT rect;

	hdc = GetDC(NULL);

	SetRect(&rect, 0, 0, MaxLength, 0);
	rect.bottom = DrawText(hdc, strText, (int)_tcslen(strText), &rect, format);

	if (hFont)
		SelectObject(hdc, hFont);

	if (rect.bottom > 16)
	{
		sizeText.cx = rect.right;
		sizeText.cy = rect.bottom;
	}
	else
		GetTextExtentPoint32(hdc, strText, (int)_tcslen(strText), &sizeText);

	ReleaseDC(NULL, hdc);

	CreateOffScreenSurface(pSurface, sizeText.cx, sizeText.cy);

	return rect;
}

bool Display::LoadText(Surface *pSurface, HFONT hFont, int nText, COLORREF crBackground, COLORREF crForeground)
{
	char buffer[9];
	HDC hdc = NULL;
	SIZE sizeText;

	hdc = GetDC(NULL);

	if (hFont)
		SelectObject(hdc, hFont);

	if (sprintf_s(buffer, 9, "%d", nText))
	{
		GetTextExtentPoint32(hdc, buffer, (int)_tcslen(buffer), &sizeText);
		ReleaseDC(NULL, hdc);
		CreateOffScreenSurface(pSurface, sizeText.cx, sizeText.cy);
		return pSurface->DrawText(hFont, buffer, 0, 0, crBackground, crForeground);
	}

	return false;
}

void Display::CreateOffScreenSurface(Surface *pSurface, int dx, int dy)
{
	DDSURFACEDESC ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = dx;
	ddsd.dwHeight = dy;

	pSurface->Destroy();
	pSurface->Create(ddsd, lpDD);
}

void Display::Blit(int x, int y, Surface *pSurface, RECT *SurfaceLoc, RECT *BkgZone, int option)
{
	RECT rcRect1, rcRect2;

	SetRect(&rcRect1, x, y, x + pSurface->GetWidth(), y + pSurface->GetHeight());

	if (BkgZone)
	{
		BkgZone->left = x;
		BkgZone->right = x + pSurface->GetWidth();
		BkgZone->top = y;
		BkgZone->bottom = y + pSurface->GetHeight();
	}

	if (!SurfaceLoc)
	{
		SetRect(&rcRect2, 0, 0, pSurface->GetWidth(), pSurface->GetHeight());
		ReportDxError(lpDDSBack->Blt(&rcRect1, pSurface->GetDDSurface(), &rcRect2, option, pSurface->GetDDBltFX()), DIRECTDRAW, ERR_DD_BLT);
	}
	else
		ReportDxError(lpDDSBack->Blt(&rcRect1, pSurface->GetDDSurface(), SurfaceLoc, option, pSurface->GetDDBltFX()), DIRECTDRAW, ERR_DD_BLT);
}

void Display::Blit(RECT *Zone, Surface *pSurface, RECT *SurfaceLoc, int option)
{
	RECT rcRect1;

	if (!SurfaceLoc)
	{
		SetRect(&rcRect1, 0, 0, pSurface->GetWidth(), pSurface->GetHeight());
		ReportDxError(lpDDSBack->Blt(Zone, pSurface->GetDDSurface(), &rcRect1, option, pSurface->GetDDBltFX()), DIRECTDRAW, ERR_DD_BLT);
	}
	else
		ReportDxError(lpDDSBack->Blt(Zone, pSurface->GetDDSurface(), SurfaceLoc, option, pSurface->GetDDBltFX()), DIRECTDRAW, ERR_DD_BLT);
}

void Display::BlitColor(RECT *pRect, LPDDBLTFX lpDDBltFX, COLORREF color, int option)
{
	lpDDBltFX->dwFillColor = color;

	ReportDxError(lpDDSBack->Blt(pRect, NULL, NULL, option, lpDDBltFX), DIRECTDRAW, ERR_DD_BLT);
}

void Display::ShowText(LPCSTR strText, RECT *TextArea, COLORREF crBackground, COLORREF crForeground, int format)
{
	HDC hdc = NULL;

	lpDDSBack->GetDC(&hdc);

	SetBkColor(hdc, crBackground);
    SetTextColor(hdc, crForeground);

	DrawText(hdc, strText, (int)_tcslen(strText), TextArea, format);

	lpDDSBack->ReleaseDC(hdc);
}

void Display::FlipFlap()
{
	RECT rcRectSrc, rcRectDest;
	POINT p;
	HRESULT hr;

	if (CurrentMode.GetWindowed())
	{
		p.x = 0;
		p.y = 0;
		ClientToScreen(dxWnd, &p);
		GetClientRect(dxWnd, &rcRectDest);
		OffsetRect(&rcRectDest, p.x, p.y);
		SetRect(&rcRectSrc, 0, 0, CurrentMode.GetWidth(), CurrentMode.GetHeight());
		hr = lpDDSPrimary->Blt(&rcRectDest, lpDDSBack, &rcRectSrc, DDBLT_WAIT, NULL);
	}
	else
		hr = lpDDSPrimary->Flip(NULL, DDFLIP_WAIT); //Inverse la surface avant avec la surface arrière
	
	ReportDxError(hr, DIRECTDRAW, ERR_DD_SURFACELOST);
	
	if (!FPSFrequency.goCounter(1, 0, 1, false))
	{
		fps = FlipCounter;
		FlipCounter = 0;
	}
	else
		if (hr == DD_OK)
			FlipCounter++;
}

PBITMAPINFO Display::CreateBitmapInfoStruct(HBITMAP hBmp)
{
    BITMAP bmp; 
    PBITMAPINFO pbmi; 
    WORD    cClrBits; 

    // Retrieve the bitmap color format, width, and height. 
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) 
        return NULL; 

    // Convert the color format to a count of bits. 
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
    if (cClrBits == 1) 
        cClrBits = 1; 
    else if (cClrBits <= 4)
		cClrBits = 4; 
    else if (cClrBits <= 8) 
        cClrBits = 8; 
    else if (cClrBits <= 16) 
        cClrBits = 16; 
    else if (cClrBits <= 24) 
        cClrBits = 24; 
    else
		cClrBits = 32; 

    // Allocate memory for the BITMAPINFO structure. (This structure 
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
    // data structures.) 
     if (cClrBits != 24) 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< cClrBits)); 
     // There is no RGBQUAD array for the 24-bit-per-pixel format. 
     else 
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)); 

    // Initialize the fields in the BITMAPINFO structure. 
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if (cClrBits < 24) 
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

    // If the bitmap is not compressed, set the BI_RGB flag. 
    pbmi->bmiHeader.biCompression = BI_RGB; 

    // Compute the number of bytes in the array of color 
    // indices and store the result in biSizeImage. 
    // For Windows NT, the width must be DWORD aligned unless 
    // the bitmap is RLE compressed. This example shows this. 
    // For Windows 95/98/Me, the width must be WORD aligned unless the 
    // bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) /8 * pbmi->bmiHeader.biHeight; 
    
	// Set biClrImportant to 0, indicating that all of the 
    // device colors are important. 
     pbmi->bmiHeader.biClrImportant = 0;

     return pbmi;
}

bool Display::CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC)
{ 
	HANDLE hf;                 // file handle 
    BITMAPFILEHEADER hdr;       // bitmap file-header 
    PBITMAPINFOHEADER pbih;     // bitmap info-header 
    LPBYTE lpBits;              // memory pointer 
    DWORD dwTotal;              // total count of bytes 
    DWORD cb;                   // incremental count of bytes 
    BYTE *hp;                   // byte pointer 
    DWORD dwTmp; 

    pbih = (PBITMAPINFOHEADER) pbi; 
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (!lpBits)
		return false;

    // Retrieve the color table (RGBQUAD array) and the bits 
    // (array of palette indices) from the DIB. 
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS))
        return false;

    // Create the .BMP file. 
    hf = CreateFile(pszFile, 
                   GENERIC_READ | GENERIC_WRITE, 
                   (DWORD) 0, 
                    NULL, 
                   CREATE_ALWAYS, 
                   FILE_ATTRIBUTE_NORMAL, 
                   (HANDLE) NULL);

    if (hf == INVALID_HANDLE_VALUE) 
        return false;

    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"

    // Compute the size of the entire file. 
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage); 
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 

    // Compute the offset to the array of color indices. 
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof (RGBQUAD); 

    // Copy the BITMAPFILEHEADER into the .BMP file. 
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp,  NULL))
		return false;

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof (RGBQUAD), (LPDWORD) &dwTmp, ( NULL)))
        return false;

    // Copy the array of color indices into the .BMP file. 
    dwTotal = cb = pbih->biSizeImage; 
    hp = lpBits; 
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL))
		return false; 

    // Close the .BMP file. 
     if (!CloseHandle(hf))
		 return false; 

    // Free memory. 
    GlobalFree((HGLOBAL)lpBits);

	return true;
}
