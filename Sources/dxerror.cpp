#include <dxerror.h>
#include <cstr.h>
#include <cobject.h>
#include <clist.h>
#include <cclock.h>
#include <cmsgreporter.h>

#define TOTAL_ERROR_WIN32 3
#define TOTAL_ERROR_D3D 18
#define TOTAL_ERROR_DD 15
#define TOTAL_ERROR_DI 5
#define TOTAL_ERROR_GDI 1

#ifdef _DEBUG
MsgReporter DXErrorReporter("DXError.log", TOTAL_ERROR_TYPE);
#endif

Str Win32ErrorMsg[TOTAL_ERROR_WIN32] =
{
	"ERR_REGISTERCLASS: Error when registering class",
	"ERR_CREATEWIN: Error when creating window",
	"ERR_SETTIMER: Error when setting up timer"
};

Str D3DErrorMsg[TOTAL_ERROR_D3D] =
{
	"ERR_D3D_CREATEDEVICE: Error when creating device",
	"ERR_D3D_CREATETEXTURE: Error when creating texture",
	"ERR_D3D_CREATEVERTEXBUFFER: Error when creating vertex buffer",
	"ERR_D3D_SETRENDERSTATE: Error when setting render state",
	"ERR_D3D_SETTRANSFORM: Error when transforming a matrice",
	"ERR_D3D_SETLIGHT: Error when setting light",
	"ERR_D3D_LIGHTENABLE: Error when enabling light",
	"ERR_D3D_SETTEXTURE: Error when setting texture",
	"ERR_D3D_SETTEXTURESTAGESTATE: Error when setting texture stage state",
	"ERR_D3D_SETSTREAMSOURCE: Error when setting stream source",
	"ERR_D3D_SETFVF: Error when defining vertex data layout",
	"ERR_D3D_LOCK: Error when locking",
	"ERR_D3D_UNLOCK: Error when unlocking",
	"ERR_D3D_CLEAR: Error when clearing back buffer",
	"ERR_D3D_BEGINSCENE: Error when begining scene",
	"ERR_D3D_DRAWPRIMITIVE: Error when drawing primitive",
	"ERR_D3D_ENDSCENE: Error when ending scene",
	"ERR_D3D_PRESENT: Error when presenting"
};

Str DDErrorMsg[TOTAL_ERROR_DD] =
{
	"ERR_DD_DIRECTDRAWCREATE: Error when creating DirectDraw object",
	"ERR_DD_SETCOOPERATIVELEVEL: Error when setting cooperative level",
	"ERR_DD_SETDISPLAYMODE: Error when changing display mode",
	"ERR_DD_ENUMDISPLAYMODES: Error when enumerating display modes",
	"ERR_DD_SETHWND: Error when setting window to the clipper",
	"ERR_DD_SETCLIPPER: Error when setting clipper",
	"ERR_DD_CREATESURFACE: Error when creating DirectDraw surface",
	"ERR_DD_DESTROYSURFACE: Error when destroying DirectDraw surface",
	"ERR_DD_CREATECLIPPER: Error when creating clipper",
	"ERR_DD_GETATTACHEDSURFACE: Error when requesting for attached surfaces",
	"ERR_DD_BLT: Error when blitting a surface",
	"ERR_DD_SURFACELOST: Access to surface refused because surface memory is gone",
	"ERR_DD_GETDEVICECONTEXT: Error when getting Device Context",
	"ERR_DD_DELDEVICECONTEXT: Error when deleting/releasing Device Context",
	"ERR_DD_GETROTATEDBITMAP: The bitmap couldn't be rotated"
};

Str DIErrorMsg[TOTAL_ERROR_DI] =
{
	"ERR_DI_DIRECTINPUTCREATE: Error when creating Directinput object",
	"ERR_DI_CREATEDEVICE: Error when creating device",
	"ERR_DI_SETDATAFORMAT: Error when setting data format",
	"ERR_DI_SETCOOPERATIVELEVEL: Error when setting cooperative level",
	"ERR_DI_GETDEVICESTATE: Error when retrieving data from device"
};

Str DMErrorMsg[1] =
{
	"ERR_DM_ANY: Error occured",
};

Str GDIErrorMsg[TOTAL_ERROR_GDI] =
{
	"ERR_GDI_GETDC: Error when requesting device context"
};

void InitDXErrorMsg()
{
#ifdef _DEBUG
	DXErrorReporter.AddMsgPack(WIN32_, Win32ErrorMsg, TOTAL_ERROR_WIN32, 0);
	DXErrorReporter.AddMsgPack(DIRECT3D, D3DErrorMsg, TOTAL_ERROR_D3D, 0);
	DXErrorReporter.AddMsgPack(DIRECTDRAW, DDErrorMsg, TOTAL_ERROR_DD, 0);
	DXErrorReporter.AddMsgPack(DIRECTINPUT, DIErrorMsg, TOTAL_ERROR_DI, 0);
	DXErrorReporter.AddMsgPack(DIRECTMUSIC, DMErrorMsg, 1, 0);
	DXErrorReporter.AddMsgPack(GDI, GDIErrorMsg, TOTAL_ERROR_GDI, 0);
#endif
}

void ReportDxError(int result, int msgType, int msgId)
{
#ifdef _DEBUG
	if (result != S_OK)
		DXErrorReporter.SetAndWriteMessage(msgType, msgId);
#endif
}
