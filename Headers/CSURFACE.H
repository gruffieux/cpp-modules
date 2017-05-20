#ifndef CSURFACE
#define CSURFACE

#include <clist.h>
#include <ddraw.h>

#define COLORREF2RGB(Color) (Color & 0xff00) | ((Color >> 16) & 0xff) | ((Color << 16) & 0xff0000)

class Surface
{
private:
	int CurrentAngle, RefAngle;
	DDSURFACEDESC ddsd;
	LPDIRECTDRAWSURFACE lpDDS;
	DDBLTFX DDBltFX;
	List DynColors;
public:
	Surface();
	Surface(Surface &model);
	~Surface() {}
	void Create(DDSURFACEDESC Surface_ddsd, LPDIRECTDRAW lpDD);				//Crée la surface
	bool Destroy();															//Détruit la surface
	void DrawBitmap(HBITMAP hbmSrc, int dx, int dy, int angle,
	int Surface_RefAngle);												//Affiche l'image contenue dans la surface en la tournant dans l'angle voulut (0, 90, 180, 270)
	bool DrawText(HFONT hFont, LPCSTR strText, DWORD dwOriginX,
	DWORD dwOriginY, COLORREF crBackground, COLORREF crForeground);		//Affiche le texte contenu dans la surface
	void SetColorKey(unsigned long color);									//Définit la transparence pour la surface
	static SIZE GetBmpSize(HBITMAP hBmp);
	static LPVOID GetBmpData(HDC hdc, HBITMAP hBmp);
	static HBITMAP GetRotatedBitmap(HDC hdc, HBITMAP hBmpSrc, int nAngle);
	HBITMAP ReplaceColor(HBITMAP hBmp, HDC hBmpDC);
	int GetCurrentAngle() {return CurrentAngle;}
	int GetRefAngle() {return RefAngle;}
	int GetWidth() {return ddsd.dwWidth;}									//Obtient la largeur de la surface
	int GetHeight() {return ddsd.dwHeight;}									//Obtient la hauteur de la surface
	LPDIRECTDRAWSURFACE GetDDSurface() {return lpDDS;}						//Obtient la surface
	DDBLTFX * GetDDBltFX() {return &DDBltFX;}
	List * GetDynColors() {return &DynColors;}
};

#endif
