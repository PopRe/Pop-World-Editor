
/*

Alacn
alacn.uhahaa@gmail.com
http://alacn.dnsalias.org:8080/

*/

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "ddraw.lib")
#define D3D_OVERLOADS
#include <ddraw.h>
#include <d3d.h>
#include <vector>
#include "Pop.h"


#define COLOR_RENDER_BG			0x007CF0
#define COLOR_GRID				0x000000
#define COLOR_SELECTED			0xFFFFFF

#define PI 3.141592f
#define TORAD(d) ((d) * PI / 180.0f)
#define TODEG(r) ((r) * 180.0f / PI)
#define CURXTO3D(x) ((float)x / ((float)dwScreenWidth / 2.0f) - 1.0f)
#define CURYTO3D(y) (1.0f - (float)y / ((float)dwScreenHeight / 2.0f))



#define SZ_ENGINE_CONFIG							"popedt.dat"
#define SZ_ENGINE_TMP_MAP							"tmp.dat"

#define SZ_ENGINE_FPS								"%s - %dx%dx%d - %.3f fps - %d ms"

#define SZ_ENGINE_RELEASE_REF						"Ref: %s: %d"
#define SZ_ENGINE_RELEASE_REF_D3DDEVICE				"D3DDevice"
#define SZ_ENGINE_RELEASE_REF_DDSPRIMARY			"DDSPrimary"
#define SZ_ENGINE_RELEASE_REF_DDSBACKBUFFER			"DDSBackBuffer"
#define SZ_ENGINE_RELEASE_REF_DDSZBUFFER			"DDSZBuffer"
#define SZ_ENGINE_RELEASE_REF_D3D					"D3D"
#define SZ_ENGINE_RELEASE_REF_DD					"DD"

#define SZ_ENGINE_CREATE							"Creating Engine: %dx%dx%d %s - %s"
#define SZ_ENGINE_DESTROY							"Engine Destroyed"
#define SZ_ENGINE_MODE_FS							"FullScreen"
#define SZ_ENGINE_MODE_WIN							"Windowed"

#define SZ_ERR_ENGINE_DIRECTDRAWCREATEEX			"DirectDrawCreateEx: 0x%X"
#define SZ_ERR_ENGINE_SETCOOPERATIVELEVEL			"SetCooperativeLevel: 0x%X"
#define SZ_ERR_ENGINE_SETDISPLAYMODE				"SetDisplayMode: 0x%X"
#define SZ_ERR_ENGINE_PRIMARYSURFACE				"Create Primary Surface: 0x%X"
#define SZ_ERR_ENGINE_CREATECLIPPER					"CreateClipper: 0x%X"
#define SZ_ERR_ENGINE_CLIPPER_SETHWND				"Clipper->SetHWnd: 0x%X"
#define SZ_ERR_ENGINE_SETCLIPPER					"SetClipper: 0x%X"
#define SZ_ERR_ENGINE_BACKBUFFER					"BackBuffer: 0x%X"
#define SZ_ERR_ENGINE_QUERYINTERFACE_D3D			"QueryInterface->D3D: 0x%X"
#define SZ_ERR_ENGINE_ENUMZBUFFER					"EnumZBuffer: 0x%X"
#define SZ_ERR_ENGINE_ZBUFFER						"ZBuffer: 0x%X"
#define SZ_ERR_ENGINE_ADDATTACHEDSURFACE_ZBUFFER	"AddAttachedSurface->ZBuffer: 0x%X"
#define SZ_ERR_ENGINE_CREATEDEVICE					"CreateDevice: 0x%X"
#define SZ_ERR_ENGINE_SETVIEWPORT					"SetViewPort: 0x%X"
#define SZ_ERR_ENGINE_ENUMTEXTUREFORMATS			"Cannot find texture with %d bit depth"
#define SZ_ERR_ENGINE_CREATETEXTURE					"Create Texture %s: 0x%X"
#define SZ_ERR_ENGINE_GETDC							"%s - GetDC: 0x%X"
#define SZ_ERR_ENGINE_RELEASEDC						"%s - ReleaseDC: 0x%X"
#define SZ_ERR_ENGINE_BLT							"%s - Blt: 0x%X"
#define SZ_ERR_ENGINE_BEGINSCENE					"BeginScene: 0x%X"
#define SZ_ERR_ENGINE_ENDSCENE						"EndScene: 0x%X"
#define SZ_ERR_ENGINE_CLEAR							"Clear: 0x%X"
#define SZ_ERR_ENGINE_LOADTEXTURE					"LoadTexture Failed: %s"
#define SZ_ERR_ENGINE_CREATEDC						"CreateDC: %s"
#define SZ_ERR_ENGINE_RESTORE_FAILED				"Restore Surface Failed: 0x%X"

#define SZ_ERR_CREATEFILE							"CreateFile Failed: %s"
#define SZ_ERR_READERROR							"Read Error at: %s"
#define SZ_ERR_WRITEERROR							"Write Error at: %s"
#define SZ_ERR_MAGIC_MISMATCH						"LEVEL3 Keyword not found, reverting to Level2 format: %s"

#define GROUND_X_SIZE					128
#define GROUND_Z_SIZE					128
#define GROUND_BLOCK_HEIGHT				0.005f

#define VIEW_RANGE						25
#define VIEW_RANGE_2					50

#define VIEW_RANGE_CLICK_MIN			15 // VIEW_RANGE -+ 10
#define VIEW_RANGE_CLICK_MAX			35
#define VIEW_RANGE_CLICK				10

#define SPHERE_RATIO					0.01f
#define GROUNDEDIT_RANGE				15
#define GROUND_HEIGHT_MAX				0x0700

#define MINIMAP_TEXTURE_SIZE			128
#define PERSON_TEXTURE_SIZE				64
#define TEXTURE_SIZE					32

#if USE_FOG
  #if USE_NEW_VIEW
    #define FOG_START					30.0f
    #define FOG_END						35.0f
  #else
    #define FOG_START					20.0f
    #define FOG_END						25.0f
  #endif
#endif

#define LIGHT_Y							15.0f

#define SPEED_POS_X						0.00000000007f
#define SPEED_POS_Y						0.00000000007f
#define SPEED_POS_Z						0.00000000007f
#define SPEED_ROT_X						0.0000000007f
#define SPEED_ROT_Y						0.0000000007f
#define SPEED_ROT_Z						0.0000000007f

#define MINIMAP_Z						5.0f
#define MINIMAP_X_ORIENTATION			1.0f
#define MINIMAP_Y_ORIENTATION		   -1.0f
#define MINIMAP_X_SPACE					0.75f
#define MINIMAP_Y_SPACE				   -0.75f

// Engine Flags
#define EF_FRAMERATE					(1 << 0)
#define EF_DITHER						(1 << 1)
#define EF_TEXTURE_FILTER				(1 << 2)
#define EF_WIREFRAME_LAND				(1 << 3)
#define EF_WIREFRAME_OBJS				(1 << 4)
#define EF_SHOW_GRID					(1 << 5)
#define EF_GRID_SEE_THROUGH				(1 << 6)
#define EF_TRANSPARENT_MINIMAP			(1 << 7)
#define EF_SHOW_MINIMAP					(1 << 8)
#define EF_HIDE_OBJECTS					(1 << 9)
#define EF_BOUNDINGBOX					(1 << 10)
#define EF_HIDE_OBJS_ON_MINIMAP			(1 << 11)
#define EF_SHOW_MARKERS					(1 << 12)



struct GROUNDHEIGHT
{
	float	height;
	DWORD	color;
};


struct TEXTURE
{
	TEXTURE						*Next,
								*Prev;
	LPDIRECTDRAWSURFACE7		lpDDSTexture;
	WORD						Width,
								Height;
	char						*Name;
	WORD						Resource;
};


struct BOUND_BOX
{
	float						minX,
								minY,
								minZ,
								maxX,
								maxY,
								maxZ;
	D3DVERTEX					v[8];
};


struct OBJ3D_PART
{
	OBJ3D_PART					*Next,
								*Prev;
	D3DMATERIAL7				Mtrl;
	TEXTURE						*texture;
	WORD						VertsCount;
	D3DVERTEX					*Verts;
};


struct OBJ3D
{
	OBJ3D						*Next,
								*Prev;
	OBJ3D_PART					*Parts;
	BOUND_BOX					BoundBox;
};


struct SCREEN_INFO
{
	SCREEN_INFO					*Next,
								*Prev;
	DWORD						Width,
								Height,
								Depth;
};


struct DEVICE_INFO
{
	DEVICE_INFO					*Next,
								*Prev;
	char						*szName;
	GUID						guid;
	bool						fHw;
};



#define FVF_COLORVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


struct COLORVERTEX
{
	float x,
		  y,
		  z;
	float nx,
		  ny,
		  nz;
	DWORD color;
	float tu,
		  tv;
};



enum MOUSEBUTTON
{
	MouseButtonLeft,
	MouseButtonRight,
	MouseButtonMiddle
};


extern char						szLevel[MAX_PATH];
extern DWORD					dwEngineFlags,
								dwScreenWidth,
								dwScreenHeight,
								dwScreenDepth,
								dwScreenWidthN,
								dwScreenHeightN;
extern char						szDeviceName[STR_SIZE];
extern GUID						guidDevice;
extern bool						fHwDevice;
extern LPDIRECTDRAW7			lpDD;
extern LPDIRECTDRAWSURFACE7		lpDDSPrimary,
								lpDDSBackBuffer;
extern LEVELDATv3				*leveldat;
extern LPDIRECT3DDEVICE7		lpD3DDevice;
extern WORD						wEngineGround[GROUND_X_SIZE * GROUND_Z_SIZE];
extern MARKER					Markers[256];
extern int						MarkerSelected;
extern LEVELVERSION				LevelVersion;
extern float					fEnginePosX,
								fEnginePosY,
								fEnginePosZ,
								fEngineRotX,
								fEngineRotY,
								fEngineRotZ;
extern int						GroundEditBrushSize,
								GroundEditBrushSpeed,
								ObjectsCount;
extern TEXTURE					*Textures;
extern OBJ3D					*Objs3D;
extern THING					*Things,
								*ThingSelected,
								*ThingLink;
extern SCREEN_INFO				*ScreenInfo;
extern DEVICE_INFO				*DeviceInfo;
extern bool						fEngineEditLand,
								fEngineEditObjs,
								fEngineEditMarkers,
								fCaptured,
								fLandEditUpdate,
								fFlatten,
								fSmooth;
extern POINT					ptCursor,
								ptCaptured;
extern MOUSEBUTTON				MouseButton;
extern DWORD					LANDSCAPE_COLORS[256];

extern std::vector<THINGSAVE> Objects;



long EnginePrepare();
long EngineFinish();
long EngineCreate();
long EngineDestroy();
long EngineRestore();
long EngineInitialize();
void EngineGetPick(D3DVECTOR *vPickRayOrig, D3DVECTOR *vPickRayDir);
int  EngineTriangleIntersect(D3DVECTOR orig, D3DVECTOR dir, D3DVECTOR v0, D3DVECTOR v1, D3DVECTOR v2);
int EngineTriangleIntersect(D3DVECTOR orig, D3DVECTOR dir, D3DVERTEX v0, D3DVERTEX v1, D3DVERTEX v2);
bool EngineGetIntersectMapSquare(D3DVECTOR orig, D3DVECTOR dir, int *sx, int *sz, float *ex, float *ez);
long EngineUpdateFrame();
long EngineDrawFrameRate();
long EngineDrawLandscape();
long EngineDrawObjects();
long EngineDrawMiniMap();
long EngineLoadLevel(char *filename);
long EngineSaveLevel(char *filename);
long EngineLoadLevelV3(char *filename);
long EngineSaveLevelV3(char *filename);
long EngineUpdateView();
WORD EngineGetGroundHeight(int x, int z);
void EngineSetGroundHeight(int x, int z, WORD h);
long EngineUpdateKeys();
long EngineUpdateMiniMap();
TEXTURE* EngineCreateTexture(WORD Width, WORD Height, char *Name, WORD Resource);
long EngineDestroyTexture(TEXTURE *texture);
long EngineTextureCreateSurface(TEXTURE *texture);
long EngineTextureDestroySurface(TEXTURE *texture);
long EngineLoadTexture(TEXTURE *texture);
long EngineRestoreTextures();
long EngineReLoadAllTextures();
long EngineUnLoadAllTextures();
bool EngineLoadObjects();
bool EngineDestroyObjects();
long EngineDrawObj3D(OBJ3D *obj, float x, float y, float z, DWORD angle);
void EngineSetTreeType();
void EngineNewMap();
void EngineLoadConfig();
void EngineSaveConfig();
long __stdcall EnumModes(LPDDSURFACEDESC2 ddsd, void*);
long __stdcall EnumDevices(char *szDescription, char *szDeviceName, LPD3DDEVICEDESC7 lpD3DDeviceDesc, void*);
void EngineMouseSetCapture();
void EngineMouseReleaseCapture();
void EngineMouseLDown();
void EngineMouseLUp();
void EngineMouseRDown();
void EngineMouseRUp();
void EngineMouseWheel(int delta);
bool IsNotNeutral(THING *t);
