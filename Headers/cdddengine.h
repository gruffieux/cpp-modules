#ifndef CDDDENGINE
#define CDDDENGINE

#include <dddengine.h>

class DDDEngine
{
private:
	LPDIRECT3D9 LpD3D;
	LPDIRECT3DDEVICE9 LpD3DDevice;
	LPDIRECT3DVERTEXBUFFER9 LpD3DVB[MAX_PRIMITIVE];
	LPDIRECT3DTEXTURE9 LpD3DTexture[MAX_TEXTURE];
	DDDAxe Camera, CameraTarget;
	int PrimitiveCounter, TextureCounter, PolygonCounter[MAX_PRIMITIVE], TextureID[MAX_TEXTURE];
public:
	DDDEngine();
	DDDEngine(float CameraX, float CameraY, float CameraZ);
	void Destroy();
	void InitDirect3D(HWND dxWnd);
	void CreateCube(DDDAxe *Position, DDDAxe *Dimension, int texture, int id);
	void CreateCylinder(DDDAxe *Position, DDDAxe *Dimension, int side, int texture);
	void CreateTexture(char *name);
	void SetMatrices();
	void SetLights();
	void SetTexture(int id);
	void Render();
	DDDAxe * GetCamera() {return &Camera;}
	DDDAxe * GetCameraTarget() {return &CameraTarget;}
};

#endif
