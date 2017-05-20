#ifndef DDDENGINE
#define DDDENGINE

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX1)
#define MAX_PRIMITIVE 100
#define MAX_TEXTURE 100

#include <d3dx9.h>

struct DDDAxe
{
	float x;
	float y;
	float z;
};

struct CUSTOMVERTEX
{
	D3DXVECTOR3 position;	// The 3-D position for the vertex.
	D3DXVECTOR3 normal;		// The surface normal for the vertex.
	D3DCOLOR color;			// The color
	float tu, tv;			// The texture coordinates
};

#endif