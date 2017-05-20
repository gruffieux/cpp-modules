////////////////////////////////////////////////////////////////////////////
//
//								Class Display
//							---------------------
//
//	NOTES:
//	- Le mode fenêtré est beaucoup plus rapide si on affiche les gros éléments de manière répètitive (mosaic) (WinXP seulement, Vista ça provoque un relentissement)
//
/////////////////////////////////////////////////////////////////////////////

#ifndef CDISPLAY
#define CDISPLAY

#include <cclock.h>
#include <gab.h>
#include <clist.h>
#include <csurface.h>
#include <cdisplaymode.h>

class Display : public List
{
private:
	int FlipCounter, fps;
	LPDIRECTDRAW lpDD;								//Représente la carte vidéo
	LPDIRECTDRAWSURFACE lpDDSPrimary, lpDDSBack;	//Représente la mémoire vidéo
													//Les données à afficher se stockent dans la surface arrière
	LPDIRECTDRAWCLIPPER lpDDClipper;
	HWND dxWnd;
	HINSTANCE ResInst;
	Clock FPSFrequency;
	DisplayMode CurrentMode;
	Axe GetRotatedBmpDimension(int angle, BITMAP bm);
public:
	Display();
	Display(HWND Display_dxWnd, HINSTANCE Display_ResInst=NULL);
	Display(HWND Display_dxWnd, DWORD Display_width, DWORD Display_height, DWORD Display_bit, bool Display_windowed, HINSTANCE Display_ResInst=NULL);
	~Display() {}
	void SetDxWnd(HWND Display_DxWnd) {dxWnd = Display_DxWnd;}
	void SetResInst(HINSTANCE Display_ResInst) {ResInst = Display_ResInst;}
	void Destroy();																									//Détruit tous les objets DirectDraw
	void SetClipper();																								//Le clipper sert à afficher les sprites dépassants des bords de l'écran
	void SetFullScreen();																							//Définit l'affichage plein écran
	bool SetWindowedScreen();																						//Définit l'affichage en mode fenêtre
	void SetDisplayMode();																							//Définit le mode d'affichage
	void EnumDisplayMode(DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK lpEnumModesCallback);
	void LoadBitmap(char *filename, Surface *pSurface, int angle, int BmpRefAngle);									//Charge une image bitmap dans une surface
	void LoadBitmap(int BmpId, Surface *pSurface, int angle, int BmpRefAngle);
	bool ReplaceBitmapColors(int BmpId, Surface *pSurface);
	bool LoadText(Surface *pSurface, HFONT hFont, char *strText, COLORREF crBackground, COLORREF crForeground);		//Charge une chaîne de caractère dans une surface
	RECT LoadText(Surface *pSurface, HFONT hFont, char *strText, int MaxLength, int format);						//Charge une chaîne de caractère dans une surface en spécifiant un format (saut à la ligne)
	bool LoadText(Surface *pSurface, HFONT hFont, int nText, COLORREF crBackground, COLORREF crForeground);			//Charge un nombre dans une surface
	void CreateOffScreenSurface(Surface *pSurface, int dx, int dy);													//Crée une surface d'une dimension spécifiée pour y contenir des données
	void Blit(int x, int y, Surface *pSurface, RECT *SurfaceLoc, RECT *BkgZone, int option);						//Affiche une donnée contenue dans la surface arrière
	void Blit(RECT *Zone, Surface *pSurface, RECT *SurfaceLoc, int option);											//
	void BlitColor(RECT *pRect, LPDDBLTFX lpDDBltFX, COLORREF color, int option);									//Affiche un fond coloré
	void ShowText(LPCSTR strText, RECT *TextArea, COLORREF crBackground, COLORREF crForeground, int format);		//Affiche un texte avec un format
	void FlipFlap();
	static PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp);
	static bool CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);
	int GetFlipCounter() {return FlipCounter;}
	int GetFPS() {return fps;}
	HWND GetDxWnd() {return dxWnd;}
	HINSTANCE GetResInst() {return ResInst;}
	DisplayMode * GetCurrentMode() {return &CurrentMode;}
};

#endif
