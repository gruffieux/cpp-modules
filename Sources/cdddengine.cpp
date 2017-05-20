#include <cdddengine.h>
#include <clist.h>
#include <cmsgreporter.h>
#include <dxerror.h>

DDDEngine::DDDEngine()
{
	int i;

	PrimitiveCounter = TextureCounter = 0;
	for (i = 0; i < MAX_PRIMITIVE; i++)
		PolygonCounter[i] = 0;
	for (i = 0; i < MAX_TEXTURE; i++)
		TextureID[i] = 0;
	Camera.x = Camera.y = Camera.z = 0.0f;
	CameraTarget.x = CameraTarget.y = CameraTarget.z = 0.0f;
}

DDDEngine::DDDEngine(float CameraX, float CameraY, float CameraZ)
{
	int i;

	PrimitiveCounter = TextureCounter = 0;
	for (i = 0; i < MAX_PRIMITIVE; i++)
		PolygonCounter[i] = 0;
	for (i = 0; i < MAX_TEXTURE; i++)
		TextureID[i] = 0;
	Camera.x = CameraX;
	Camera.y = CameraY;
	Camera.z = CameraZ;
	CameraTarget.x = CameraTarget.y = CameraTarget.z = 0.0f;
}

void DDDEngine::Destroy()
{
	int i;

	for (i = 0; i < TextureCounter; i++)
		LpD3DTexture[i]->Release();
	for (i = 0; i < PrimitiveCounter; i++)
		LpD3DVB[i]->Release();
	LpD3DDevice->Release();
	LpD3D->Release();
}

void DDDEngine::InitDirect3D(HWND dxWnd)
{
	D3DPRESENT_PARAMETERS d3dpp;

	LpD3D = Direct3DCreate9(D3D_SDK_VERSION);

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	ReportDxError(LpD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, dxWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &LpD3DDevice), DIRECT3D, ERR_D3D_CREATEDEVICE);

	//Turn off culling, so we see the front and back of the triangle
	ReportDxError(LpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE), DIRECT3D, ERR_D3D_SETRENDERSTATE);

	//Turn off D3D lighting, since we are providing our own vertex colors
	ReportDxError(LpD3DDevice->SetRenderState(D3DRS_LIGHTING, false), DIRECT3D, ERR_D3D_SETRENDERSTATE);

	// Turn on the zbuffer
    ReportDxError(LpD3DDevice->SetRenderState(D3DRS_ZENABLE, true), DIRECT3D, ERR_D3D_SETRENDERSTATE);
}

void DDDEngine::CreateCube(DDDAxe *Position, DDDAxe *Dimension, int texture, int id)
{
	CUSTOMVERTEX *Cube;

	//Si c'est un nouveau cube
	if (!id)
	{
		PrimitiveCounter++;
		id = PrimitiveCounter - 1;
		PolygonCounter[id] = 16;
		TextureID[id] = texture;
	}

	ReportDxError(LpD3DDevice->CreateVertexBuffer(18 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &LpD3DVB[id], NULL), DIRECT3D, ERR_D3D_CREATEVERTEXBUFFER);
	ReportDxError(LpD3DVB[id]->Lock(0, 0, (void**)&Cube, 0), DIRECT3D, ERR_D3D_LOCK);

	//Structure du cube
	Cube[0].position = D3DXVECTOR3(Position->x - Dimension->x, Position->y - Dimension->y, Position->z - Dimension->z);
	Cube[0].color = 0xffffffff;
	Cube[0].normal = Cube[0].position;
	Cube[0].tu =-Dimension->x;
	Cube[0].tv =-Dimension->y;
	Cube[1].position = D3DXVECTOR3(Position->x - Dimension->x, Position->y + Dimension->y, Position->z - Dimension->z);
	Cube[1].normal = Cube[1].position;
	Cube[1].color = 0xffffffff;
	Cube[1].tu =-Dimension->x;
	Cube[1].tv = Dimension->y;
	Cube[2].position = D3DXVECTOR3(Position->x + Dimension->x, Position->y - Dimension->y, Position->z - Dimension->z);
	Cube[2].normal = Cube[2].position;
	Cube[2].color = 0xffffffff;
	Cube[2].tu = Dimension->x;
	Cube[2].tv =-Dimension->y;
	Cube[3].position = D3DXVECTOR3(Position->x + Dimension->x, Position->y + Dimension->y, Position->z - Dimension->z);
	Cube[3].normal = Cube[3].position;
	Cube[3].color = 0xffffffff;
	Cube[3].tu = Dimension->x;
	Cube[3].tv = Dimension->y;
	Cube[4].position = D3DXVECTOR3(Position->x + Dimension->x, Position->y - Dimension->y, Position->z + Dimension->z);
	Cube[4].normal = Cube[4].position;
	Cube[4].color = 0xffffffff;
	Cube[4].tu =-Dimension->x;
	Cube[4].tv =-Dimension->y;
	Cube[5].position = D3DXVECTOR3(Position->x + Dimension->x, Position->y + Dimension->y, Position->z + Dimension->z);
	Cube[5].normal = Cube[5].position;
	Cube[5].color = 0xffffffff;
	Cube[5].tu =-Dimension->x;
	Cube[5].tv = Dimension->y;
	Cube[6].position = D3DXVECTOR3(Position->x - Dimension->x, Position->y - Dimension->y, Position->z + Dimension->z);
	Cube[6].normal = Cube[6].position;
	Cube[6].color = 0xffffffff;
	Cube[6].tu = Dimension->x;
	Cube[6].tv =-Dimension->y;
	Cube[7].position = D3DXVECTOR3(Position->x - Dimension->x, Position->y + Dimension->y, Position->z + Dimension->z);
	Cube[7].normal = Cube[7].position;
	Cube[7].color = 0xffffffff;
	Cube[7].tu = Dimension->x;
	Cube[7].tv = Dimension->y;
	Cube[8].position = D3DXVECTOR3(Position->x - Dimension->x, Position->y + Dimension->y, Position->z + Dimension->z);
	Cube[8].normal = Cube[8].position;
	Cube[8].color = 0xffffffff;
	Cube[8].tu =-Dimension->x;
	Cube[8].tv =-Dimension->y;
	Cube[9].position = D3DXVECTOR3(Position->x - Dimension->x, Position->y + Dimension->y, Position->z - Dimension->z);
	Cube[9].normal = Cube[9].position;
	Cube[9].color = 0xffffffff;
	Cube[9].tu =-Dimension->x;
	Cube[9].tv = Dimension->y;
	Cube[10].position = D3DXVECTOR3(Position->x + Dimension->x, Position->y + Dimension->y, Position->z + Dimension->z);
	Cube[10].normal = Cube[10].position;
	Cube[10].color = 0xffffffff;
	Cube[10].tu = Dimension->x;
	Cube[10].tv =-Dimension->y;
	Cube[11].position = D3DXVECTOR3(Position->x + Dimension->x, Position->y + Dimension->y, Position->z - Dimension->z);
	Cube[11].normal = Cube[11].position;
	Cube[11].color = 0xffffffff;
	Cube[11].tu = Dimension->x;
	Cube[11].tv = Dimension->y;
	Cube[12].position = D3DXVECTOR3(Position->x + Dimension->x, Position->y - Dimension->y, Position->z + Dimension->z);
	Cube[12].normal = Cube[12].position;
	Cube[12].color = 0xffffffff;
	Cube[12].tu =-Dimension->x;
	Cube[12].tv =-Dimension->y;
	Cube[13].position = D3DXVECTOR3(Position->x + Dimension->x, Position->y - Dimension->y, Position->z - Dimension->z);
	Cube[13].normal = Cube[13].position;
	Cube[13].color = 0xffffffff;
	Cube[13].tu =-Dimension->x;
	Cube[13].tv = Dimension->y;
	Cube[14].position = D3DXVECTOR3(Position->x - Dimension->x, Position->y - Dimension->y, Position->z + Dimension->z);
	Cube[14].normal = Cube[14].position;
	Cube[14].color = 0xffffffff;
	Cube[14].tu = Dimension->x;
	Cube[14].tv =-Dimension->y;
	Cube[15].position = D3DXVECTOR3(Position->x - Dimension->x, Position->y - Dimension->y, Position->z - Dimension->z);
	Cube[15].color = 0xffffffff;
	Cube[15].normal = Cube[15].position;
	Cube[15].tu = Dimension->x;
	Cube[15].tv = Dimension->y;
	Cube[16].position = D3DXVECTOR3(Position->x - Dimension->x, Position->y + Dimension->y, Position->z + Dimension->z);
	Cube[16].normal = Cube[16].position;
	Cube[16].color = 0xffffffff;
	Cube[16].tu =-Dimension->x;
	Cube[16].tv =-Dimension->y;
	Cube[17].position = D3DXVECTOR3(Position->x - Dimension->x, Position->y + Dimension->y, Position->z - Dimension->z);
	Cube[17].normal = Cube[17].position;
	Cube[17].color = 0xffffffff;
	Cube[17].tu =-Dimension->x;
	Cube[17].tv = Dimension->y;

	ReportDxError(LpD3DVB[id]->Unlock(), DIRECT3D, ERR_D3D_UNLOCK);
}

void DDDEngine::CreateCylinder(DDDAxe *Position, DDDAxe *Dimension, int side, int texture)
{
	int i;
	float theta = 0;
	CUSTOMVERTEX *Cylinder;

	side++;
	PrimitiveCounter++;
	PolygonCounter[PrimitiveCounter - 1] = side * 2 - 2;
	TextureID[PrimitiveCounter - 1] = texture;

	ReportDxError(LpD3DDevice->CreateVertexBuffer((side * 2) * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &LpD3DVB[PrimitiveCounter - 1], NULL), DIRECT3D, ERR_D3D_CREATEVERTEXBUFFER);
	ReportDxError(LpD3DVB[PrimitiveCounter - 1]->Lock(0, 0, (void**)&Cylinder, 0), DIRECT3D, ERR_D3D_LOCK);

	//Structure du cylindre
	for(i = 0; i < side; i++)
	{
		theta = (2 * D3DX_PI * i) / (side - 1);
		Cylinder[2*i+0].position = D3DXVECTOR3(Position->x + Dimension->x * sinf(theta), Position->y - Dimension->y, Position->z + Dimension->z * cosf(theta));
		Cylinder[2*i+0].normal   = Cylinder[2*i+0].position;
		Cylinder[2*i+0].color    = 0xffffffff;
		Cylinder[2*i+0].tu       = ((float)i) / (side - 1);
		Cylinder[2*i+0].tv       = 1.0f;
		Cylinder[2*i+1].position = D3DXVECTOR3(Position->x + Dimension->x * sinf(theta), Position->y + Dimension->y, Position->z + Dimension->z * cosf(theta));
		Cylinder[2*i+1].normal   = Cylinder[2*i+1].position;
		Cylinder[2*i+1].color    = 0xffffffff;
		Cylinder[2*i+1].tu       = ((float)i) / (side - 1);
		Cylinder[2*i+1].tv       = 0.0f;
	}

	ReportDxError(LpD3DVB[PrimitiveCounter - 1]->Unlock(), DIRECT3D, ERR_D3D_UNLOCK);
}

void DDDEngine::CreateTexture(char *name)
{
	TextureCounter++;

	ReportDxError(D3DXCreateTextureFromFile(LpD3DDevice, name, &LpD3DTexture[TextureCounter - 1]), DIRECT3D, ERR_D3D_CREATETEXTURE);
}

void DDDEngine::SetMatrices()
{
	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;

	//Defining the World Transformation Matrix
	D3DXMatrixRotationY(&matWorld, 0.0f);
	ReportDxError(LpD3DDevice->SetTransform(D3DTS_WORLD, &matWorld), DIRECT3D, ERR_D3D_SETTRANSFORM);

	//Defining the View Transformation Matrix
	D3DXMatrixLookAtLH(&matView, &D3DXVECTOR3(Camera.x, Camera.y, Camera.z), &D3DXVECTOR3(CameraTarget.x, CameraTarget.y, CameraTarget.z), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	ReportDxError(LpD3DDevice->SetTransform(D3DTS_VIEW, &matView), DIRECT3D, ERR_D3D_SETTRANSFORM);

	//Defining the Projection Transformation Matrix
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.4f, 1.0f, 100.0f);
	ReportDxError(LpD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj), DIRECT3D, ERR_D3D_SETTRANSFORM);
}

void DDDEngine::SetLights()
{
	D3DMATERIAL9 mtrl;
	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;

	ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	LpD3DDevice->SetMaterial(&mtrl);

	ZeroMemory(&light, sizeof(light));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Range = 1000.0f;

	vecDir = D3DXVECTOR3(0.0f, -3.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);

	ReportDxError(LpD3DDevice->SetLight(0, &light), DIRECT3D, ERR_D3D_SETLIGHT);
	ReportDxError(LpD3DDevice->LightEnable(0, true), DIRECT3D, ERR_D3D_LIGHTENABLE);
	ReportDxError(LpD3DDevice->SetRenderState(D3DRS_LIGHTING, true), DIRECT3D, ERR_D3D_SETRENDERSTATE);
	ReportDxError(LpD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020), DIRECT3D, ERR_D3D_SETRENDERSTATE);
}

void DDDEngine::SetTexture(int id)
{
	// Setup our texture. Using textures introduces the texture stage states,
	// which govern how textures get blended together (in the case of multiple
	// textures) and lighting information. In this case, we are modulating
	// (blending) our texture with the diffuse color of the vertices.
	ReportDxError(LpD3DDevice->SetTexture(0, LpD3DTexture[id - 1]), DIRECT3D, ERR_D3D_SETTEXTURE);
	ReportDxError(LpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE), DIRECT3D, ERR_D3D_SETTEXTURESTAGESTATE);
	ReportDxError(LpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE), DIRECT3D, ERR_D3D_SETTEXTURESTAGESTATE);
	ReportDxError(LpD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE), DIRECT3D, ERR_D3D_SETTEXTURESTAGESTATE);
	ReportDxError(LpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE), DIRECT3D, ERR_D3D_SETTEXTURESTAGESTATE);
}

void DDDEngine::Render()
{
	int i;

	// Clear the back buffer to a blue color
	ReportDxError(LpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0), DIRECT3D, ERR_D3D_CLEAR);

	// Begin the scene.
	ReportDxError(LpD3DDevice->BeginScene(), DIRECT3D, ERR_D3D_BEGINSCENE);

	// Render the vertex buffer contents
	for (i = 0; i < PrimitiveCounter; i++)
	{
		if (TextureID[i])
			SetTexture(TextureID[i]);
		ReportDxError(LpD3DDevice->SetStreamSource(0, LpD3DVB[i], 0, sizeof(CUSTOMVERTEX)), DIRECT3D, ERR_D3D_SETSTREAMSOURCE);
		ReportDxError(LpD3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX), DIRECT3D, ERR_D3D_SETFVF);
		ReportDxError(LpD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PolygonCounter[i]), DIRECT3D, ERR_D3D_DRAWPRIMITIVE);
	}

	// End the scene.
	ReportDxError(LpD3DDevice->EndScene(), DIRECT3D, ERR_D3D_ENDSCENE);

	// Render
	ReportDxError(LpD3DDevice->Present(NULL, NULL, NULL, NULL), DIRECT3D, ERR_D3D_PRESENT);
}
