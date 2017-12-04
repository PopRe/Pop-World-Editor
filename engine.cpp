
/*

Alacn
alacn.uhahaa@gmail.com
http://alacn.dnsalias.org:8080/

*/

#include "std.h"
#include "popedt.h"
#include "pop.h"
#include "engine.h"
#include "log.h"
#include "3ds.h"
#include "dialogs.h"
#include "user_storage.h"

std::vector<THINGSAVE> Objects;


LPDIRECTDRAW7			lpDD			= 0;
LPDIRECTDRAWSURFACE7	lpDDSPrimary    = 0,
						lpDDSBackBuffer = 0,
						lpDDSZBuffer    = 0;
LPDIRECT3D7				lpD3D           = 0;
LPDIRECT3DDEVICE7		lpD3DDevice     = 0;
char					szDeviceName[STR_SIZE];
GUID					guidDevice;
bool					fHwDevice;
DWORD					dwScreenWidth	= 0,
						dwScreenHeight	= 0,
						dwScreenDepth	= 0,
						dwScreenWidthN	= 0,
						dwScreenHeightN	= 0;
SCREEN_INFO				*ScreenInfo = 0;
DEVICE_INFO				*DeviceInfo = 0;
D3DMATRIX				matIdentify (
							1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f ),
						matEngineProj (
							2.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 2.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 1.0f,
							0.0f, 0.0f,-1.0f, 0.0f ),
						matEngineView (
							1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f ),
						matObjects (
							1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f );

//

char					szLevel[MAX_PATH]		= "";
DWORD					dwEngineFlags			= 0,
						dwEngineLastTick		= 0,
						dwEngineTick			= 0;
bool					fEngineEditLand			= false,
						fEngineEditObjs			= false,
						fEngineEditMarkers		= false,
						fCaptured				= false,
						fMoving					= false,
						fLandEditUpdate			= true,
						fFlatten				= false,
						fSmooth					= false;
POINT					ptCursor,
						ptCaptured;
MOUSEBUTTON				MouseButton				= MouseButtonLeft;
WORD					wEngineGround[GROUND_X_SIZE * GROUND_Z_SIZE];
LEVELDATv3				*leveldat;
MARKER					Markers[256];
int						MarkerSelected = -1;
LEVELVERSION			LevelVersion = { 0x0B, "ALACN Populous World Editor v3", __DATE__ ", " __TIME__, 0 };
GROUNDHEIGHT			GroundHeight[VIEW_RANGE_2][VIEW_RANGE_2];
float					fEnginePosX				= 0,
						fEnginePosY				= 5.5f,
						fEnginePosZ				= 0,
						fEngineRotX				= 35.0f,
						fEngineRotY				= 0,
						fEngineRotZ				= 0;
DDPIXELFORMAT			DDPixelFormat;
TEXTURE					*Textures				= 0;
THING					*Things					= 0,
						*ThingSelected			= 0,
						*ThingLink				= 0;
OBJ3D					*Objs3D					= 0;
D3DMATERIAL7			mtrlNormal;
D3DLIGHT7				lightLandscape,
						lightObjects;
int						GroundEditBrushSize		= 3,
						GroundEditBrushSpeed	= 4,
						ObjectsCount			= 0,
						LevelFormatMode			= LEVEL_FORMAT_MODE_V3;

const int GroundEditBrushSizeList[BRUSH_SIZE_MAX-BRUSH_SIZE_MIN+1] = {
	30000, 25000, 20000, 15000, 10000, 7000, 5000, 2500, 1000
};


D3DVERTEX				vObj2D[4] = { // 64x64
							D3DVERTEX(D3DVECTOR( 1.0f, 0.0f, 0.0f), D3DVECTOR(0.0f, 0.0f, -1.0f), 1.0f, 1.0f),
							D3DVERTEX(D3DVECTOR(-1.0f, 0.0f, 0.0f), D3DVECTOR(0.0f, 0.0f, -1.0f), 0.0f, 1.0f),
							D3DVERTEX(D3DVECTOR( 1.0f, 2.0f, 0.0f), D3DVECTOR(0.0f, 0.0f, -1.0f), 1.0f, 0.0f),
							D3DVERTEX(D3DVECTOR(-1.0f, 2.0f, 0.0f), D3DVECTOR(0.0f, 0.0f, -1.0f), 0.0f, 0.0f)
							};
D3DVERTEX				vObj2Dsmall[4] = { // 32x32
							D3DVERTEX(D3DVECTOR( 0.5f, 0.0f, 0.0f), D3DVECTOR(0.0f, 0.0f, -1.0f), 1.0f, 1.0f),
							D3DVERTEX(D3DVECTOR(-0.5f, 0.0f, 0.0f), D3DVECTOR(0.0f, 0.0f, -1.0f), 0.0f, 1.0f),
							D3DVERTEX(D3DVECTOR( 0.5f, 1.0f, 0.0f), D3DVECTOR(0.0f, 0.0f, -1.0f), 1.0f, 0.0f),
							D3DVERTEX(D3DVECTOR(-0.5f, 1.0f, 0.0f), D3DVECTOR(0.0f, 0.0f, -1.0f), 0.0f, 0.0f)
							};


/*
		  3------2
		 /|     /|
		0------1 |
		| |    | |
		| 7----|-6
		|/     |/
		4------5
*/

WORD BoundBoxIdxs[24] = {
		0, 1, 1, 2, 2, 3, 3, 0,
		4, 5, 5, 6, 6, 7, 7, 4,
		0, 4, 1, 5, 2, 6, 3, 7
	};



// mini map

TEXTURE					*txMiniMap;
D3DMATERIAL7			mtrlMiniMap;
D3DLIGHT7				lightMiniMap;
D3DMATRIX				matMiniMap (
							1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, MINIMAP_Z, 1.0f );
D3DVERTEX				vMiniMap[4] = {
							D3DVERTEX(D3DVECTOR( 0.5f, -0.5f, 0.0f), D3DVECTOR(0.0f, 0.0f, -1.0f), 0.0f, 0.0f),
							D3DVERTEX(D3DVECTOR(-0.5f, -0.5f, 0.0f), D3DVECTOR(0.0f, 0.0f, -1.0f), 0.0f, 0.0f),
							D3DVERTEX(D3DVECTOR( 0.5f,  0.5f, 0.0f), D3DVECTOR(0.0f, 0.0f, -1.0f), 0.0f, 0.0f),
							D3DVERTEX(D3DVECTOR(-0.5f,  0.5f, 0.0f), D3DVECTOR(0.0f, 0.0f, -1.0f), 0.0f, 0.0f)
							};

COLORVERTEX				vMiniMapDir[4] = {
							{0.25f, 0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0xFF0000, 0.0f, 0.0f},
							{0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0xFF0000, 0.0f, 0.0f},
							{0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0xFF0000, 0.0f, 0.0f},
							{0.5f,  0.25f, 0.0f, 0.0f, 0.0f, -1.0f, 0xFF0000, 0.0f, 0.0f}
							};

// frame rate

DWORD					dwEngineFpsLastTick = 0,
						dwEngineFpsFrames = 0,
						dwEngineMsLast;
float					fEngineFps = 0.0f;

// objects

OBJ3D					*objAodBlue = 0,
						*objAodRed = 0,
						*objAodYellow = 0,
						*objAodGreen = 0,
						*objBoat = 0,
						*objBoatBlue = 0,
						*objBoatRed = 0,
						*objBoatYellow = 0,
						*objBoatGreen = 0,
						*objBallon = 0,
						*objBallonBlue = 0,
						*objBallonRed = 0,
						*objBallonYellow = 0,
						*objBallonGreen = 0,
						*objTree1 = 0,
						*objTree2 = 0,
						*objTree3 = 0,
						*objTree01 = 0,
						*objTree02 = 0,
						*objTree03 = 0,
						*objTree04 = 0,
						*objTree05 = 0,
						*objTree06 = 0,
						*objTree07 = 0,
						*objTree08 = 0,
						*objTree09 = 0,
						*objTree10 = 0,
						*objTree11 = 0,
						*objTree12 = 0,
						*objTree13 = 0,
						*objTree14 = 0,
						*objKnowledge = 0,
						*objStoneHead = 0,
						*objHut1Blue = 0,
						*objHut2Blue = 0,
						*objHut3Blue = 0,
						*objHut1Red = 0,
						*objHut2Red = 0,
						*objHut3Red = 0,
						*objHut1Yellow = 0,
						*objHut2Yellow = 0,
						*objHut3Yellow = 0,
						*objHut1Green = 0,
						*objHut2Green = 0,
						*objHut3Green = 0,
						*objTowerBlue = 0,
						*objTowerRed = 0,
						*objTowerYellow = 0,
						*objTowerGreen = 0,
						*objSpyTrainBlue = 0,
						*objSpyTrainRed = 0,
						*objSpyTrainYellow = 0,
						*objSpyTrainGreen = 0,
						*objWarriorTrainBlue = 0,
						*objWarriorTrainRed = 0,
						*objWarriorTrainYellow = 0,
						*objWarriorTrainGreen = 0,
						*objTempleBlue = 0,
						*objTempleRed = 0,
						*objTempleYellow = 0,
						*objTempleGreen = 0,
						*objFwTrainBlue = 0,
						*objFwTrainRed = 0,
						*objFwTrainYellow = 0,
						*objFwTrainGreen = 0,
						*objBoatHutBlue = 0,
						*objBoatHutRed = 0,
						*objBoatHutYellow = 0,
						*objBoatHutGreen = 0,
						*objBallonHutBlue = 0,
						*objBallonHutRed = 0,
						*objBallonHutYellow = 0,
						*objBallonHutGreen = 0,
						*objPrison = 0,
						*objRSPillar = 0,
						*objRSPillarBlue = 0,
						*objRSPillarRed = 0,
						*objRSPillarYellow = 0,
						*objRSPillarGreen = 0,
						*objSpecialA = 0,
						*objSpecialB = 0,
						*objSpecialC = 0,
						*objSpecialD = 0,
						*objSpecialE = 0,
						*objSpecialF = 0,
						*objEagle = 0;
TEXTURE					*txUnknow = 0,
						*txTrigger = 0,
						*txDiscovery = 0,
						*txSpell = 0,
						*txEffect = 0,
						*txAddon = 0,
						*txWild = 0,
						*txBraveBlue = 0,
						*txBraveRed = 0,
						*txBraveYellow = 0,
						*txBraveGreen = 0,
						*txSpyBlue = 0,
						*txSpyRed = 0,
						*txSpyYellow = 0,
						*txSpyGreen = 0,
						*txWarriorBlue = 0,
						*txWarriorRed = 0,
						*txWarriorYellow = 0,
						*txWarriorGreen = 0,
						*txPriestBlue = 0,
						*txPriestRed = 0,
						*txPriestYellow = 0,
						*txPriestGreen = 0,
						*txFwBlue = 0,
						*txFwRed = 0,
						*txFwYellow = 0,
						*txFwGreen = 0,
						*txShamanBlue = 0,
						*txShamanRed = 0,
						*txShamanYellow = 0,
						*txShamanGreen = 0,
						*txPlant1 = 0,
						*txPlant2 = 0,
						*txWoodPile = 0,
						*txRock = 0,
						*txFire = 0,
						*txLight = 0,
						*txLandBridge = 0,
						*txMarker;


// landscape

DWORD LANDSCAPE_COLORS[256] = {
	0x2D2D96, 0x028000, 0x048000, 0x078000, 0x088000, 0x0A8000, 0x0C8000, 0x0E8000,
	0x108000, 0x128000, 0x158000, 0x168000, 0x188000, 0x1A8000, 0x1C8000, 0x1E8000,
	0x208000, 0x228000, 0x248000, 0x278000, 0x288000, 0x2A8000, 0x2C8000, 0x2E8000,
	0x308000, 0x328000, 0x348000, 0x378000, 0x388000, 0x3A8000, 0x3C8000, 0x3E8000,
	0x408000, 0x428000, 0x458000, 0x468000, 0x488000, 0x4A8000, 0x4D8000, 0x4E8000,
	0x508000, 0x528000, 0x548000, 0x568000, 0x588000, 0x5A8000, 0x5C8000, 0x5E8000,
	0x608100, 0x628100, 0x648100, 0x668100, 0x688100, 0x6A8100, 0x6C8100, 0x6E8100,
	0x708100, 0x728100, 0x748100, 0x768100, 0x788100, 0x7A8100, 0x7C8100, 0x7E8100,
	0x808100, 0x828100, 0x848100, 0x868100, 0x888100, 0x8A8100, 0x8C8100, 0x8E8100,
	0x908200, 0x928200, 0x948200, 0x968200, 0x988200, 0x9A8200, 0x9C8200, 0x9E8200,
	0xA08200, 0xA28200, 0xA48200, 0xA68200, 0xA88200, 0xAA8200, 0xAC8200, 0xAE8200,
	0xB08200, 0xB28200, 0xB48200, 0xB68200, 0xB88200, 0xBA8200, 0xBC8200, 0xBE8200,
	0xC08300, 0xC08402, 0xC18504, 0xC18606, 0xC28709, 0xC2880A, 0xC3890D, 0xC3890E,
	0xC38A0F, 0xC48B10, 0xC48B11, 0xC48C12, 0xC58D14, 0xC58D15, 0xC58E16, 0xC58E17,
	0xC68F18, 0xC68F1A, 0xC6901B, 0xC6901C, 0xC7911E, 0xC7921F, 0xC79220, 0xC89321,
	0xC89422, 0xC89423, 0xC99524, 0xC99526, 0xC99627, 0xCA9728, 0xCA972A, 0xCA982B,
	0xCA982C, 0xCB992D, 0xCB992E, 0xCB9A2F, 0xCC9B30, 0xCC9B32, 0xCC9C33, 0xCC9C34,
	0xCD9D35, 0xCD9E37, 0xCD9E38, 0xCE9F39, 0xCE9F3A, 0xCEA03C, 0xCFA13D, 0xCFA13E,
	0xCFA23F, 0xCFA240, 0xD0A341, 0xD0A342, 0xD0A444, 0xD1A545, 0xD1A546, 0xD1A648,
	0xD2A749, 0xD2A74A, 0xD2A84B, 0xD2A84C, 0xD3A94D, 0xD3AA4F, 0xD3AA50, 0xD4AB51,
	0xD4AB52, 0xD4AC54, 0xD4AC55, 0xD5AD56, 0xD5AE57, 0xD5AE58, 0xD6AF5A, 0xD6B05C,
	0xD7B15E, 0xD7B25F, 0xD7B261, 0xD8B362, 0xD8B463, 0xD8B465, 0xD9B566, 0xD9B668,
	0xDAB76A, 0xDAB86B, 0xDAB86D, 0xDBB96E, 0xDBBA6F, 0xDBBA70, 0xDCBB72, 0xDCBC74,
	0xDDBD77, 0xDDBE78, 0xDEBF7A, 0xDEC07C, 0xDFC17E, 0xDFC282, 0xE0C384, 0xE0C486,
	0xE1C689, 0xE2C68B, 0xE2C78D, 0xE3C88E, 0xE3C991, 0xE4CA93, 0xE4CB94, 0xE5CC96,
	0xE5CD98, 0xE5CE9A, 0xE6CE9B, 0xE6CF9D, 0xE7D1A0, 0xE7D1A2, 0xE8D2A3, 0xE8D3A5,
	0xE9D4A7, 0xE9D5A9, 0xEAD6AA, 0xEAD7AC, 0xEBD8AF, 0xEBD9B0, 0xECD9B2, 0xECDAB4,
	0xEDDBB6, 0xEDDCB7, 0xEDDDB9, 0xEEDEBB, 0xEEDFBD, 0xEFE0BF, 0xEFE1C1, 0xF0E2C3,
	0xF0E2C5, 0xF1E3C6, 0xF1E4C8, 0xF2E5CA, 0xF2E6CC, 0xF3E7CE, 0xF3E8D0, 0xF4E9D2,
	0xF4EAD3, 0xF5EAD5, 0xF5EBD7, 0xF5ECD9, 0xF6EDDA, 0xF7EEDD, 0xF7EFDF, 0xF7F0E1,
	0xF8F1E2, 0xF8F2E4, 0xF9F2E6, 0xF9F3E8, 0xFAF4E9, 0xFAF5EC, 0xFBF6EE, 0xFCF8F1,
	0xFCF9F3, 0xFDFAF5, 0xFDFBF6, 0xFDFBF8, 0xFEFCFA, 0xFFFEFD, 0xFFFEFE, 0xFFFFFF };




void VectorMatrixMultiply(D3DVECTOR *v, D3DMATRIX *mat)
{
	float x = v->x*mat->_11 + v->y*mat->_21 + v->z*mat->_31 + mat->_41;
	float y = v->x*mat->_12 + v->y*mat->_22 + v->z*mat->_32 + mat->_42;
	float z = v->x*mat->_13 + v->y*mat->_23 + v->z*mat->_33 + mat->_43;
	float w = v->x*mat->_14 + v->y*mat->_24 + v->z*mat->_34 + mat->_44;

	v->x = x/w;
	v->y = y/w;
	v->z = z/w;
}


void VertexMatrixMultiply(D3DVERTEX *v, D3DMATRIX *mat)
{
	float x = v->x*mat->_11 + v->y*mat->_21 + v->z*mat->_31 + mat->_41;
	float y = v->x*mat->_12 + v->y*mat->_22 + v->z*mat->_32 + mat->_42;
	float z = v->x*mat->_13 + v->y*mat->_23 + v->z*mat->_33 + mat->_43;
	float w = v->x*mat->_14 + v->y*mat->_24 + v->z*mat->_34 + mat->_44;

	v->x = x/w;
	v->y = y/w;
	v->z = z/w;
}


// backward subsitution
void lubksb(D3DMATRIX & a, int *indx, float *b)
{
	int		i, j, ii=-1, ip;
	float	sum;

	for (i=0; i<4; i++) {
		ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];
		if (ii>=0) {
			for (j=ii; j<=i-1; j++) {
				sum -= a(i, j) * b[j];
			}
		} else if (sum != 0.0) {
			ii = i;
		}
		b[i] = sum;
	}
	for (i=3; i>=0; i--) {
		sum = b[i];
		for (j=i+1; j<4; j++) {
			sum -= a(i, j) * b[j];
		}
		b[i] = sum/a(i, i);
	}
}


// LU decomposition
void ludcmp(D3DMATRIX & a, int *indx, float *d)
{
	float	vv[4];               /* implicit scale for each row */
	float	big, dum, sum, tmp;
	int		i, imax, j, k;

	*d = 1.0f;
	for (i=0; i<4; i++) {
		big = 0.0f;
		for (j=0; j<4; j++) {
			if ((tmp = (float) fabs(a(i, j))) > big) {
				big = tmp;
			}
		}
		/*
		if (big == 0.0f) {
			printf("ludcmp(): singular matrix found...\n");
			exit(1);
		}
		*/
		vv[i] = 1.0f/big;
	}
	for (j=0; j<4; j++) {
		for (i=0; i<j; i++) {
			sum = a(i, j);
			for (k=0; k<i; k++) {
				sum -= a(i, k) * a(k, j);
			}
			a(i, j) = sum;
		}
		big = 0.0f;
		for (i=j; i<4; i++) {
			sum = a(i, j);
			for (k=0; k<j; k++) {
				sum -= a(i, k)*a(k, j);
			}
			a(i, j) = sum;
			if ((dum = vv[i] * (float)fabs(sum)) >= big) {
				big = dum;
				imax = i;
			}
		}
		if (j != imax) {
			for (k=0; k<4; k++) {
				dum = a(imax, k);
				a(imax, k) = a(j, k);
				a(j, k) = dum;
			}
			*d = -(*d);
			vv[imax] = vv[j];
		}
		indx[j] = imax;
		if (a(j, j) == 0.0f) {
			a(j, j) = 1.0e-20f;      /* can be 0.0 also... */
		}
		if (j != 3) {
			dum = 1.0f/a(j, j);
			for (i=j+1; i<4; i++) {
				a(i, j) *= dum;
			}
		}
	}
}


D3DMATRIX MatrixInverse(const D3DMATRIX & m)
{
	D3DMATRIX	n, y;
	int			i, j, indx[4];
	float		d, col[4];

	n = m;
	ludcmp(n, indx, &d);

	for (j=0; j<4; j++) {
		for (i=0; i<4; i++) {
			col[i] = 0.0f;
		}
		col[j] = 1.0f;
		lubksb(n, indx, col);
		for (i=0; i<4; i++) {
			y(i, j) = col[i];
		}
	}
	return y;
}


long __stdcall EnumZBuffer(DDPIXELFORMAT *ddpf, void *ddpfDesired)
{
	if(ddpf->dwFlags == DDPF_ZBUFFER)
	{
		memcpy(ddpfDesired, ddpf, sizeof(DDPIXELFORMAT));
		return D3DENUMRET_CANCEL;
	}
	return D3DENUMRET_OK;
}


long __stdcall EnumTextureFormats(DDPIXELFORMAT *ddpf, void *param)
{
	if(ddpf->dwFlags & (DDPF_LUMINANCE | DDPF_BUMPLUMINANCE | DDPF_BUMPDUDV)) return DDENUMRET_OK;
	if(ddpf->dwFourCC != 0) return DDENUMRET_OK;
	if(ddpf->dwFlags & DDPF_ALPHAPIXELS) return DDENUMRET_OK;
	if(ddpf->dwRGBBitCount != dwScreenDepth) return DDENUMRET_OK;
	memcpy(param, ddpf, sizeof(DDPIXELFORMAT));
	return DDENUMRET_CANCEL;
}


long EnginePrepare()
{
	dwScreenWidthN = GetSystemMetrics(SM_CXSCREEN);
	dwScreenHeightN = GetSystemMetrics(SM_CYSCREEN);

	leveldat = (LEVELDATv3*)malloc(sizeof(LEVELDATv3));
	memset(leveldat, 0, sizeof(LEVELDATv3));

	memset(&mtrlNormal, 0, sizeof(mtrlNormal));
	mtrlNormal.diffuse.r  =
	mtrlNormal.diffuse.g  =
	mtrlNormal.diffuse.b  = 1.0f;

	memset(&mtrlMiniMap, 0, sizeof(mtrlMiniMap));
	mtrlMiniMap.diffuse.r = 1.0f;
	mtrlMiniMap.diffuse.g = 1.0f;
	mtrlMiniMap.diffuse.b = 1.0f;
	mtrlMiniMap.diffuse.a = 0.5f;

	memset(&lightLandscape, 0, sizeof(D3DLIGHT7));
	lightLandscape.dltType        = D3DLIGHT_POINT;
	lightLandscape.dcvAmbient.r   =
	lightLandscape.dcvAmbient.g   =
	lightLandscape.dcvAmbient.b   =
	lightLandscape.dcvSpecular.r  =
	lightLandscape.dcvSpecular.g  =
	lightLandscape.dcvSpecular.b  =
	lightLandscape.dcvDiffuse.r   =
	lightLandscape.dcvDiffuse.g   =
	lightLandscape.dcvDiffuse.b   = 1.0f;
	lightLandscape.dvPosition     = D3DVECTOR(0.0f, 0.0f, 0.0f);
	lightLandscape.dvRange        = D3DLIGHT_RANGE_MAX;
	lightLandscape.dvAttenuation0 = 1.0f;

	memset(&lightMiniMap, 0, sizeof(D3DLIGHT7));
	lightMiniMap.dltType       = D3DLIGHT_DIRECTIONAL;
	lightMiniMap.dcvAmbient.r  =
	lightMiniMap.dcvAmbient.g  =
	lightMiniMap.dcvAmbient.b  =
	lightMiniMap.dcvSpecular.r =
	lightMiniMap.dcvSpecular.g =
	lightMiniMap.dcvSpecular.b =
	lightMiniMap.dcvDiffuse.r  =
	lightMiniMap.dcvDiffuse.g  =
	lightMiniMap.dcvDiffuse.b  = 1.0f;
	lightMiniMap.dvDirection   = D3DVECTOR(0.0f, 0.0f, 1.0f);

	memset(&lightObjects, 0, sizeof(D3DLIGHT7));
	lightObjects.dltType       = D3DLIGHT_DIRECTIONAL;
	lightObjects.dcvAmbient.r  =
	lightObjects.dcvAmbient.g  =
	lightObjects.dcvAmbient.b  =
	lightObjects.dcvSpecular.r =
	lightObjects.dcvSpecular.g =
	lightObjects.dcvSpecular.b =
	lightObjects.dcvDiffuse.r  =
	lightObjects.dcvDiffuse.g  =
	lightObjects.dcvDiffuse.b  = 1.0f;
	lightObjects.dvDirection   = D3DVECTOR(0.0f, 0.0f, 1.0f);

	txMiniMap = EngineCreateTexture(MINIMAP_TEXTURE_SIZE, MINIMAP_TEXTURE_SIZE, 0, 0);
	EngineLoadObjects();

	EngineNewMap();

	return S_OK;
}


long EngineFinish()
{
	EngineDestroyObjects();

	while(Textures) EngineDestroyTexture(Textures);

	if (leveldat) {
		free(leveldat);
	}

	while(ScreenInfo)
	{
		SCREEN_INFO *si;
		si = ScreenInfo;
		UNLINK(ScreenInfo, si);
		delete si;
	}

	while(DeviceInfo)
	{
		DEVICE_INFO *di;
		di = DeviceInfo;
		UNLINK(DeviceInfo, di);
		if(di->szName) free(di->szName);
		delete di;
	}

	while(Things)
	{
		THING *thing = Things;
		UNLINK(Things, thing);
		delete thing;
	}

	return S_OK;
}

long EngineCreate()
{
	if((!fFullScreen) && ((dwScreenWidth >= dwScreenWidthN) || (dwScreenHeight >= dwScreenHeightN))) fFullScreen = true;

	int		x, y, w, h;
	DWORD	dwStyle;

	if(fFullScreen)
	{
		dwStyle = WS_POPUP;
		x = y = 0;
		w = dwScreenWidth;
		h = dwScreenHeight;
	}
	else
	{
		dwStyle = WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION;
		RECT rc = {0, 0, dwScreenWidth, dwScreenHeight};
		AdjustWindowRectEx(&rc, dwStyle, false, GetWindowLong(hMainWnd, GWL_EXSTYLE));
		w = rc.right - rc.left;
		h = rc.bottom - rc.top;
		x = (dwScreenWidthN - w) / 2;
		y = (dwScreenHeightN - h) / 2;
	}

	ShowWindow(hMainWnd, SW_HIDE);
	SetWindowLong(hMainWnd, GWL_STYLE, dwStyle);
	SetWindowPos(hMainWnd, 0, x, y, w, h, 0);
	ShowWindow(hMainWnd, SW_SHOW);
	UpdateWindow(hMainWnd);

	sprintf(str, SZ_ENGINE_CREATE, dwScreenWidth, dwScreenHeight, dwScreenDepth, fFullScreen? SZ_ENGINE_MODE_FS : SZ_ENGINE_MODE_WIN, szDeviceName);
	LogWrite(str);

	rs = DirectDrawCreateEx(0, (void**)&lpDD, IID_IDirectDraw7, 0);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_DIRECTDRAWCREATEEX, rs);
		LogWrite(str);
		return rs;
	}

	rs = lpDD->QueryInterface(IID_IDirect3D7, (void**)&lpD3D);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_QUERYINTERFACE_D3D);
		LogWrite(str);
		return rs;
	}

	if(!ScreenInfo) lpDD->EnumDisplayModes(0, 0, 0, EnumModes);
	if(!DeviceInfo) lpD3D->EnumDevices(EnumDevices, 0);

	if(fFullScreen)
	{
		rs = lpDD->SetCooperativeLevel(hMainWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT);
	}
	else
	{
		rs = lpDD->SetCooperativeLevel(hMainWnd, DDSCL_NORMAL | DDSCL_FPUSETUP);
	}
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_SETCOOPERATIVELEVEL, rs);
		LogWrite(str);
		return rs;
	}

	if(fFullScreen)
	{
		rs = lpDD->SetDisplayMode(dwScreenWidth, dwScreenHeight, dwScreenDepth, 0, 0);
		if FAILED(rs)
		{
			sprintf(str, SZ_ERR_ENGINE_SETDISPLAYMODE, rs);
			LogWrite(str);
			return rs;
		}
	}


	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);

	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	if(fHwDevice)
		ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
	else
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

	rs = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, 0);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_PRIMARYSURFACE, rs);
		LogWrite(str);
		return rs;
	}


	LPDIRECTDRAWCLIPPER lpClipper;

	rs = lpDD->CreateClipper(0, &lpClipper, 0);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_CREATECLIPPER, rs);
		LogWrite(str);
		return rs;
	}

	rs = lpClipper->SetHWnd(0, hMainWnd);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_CLIPPER_SETHWND, rs);
		LogWrite(str);
		return rs;
	}

	rs = lpDDSPrimary->SetClipper(lpClipper);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_SETCLIPPER, rs);
		LogWrite(str);
		return rs;
	}

	lpClipper->Release();


	ddsd.dwFlags |= DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
	ddsd.dwWidth = dwScreenWidth;
	ddsd.dwHeight = dwScreenHeight;

	if(fHwDevice)
		ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
	else
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

	rs = lpDD->CreateSurface(&ddsd, &lpDDSBackBuffer, 0);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_BACKBUFFER, rs);
		LogWrite(str);
		return rs;
	}

	/*
	rs = lpDD->QueryInterface(IID_IDirect3D7, (void**)&lpD3D);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_QUERYINTERFACE_D3D);
		LogWrite(str);
		return rs;
	}
	*/

	rs = lpD3D->EnumZBufferFormats(guidDevice, EnumZBuffer, &ddsd.ddpfPixelFormat);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_ENUMZBUFFER, rs);
		LogWrite(str);
		return rs;
	}

	ddsd.dwFlags |= DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_ZBUFFER;

	if(fHwDevice)
		ddsd.ddsCaps.dwCaps |= DDSCAPS_VIDEOMEMORY;
	else
		ddsd.ddsCaps.dwCaps |= DDSCAPS_SYSTEMMEMORY;

	rs = lpDD->CreateSurface(&ddsd, &lpDDSZBuffer, 0);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_ZBUFFER, rs);
		LogWrite(str);
		return rs;
	}

	rs = lpDDSBackBuffer->AddAttachedSurface(lpDDSZBuffer);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_ADDATTACHEDSURFACE_ZBUFFER, rs);
		LogWrite(str);
		return rs;
	}

	rs = lpD3D->CreateDevice(guidDevice, lpDDSBackBuffer, &lpD3DDevice);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_CREATEDEVICE, rs);
		LogWrite(str);
		return rs;
	}

	lpD3DDevice->EnumTextureFormats(EnumTextureFormats, &DDPixelFormat);
	if(0 == DDPixelFormat.dwRGBBitCount)
	{
		sprintf(str, SZ_ERR_ENGINE_ENUMTEXTUREFORMATS, dwScreenDepth);
		LogWrite(str);
		return rs;
	}

	fEngineActive = true;
	EngineReLoadAllTextures();
	EngineInitialize();

	return S_OK;
}


long EngineDestroy()
{
	fEngineActive = false;

	EngineUnLoadAllTextures();

	if(lpDD) lpDD->SetCooperativeLevel(hMainWnd, DDSCL_NORMAL);

	if(lpD3DDevice)
	{
		rs = lpD3DDevice->Release();
		lpD3DDevice = 0;

		if(rs != 0)
		{
			sprintf(str, SZ_ENGINE_RELEASE_REF, SZ_ENGINE_RELEASE_REF_D3DDEVICE, rs);
			LogWrite(str);
		}
	}

	if(lpDDSPrimary)
	{
		rs = lpDDSPrimary->Release();
		lpDDSPrimary = 0;

		if(rs != 0)
		{
			sprintf(str, SZ_ENGINE_RELEASE_REF, SZ_ENGINE_RELEASE_REF_DDSPRIMARY, rs);
			LogWrite(str);
		}
	}

	if(lpDDSBackBuffer)
	{
		rs = lpDDSBackBuffer->Release();
		lpDDSBackBuffer = 0;

		if(rs != 0)
		{
			sprintf(str, SZ_ENGINE_RELEASE_REF, SZ_ENGINE_RELEASE_REF_DDSBACKBUFFER, rs);
			LogWrite(str);
		}
	}

	if(lpDDSZBuffer)
	{
		rs = lpDDSZBuffer->Release();
		lpDDSZBuffer = 0;

		if(rs != 0)
		{
			sprintf(str, SZ_ENGINE_RELEASE_REF, SZ_ENGINE_RELEASE_REF_DDSZBUFFER, rs);
			LogWrite(str);
		}
	}

	if(lpD3D)
	{
		rs = lpD3D->Release();
		lpD3D = 0;

		if(rs > 1)
		{
			sprintf(str, SZ_ENGINE_RELEASE_REF, SZ_ENGINE_RELEASE_REF_D3D, rs);
			LogWrite(str);
		}
	}

	if(lpDD)
	{
		rs = lpDD->Release();
		lpDD = 0;

		if(rs != 0)
		{
			sprintf(str, SZ_ENGINE_RELEASE_REF, SZ_ENGINE_RELEASE_REF_DD, rs);
			LogWrite(str);
		}
	}

	LogWrite(SZ_ENGINE_DESTROY);

	InvalidateRect(hMainWnd, 0, true);

	return S_OK;
}


long EngineInitialize()
{
	if(!fEngineActive) return -1;

	D3DVIEWPORT7 vp = {0, 0, dwScreenWidth, dwScreenHeight, 0.0f, 1.0f};
	rs = lpD3DDevice->SetViewport(&vp);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_SETVIEWPORT, rs);
		LogWrite(str);
	}

	if(dwEngineFlags & EF_DITHER)
		lpD3DDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, true);
	else
		lpD3DDevice->SetRenderState(D3DRENDERSTATE_DITHERENABLE, false);

	if(dwEngineFlags & EF_TEXTURE_FILTER)
	{
		lpD3DDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
		lpD3DDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_LINEAR);
	}
	else
	{
		lpD3DDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_POINT);
		lpD3DDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_POINT);
	}

	//matEngineProj._11 = (float)dwScreenHeight / (float)dwScreenWidth * matEngineProj._22;
	matEngineProj._22 = (float)dwScreenWidth / (float)dwScreenHeight * matEngineProj._11;
	lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &matEngineProj);
	
	matMiniMap._41 = MINIMAP_X_ORIENTATION / matEngineProj._11 * matMiniMap._43 - MINIMAP_X_SPACE;
	matMiniMap._42 = MINIMAP_Y_ORIENTATION / matEngineProj._22 * matMiniMap._43 - MINIMAP_Y_SPACE;

	lpD3DDevice->SetRenderState(D3DRENDERSTATE_AMBIENT, 0);
	lpD3DDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, true);
	lpD3DDevice->LightEnable(0, true);
	lpD3DDevice->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

#if USE_FOG
	lpD3DDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, true);
	lpD3DDevice->SetRenderState(D3DRENDERSTATE_FOGCOLOR, COLOR_RENDER_BG);
	lpD3DDevice->SetRenderState(D3DRENDERSTATE_FOGVERTEXMODE, D3DFOG_LINEAR);

	float fStart = FOG_START,
		  fEnd   = FOG_END;

	lpD3DDevice->SetRenderState(D3DRENDERSTATE_FOGSTART, *(DWORD*)&fStart);
	lpD3DDevice->SetRenderState(D3DRENDERSTATE_FOGEND, *(DWORD*)&fEnd);
#endif

	if FAILED(rs = EngineUpdateMiniMap()) return rs;
	return EngineUpdateView();
}


void EngineGetPick(D3DVECTOR *vPickRayOrig, D3DVECTOR *vPickRayDir)
{
	D3DVECTOR v;
	v.x = (((2.0f * ptCursor.x) / dwScreenWidth) - 1) / matEngineProj._11;
	v.y = -(((2.0f * ptCursor.y) / dwScreenHeight) - 1) / matEngineProj._22;
	v.z = 1.0f;

	D3DMATRIX m = MatrixInverse(matEngineView);

	vPickRayDir->x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
	vPickRayDir->y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
	vPickRayDir->z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
	vPickRayOrig->x = m._41;
	vPickRayOrig->y = m._42;
	vPickRayOrig->z = m._43;
}


int EngineTriangleIntersect(D3DVECTOR orig, D3DVECTOR dir, D3DVECTOR v0, D3DVECTOR v1, D3DVECTOR v2)
{
	D3DVECTOR edge1, edge2, tvec, pvec, qvec;
	float det, u, v;

	edge1 = v1 - v0;
	edge2 = v2 - v0;

	pvec = CrossProduct(dir, edge2);
	det = DotProduct(edge1, pvec);

	if(det < 1.0e-5f) return 0;

	tvec = orig - v0;

	u = DotProduct(tvec, pvec);
	if(u < 0.0f || u > det) return 0;

	qvec = CrossProduct(tvec, edge1);

	v = DotProduct(dir, qvec);
	if(v < 0.0f || u + v > det) return 0;

	return 1;
}


int EngineTriangleIntersect(D3DVECTOR orig, D3DVECTOR dir, D3DVERTEX v0, D3DVERTEX v1, D3DVERTEX v2)
{
	return EngineTriangleIntersect(orig, dir, D3DVECTOR(v0.x, v0.y, v0.z), D3DVECTOR(v1.x, v1.y, v1.z), D3DVECTOR(v2.x, v2.y, v2.z));
}


bool EngineGetIntersectMapSquare(D3DVECTOR orig, D3DVECTOR dir, int *sx, int *sz, float *ex, float *ez)
{
	float dist, sdist;
	bool f = false;
	D3DVECTOR v[4];

	int x = (int)fEnginePosX - VIEW_RANGE_CLICK,
		z = (int)fEnginePosZ - VIEW_RANGE_CLICK;

	for(int az = VIEW_RANGE_CLICK_MIN; az < VIEW_RANGE_CLICK_MAX; az++, z++, x = (int)fEnginePosX - VIEW_RANGE_CLICK)
	for(int ax = VIEW_RANGE_CLICK_MIN; ax < VIEW_RANGE_CLICK_MAX; ax++, x++)
	{
		v[0].x = (float)x;
		v[0].z = (float)z;
		v[0].y = GroundHeight[ax][az].height;

		v[1].x = (float)x;
		v[1].z = (float)(z + 1);
		v[1].y = GroundHeight[ax][az + 1].height;

		v[2].x = (float)(x + 1);
		v[2].z = (float)(z + 1);
		v[2].y = GroundHeight[ax + 1][az + 1].height;

		v[3].x = (float)(x + 1);
		v[3].z = (float)z;
		v[3].y = GroundHeight[ax + 1][az].height;

		if(EngineTriangleIntersect(orig, dir, v[0], v[1], v[2]) || EngineTriangleIntersect(orig, dir, v[2], v[3], v[0]))
		{
			dist = ((v[0].x + v[1].x + v[2].x + v[3].x) / 4.0f - orig.x) + ((v[0].z + v[1].z + v[2].z + v[3].z) / 4.0f - orig.z);
			if(dist < 0) dist *= -1;

			if(!f || (dist < sdist))
			{
				f = true;
				if(sx) *sx = ax;
				if(sz) *sz = az;
				if(ex) *ex = (float)x;
				if(ez) *ez = (float)z;
				sdist = dist;
			}
		}
	}

	return f;
}


long EngineUpdateFrame()
{
	if(!fEngineActive) return -1;

	GetCursorPos(&ptCursor);
	ScreenToClient(hMainWnd, &ptCursor);

	// if(fCaptured && ((ptCursor.x != ptCaptured.x) || (ptCursor.y != ptCaptured.y)))
	if(fCaptured &&
		((ptCursor.x < ptCaptured.x - 5) || (ptCursor.x > ptCaptured.x + 5) ||
		 (ptCursor.y < ptCaptured.y - 5) || (ptCursor.y > ptCaptured.y + 5)))
	{
		fMoving = true;
		fLandEditUpdate = true;
	}

	DWORD dw = GetTickCount();
	dwEngineTick = dwEngineLastTick - dw;
	dwEngineLastTick = dw;

	if FAILED(rs = EngineUpdateKeys()) return rs;

	DDBLTFX	bltfx;
	memset(&bltfx, 0, sizeof(bltfx));
	bltfx.dwSize = sizeof(bltfx);

	bltfx.dwFillColor = 0;
	rs = lpDDSBackBuffer->Blt(0, 0, 0, DDBLT_COLORFILL, &bltfx);
	if FAILED(rs)
	{
		if(DDERR_SURFACELOST == rs) return EngineRestore();

		sprintf(str, SZ_ERR_ENGINE_BLT, "UpdateFrame - BackBuffer FillColor", rs);
		LogWrite(str);

		return rs;
	}

	rs = lpD3DDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, COLOR_RENDER_BG, 1.0f, 0);
	if FAILED(rs)
	{
		if(rs == DDERR_SURFACELOST) return EngineRestore();

		sprintf(str, SZ_ERR_ENGINE_CLEAR, rs);
		LogWrite(str);

		return rs;
	}

	rs = lpD3DDevice->BeginScene();
	if FAILED(rs)
	{
		if(rs == DDERR_SURFACELOST) return EngineRestore();

		sprintf(str, SZ_ERR_ENGINE_BEGINSCENE, rs);
		LogWrite(str);

		return rs;
	}

	if FAILED(rs = EngineDrawLandscape())
	{
		lpD3DDevice->EndScene();
		return rs;
	}

	if(!(dwEngineFlags & EF_HIDE_OBJECTS)) if FAILED(rs = EngineDrawObjects())
	{
		lpD3DDevice->EndScene();
		return rs;
	}

	if(dwEngineFlags & EF_SHOW_MINIMAP) if FAILED(rs = EngineDrawMiniMap())
	{
		lpD3DDevice->EndScene();
		return rs;
	}

	rs = lpD3DDevice->EndScene();
	if FAILED(rs)
	{
		if(rs == DDERR_SURFACELOST) return EngineRestore();

		sprintf(str, SZ_ERR_ENGINE_ENDSCENE, rs);
		LogWrite(str);

		return rs;
	}

	if(dwEngineFlags & EF_FRAMERATE)
		if FAILED(rs = EngineDrawFrameRate())
		{
			if(rs == DDERR_SURFACELOST)
				return EngineRestore();
			else
				return rs;
		}

	if(fFullScreen)
		rs = lpDDSPrimary->Blt(0, lpDDSBackBuffer, 0, 0, 0);
	else
		rs = lpDDSPrimary->Blt(&rcClient, lpDDSBackBuffer, 0, 0, 0);

	if FAILED(rs)
	{
		if(rs == DDERR_SURFACELOST) return EngineRestore();

		sprintf(str, SZ_ERR_ENGINE_BLT, "UpdateFrame - Primary", rs);
		LogWrite(str);

		return rs;
	}

	return S_OK;
}


long EngineRestore()
{
	if SUCCEEDED(rs = lpDDSPrimary->Restore())
	{
		lpDDSBackBuffer->Restore();
		lpDDSZBuffer->Restore();
		EngineRestoreTextures();
	}
	else
	{
		sprintf(str, SZ_ERR_ENGINE_RESTORE_FAILED, rs);
		LogWrite(str);
		return rs;
	}

	return EngineInitialize();
}


long EngineDrawFrameRate()
{
	HDC		hDC;

	rs = lpDDSBackBuffer->GetDC(&hDC);
	if FAILED(rs)
	{
		if(rs == DDERR_SURFACELOST) return rs;

		sprintf(str, SZ_ERR_ENGINE_GETDC, "DrawFrameRate", rs);
		LogWrite(str);

		return rs;
	}

	DWORD dwTick = GetTickCount();

	dwEngineFpsFrames++;
	if((dwTick - dwEngineFpsLastTick) > 1000)
	{
		fEngineFps = (float)dwEngineFpsFrames / (((float)dwTick - (float)dwEngineFpsLastTick) / 1000.0f);
		dwEngineFpsFrames = 0;
		dwEngineFpsLastTick = dwTick;
	}

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, 0x00FFFF);

	sprintf(str, SZ_ENGINE_FPS,
		szDeviceName, dwScreenWidth, dwScreenHeight, dwScreenDepth,
		fEngineFps, (dwTick - dwEngineMsLast));

	int len = strlen(str);

	SIZE sz;
	GetTextExtentPoint(hDC, str, len, &sz);

	TextOut(hDC, 1, dwScreenHeight - sz.cy - 1, str, len);

	dwEngineMsLast = dwTick;

	rs = lpDDSBackBuffer->ReleaseDC(hDC);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_RELEASEDC, "DrawFrameRate", rs);
		LogWrite(str);
		return rs;
	}

	return S_OK;
}


long EngineDrawLandscape()
{
	if (dwEngineFlags & EF_WIREFRAME_LAND) lpD3DDevice->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_WIREFRAME);

	lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, &matEngineView);
	lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matIdentify);
	lpD3DDevice->SetLight(0, &lightLandscape);

	lpD3DDevice->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	lpD3DDevice->SetMaterial(&mtrlNormal);
	lpD3DDevice->SetTexture(0, 0);

	COLORVERTEX v[6];
	memset(v, 0, sizeof(v));
	v[0].ny = 1.0f;
	v[1].ny = 1.0f;
	v[2].ny = 1.0f;
	v[3].ny = 1.0f;
	v[4].ny = 1.0f;
	v[5].ny = 1.0f;

	v[0].tu = 1.0f;
	v[0].tv = 0.0f;
	v[1].tu = 0.0f;
	v[1].tv = 0.0f;
	v[2].tu = 1.0f;
	v[2].tv = 1.0f;
	v[3].tu = 1.0f;
	v[3].tv = 1.0f;
	v[4].tu = 0.0f;
	v[4].tv = 0.0f;
	v[5].tu = 0.0f;
	v[5].tv = 1.0f;

	int x = (int)fEnginePosX - VIEW_RANGE,
		z = (int)fEnginePosZ - VIEW_RANGE;

	for (int az = 0; az < VIEW_RANGE_2 - 1; az++, z++, x = (int)fEnginePosX - VIEW_RANGE)
		for (int ax = 0; ax < VIEW_RANGE_2 - 1; ax++, x++)
		{
			v[0].x = (float)(x + 1);
			v[0].z = (float)z;
			v[0].y = GroundHeight[ax + 1][az].height;
			v[0].color = GroundHeight[ax + 1][az].color;

			v[1].x = (float)x;
			v[1].z = (float)z;
			v[1].y = GroundHeight[ax][az].height;
			v[1].color = GroundHeight[ax][az].color;

			v[2].x = (float)(x + 1);
			v[2].z = (float)(z + 1);
			v[2].y = GroundHeight[ax + 1][az + 1].height;
			v[2].color = GroundHeight[ax + 1][az + 1].color;

			v[3] = v[2];

			v[4] = v[1];

			v[5].x = (float)x;
			v[5].z = (float)(z + 1);
			v[5].y = GroundHeight[ax][az + 1].height;
			v[5].color = GroundHeight[ax][az + 1].color;

			lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, FVF_COLORVERTEX, v, 6, 0);
		}

#if USE_FOG
	bool inSphericalView = (fEnginePosY < MAX_SPERICAL_POS_Y);
	if (inSphericalView)
	{
		lpD3DDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, true);
	}
	else
	{
		lpD3DDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, false);
	}
#endif

	if((dwEngineFlags & EF_GRID_SEE_THROUGH) && inSphericalView)
	{
#if USE_FOG
		lpD3DDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, false);
#endif
		lpD3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
	}

	if(dwEngineFlags & EF_SHOW_GRID)
	{
		v[0].nx = 0.0f;
		v[0].ny = 1.0f;
		v[0].nz = 0.0f;

		v[1].nx = 0.0f;
		v[1].ny = 1.0f;
		v[1].nz = 0.0f;

		v[2].nx = 0.0f;
		v[2].ny = 1.0f;
		v[2].nz = 0.0f;

		v[0].color =
		v[1].color =
		v[2].color = COLOR_GRID;

		x = (int)fEnginePosX - VIEW_RANGE + 1; // +1 skip border, no square border effect
		z = (int)fEnginePosZ - VIEW_RANGE + 1;

		for(int az = 1; az < VIEW_RANGE_2 - 1; az++, z++, x = (int)fEnginePosX - VIEW_RANGE + 1)
		for(int ax = 1; ax < VIEW_RANGE_2 - 1; ax++, x++)
		{
			v[0].x = (float)(x + 1);
			v[0].z = (float)z;
			v[0].y = GroundHeight[ax + 1][az].height;

			v[1].x = (float)x;
			v[1].z = (float)z;
			v[1].y = GroundHeight[ax][az].height;

			v[2].x = (float)x;
			v[2].z = (float)(z + 1);
			v[2].y = GroundHeight[ax][az + 1].height;

			lpD3DDevice->DrawPrimitive(D3DPT_LINESTRIP, FVF_COLORVERTEX, v, 3, 0);
		}

		// no square border effect

		x = (int)fEnginePosX - VIEW_RANGE;
		z = (int)fEnginePosZ - VIEW_RANGE;

		for(int ax = 0; ax < VIEW_RANGE_2 - 1; ax++, x++)
		{
			v[0].x = (float)x;
			v[0].z = (float)z;
			v[0].y = GroundHeight[ax][0].height;

			v[1].x = (float)x;
			v[1].z = (float)(z + 1);
			v[1].y = GroundHeight[ax][1].height;

			lpD3DDevice->DrawPrimitive(D3DPT_LINESTRIP, FVF_COLORVERTEX, v, 2, 0);
		}

		x = (int)fEnginePosX - VIEW_RANGE;
		//z = (int)fEnginePosZ - VIEW_RANGE; // set above and not changed

		for(int az = 0; az < VIEW_RANGE_2 - 1; az++, z++)
		{
			v[0].x = (float)x;
			v[0].z = (float)z;
			v[0].y = GroundHeight[0][az].height;

			v[1].x = (float)(x + 1);
			v[1].z = (float)z;
			v[1].y = GroundHeight[1][az].height;

			lpD3DDevice->DrawPrimitive(D3DPT_LINESTRIP, FVF_COLORVERTEX, v, 2, 0);
		}
	}

	if(fEngineEditLand)
	{
		D3DVECTOR r0, r1;
		EngineGetPick(&r0, &r1);

		int sx, sz;
		float ex, ez;

		static int _sx, _sz;
		static float _ex, _ez;

		if(fCaptured && !fLandEditUpdate)
		{
			sx = _sx;
			sz = _sz;
			ex = _ex;
			ez = _ez;
			goto _skip;
		}

		if(EngineGetIntersectMapSquare(r0, r1, &sx, &sz, &ex, &ez))
		{
			_sx = sx;
			_sz = sz;
			_ex = ex;
			_ez = ez;
			ptCaptured = ptCursor;
			fLandEditUpdate = false;
_skip:

			//

			v[0].color =
			v[1].color =
			v[2].color =
			v[3].color = COLOR_SELECTED;

			if(GroundEditBrushSize == 0)
			{
				v[0].x = ex - 1.0f;
				v[0].y = GroundHeight[sx-1][sz].height;
				v[0].z = ez;

				v[1].x = ex;
				v[1].y = GroundHeight[sx][sz].height;
				v[1].z = ez;

				v[2].x = ex + 1.0f;
				v[2].y = GroundHeight[sx+1][sz].height;
				v[2].z = ez;

				lpD3DDevice->DrawPrimitive(D3DPT_LINESTRIP, FVF_COLORVERTEX, v, 3, 0);

				v[0].x = ex;
				v[0].y = GroundHeight[sx][sz-1].height;
				v[0].z = ez - 1.0f;

				v[1].x = ex;
				v[1].y = GroundHeight[sx][sz].height;
				v[1].z = ez;

				v[2].x = ex;
				v[2].y = GroundHeight[sx][sz+1].height;
				v[2].z = ez + 1.0f;

				lpD3DDevice->DrawPrimitive(D3DPT_LINESTRIP, FVF_COLORVERTEX, v, 3, 0);
			}
			else
			{
				int a, b = GroundEditBrushSize / 2;

				a = 0;
				while(a < GroundEditBrushSize)
				{
					v[0].x = (float)(ex + a - b);
					v[0].z = (float)(ez - b);
					v[0].y = GroundHeight[sx + a - b][sz - b].height;

					v[1].x = (float)(ex + a + 1 - b);
					v[1].z = (float)(ez - b);
					v[1].y = GroundHeight[sx + a + 1 - b][sz - b].height;

					v[2].x = (float)(ex + a - b);
					v[2].z = (float)(ez + GroundEditBrushSize - b);
					v[2].y = GroundHeight[sx + a - b][sz + GroundEditBrushSize - b].height;

					v[3].x = (float)(ex + a + 1 - b);
					v[3].z = (float)(ez + GroundEditBrushSize - b);
					v[3].y = GroundHeight[sx + a + 1 - b][sz + GroundEditBrushSize - b].height;

					lpD3DDevice->DrawPrimitive(D3DPT_LINELIST, FVF_COLORVERTEX, v, 4, 0);

					a++;
				}

				a = 0;
				while(a < GroundEditBrushSize)
				{
					v[0].x = (float)(ex - b);
					v[0].z = (float)(ez + a - b);
					v[0].y = GroundHeight[sx - b][sz + a - b].height;

					v[1].x = (float)(ex - b);
					v[1].z = (float)(ez + a + 1 - b);
					v[1].y = GroundHeight[sx - b][sz + a + 1 - b].height;

					v[2].x = (float)(ex + GroundEditBrushSize - b);
					v[2].z = (float)(ez + a - b);
					v[2].y = GroundHeight[sx + GroundEditBrushSize - b][sz + a - b].height;

					v[3].x = (float)(ex + GroundEditBrushSize - b);
					v[3].z = (float)(ez + a + 1 - b);
					v[3].y = GroundHeight[sx + GroundEditBrushSize - b][sz + a + 1 - b].height;

					lpD3DDevice->DrawPrimitive(D3DPT_LINELIST, FVF_COLORVERTEX, v, 4, 0);

					a++;
				}
			}

			//

			if(fCaptured)
			{
				WORD m;

				if(fFlatten) m = EngineGetGroundHeight((int)ex, (int)ez);

				x = (int)ex - (GroundEditBrushSize / 2);
				z = (int)ez - (GroundEditBrushSize / 2);
				
				WORD w = EngineGetGroundHeight(x, z),
					 v = (WORD)dwEngineTick / GroundEditBrushSizeList[GroundEditBrushSpeed];

				bool b = false;
				int ax, az;

				if(fSmooth)
				{
					az = 0;
					while(az <= GroundEditBrushSize)
					{
						ax = 0;
						while(ax <= GroundEditBrushSize)
						{
							int c = 1;
							w = EngineGetGroundHeight(x+ax, z+az);
							WORD h, h2 = w;

							h = EngineGetGroundHeight(x+ax - 1, z+az - 1);
                            if(h > 7)
							{
								h2 += h;
								c++;
							}
							h = EngineGetGroundHeight(x+ax    , z+az - 1);
                            if(h > 7)
							{
								h2 += h;
								c++;
							}
							h = EngineGetGroundHeight(x+ax + 1, z+az - 1);
                            if(h > 7)
							{
								h2 += h;
								c++;
							}
							h = EngineGetGroundHeight(x+ax - 1, z+az    );
                            if(h > 7)
							{
								h2 += h;
								c++;
							}
							h = EngineGetGroundHeight(x+ax + 1, z+az    );
                            if(h > 7)
							{
								h2 += h;
								c++;
							}
							h = EngineGetGroundHeight(x+ax - 1, z+az + 1);
                            if(h > 7)
							{
								h2 += h;
								c++;
							}
							h = EngineGetGroundHeight(x+ax    , z+az + 1);
                            if(h > 7)
							{
								h2 += h;
								c++;
							}
							h = EngineGetGroundHeight(x+ax + 1, z+az + 1);
                            if(h > 7)
							{
								h2 += h;
								c++;
							}

							if(c > 1)
							{
								h2 /= c;

								if(w < h2)
								{
									if(w + v > h2)
										EngineSetGroundHeight(x+ax, z+az, h2);
									else
										EngineSetGroundHeight(x+ax, z+az, w + v);
								}
								else if(w > h2)
								{
									if(w - v < h2)
										EngineSetGroundHeight(x+ax, z+az, h2);
									else
										EngineSetGroundHeight(x+ax, z+az, w - v);
								}
							}

							ax++;
						}
						az++;
					}
				}
				else if(fFlatten)
				{
					az = 0;
					while(az <= GroundEditBrushSize)
					{
						ax = 0;
						while(ax <= GroundEditBrushSize)
						{
							w = EngineGetGroundHeight(x + ax, z + az);

							if(w < m)
							{
								if(w + v > m)
									EngineSetGroundHeight(x + ax, z + az, m);
								else
									EngineSetGroundHeight(x + ax, z + az, w + v);
							}
							else if(w > m)
							{
								if(w - v < m)
									EngineSetGroundHeight(x + ax, z + az, m);
								else
									EngineSetGroundHeight(x + ax, z + az, w - v);
							}
							ax++;
						}
						az++;
					}
				}
				else if(MouseButton == MouseButtonLeft)
				{
					m = EngineGetGroundHeight(x, z);
					az = 0;
					while(az <= GroundEditBrushSize)
					{
						ax = 0;
						while(ax <= GroundEditBrushSize)
						{
							w = EngineGetGroundHeight(x + ax, z + az);
							if(w != m) b = true;

							/* min progressive
							if(w < m) m = w;
							*/

							if(w > m) m = w;
							ax++;
						}
						az++;
					}

					az = 0;
					while(az <= GroundEditBrushSize)
					{
						ax = 0;
						while(ax <= GroundEditBrushSize)
						{
							w = EngineGetGroundHeight(x + ax, z + az);
							
							/* min progressive
							if((w == m) || (!b))
							{
								if(w + v > GROUND_HEIGHT_MAX)
									EngineSetGroundHeight(x + ax, z + az, GROUND_HEIGHT_MAX);
								else
									EngineSetGroundHeight(x + ax, z + az, w + v);
							}
							*/

							if(b)
							{
								if(w < m)
								{
									if(w + v > m)
										EngineSetGroundHeight(x + ax, z + az, m);
									else
										EngineSetGroundHeight(x + ax, z + az, w + v);
								}
							}
							else
							{
								if(w + v > GROUND_HEIGHT_MAX)
									EngineSetGroundHeight(x + ax, z + az, GROUND_HEIGHT_MAX);
								else
									EngineSetGroundHeight(x + ax, z + az, w + v);
							}

							ax++;
						}
						az++;
					}
				}
				else if(MouseButton == MouseButtonRight)
				{
					m = EngineGetGroundHeight(x, z);
					az = 0;
					while(az <= GroundEditBrushSize)
					{
						ax = 0;
						while(ax <= GroundEditBrushSize)
						{
							w = EngineGetGroundHeight(x + ax, z + az);
							if(w != m) b = true;
							
							/* min progressive
							if(w > m) m = w;
							*/

							if(w < m) m = w;
							ax++;
						}
						az++;
					}

					az = 0;
					while(az <= GroundEditBrushSize)
					{
						ax = 0;
						while(ax <= GroundEditBrushSize)
						{
							w = EngineGetGroundHeight(x + ax, z + az);

							/* min progressive
							if((w == m) || (!b))
							{
								if(w >= v)
									EngineSetGroundHeight(x + ax, z + az, w - v);
								else
									EngineSetGroundHeight(x + ax, z + az, 0);
							}
							*/

							if(b)
							{
								if(w > m)
								{
									if(w >= v && w - v > m)
										EngineSetGroundHeight(x + ax, z + az, w - v);
									else
										EngineSetGroundHeight(x + ax, z + az, m);
								}
							}
							else
							{
								if(w >= v)
									EngineSetGroundHeight(x + ax, z + az, w - v);
								else
									EngineSetGroundHeight(x + ax, z + az, 0);
							}

							ax++;
						}
						az++;
					}
				}

				EngineUpdateView();
			}

		}
	}

	if((dwEngineFlags & EF_GRID_SEE_THROUGH) && inSphericalView)
	{
#if USE_FOG
		lpD3DDevice->SetRenderState(D3DRENDERSTATE_FOGENABLE, true);
#endif
		lpD3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_TRUE);
	}

	if(dwEngineFlags & EF_WIREFRAME_LAND) lpD3DDevice->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_SOLID);

	lpD3DDevice->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

	return S_OK;
}


long EngineDrawObjects()
{
	lpD3DDevice->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, true);
	lpD3DDevice->SetMaterial(&mtrlNormal);
	lpD3DDevice->SetLight(0, &lightObjects);

	if(Things)
	{
		if(fCaptured && fMoving && fEngineEditObjs && ThingSelected)
		{
			D3DVECTOR r0, r1;
			EngineGetPick(&r0, &r1);

			int sx, sz;
			float ex, ez;

			if(EngineGetIntersectMapSquare(r0, r1, &sx, &sz, &ex, &ez))
			{
				float cx = ex, //(float)((int)fEnginePosX - VIEW_RANGE_CLICK + x - VIEW_RANGE_CLICK_MIN),
					cz = ez, //(float)((int)fEnginePosZ - VIEW_RANGE_CLICK + z - VIEW_RANGE_CLICK_MIN),
					cy = GroundHeight[sx][sz].height + GroundHeight[sx+1][sz].height + GroundHeight[sx][sz+1].height + GroundHeight[sx+1][sz+1].height;

				int bx = (int)cx,
					bz = (int)cz;

				cx += 0.5f;
				cz += 0.5f;
				cy /= 4.0f;

				while(bx < 0) bx += GROUND_X_SIZE;
				while(bz < 0) bz += GROUND_Z_SIZE;
				while(bx >= GROUND_X_SIZE) bx -= GROUND_X_SIZE;
				while(bz >= GROUND_Z_SIZE) bz -= GROUND_Z_SIZE;

				if((ThingSelected->Thing.Type == T_EFFECT) && (ThingSelected->Thing.Model == M_EFFECT_LAND_BRIDGE) && (ThingSelected->flags & TF_EDIT_LANDBRIDGE))
				{
					ThingSelected->LandBridge.x = (float)bx + 0.5f;
					ThingSelected->LandBridge.z = (float)bz + 0.5f;
					ThingSelected->Thing.Bluff[0] = (bx * 2) << 8;
					ThingSelected->Thing.Bluff[1] = (bz * 2) << 8;
					ThingSelected->LandBridge.ex = cx;
					ThingSelected->LandBridge.ez = cz;
					ThingSelected->LandBridge.ey = cy;
				}
				else
				{
					ThingSelected->x = (float)bx + 0.5f;
					ThingSelected->z = (float)bz + 0.5f;
					ThingSelected->Thing.PosX = (bx * 2) << 8;
					ThingSelected->Thing.PosZ = (bz * 2) << 8;
					ThingSelected->ex = cx;
					ThingSelected->ez = cz;
					ThingSelected->ey = cy;
				}
			}

			/*
			D3DVECTOR v[4];

			float dist, sdist, cx, cy, cz;
			bool f = false;

			int x = (int)fEnginePosX - VIEW_RANGE_CLICK,
				z = (int)fEnginePosZ - VIEW_RANGE_CLICK;

			for(int az = VIEW_RANGE_CLICK_MIN; az < VIEW_RANGE_CLICK_MAX; az++, z++, x = (int)fEnginePosX - VIEW_RANGE_CLICK)
			for(int ax = VIEW_RANGE_CLICK_MIN; ax < VIEW_RANGE_CLICK_MAX; ax++, x++)
			{
				v[0].x = (float)x;
				v[0].z = (float)z;
				v[0].y = GroundHeight[ax][az].height;

				v[1].x = (float)x;
				v[1].z = (float)(z + 1);
				v[1].y = GroundHeight[ax][az + 1].height;

				v[2].x = (float)(x + 1);
				v[2].z = (float)(z + 1);
				v[2].y = GroundHeight[ax + 1][az + 1].height;

				v[3].x = (float)(x + 1);
				v[3].z = (float)z;
				v[3].y = GroundHeight[ax + 1][az].height;

				if(EngineTriangleIntersect(r0, r1, v[0], v[1], v[2]) || EngineTriangleIntersect(r0, r1, v[2], v[3], v[0]))
				{
					dist = ((v[0].x + v[1].x + v[2].x + v[3].x) / 4.0f - r0.x) + ((v[0].z + v[1].z + v[2].z + v[3].z) / 4.0f - r0.z);
					if(dist < 0) dist *= -1;

					if(!f || (dist < sdist))
					{
						f = true;
						cx = (float)x;
						cz = (float)z;
						cy = v[0].y + v[1].y + v[2].y + v[3].y;

						sdist = dist;
					}
				}
			}

			if(f)
			{
				int bx = (int)cx,
					bz = (int)cz;
				cx += 0.5f;
				cz += 0.5f;
				cy /= 4.0f;

				while(bx < 0) bx += GROUND_X_SIZE;
				while(bz < 0) bz += GROUND_Z_SIZE;
				while(bx >= GROUND_X_SIZE) bx -= GROUND_X_SIZE;
				while(bz >= GROUND_Z_SIZE) bz -= GROUND_Z_SIZE;

				if((ThingSelected->Thing.Type == T_EFFECT) && (ThingSelected->Thing.Model == M_EFFECT_LAND_BRIDGE) && (ThingSelected->flags & TF_EDIT_LANDBRIDGE))
				{
					ThingSelected->LandBridge.x = (float)bx + 0.5f;
					ThingSelected->LandBridge.z = (float)bz + 0.5f;
					ThingSelected->Thing.Bluff[0] = (bx * 2) << 8;
					ThingSelected->Thing.Bluff[1] = (bz * 2) << 8;
					ThingSelected->LandBridge.ex = cx;
					ThingSelected->LandBridge.ez = cz;
					ThingSelected->LandBridge.ey = cy;
				}
				else
				{
					ThingSelected->x = (float)bx + 0.5f;
					ThingSelected->z = (float)bz + 0.5f;
					ThingSelected->Thing.PosX = (bx * 2) << 8;
					ThingSelected->Thing.PosZ = (bz * 2) << 8;
					ThingSelected->ex = cx;
					ThingSelected->ez = cz;
					ThingSelected->ey = cy;
				}
			}
			*/
			//*******************
		}

		THING *thing = Things;
		do
		{
			if((thing->flags & TF_DRAW_LANDBRIDGE) && (thing->Thing.Type == T_EFFECT) && (thing->Thing.Model == M_EFFECT_LAND_BRIDGE))
			{
				if(thing->flags & TF_DRAW)
				{
					//lpD3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);

					lpD3DDevice->SetTexture(0, 0);
					D3DVERTEX vl[2];
					memset(vl, 0, sizeof(vl));
					vl[0].ny = 1.0f;
					vl[1].ny = 1.0f;
					vl[0].x = thing->ex;
					vl[0].y = thing->ey + 1.0f;
					vl[0].z = thing->ez;
					vl[1].x = thing->LandBridge.ex;
					vl[1].y = thing->LandBridge.ey + 1.0f;
					vl[1].z = thing->LandBridge.ez;
					lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matIdentify);
					lpD3DDevice->DrawPrimitive(D3DPT_LINELIST, D3DFVF_VERTEX, vl, 2, 0);

					//lpD3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_TRUE);
				}

				if(fEngineEditObjs && (ThingSelected == thing) && (thing->flags & TF_EDIT_LANDBRIDGE))
				{
					lightObjects.dcvAmbient.r  =
					lightObjects.dcvSpecular.r =
					lightObjects.dcvDiffuse.r  = 1.0f;
					lightObjects.dcvAmbient.g  =
					lightObjects.dcvSpecular.g =
					lightObjects.dcvDiffuse.g  =
					lightObjects.dcvAmbient.b  =
					lightObjects.dcvSpecular.b =
					lightObjects.dcvDiffuse.b  = 0.0f;
					lpD3DDevice->SetLight(0, &lightObjects);
				}

				lpD3DDevice->SetTexture(0, txLandBridge->lpDDSTexture);
				matObjects._41 = thing->LandBridge.ex;
				matObjects._42 = thing->LandBridge.ey;
				matObjects._43 = thing->LandBridge.ez;
				lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matObjects);
				lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_VERTEX, vObj2Dsmall, 4, 0);

				if(fEngineEditObjs && (ThingSelected == thing) && (thing->flags & TF_EDIT_LANDBRIDGE))
				{
					lightObjects.dcvAmbient.r  =
					lightObjects.dcvAmbient.g  =
					lightObjects.dcvAmbient.b  =
					lightObjects.dcvSpecular.r =
					lightObjects.dcvSpecular.g =
					lightObjects.dcvSpecular.b =
					lightObjects.dcvDiffuse.r  =
					lightObjects.dcvDiffuse.g  =
					lightObjects.dcvDiffuse.b  = 1.0f;
					lpD3DDevice->SetLight(0, &lightObjects);
				}
			}

			if(thing->flags & TF_DRAW)
			{
				if(fEngineEditObjs && (ThingSelected == thing) && ((thing->Thing.Type != T_EFFECT) || (thing->Thing.Model != M_EFFECT_LAND_BRIDGE) || (!(thing->flags & TF_EDIT_LANDBRIDGE))))
				{
					lightObjects.dcvAmbient.r  =
					lightObjects.dcvSpecular.r =
					lightObjects.dcvDiffuse.r  = 1.0f;
					lightObjects.dcvAmbient.g  =
					lightObjects.dcvSpecular.g =
					lightObjects.dcvDiffuse.g  =
					lightObjects.dcvAmbient.b  =
					lightObjects.dcvSpecular.b =
					lightObjects.dcvDiffuse.b  = 0.0f;
					lpD3DDevice->SetLight(0, &lightObjects);
				}

				switch(thing->Thing.Type)
				{
				case T_PERSON:
					switch(thing->Thing.Model)
					{
					case M_PERSON_WILD:
						lpD3DDevice->SetTexture(0, txWild->lpDDSTexture);
						break;
					case M_PERSON_BRAVE:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:	lpD3DDevice->SetTexture(0, txBraveBlue->lpDDSTexture);   break;
						case OWNER_RED:		lpD3DDevice->SetTexture(0, txBraveRed->lpDDSTexture);    break;
						case OWNER_YELLOW:	lpD3DDevice->SetTexture(0, txBraveYellow->lpDDSTexture); break;
						case OWNER_GREEN:	lpD3DDevice->SetTexture(0, txBraveGreen->lpDDSTexture);  break;
						default: goto unknow;
						}
						break;
					case M_PERSON_WARRIOR:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:	lpD3DDevice->SetTexture(0, txWarriorBlue->lpDDSTexture);   break;
						case OWNER_RED:		lpD3DDevice->SetTexture(0, txWarriorRed->lpDDSTexture);    break;
						case OWNER_YELLOW:	lpD3DDevice->SetTexture(0, txWarriorYellow->lpDDSTexture); break;
						case OWNER_GREEN:	lpD3DDevice->SetTexture(0, txWarriorGreen->lpDDSTexture);  break;
						default: goto unknow;
						}
						break;
					case M_PERSON_PRIEST:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:	lpD3DDevice->SetTexture(0, txPriestBlue->lpDDSTexture);   break;
						case OWNER_RED:		lpD3DDevice->SetTexture(0, txPriestRed->lpDDSTexture);    break;
						case OWNER_YELLOW:	lpD3DDevice->SetTexture(0, txPriestYellow->lpDDSTexture); break;
						case OWNER_GREEN:	lpD3DDevice->SetTexture(0, txPriestGreen->lpDDSTexture);  break;
						default: goto unknow;
						}
						break;
					case M_PERSON_SPY:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:	lpD3DDevice->SetTexture(0, txSpyBlue->lpDDSTexture);   break;
						case OWNER_RED:		lpD3DDevice->SetTexture(0, txSpyRed->lpDDSTexture);    break;
						case OWNER_YELLOW:	lpD3DDevice->SetTexture(0, txSpyYellow->lpDDSTexture); break;
						case OWNER_GREEN:	lpD3DDevice->SetTexture(0, txSpyGreen->lpDDSTexture);  break;
						default: goto unknow;
						}
						break;
					case M_PERSON_FIREWARRIOR:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:	lpD3DDevice->SetTexture(0, txFwBlue->lpDDSTexture);   break;
						case OWNER_RED:		lpD3DDevice->SetTexture(0, txFwRed->lpDDSTexture);    break;
						case OWNER_YELLOW:	lpD3DDevice->SetTexture(0, txFwYellow->lpDDSTexture); break;
						case OWNER_GREEN:	lpD3DDevice->SetTexture(0, txFwGreen->lpDDSTexture);  break;
						default: goto unknow;
						}
						break;
					case M_PERSON_SHAMAN:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:	lpD3DDevice->SetTexture(0, txShamanBlue->lpDDSTexture);   break;
						case OWNER_RED:		lpD3DDevice->SetTexture(0, txShamanRed->lpDDSTexture);    break;
						case OWNER_YELLOW:	lpD3DDevice->SetTexture(0, txShamanYellow->lpDDSTexture); break;
						case OWNER_GREEN:	lpD3DDevice->SetTexture(0, txShamanGreen->lpDDSTexture);  break;
						default: goto unknow;
						}
						break;
					case M_PERSON_ANGELOFDEATH:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:   EngineDrawObj3D(objAodBlue, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle);   goto draw_3d;
						case OWNER_RED:    EngineDrawObj3D(objAodRed, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle);    goto draw_3d;
						case OWNER_YELLOW: EngineDrawObj3D(objAodYellow, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle); goto draw_3d;
						case OWNER_GREEN:  EngineDrawObj3D(objAodGreen, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle);  goto draw_3d;
						default: goto unknow;
						}
						break;
					default: goto unknow;
					}
					goto draw_2d;
				case T_BUILDING:
					switch(thing->Thing.Model)
					{
					case M_BUILDING_HUT1:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:   EngineDrawObj3D(objHut1Blue, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);   break;
						case OWNER_RED:	   EngineDrawObj3D(objHut1Red, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);    break;
						case OWNER_YELLOW: EngineDrawObj3D(objHut1Yellow, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle); break;
						case OWNER_GREEN:  EngineDrawObj3D(objHut1Green, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);  break;
						default: goto unknow;
						}
						break;
					case M_BUILDING_HUT2:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:   EngineDrawObj3D(objHut2Blue, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);   break;
						case OWNER_RED:	   EngineDrawObj3D(objHut2Red, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);    break;
						case OWNER_YELLOW: EngineDrawObj3D(objHut2Yellow, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle); break;
						case OWNER_GREEN:  EngineDrawObj3D(objHut2Green, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);  break;
						default: goto unknow;
						}
						break;
					case M_BUILDING_HUT3:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:   EngineDrawObj3D(objHut3Blue, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);   break;
						case OWNER_RED:	   EngineDrawObj3D(objHut3Red, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);    break;
						case OWNER_YELLOW: EngineDrawObj3D(objHut3Yellow, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle); break;
						case OWNER_GREEN:  EngineDrawObj3D(objHut3Green, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);  break;
						default: goto unknow;
						}
						break;
					case M_BUILDING_TOWER:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:   EngineDrawObj3D(objTowerBlue, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);   break;
						case OWNER_RED:    EngineDrawObj3D(objTowerRed, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);    break;
						case OWNER_YELLOW: EngineDrawObj3D(objTowerYellow, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle); break;
						case OWNER_GREEN:  EngineDrawObj3D(objTowerGreen, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);  break;
						default: goto unknow;
						}
						break;
					case M_BUILDING_SPY_TRAIN:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:   EngineDrawObj3D(objSpyTrainBlue, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);   break;
						case OWNER_RED:    EngineDrawObj3D(objSpyTrainRed, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);    break;
						case OWNER_YELLOW: EngineDrawObj3D(objSpyTrainYellow, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle); break;
						case OWNER_GREEN:  EngineDrawObj3D(objSpyTrainGreen, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);  break;
						default: goto unknow;
						}
						break;
					case M_BUILDING_WARRIOR_TRAIN:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:   EngineDrawObj3D(objWarriorTrainBlue, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);   break;
						case OWNER_RED:    EngineDrawObj3D(objWarriorTrainRed, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);    break;
						case OWNER_YELLOW: EngineDrawObj3D(objWarriorTrainYellow, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle); break;
						case OWNER_GREEN:  EngineDrawObj3D(objWarriorTrainGreen, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);  break;
						default: goto unknow;
						}
						break;
					case M_BUILDING_TEMPLE:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:   EngineDrawObj3D(objTempleBlue, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);   break;
						case OWNER_RED:    EngineDrawObj3D(objTempleRed, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);    break;
						case OWNER_YELLOW: EngineDrawObj3D(objTempleYellow, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle); break;
						case OWNER_GREEN:  EngineDrawObj3D(objTempleGreen, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);  break;
						default: goto unknow;
						}
						break;
					case M_BUILDING_FIREWARRIOR_TRAIN:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:   EngineDrawObj3D(objFwTrainBlue, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);   break;
						case OWNER_RED:    EngineDrawObj3D(objFwTrainRed, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);    break;
						case OWNER_YELLOW: EngineDrawObj3D(objFwTrainYellow, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle); break;
						case OWNER_GREEN:  EngineDrawObj3D(objFwTrainGreen, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);  break;
						default: goto unknow;
						}
						break;
					case M_BUILDING_BOAT_HUT_1:
					case M_BUILDING_BOAT_HUT_2:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:   EngineDrawObj3D(objBoatHutBlue, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);   break;
						case OWNER_RED:    EngineDrawObj3D(objBoatHutRed, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);    break;
						case OWNER_YELLOW: EngineDrawObj3D(objBoatHutYellow, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle); break;
						case OWNER_GREEN:  EngineDrawObj3D(objBoatHutGreen, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);  break;
						default: goto unknow;
						}
						break;
					case M_BUILDING_AIRSHIP_HUT_1:
					case M_BUILDING_AIRSHIP_HUT_2:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:   EngineDrawObj3D(objBallonHutBlue, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);   break;
						case OWNER_RED:    EngineDrawObj3D(objBallonHutRed, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);    break;
						case OWNER_YELLOW: EngineDrawObj3D(objBallonHutYellow, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle); break;
						case OWNER_GREEN:  EngineDrawObj3D(objBallonHutGreen, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);  break;
						default: goto unknow;
						}
						break;
					case M_BUILDING_KNOWLEDGE:
						EngineDrawObj3D(objKnowledge, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);
						break;
					case M_BUILDING_PRISON:
						EngineDrawObj3D(objPrison, thing->ex, thing->ey, thing->ez, thing->Thing.Building.Angle);
						break;
					default: goto unknow;
					}
					goto draw_3d;
				case T_SCENERY:
					if(thing->Thing.Model == M_SCENERY_TOP_LEVEL_SCENERY || thing->Thing.Model == M_SCENERY_SUB_LEVEL_SCENERY)
					{
						switch(thing->Thing.Scenery.IslandNum)
						{
						case M_SPECIAL_A: EngineDrawObj3D(objSpecialA, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle); break;
						case M_SPECIAL_B: EngineDrawObj3D(objSpecialB, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle); break;
						case M_SPECIAL_C: EngineDrawObj3D(objSpecialC, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle); break;
						case M_SPECIAL_D: EngineDrawObj3D(objSpecialD, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle); break;
						case M_SPECIAL_E: EngineDrawObj3D(objSpecialE, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle); break;
						case M_SPECIAL_F: EngineDrawObj3D(objSpecialF, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle); break;
						default: goto unknow;
						}
						goto draw_3d;
					}
					switch(thing->Thing.Model)
					{
					case M_SCENERY_PLANT_1:
						lpD3DDevice->SetTexture(0, txPlant1->lpDDSTexture);
						goto draw_2d_small;
					case M_SCENERY_PLANT_2:
						lpD3DDevice->SetTexture(0, txPlant2->lpDDSTexture);
						goto draw_2d_small;
					case M_SCENERY_TREE_1:
					case M_SCENERY_TREE_4:
						EngineDrawObj3D(objTree1, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle);
						break;
					case M_SCENERY_TREE_2:
					case M_SCENERY_TREE_5:
						EngineDrawObj3D(objTree2, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle);
						break;
					case M_SCENERY_TREE_3:
					case M_SCENERY_TREE_6:
						EngineDrawObj3D(objTree3, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle);
						break;
					case M_SCENERY_STONE_HEAD:
						EngineDrawObj3D(objStoneHead, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle);
						break;
					case M_SCENERY_WOOD_PILE:
						lpD3DDevice->SetTexture(0, txWoodPile->lpDDSTexture);
						goto draw_2d_small;
					case M_SCENERY_RS_PILLAR:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:   EngineDrawObj3D(objRSPillarBlue, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle);   break;
						case OWNER_RED:    EngineDrawObj3D(objRSPillarRed, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle);    break;
						case OWNER_YELLOW: EngineDrawObj3D(objRSPillarYellow, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle); break;
						case OWNER_GREEN:  EngineDrawObj3D(objRSPillarGreen, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle);  break;
						default: EngineDrawObj3D(objRSPillar, thing->ex, thing->ey, thing->ez, thing->Thing.Scenery.Angle);
						}
						break;
					case M_SCENERY_ROCK:
						lpD3DDevice->SetTexture(0, txRock->lpDDSTexture);
						goto draw_2d_small;
					case M_SCENERY_FIRE:
						lpD3DDevice->SetTexture(0, txFire->lpDDSTexture);
						goto draw_2d_small;
					default: goto unknow;
					}
					goto draw_3d;
				case T_VEHICLE:
					switch(thing->Thing.Model)
					{
					case M_VEHICLE_BOAT_1:
					case M_VEHICLE_BOAT_2:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:   EngineDrawObj3D(objBoatBlue, thing->ex, thing->ey, thing->ez, 0);   break;
						case OWNER_RED:    EngineDrawObj3D(objBoatRed, thing->ex, thing->ey, thing->ez, 0);    break;
						case OWNER_YELLOW: EngineDrawObj3D(objBoatYellow, thing->ex, thing->ey, thing->ez, 0); break;
						case OWNER_GREEN:  EngineDrawObj3D(objBoatGreen, thing->ex, thing->ey, thing->ez, 0);  break;
						default: EngineDrawObj3D(objBoat, thing->ex, thing->ey, thing->ez, 0); break;
						}
						break;
					case M_VEHICLE_AIRSHIP_1:
					case M_VEHICLE_AIRSHIP_2:
						switch(thing->Thing.Owner)
						{
						case OWNER_BLUE:   EngineDrawObj3D(objBallonBlue, thing->ex, thing->ey, thing->ez, 0);   break;
						case OWNER_RED:    EngineDrawObj3D(objBallonRed, thing->ex, thing->ey, thing->ez, 0);    break;
						case OWNER_YELLOW: EngineDrawObj3D(objBallonYellow, thing->ex, thing->ey, thing->ez, 0); break;
						case OWNER_GREEN:  EngineDrawObj3D(objBallonGreen, thing->ex, thing->ey, thing->ez, 0);  break;
						default: EngineDrawObj3D(objBallon, thing->ex, thing->ey, thing->ez, 0); break;
						}
						break;
					default: goto unknow;
					}
					goto draw_3d;
				case T_GENERAL:
					switch(thing->Thing.Model)
					{
					case M_GENERAL_TRIGGER:         lpD3DDevice->SetTexture(0, txTrigger->lpDDSTexture);   break;
					case M_GENERAL_DISCOVERY:       lpD3DDevice->SetTexture(0, txDiscovery->lpDDSTexture); break;
					case M_GENERAL_BUILDING_ADD_ON: lpD3DDevice->SetTexture(0, txAddon->lpDDSTexture);     break;
					case M_GENERAL_LIGHT:           lpD3DDevice->SetTexture(0, txLight->lpDDSTexture);     break;
					default: goto unknow;
					}
					goto draw_2d_small;
				case T_SPELL:
					lpD3DDevice->SetTexture(0, txSpell->lpDDSTexture);
					goto draw_2d_small;
				case T_EFFECT:
					lpD3DDevice->SetTexture(0, txEffect->lpDDSTexture);
					goto draw_2d_small;
                case T_SHOT:
                    lpD3DDevice->SetTexture(0, txEffect->lpDDSTexture);
                    goto draw_2d_small;
				case T_CREATURE:
					if(thing->Thing.Model == M_CREATURE_EAGLE)
					{
						EngineDrawObj3D(objEagle, thing->ex, thing->ey, thing->ez, 0);
						goto draw_3d;
					}
					else goto unknow;
				default: goto unknow;
				}
unknow:
				lpD3DDevice->SetTexture(0, txUnknow->lpDDSTexture);
draw_2d_small:
				matObjects._41 = thing->ex;
				matObjects._42 = thing->ey;
				matObjects._43 = thing->ez;
				lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matObjects);
				lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_VERTEX, vObj2Dsmall, 4, 0);

#if DRAW_SPRITE_BOUNDBOXES
				if(dwEngineFlags & EF_BOUNDINGBOX)
				{
					WORD idxs[] = { 0, 1, 3, 2, 0 };
					lpD3DDevice->SetTexture(0, 0);
					lpD3DDevice->DrawIndexedPrimitive(D3DPT_LINESTRIP, D3DFVF_VERTEX, vObj2Dsmall, 4, idxs, 5, 0);
				}
#endif
				goto skip;
draw_2d:
				matObjects._41 = thing->ex;
				matObjects._42 = thing->ey;
				matObjects._43 = thing->ez;
				lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matObjects);
				lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_VERTEX, vObj2D, 4, 0);

#if DRAW_SPRITE_BOUNDBOXES
				if(dwEngineFlags & EF_BOUNDINGBOX)
				{
					WORD idxs[] = { 0, 1, 3, 2, 0 };
					lpD3DDevice->SetTexture(0, 0);
					lpD3DDevice->DrawIndexedPrimitive(D3DPT_LINESTRIP, D3DFVF_VERTEX, vObj2Dsmall, 4, idxs, 5, 0);
				}
#endif
				goto skip;
draw_3d:
				lpD3DDevice->SetMaterial(&mtrlNormal);
skip:;
				if(fEngineEditObjs && (ThingSelected == thing))
				{
					lightObjects.dcvAmbient.r  =
					lightObjects.dcvAmbient.g  =
					lightObjects.dcvAmbient.b  =
					lightObjects.dcvSpecular.r =
					lightObjects.dcvSpecular.g =
					lightObjects.dcvSpecular.b =
					lightObjects.dcvDiffuse.r  =
					lightObjects.dcvDiffuse.g  =
					lightObjects.dcvDiffuse.b  = 1.0f;
					lpD3DDevice->SetLight(0, &lightObjects);
				}

#if DRAW_OBJECTS_LINES
				COLORVERTEX a[2];
				memset(&a, 0, sizeof(a));

				a[0].ny =
				a[1].ny = 1.0f;

				a[0].color =
				a[1].color = COLOR_GRID;

				a[0].x = thing->ex;
				a[0].y = thing->ey;
				a[0].z = thing->ez;

				a[1].x = fEnginePosX;
				a[1].y = LIGHT_Y;
				a[1].z = fEnginePosZ;

				lpD3DDevice->SetTexture(0, 0);
				lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matIdentify);
				lpD3DDevice->DrawPrimitive(D3DPT_LINESTRIP, FVF_COLORVERTEX, a, 2, 0);
#endif
			}
			thing = thing->Next;
		}
		while(thing != Things);
	}

	if(fEngineEditMarkers || (dwEngineFlags & EF_SHOW_MARKERS))
	{
		if(fCaptured && fMoving && fEngineEditMarkers && (MarkerSelected != -1))
		{
			D3DVECTOR r0, r1;
			EngineGetPick(&r0, &r1);

			int sx, sz;
			float ex, ez;

			if(EngineGetIntersectMapSquare(r0, r1, &sx, &sz, &ex, &ez))
			{
				float cx = ex,
					  cz = ez,
					  cy = GroundHeight[sx][sz].height + GroundHeight[sx+1][sz].height + GroundHeight[sx][sz+1].height + GroundHeight[sx+1][sz+1].height;

				int bx = (int)cx,
					bz = (int)cz;

				cx += 0.5f;
				cz += 0.5f;
				cy /= 4.0f;

				while(bx < 0) bx += GROUND_X_SIZE;
				while(bz < 0) bz += GROUND_Z_SIZE;
				while(bx >= GROUND_X_SIZE) bx -= GROUND_X_SIZE;
				while(bz >= GROUND_Z_SIZE) bz -= GROUND_Z_SIZE;

				Markers[MarkerSelected].x = (float)bx + 0.5f;
				Markers[MarkerSelected].z = (float)bz + 0.5f;
				leveldat->Header.v2.Markers[MarkerSelected] = ((bz * 2) << 8) | (bx * 2);
				Markers[MarkerSelected].ex = cx;
				Markers[MarkerSelected].ez = cz;
				Markers[MarkerSelected].ey = cy;
			}

			DlgMarkersUpdate(hDlgMarkers);
		}

		lpD3DDevice->SetTexture(0, txMarker->lpDDSTexture);

		for(int a = 255; a > -1; a--)
		{
			if(MarkerSelected == a)
			{
				lightObjects.dcvAmbient.r  =
				lightObjects.dcvSpecular.r =
				lightObjects.dcvDiffuse.r  = 1.0f;
				lightObjects.dcvAmbient.g  =
				lightObjects.dcvSpecular.g =
				lightObjects.dcvDiffuse.g  =
				lightObjects.dcvAmbient.b  =
				lightObjects.dcvSpecular.b =
				lightObjects.dcvDiffuse.b  = 0.0f;
				lpD3DDevice->SetLight(0, &lightObjects);
			}

			if(Markers[a].flags & TF_DRAW)
			{
				matObjects._41 = Markers[a].ex;
				matObjects._42 = Markers[a].ey;
				matObjects._43 = Markers[a].ez;
				lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matObjects);
				lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_VERTEX, vObj2Dsmall, 4, 0);
			}

			if(MarkerSelected == a)
			{
				lightObjects.dcvAmbient.r  =
				lightObjects.dcvAmbient.g  =
				lightObjects.dcvAmbient.b  =
				lightObjects.dcvSpecular.r =
				lightObjects.dcvSpecular.g =
				lightObjects.dcvSpecular.b =
				lightObjects.dcvDiffuse.r  =
				lightObjects.dcvDiffuse.g  =
				lightObjects.dcvDiffuse.b  = 1.0f;
				lpD3DDevice->SetLight(0, &lightObjects);
			}
		}
	}

	lpD3DDevice->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, false);

	return S_OK;
}


long EngineDrawMiniMap()
{
	lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &matMiniMap);
	lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, &matIdentify);

	lpD3DDevice->SetLight(0, &lightMiniMap);
	lpD3DDevice->SetMaterial(&mtrlMiniMap);

	lpD3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);

	lpD3DDevice->SetTexture(0, txMiniMap->lpDDSTexture);

	if(dwEngineFlags & EF_TRANSPARENT_MINIMAP)
	{
		lpD3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, true);
		lpD3DDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_INVSRCALPHA);
		lpD3DDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCALPHA);
		lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	}

	lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_VERTEX, vMiniMap, 4, 0);

	if(dwEngineFlags & EF_TRANSPARENT_MINIMAP)
	{
		lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		lpD3DDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, false);
	}

	lpD3DDevice->SetTexture(0, 0);

	// minimap dir

	lpD3DDevice->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	lpD3DDevice->DrawPrimitive(D3DPT_LINELIST, FVF_COLORVERTEX, vMiniMapDir, 4, 0);
	lpD3DDevice->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

	//

#if DRAW_MINIMAP_LINES
	lpD3DDevice->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

	COLORVERTEX v[4];
	memset(&v, 0, sizeof(v));

	v[0].color =
	v[1].color =
	v[2].color =
	v[3].color = 0xFF0000;

	v[0].nz =
	v[1].nz =
	v[2].nz =
	v[3].nz = -1.0f;

	v[0].x = vMiniMap[0].x;
	v[0].y = vMiniMap[0].y;
	v[0].z = vMiniMap[0].z;

	v[1].x = vMiniMap[3].x;
	v[1].y = vMiniMap[3].y;
	v[1].z = vMiniMap[3].z;

	v[2].x = vMiniMap[1].x;
	v[2].y = vMiniMap[1].y;
	v[2].z = vMiniMap[1].z;

	v[3].x = vMiniMap[2].x;
	v[3].y = vMiniMap[2].y;
	v[3].z = vMiniMap[2].z;

	lpD3DDevice->DrawPrimitive(D3DPT_LINELIST, FVF_COLORVERTEX, &v, 4, 0);

	lpD3DDevice->SetRenderState(D3DRENDERSTATE_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
#endif

	lpD3DDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_TRUE);

	return S_OK;
}


long EngineLoadLevelV3(char *filename)
{
	EngineNewMap();

	// -=-=- .dat -=-=-

	HANDLE h = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(h == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	memset(leveldat, 0, sizeof(LEVELDATv3));

	Objects.clear();

	dwRW = 0;
	ReadFile(h, leveldat, sizeof(LEVELDATv3), &dwRW, 0);
	if(dwRW != sizeof(LEVELDATv3))
	{
		CloseHandle(h);
		sprintf(str, SZ_ERR_READERROR, filename);
		LogWrite(str);
		EngineNewMap();
		return -1;
	}

	const size_t MAGIC_SIZE = 5;
	const char MAGICDEF[MAGIC_SIZE] = { 'L','E','V','L','3' };
	for (int i = 0; i < MAGIC_SIZE; i++) {
		if (leveldat->MAGIC[i] != MAGICDEF[i]) {
			CloseHandle(h);
			sprintf(str, SZ_ERR_MAGIC_MISMATCH, filename);
			LogWrite(str);

			// MAGIC IS WRONG! Lets try V2 next
			return EngineLoadLevel(filename);
		}
	}

	for (int i = 0; i < leveldat->Header.MaxNumObjects; i++) {
		THINGSAVE t;
		dwRW = 0;
		ReadFile(h, &t, sizeof(THINGSAVE), &dwRW, 0);
		if (dwRW != sizeof(THINGSAVE))
		{
			CloseHandle(h);
			sprintf(str, SZ_ERR_READERROR, filename);
			LogWrite(str);
			EngineNewMap();
			return -1;
		}
		Objects.push_back(t);
	}

	memcpy(wEngineGround, leveldat->GroundHeight, sizeof(wEngineGround));

	DWORD dw = 0;
	THING *thing;
	UWORD idx = 1;

	for(int a = 0; a < Objects.size(); a++)
	{
		if(Objects[a].Model != 0)
		{
			thing = new THING;
			memset(thing, 0, sizeof(THING));
			memcpy(&thing->Thing, &Objects[a], sizeof(THINGSAVE));
			thing->x = (float)((thing->Thing.PosX >> 8) / 2) + 0.5f;
			thing->z = (float)((thing->Thing.PosZ >> 8) / 2) + 0.5f;
			thing->Idx = idx;

			if(thing->Thing.Type == T_EFFECT && thing->Thing.Model == M_EFFECT_LAND_BRIDGE)
			{
				thing->LandBridge.x = (float)(((thing->Thing.Bluff[0] & 0xFFFF) >> 8) / 2) + 0.5f;
				thing->LandBridge.z = (float)(((thing->Thing.Bluff[1] & 0xFFFF) >> 8) / 2) + 0.5f;
			}
			else if(thing->Thing.Type == T_PERSON && thing->Thing.Model == M_PERSON_SHAMAN && thing->Thing.Owner == OWNER_BLUE)
			{
				fEnginePosX = thing->x;
				fEnginePosZ = thing->z;
			}

			LINK(Things, thing);
			ObjectsCount++;
		}
		idx++;
	}

	DlgObjectIdxToLink();
	CloseHandle(h);

	for(int a = 0; a < 256; a++)
	{
		Markers[a].x = (float)((leveldat->Header.v2.Markers[a] & 0xFF) / 2) + 0.5f;
		Markers[a].z = (float)((leveldat->Header.v2.Markers[a] >> 8) / 2) + 0.5f;
	}
	// -=-=-

	EngineSetTreeType();

	DlgObjectUpdateInfo(hDlgObject);
	UpdateHeaderDialogs();
	DlgMarkersUpdate(hDlgMarkers);
	
	EngineUpdateView();
	EngineUpdateMiniMap();

	strcpy(szLevel, filename);
	DlgInfoUpdate(hDlgInfo);

	LevelFormatMode = LEVEL_FORMAT_MODE_V3;
	return S_OK;
}


long EngineLoadLevel(char *filename)
{
	EngineNewMap();

	// -=-=- .dat -=-=-

	HANDLE h = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(h == INVALID_HANDLE_VALUE)
	{
		//
		//sprintf(str, SZ_ERR_CREATEFILE, filename);
		//LogWrite(str);
		//
		return -1;
	}

	LEVELDATv2 *level2dat;
	level2dat = (LEVELDATv2*)malloc(sizeof(LEVELDATv2));
	memset(level2dat, 0, sizeof(LEVELDATv2));

	dwRW = 0;
	ReadFile(h, level2dat, sizeof(LEVELDATv2), &dwRW, 0);
	if(dwRW != sizeof(LEVELDATv2))
	{
		free(level2dat);
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, filename);
		LogWrite(str);

		EngineNewMap();
		return -1;
	}

	memcpy(wEngineGround, level2dat->GroundHeight, sizeof(wEngineGround));

	DWORD dw = 0;
	THING *thing;
	UWORD idx = 1;

	for(int a = 0; a < MAX_V2_THINGS; a++)
	{
		if(level2dat->Things[a].Model != 0)
		{
			thing = new THING;
			memset(thing, 0, sizeof(THING));
			memcpy(&thing->Thing, &level2dat->Things[a], sizeof(THINGSAVE));
			thing->x = (float)((thing->Thing.PosX >> 8) / 2) + 0.5f;
			thing->z = (float)((thing->Thing.PosZ >> 8) / 2) + 0.5f;
			thing->Idx = idx;

			if(thing->Thing.Type == T_EFFECT && thing->Thing.Model == M_EFFECT_LAND_BRIDGE)
			{
				thing->LandBridge.x = (float)(((thing->Thing.Bluff[0] & 0xFFFF) >> 8) / 2) + 0.5f;
				thing->LandBridge.z = (float)(((thing->Thing.Bluff[1] & 0xFFFF) >> 8) / 2) + 0.5f;
			}
			else if(thing->Thing.Type == T_PERSON && thing->Thing.Model == M_PERSON_SHAMAN && thing->Thing.Owner == OWNER_BLUE)
			{
				fEnginePosX = thing->x;
				fEnginePosZ = thing->z;
			}

			LINK(Things, thing);
			ObjectsCount++;
		}
		idx++;
	}

	DlgObjectIdxToLink();

	free(level2dat);

	//

	CloseHandle(h);

	// -=-=- .hdr -=-=-

	char *fname;

	fname = (char*)malloc(lstrlen(filename) + 1);
	strcpy(fname, filename);

	char *ext = PathFindExtension(fname);
	if(lstrcmpi(LEVEL_PREFIX, ext) != 0) goto _skip;
	strcpy(ext, HEADER_PREFIX);

	h = CreateFile(fname, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(h == INVALID_HANDLE_VALUE)
	{
		sprintf(str, SZ_ERR_CREATEFILE, fname);
		LogWrite(str);
		goto _skip;
	}

	dwRW = 0;
	ReadFile(h, &leveldat->Header.v2, sizeof(leveldat->Header.v2), &dwRW, 0);
	if (dwRW == 0)
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, fname);
		LogWrite(str);
		free(fname);

		EngineNewMap();
		return -1;
	}

	CloseHandle(h);

    // Populate the computer player indices from the old computer player indices.
    leveldat->Header.ComputerPlayerIndex[0] = leveldat->Header.v2.BlueComputerPlayerIndex;
    for (int i = 1; i < 4; ++i)
    {
        leveldat->Header.ComputerPlayerIndex[i] = leveldat->Header.v2.OldComputerPlayerIndex[i - 1];
    }

	for(int a = 0; a < 256; a++)
	{
		Markers[a].x = (float)((leveldat->Header.v2.Markers[a] & 0xFF) / 2) + 0.5f;
		Markers[a].z = (float)((leveldat->Header.v2.Markers[a] >> 8) / 2) + 0.5f;
	}

_skip:
	free(fname);

	// -=-=-

	EngineSetTreeType();

	DlgObjectUpdateInfo(hDlgObject);
	UpdateHeaderDialogs();
	DlgMarkersUpdate(hDlgMarkers);
	
	EngineUpdateView();
	EngineUpdateMiniMap();

	strcpy(szLevel, filename);
	DlgInfoUpdate(hDlgInfo);

	LevelFormatMode = LEVEL_FORMAT_MODE_V2;
	return S_OK;
}


long EngineSaveLevelV3(char *filename)
{
	// -=-=- .dat -=-=-

	HANDLE h = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(h == INVALID_HANDLE_VALUE)
	{
		sprintf(str, SZ_ERR_CREATEFILE, filename);
		LogWrite(str);
		return -1;
	}

	// idxs
	DlgObjectLinkToIdx();

	memcpy(leveldat->GroundHeight, wEngineGround, sizeof(wEngineGround));
	Objects.clear();
	THINGSAVE ts;

	THING *t = Things;
	if(t) do
	{
		memcpy(&ts, &t->Thing, sizeof(THINGSAVE));
		Objects.push_back(ts);
		t = t->Next;
	}
	while(t != Things);

	// Level 2 Spec
	leveldat->Header.MaxNumObjects = Objects.size();
	leveldat->Header.MaxNumPlayers = 4;// Players.
	leveldat->Header.MaxAltPoints  = 128 * 128;
	leveldat->Header.Version	   = 1;

	dwRW = 0;
	WriteFile(h, leveldat, sizeof(LEVELDATv3), &dwRW, 0);

	if(dwRW != sizeof(LEVELDATv3))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_WRITEERROR, filename);
		LogWrite(str);

		return -1;
	}

	for (int i = 0; i < Objects.size(); i++) {
		dwRW = 0;
		WriteFile(h, &Objects[i], sizeof(THINGSAVE), &dwRW, 0);
		if (dwRW != sizeof(THINGSAVE))
		{
			CloseHandle(h);

			sprintf(str, SZ_ERR_WRITEERROR, filename);
			LogWrite(str);

			return -1;
		}
	}

	CloseHandle(h);

    // Populate the computer player indices from the old computer player indices.
    leveldat->Header.ComputerPlayerIndex[0] = leveldat->Header.v2.BlueComputerPlayerIndex;
    for (int i = 1; i < 4; ++i)
    {
        leveldat->Header.ComputerPlayerIndex[i] = leveldat->Header.v2.OldComputerPlayerIndex[i - 1];
    }

    for (int a = 0; a < 256; a++)
    {
        Markers[a].x = (float)((leveldat->Header.v2.Markers[a] & 0xFF) / 2) + 0.5f;
        Markers[a].z = (float)((leveldat->Header.v2.Markers[a] >> 8) / 2) + 0.5f;
    }

	strcpy(szLevel, filename);
	DlgInfoUpdate(hDlgInfo);

	LevelFormatMode = LEVEL_FORMAT_MODE_V3;
	return S_OK;
}


long EngineSaveLevel(char *filename)
{
	//level2 compatibility check
	if (ObjectsCount > MAX_V2_THINGS) {
		if (ModalMsg(SZ_CONFIRM_MAX_L2_OBJECTS, APPNAME, MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2) == IDYES) {
			return EngineSaveLevelV3(filename);
		}
	}

	// -=-=- .dat -=-=-

	HANDLE h = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(h == INVALID_HANDLE_VALUE)
	{
		sprintf(str, SZ_ERR_CREATEFILE, filename);
		LogWrite(str);
		return -1;
	}

	// idxs

	DlgObjectLinkToIdx();

	//

	LEVELDATv2 *level2dat;
	level2dat = (LEVELDATv2*)malloc(sizeof(LEVELDATv2));
	memset(level2dat, 0, sizeof(LEVELDATv2));

	memcpy(level2dat->GroundHeight, wEngineGround, sizeof(wEngineGround));

	THINGSAVE *ts = level2dat->Things;

	THING *t = Things;
	int savedThings = 0;
	if(t) do
	{
		memcpy(ts++, &t->Thing, sizeof(THINGSAVE));
		savedThings++;
		t = t->Next;

		//if we reached the max things supported by v2, bail out
		if (savedThings == MAX_V2_THINGS) {
			break;
		}
	}
	while(t != Things);

	dwRW = 0;
	WriteFile(h, level2dat, sizeof(LEVELDATv2), &dwRW, 0);

	free(level2dat);

	if(dwRW != sizeof(LEVELDATv2))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_WRITEERROR, filename);
		LogWrite(str);

		return -1;
	}

	//

	CloseHandle(h);

	//

	char *fname;
	fname = (char*)malloc(lstrlen(filename) + 1);
	strcpy(fname, filename);

	char *ext = PathFindExtension(fname);
	if(lstrcmpi(LEVEL_PREFIX, ext) != 0) goto _skip;

	// -=-=- .hdr -=-=-

	strcpy(ext, HEADER_PREFIX);

	h = CreateFile(fname, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(h == INVALID_HANDLE_VALUE)
	{
		sprintf(str, SZ_ERR_CREATEFILE, fname);
		LogWrite(str);
		free(fname);

		return -1;
	}

	dwRW = 0;
	WriteFile(h, &leveldat->Header.v2, sizeof(leveldat->Header.v2), &dwRW, 0);
	if(dwRW != sizeof(leveldat->Header.v2))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_WRITEERROR, fname);
		LogWrite(str);
		free(fname);

		return -1;
	}

	CloseHandle(h);

	// -=-=- .ver -=-=-

	strcpy(ext, VERSION_PREFIX);

	h = CreateFile(fname, GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(h == INVALID_HANDLE_VALUE)
	{
		sprintf(str, SZ_ERR_CREATEFILE, fname);
		LogWrite(str);
		free(fname);

		return -1;
	}

	dwRW = 0;
	WriteFile(h, &LevelVersion, sizeof(LevelVersion), &dwRW, 0);
	if(dwRW != sizeof(LevelVersion))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_WRITEERROR, fname);
		LogWrite(str);
		free(fname);

		return -1;
	}

	CloseHandle(h);

_skip:
	free(fname);

	//

	strcpy(szLevel, filename);
	DlgInfoUpdate(hDlgInfo);

	LevelFormatMode = LEVEL_FORMAT_MODE_V2;
	return S_OK;
}


long EngineUpdateView()
{
	if(!fEngineActive) return -1;

	while(fEnginePosX >= (float)GROUND_X_SIZE) fEnginePosX -= GROUND_X_SIZE;
	while(fEnginePosX < 0.0f) fEnginePosX += GROUND_X_SIZE;
	while(fEnginePosZ >= (float)GROUND_Z_SIZE) fEnginePosZ -= GROUND_Z_SIZE;
	while(fEnginePosZ < 0.0f) fEnginePosZ += GROUND_Z_SIZE;

	while(fEngineRotX >= 360.0f) fEngineRotX -= 360.0f;
	while(fEngineRotX < 0.0f) fEngineRotX += 360.0f;
	while(fEngineRotY >= 360.0f) fEngineRotY -= 360.0f;
	while(fEngineRotY < 0.0f) fEngineRotY += 360.0f;
	while(fEngineRotZ >= 360.0f) fEngineRotZ -= 360.0f;
	while(fEngineRotZ < 0.0f) fEngineRotZ += 360.0f;

	lightLandscape.dvPosition = D3DVECTOR(fEnginePosX, LIGHT_Y, fEnginePosZ);

	// ground height

	int x = (int)fEnginePosX - VIEW_RANGE,
		z = (int)fEnginePosZ - VIEW_RANGE;

	for(int az = 0; az < VIEW_RANGE_2; az++, z++, x = (int)fEnginePosX - VIEW_RANGE)
	for(int ax = 0; ax < VIEW_RANGE_2; ax++, x++)
	{
		WORD w = EngineGetGroundHeight(x, z);
		GroundHeight[ax][az].height = (float)w * GROUND_BLOCK_HEIGHT;

		if(w > 8)
			w /= 8;
		else if(w > 0)
			w = 1;

        if(w > 0xFF) w = 0xFF;

		GroundHeight[ax][az].color = LANDSCAPE_COLORS[w];

#if SPHERICAL_WORLD
		if (fEnginePosY <= MAX_SPERICAL_POS_Y)
		{

			// optimizations screw up at powf
			//float f = sqrtf(powf((float)x - fEnginePosX, 2.0f) + powf((float)z - fEnginePosZ, 2.0f));
			//GroundHeight[ax][az].height -= f * f * SPHERE_RATIO;

			float f, f1, f2;
			f1 = (float)x - fEnginePosX;
			f1 = f1 * f1;
			f2 = (float)z - fEnginePosZ;
			f2 = f2 * f2;
			f = sqrtf(f1 + f2);
			GroundHeight[ax][az].height -= f * f * SPHERE_RATIO;
		}
#endif
	}

	// near water check
	/*
	x = (int)fEnginePosX - VIEW_RANGE,
	z = (int)fEnginePosZ - VIEW_RANGE;

	for(int az = 0; az < VIEW_RANGE_2; az++, z++, x = (int)fEnginePosX - VIEW_RANGE)
	for(int ax = 0; ax < VIEW_RANGE_2; ax++, x++)
	{
		if(EngineGetGroundHeight(x, z) > 7) continue;

		if(
			(EngineGetGroundHeight(x - 1, z - 1) > 7) ||
			(EngineGetGroundHeight(x,     z - 1) > 7) ||
			(EngineGetGroundHeight(x + 1, z - 1) > 7) ||
			(EngineGetGroundHeight(x - 1, z    ) > 7) ||
			(EngineGetGroundHeight(x + 1, z    ) > 7) ||
			(EngineGetGroundHeight(x - 1, z + 1) > 7) ||
			(EngineGetGroundHeight(x,     z + 1) > 7) ||
			(EngineGetGroundHeight(x + 1, z + 1) > 7)
			)
		{
			GroundHeight[ax][az].color = LANDSCAPE_COLORS[1];
		}
	}
	*/

	// view matrix

	D3DMATRIX	matPos = matIdentify,
				matX,
				matY,
				matZ;

#if USE_NEW_VIEW
	matPos._43 = 5.0f;
#endif

	matX = matY = matZ = matPos;

	matPos._41 = -fEnginePosX;
	matPos._42 = -fEnginePosY;
	matPos._43 = -fEnginePosZ;

	float f = TORAD(fEngineRotX);
	matX._22 =  cosf(f);
	matX._33 =  cosf(f);
	matX._23 = -sinf(f);
	matX._32 =  sinf(f);

	f = TORAD(fEngineRotY);
	matY._11 =  cosf(f);
	matY._33 =  cosf(f);
	matY._13 =  sinf(f);
	matY._31 = -sinf(f);

	f = TORAD(fEngineRotZ);
	matZ._11 =  cosf(f);
	matZ._22 =  cosf(f);
	matZ._12 = -sinf(f);
	matZ._21 =  sinf(f);

	matEngineView = matPos * matY * matX * matZ;

	// objects

	matX = matY = matZ = matIdentify;

	f = TORAD(-fEngineRotX);
	matX._22 =  cosf(f);
	matX._33 =  cosf(f);
	matX._23 = -sinf(f);
	matX._32 =  sinf(f);

	f = TORAD(-fEngineRotY);
	matY._11 =  cosf(f);
	matY._33 =  cosf(f);
	matY._13 =  sinf(f);
	matY._31 = -sinf(f);

	f = TORAD(-fEngineRotZ);
	matZ._11 =  cosf(f);
	matZ._22 =  cosf(f);
	matZ._12 = -sinf(f);
	matZ._21 =  sinf(f);

	matObjects = matX * matY * matZ;

	int x0 = (int)fEnginePosX - VIEW_RANGE,
		x1 = (int)fEnginePosX + VIEW_RANGE - 1,
		x2 = GROUND_X_SIZE,
		x3 = 0,
		z0 = (int)fEnginePosZ - VIEW_RANGE,
		z1 = (int)fEnginePosZ + VIEW_RANGE - 1,
		z2 = GROUND_Z_SIZE,
		z3 = 0;

	if(x0 < 0) x2 = x0 + GROUND_X_SIZE;
	if(z0 < 0) z2 = z0 + GROUND_Z_SIZE;
	if(x1 >= GROUND_X_SIZE) x3 = x1 - GROUND_X_SIZE;
	if(z1 >= GROUND_Z_SIZE) z3 = z1 - GROUND_Z_SIZE;

	THING *thing = Things;
	if(thing) do
	{
		if(
			((thing->x > x0 && thing->x < x1) || (thing->x > x2) || (thing->x < x3)) &&
			((thing->z > z0 && thing->z < z1) || (thing->z > z2) || (thing->z < z3))
			)
		{
			thing->flags |= TF_DRAW;

			if(thing->x < x3)
				thing->ex = thing->x + GROUND_X_SIZE;
			else if(thing->x > x2)
				thing->ex = thing->x - GROUND_X_SIZE;
			else
				thing->ex = thing->x;

			if(thing->z < z3)
				thing->ez = thing->z + GROUND_Z_SIZE;
			else if(thing->z > z2)
				thing->ez = thing->z - GROUND_Z_SIZE;
			else
				thing->ez = thing->z;

			int hx = (int)(thing->ex - x0),
				hz = (int)(thing->ez - z0);

			thing->ey = (float)(GroundHeight[hx][hz].height + GroundHeight[hx + 1][hz].height + GroundHeight[hx][hz + 1].height + GroundHeight[hx + 1][hz + 1].height) / 4.0f;
		}
		else
		{
			thing->flags &= ~TF_DRAW;
		}

		if(
			(thing->Thing.Type == T_EFFECT) && (thing->Thing.Model == M_EFFECT_LAND_BRIDGE) &&
			((thing->LandBridge.x > x0 && thing->LandBridge.x < x1) || (thing->LandBridge.x > x2) || (thing->LandBridge.x < x3)) &&
			((thing->LandBridge.z > z0 && thing->LandBridge.z < z1) || (thing->LandBridge.z > z2) || (thing->LandBridge.z < z3))
			)
		{
			thing->flags |= TF_DRAW_LANDBRIDGE;

			if(thing->LandBridge.x < x3)
				thing->LandBridge.ex = thing->LandBridge.x + GROUND_X_SIZE;
			else if(thing->LandBridge.x > x2)
				thing->LandBridge.ex = thing->LandBridge.x - GROUND_X_SIZE;
			else
				thing->LandBridge.ex = thing->LandBridge.x;

			if(thing->LandBridge.z < z3)
				thing->LandBridge.ez = thing->LandBridge.z + GROUND_Z_SIZE;
			else if(thing->LandBridge.z > z2)
				thing->LandBridge.ez = thing->LandBridge.z - GROUND_Z_SIZE;
			else
				thing->LandBridge.ez = thing->LandBridge.z;

			int hx = (int)(thing->LandBridge.ex - x0),
				hz = (int)(thing->LandBridge.ez - z0);

			thing->LandBridge.ey = (float)(GroundHeight[hx][hz].height + GroundHeight[hx + 1][hz].height + GroundHeight[hx][hz + 1].height + GroundHeight[hx + 1][hz + 1].height) / 4.0f;
		}
		else
		{
			thing->flags &= ~TF_DRAW_LANDBRIDGE;
		}

		thing = thing->Next;
	}
	while(thing != Things);

	for(int a = 0; a < 256; a++)
	{
		if(
			((Markers[a].x > x0 && Markers[a].x < x1) || (Markers[a].x > x2) || (Markers[a].x < x3)) &&
			((Markers[a].z > z0 && Markers[a].z < z1) || (Markers[a].z > z2) || (Markers[a].z < z3))
			)
		{
			Markers[a].flags |= TF_DRAW;

			if(Markers[a].x < x3)
				Markers[a].ex = Markers[a].x + GROUND_X_SIZE;
			else if(Markers[a].x > x2)
				Markers[a].ex = Markers[a].x - GROUND_X_SIZE;
			else
				Markers[a].ex = Markers[a].x;

			if(Markers[a].z < z3)
				Markers[a].ez = Markers[a].z + GROUND_Z_SIZE;
			else if(Markers[a].z > z2)
				Markers[a].ez = Markers[a].z - GROUND_Z_SIZE;
			else
				Markers[a].ez = Markers[a].z;

			int hx = (int)(Markers[a].ex - x0),
				hz = (int)(Markers[a].ez - z0);

			Markers[a].ey = (float)(GroundHeight[hx][hz].height + GroundHeight[hx + 1][hz].height + GroundHeight[hx][hz + 1].height + GroundHeight[hx + 1][hz + 1].height) / 4.0f;
		}
		else
		{
			Markers[a].flags &= ~TF_DRAW;
		}
	}

	lightObjects.dvDirection = D3DVECTOR(sinf(TORAD(fEngineRotY)), sinf(TORAD(-fEngineRotX)), cosf(TORAD(fEngineRotY)));

	// minimap

	f = TORAD(360.0f - fEngineRotY);
	matMiniMap._11 =  cosf(f);
	matMiniMap._22 =  cosf(f);
	matMiniMap._12 = -sinf(f);
	matMiniMap._21 =  sinf(f);


	float	fx = (GROUND_X_SIZE - fEnginePosX) / (float)GROUND_X_SIZE,
			fz = (GROUND_Z_SIZE - fEnginePosZ) / (float)GROUND_Z_SIZE;

	vMiniMap[0].tu = 1.5f - fx;
	vMiniMap[0].tv = 0.5f - fz;

	vMiniMap[1].tu = 0.5f - fx;
	vMiniMap[1].tv = 0.5f - fz;

	vMiniMap[2].tu = 1.5f - fx;
	vMiniMap[2].tv = 1.5f - fz;

	vMiniMap[3].tu = 0.5f - fx;
	vMiniMap[3].tv = 1.5f - fz;

	//

	return S_OK;
}


WORD EngineGetGroundHeight(int x, int z)
{
	while(x < 0) x += GROUND_X_SIZE;
	while(z < 0) z += GROUND_Z_SIZE;
	while(x >= GROUND_X_SIZE) x -= GROUND_X_SIZE;
	while(z >= GROUND_Z_SIZE) z -= GROUND_Z_SIZE;
	return wEngineGround[z * GROUND_Z_SIZE + x];
}


void EngineSetGroundHeight(int x, int z, WORD h)
{
	while(x < 0) x += GROUND_X_SIZE;
	while(z < 0) z += GROUND_Z_SIZE;
	while(x >= GROUND_X_SIZE) x -= GROUND_X_SIZE;
	while(z >= GROUND_Z_SIZE) z -= GROUND_Z_SIZE;
	wEngineGround[z * GROUND_Z_SIZE + x] = h;
}


long EngineUpdateKeys()
{
	bool UpdateView = false;

	if(bKeys[VK_UP])
	{
#if DEBUG
		if(bKeys[VK_CONTROL])
		{
			fEngineRotX -= (float)dwEngineTick * SPEED_ROT_X;
		}
		else
		{
			if(bKeys[VK_SHIFT])
			{
				fEnginePosY += (float)dwEngineTick * SPEED_POS_Y;
			}
			else goto up_height;
		}
		goto up_skip;
up_height:
#endif
		float f = 360.0f - fEngineRotY;
		while(f >= 360.0f) f -= 360.0f;
		while(f < 0.0f) f += 360.0f;
		fEnginePosX -= sinf(TORAD(f)) * (float)dwEngineTick * SPEED_POS_X;
		fEnginePosZ += cosf(TORAD(f)) * (float)dwEngineTick * SPEED_POS_Z;
#if DEBUG
up_skip:
#endif
		UpdateView = true;
	}

	if(bKeys[VK_DOWN])
	{
#if DEBUG
		if(bKeys[VK_CONTROL])
		{
			fEngineRotX += (float)dwEngineTick * SPEED_ROT_X;
		}
		else
		{
			if(bKeys[VK_SHIFT])
			{
				fEnginePosY -= (float)dwEngineTick * SPEED_POS_Y;
			}
			else goto down_height;
		}
		goto down_skip;
down_height:
#endif
		float f = 360.0f - fEngineRotY;
		while(f >= 360.0f) f -= 360.0f;
		while(f < 0.0f) f += 360.0f;
		fEnginePosX += sinf(TORAD(f)) * (float)dwEngineTick * SPEED_POS_X;
		fEnginePosZ -= cosf(TORAD(f)) * (float)dwEngineTick * SPEED_POS_Z;
#if DEBUG
down_skip:
#endif
		UpdateView = true;
	}

	if(bKeys[VK_LEFT])
	{
		fEngineRotY -= (float)dwEngineTick * SPEED_ROT_Y;
		UpdateView = true;
	}

	if(bKeys[VK_RIGHT])
	{
		fEngineRotY += (float)dwEngineTick * SPEED_ROT_Y;
		UpdateView = true;
	}

	if (bKeys[VK_OEM_PLUS])
	{
		if (fEngineRotX > MIN_ROT_X  && fEnginePosY < MAX_SPERICAL_POS_Y)
			fEngineRotX -= (float)dwEngineTick * SPEED_ROT_X;
		if (fEnginePosY > MIN_POS_Y)
			fEnginePosY -= (float)dwEngineTick * SPEED_POS_Y_ZOOM;
		UpdateView = true;
	}

	if (bKeys[VK_OEM_MINUS])
	{
		if (fEngineRotX < MAX_ROT_X)
			fEngineRotX += (float)dwEngineTick * SPEED_ROT_X;
		if (fEnginePosY < MAX_POS_Y)
			fEnginePosY += (float)dwEngineTick * SPEED_POS_Y_ZOOM;
		UpdateView = true;
	}

	if(UpdateView)
	{
		fLandEditUpdate = true;
		return EngineUpdateView();
	}
	else
		return S_OK;
}


long EngineUpdateMiniMap()
{
	if(!fEngineActive) return -1;
	if(!txMiniMap->lpDDSTexture) return -1;

	HDC	hDC;

	if FAILED(rs = txMiniMap->lpDDSTexture->GetDC(&hDC))
	{
		sprintf(str, SZ_ERR_ENGINE_GETDC, "EngineUpdateMiniMap", rs);
		LogWrite(str);
		return rs;
	}

	for(int z = 0; z < GROUND_Z_SIZE; z++)
	for(int x = 0; x < GROUND_X_SIZE; x++)
	{
		WORD w = EngineGetGroundHeight(x, z);

		if(w > 8)
			w /= 8;
		else if(w > 0)
			w = 1;

		if(w > 0xFF) w = 0xFF;

		DWORD dw = LANDSCAPE_COLORS[w];

		SetPixel(hDC, x, z,
			(
			((dw >> 16) & 0xFF) |
			(dw & 0xFF00) |
			((dw & 0xFF) << 16) |
			(dw & 0xFF000000)
			)
			);
	}

	if(Things && !(dwEngineFlags & EF_HIDE_OBJS_ON_MINIMAP))
	{
		THING *t = Things;
		do
		{
			DWORD c;

			if(IsNotNeutral(t))
			{
				switch(t->Thing.Owner)
				{
				case OWNER_BLUE:   c = 0xFFFF00; break;
				case OWNER_RED:    c = 0x0000FF; break;
				case OWNER_YELLOW: c = 0x00FFFF; break;
				case OWNER_GREEN:  c = 0x00FF00; break;
				default: c = 0xC0C0C0;
				}
			}
			else
				c = 0xC0C0C0;

			SetPixel(hDC, (t->Thing.PosX >> 8) / 2, (t->Thing.PosZ >> 8) / 2, c);
			t = t->Next;
		}
		while(t != Things);
	}

	rs = txMiniMap->lpDDSTexture->ReleaseDC(hDC);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_RELEASEDC, "EngineUpdateMiniMap", rs);
		LogWrite(str);
		return rs;
	}

	return S_OK;
}


TEXTURE* EngineCreateTexture(WORD Width, WORD Height, char *Name, WORD Resource)
{
	TEXTURE *texture;

	texture = new TEXTURE;
	memset(texture, 0, sizeof(TEXTURE));

	if(Name)
	{
		texture->Name = new char[strlen(Name) + 1];
		strcpy(texture->Name, Name);
	}
	texture->Resource = Resource;

	texture->Width = Width;
	texture->Height = Height;

	LINK(Textures, texture);

	return texture;
}


long EngineDestroyTexture(TEXTURE *texture)
{
	UNLINK(Textures, texture);

	EngineTextureDestroySurface(texture);

	if(texture->Name)
	{
		delete[] texture->Name;
		texture->Name = 0;
	}

	delete texture;

	return S_OK;
}


long EngineTextureCreateSurface(TEXTURE *texture)
{
	if(!lpDD) return -1;
	if(texture->lpDDSTexture) return S_OK;

	DDSURFACEDESC2 ddsd;
	memset(&ddsd, 0, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_TEXTURESTAGE;
	ddsd.ddsCaps.dwCaps	= DDSCAPS_TEXTURE;
	ddsd.dwWidth = texture->Width;
	ddsd.dwHeight = texture->Height;
	memcpy(&ddsd.ddpfPixelFormat, &DDPixelFormat, sizeof(DDPixelFormat));

	if(fHwDevice) ddsd.ddsCaps.dwCaps2 = DDSCAPS2_TEXTUREMANAGE;

	rs = lpDD->CreateSurface(&ddsd, &texture->lpDDSTexture, 0);
	if FAILED(rs)
	{
		texture->lpDDSTexture = 0;
		sprintf(str, SZ_ERR_ENGINE_CREATETEXTURE, texture->Name, rs);
		LogWrite(str);
		return -1;
	}

	DDCOLORKEY colorkey;
	colorkey.dwColorSpaceHighValue =
	colorkey.dwColorSpaceLowValue  = 0;

	texture->lpDDSTexture->SetColorKey(DDCKEY_SRCBLT, &colorkey);

	return S_OK;
}


long EngineTextureDestroySurface(TEXTURE *texture)
{
	if(texture->lpDDSTexture)
	{
		rs = texture->lpDDSTexture->Release();
		texture->lpDDSTexture = 0;

		if(rs != 0)
		{
			if(texture->Name)
				sprintf(str, SZ_ENGINE_RELEASE_REF, texture->Name, rs);
			else
				sprintf(str, SZ_ENGINE_RELEASE_REF, "Texture", rs);

			LogWrite(str);

			return -1;
		}
	}

	return S_OK;
}


long EngineLoadTexture(TEXTURE *texture)
{
	if FAILED(rs = EngineTextureCreateSurface(texture)) return rs;

	HDC hDC;
	HBITMAP hBmp = 0;

	if(texture->Name)
	{
		hBmp = (HBITMAP) LoadImage(0, texture->Name, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		if(!hBmp)
		{
			sprintf(str, SZ_ERR_ENGINE_LOADTEXTURE, texture->Name);
			LogWrite(str);
			goto failed;
		}
	}
	else if(texture->Resource)
	{
		hBmp = (HBITMAP) LoadImage(hInst, MAKEINTRESOURCE(texture->Resource), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
		if(!hBmp)
		{
			sprintf(str, SZ_ERR_ENGINE_LOADTEXTURE, "Texture");
			LogWrite(str);
			goto failed;
		}
	}
	else
	{
		DDBLTFX	bltfx;
		memset(&bltfx, 0, sizeof(bltfx));
		bltfx.dwSize = sizeof(bltfx);

		bltfx.dwFillColor = 0;
		return texture->lpDDSTexture->Blt(0, 0, 0, DDBLT_COLORFILL, &bltfx);
	}

	rs = texture->lpDDSTexture->GetDC(&hDC);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_GETDC, "LoadTexture", rs);
		LogWrite(str);
		return -1;
	}

	HDC hdcBmp = CreateCompatibleDC(hDC);
	if(!hdcBmp)
	{
		sprintf(str, SZ_ERR_ENGINE_CREATEDC, texture->Name);
		LogWrite(str);

		texture->lpDDSTexture->ReleaseDC(hDC);
		DeleteObject(hBmp);

		return -1;
	}

	SelectObject(hdcBmp, hBmp);
	BitBlt(hDC, 0, 0, texture->Width, texture->Height, hdcBmp, 0, 0, SRCCOPY);

	DeleteDC(hdcBmp);
	texture->lpDDSTexture->ReleaseDC(hDC);
	DeleteObject(hBmp);

	return S_OK;

failed:
	rs = texture->lpDDSTexture->GetDC(&hDC);
	if FAILED(rs)
	{
		sprintf(str, SZ_ERR_ENGINE_GETDC, "LoadTexture", rs);
		LogWrite(str);
		return -1;
	}

	RECT rc = { 0, 0, texture->Width, texture->Height };
	HBRUSH hBrush = CreateSolidBrush(0xFFFFFF);
	FillRect(hDC, &rc, hBrush);
	DeleteObject(hBrush);

	texture->lpDDSTexture->ReleaseDC(hDC);

	return S_FALSE;
}


long EngineRestoreTextures()
{
	char szPath[STR_SIZE];
	GetCurrentDirectory(STR_SIZE, szPath);
	SetCurrentDirectory(szStartPath);

	TEXTURE *texture = Textures;

	if(texture) do
	{
		if(texture->lpDDSTexture->IsLost())
			if SUCCEEDED(texture->lpDDSTexture->Restore())
				EngineLoadTexture(texture);

		texture = texture->Next;
	}
	while(texture != Textures);

	SetCurrentDirectory(szPath);

	return S_OK;
}


long EngineReLoadAllTextures()
{
	char szPath[STR_SIZE];
	GetCurrentDirectory(STR_SIZE, szPath);
	SetCurrentDirectory(szStartPath);

	TEXTURE *texture = Textures;

	if(texture) do
	{
		EngineLoadTexture(texture);
		texture = texture->Next;
	}
	while(texture != Textures);

	SetCurrentDirectory(szPath);

	return S_OK;
}


long EngineUnLoadAllTextures()
{
	TEXTURE *texture = Textures;

	if(texture) do
	{
		EngineTextureDestroySurface(texture);
		texture = texture->Next;
	}
	while(texture != Textures);

	return S_OK;
}


bool EngineLoadObjects()
{
	txUnknow		= EngineCreateTexture(TEXTURE_SIZE, TEXTURE_SIZE, SZ_TX_UNKNOW, 0);
	txTrigger		= EngineCreateTexture(TEXTURE_SIZE, TEXTURE_SIZE, SZ_TX_TRIGGER, 0);
	txDiscovery		= EngineCreateTexture(TEXTURE_SIZE, TEXTURE_SIZE, SZ_TX_DISCOVERY, 0);
	txSpell			= EngineCreateTexture(TEXTURE_SIZE, TEXTURE_SIZE, SZ_TX_SPELL, 0);
	txEffect		= EngineCreateTexture(TEXTURE_SIZE, TEXTURE_SIZE, SZ_TX_EFFECT, 0);
	txAddon			= EngineCreateTexture(TEXTURE_SIZE, TEXTURE_SIZE, SZ_TX_BUILDING_ADD_ON, 0);
	txWild			= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_WILD, 0);
	txBraveBlue		= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_BRAVE_BLUE, 0);
	txBraveRed		= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_BRAVE_RED, 0);
	txBraveYellow	= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_BRAVE_YELLOW, 0);
	txBraveGreen	= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_BRAVE_GREEN, 0);
	txSpyBlue		= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_SPY_BLUE, 0);
	txSpyRed		= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_SPY_RED, 0);
	txSpyYellow		= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_SPY_YELLOW, 0);
	txSpyGreen		= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_SPY_GREEN, 0);
	txWarriorBlue	= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_WARRIOR_BLUE, 0);
	txWarriorRed	= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_WARRIOR_RED, 0);
	txWarriorYellow	= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_WARRIOR_YELLOW, 0);
	txWarriorGreen	= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_WARRIOR_GREEN, 0);
	txPriestBlue	= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_PRIEST_BLUE, 0);
	txPriestRed		= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_PRIEST_RED, 0);
	txPriestYellow	= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_PRIEST_YELLOW, 0);
	txPriestGreen	= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_PRIEST_GREEN, 0);
	txFwBlue		= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_FW_BLUE, 0);
	txFwRed			= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_FW_RED, 0);
	txFwYellow		= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_FW_YELLOW, 0);
	txFwGreen		= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_FW_GREEN, 0);
	txShamanBlue	= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_SHAMAN_BLUE, 0);
	txShamanRed		= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_SHAMAN_RED, 0);
	txShamanYellow	= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_SHAMAN_YELLOW, 0);
	txShamanGreen	= EngineCreateTexture(PERSON_TEXTURE_SIZE, PERSON_TEXTURE_SIZE, SZ_TX_SHAMAN_GREEN, 0);
	txPlant1		= EngineCreateTexture(TEXTURE_SIZE, TEXTURE_SIZE, SZ_TX_PLANT1, 0);
	txPlant2		= EngineCreateTexture(TEXTURE_SIZE, TEXTURE_SIZE, SZ_TX_PLANT2, 0);
	txWoodPile		= EngineCreateTexture(TEXTURE_SIZE, TEXTURE_SIZE, SZ_TX_WOOD_PILE, 0);
	txRock			= EngineCreateTexture(TEXTURE_SIZE, TEXTURE_SIZE, SZ_TX_ROCK, 0);
	txFire			= EngineCreateTexture(TEXTURE_SIZE, TEXTURE_SIZE, SZ_TX_FIRE, 0);
	txLight			= EngineCreateTexture(TEXTURE_SIZE, TEXTURE_SIZE, SZ_TX_LIGHT, 0);
	txLandBridge	= EngineCreateTexture(TEXTURE_SIZE, TEXTURE_SIZE, SZ_TX_LANDBRIDGE, 0);
	txMarker		= EngineCreateTexture(TEXTURE_SIZE, TEXTURE_SIZE, SZ_TX_MARKER, 0);

	objAodBlue				= _3dsLoadObj(SZ_OBJ_AOD_BLUE);
	objAodRed				= _3dsLoadObj(SZ_OBJ_AOD_RED);
	objAodYellow			= _3dsLoadObj(SZ_OBJ_AOD_YELLOW);
	objAodGreen				= _3dsLoadObj(SZ_OBJ_AOD_GREEN);
	objBoat					= _3dsLoadObj(SZ_OBJ_BOAT);
	objBoatBlue				= _3dsLoadObj(SZ_OBJ_BOAT_BLUE);
	objBoatRed				= _3dsLoadObj(SZ_OBJ_BOAT_RED);
	objBoatYellow			= _3dsLoadObj(SZ_OBJ_BOAT_YELLOW);
	objBoatGreen			= _3dsLoadObj(SZ_OBJ_BOAT_GREEN);
	objBallon				= _3dsLoadObj(SZ_OBJ_BALLON);
	objBallonBlue			= _3dsLoadObj(SZ_OBJ_BALLON_BLUE);
	objBallonRed			= _3dsLoadObj(SZ_OBJ_BALLON_RED);
	objBallonYellow			= _3dsLoadObj(SZ_OBJ_BALLON_YELLOW);
	objBallonGreen			= _3dsLoadObj(SZ_OBJ_BALLON_GREEN);
	objTree01				= _3dsLoadObj(SZ_OBJ_TREE01);
	objTree02				= _3dsLoadObj(SZ_OBJ_TREE02);
	objTree03				= _3dsLoadObj(SZ_OBJ_TREE03);
	objTree04				= _3dsLoadObj(SZ_OBJ_TREE04);
	objTree05				= _3dsLoadObj(SZ_OBJ_TREE05);
	objTree06				= _3dsLoadObj(SZ_OBJ_TREE06);
	objTree07				= _3dsLoadObj(SZ_OBJ_TREE07);
	objTree08				= _3dsLoadObj(SZ_OBJ_TREE08);
	objTree09				= _3dsLoadObj(SZ_OBJ_TREE09);
	objTree10				= _3dsLoadObj(SZ_OBJ_TREE10);
	objTree11				= _3dsLoadObj(SZ_OBJ_TREE11);
	objTree12				= _3dsLoadObj(SZ_OBJ_TREE12);
	objTree13				= _3dsLoadObj(SZ_OBJ_TREE13);
	objTree14				= _3dsLoadObj(SZ_OBJ_TREE14);
	objKnowledge			= _3dsLoadObj(SZ_OBJ_KNOWLEDGE);
	objStoneHead			= _3dsLoadObj(SZ_OBJ_STONE_HEAD);
	objHut1Blue				= _3dsLoadObj(SZ_OBJ_HUT1_BLUE);
	objHut2Blue				= _3dsLoadObj(SZ_OBJ_HUT2_BLUE);
	objHut3Blue				= _3dsLoadObj(SZ_OBJ_HUT3_BLUE);
	objHut1Red				= _3dsLoadObj(SZ_OBJ_HUT1_RED);
	objHut2Red				= _3dsLoadObj(SZ_OBJ_HUT2_RED);
	objHut3Red				= _3dsLoadObj(SZ_OBJ_HUT3_RED);
	objHut1Yellow			= _3dsLoadObj(SZ_OBJ_HUT1_YELLOW);
	objHut2Yellow			= _3dsLoadObj(SZ_OBJ_HUT2_YELLOW);
	objHut3Yellow			= _3dsLoadObj(SZ_OBJ_HUT3_YELLOW);
	objHut1Green			= _3dsLoadObj(SZ_OBJ_HUT1_GREEN);
	objHut2Green			= _3dsLoadObj(SZ_OBJ_HUT2_GREEN);
	objHut3Green			= _3dsLoadObj(SZ_OBJ_HUT3_GREEN);
	objTowerBlue			= _3dsLoadObj(SZ_OBJ_TOWER_BLUE);
	objTowerRed				= _3dsLoadObj(SZ_OBJ_TOWER_RED);
	objTowerYellow			= _3dsLoadObj(SZ_OBJ_TOWER_YELLOW);
	objTowerGreen			= _3dsLoadObj(SZ_OBJ_TOWER_GREEN);
	objSpyTrainBlue			= _3dsLoadObj(SZ_OBJ_SPY_TRAIN_BLUE);
	objSpyTrainRed			= _3dsLoadObj(SZ_OBJ_SPY_TRAIN_RED);
	objSpyTrainYellow		= _3dsLoadObj(SZ_OBJ_SPY_TRAIN_YELLOW);
	objSpyTrainGreen		= _3dsLoadObj(SZ_OBJ_SPY_TRAIN_GREEN);
	objWarriorTrainBlue		= _3dsLoadObj(SZ_OBJ_WARRIOR_TRAIN_BLUE);
	objWarriorTrainRed		= _3dsLoadObj(SZ_OBJ_WARRIOR_TRAIN_RED);
	objWarriorTrainYellow	= _3dsLoadObj(SZ_OBJ_WARRIOR_TRAIN_YELLOW);
	objWarriorTrainGreen	= _3dsLoadObj(SZ_OBJ_WARRIOR_TRAIN_GREEN);
	objTempleBlue			= _3dsLoadObj(SZ_OBJ_TEMPLE_BLUE);
	objTempleRed			= _3dsLoadObj(SZ_OBJ_TEMPLE_RED);
	objTempleYellow			= _3dsLoadObj(SZ_OBJ_TEMPLE_YELLOW);
	objTempleGreen			= _3dsLoadObj(SZ_OBJ_TEMPLE_GREEN);
	objFwTrainBlue			= _3dsLoadObj(SZ_OBJ_FW_TRAIN_BLUE);
	objFwTrainRed			= _3dsLoadObj(SZ_OBJ_FW_TRAIN_RED);
	objFwTrainYellow		= _3dsLoadObj(SZ_OBJ_FW_TRAIN_YELLOW);
	objFwTrainGreen			= _3dsLoadObj(SZ_OBJ_FW_TRAIN_GREEN);
	objBoatHutBlue			= _3dsLoadObj(SZ_OBJ_BOAT_HUT_BLUE);
	objBoatHutRed			= _3dsLoadObj(SZ_OBJ_BOAT_HUT_RED);
	objBoatHutYellow		= _3dsLoadObj(SZ_OBJ_BOAT_HUT_YELLOW);
	objBoatHutGreen			= _3dsLoadObj(SZ_OBJ_BOAT_HUT_GREEN);
	objBallonHutBlue		= _3dsLoadObj(SZ_OBJ_BALLON_HUT_BLUE);
	objBallonHutRed			= _3dsLoadObj(SZ_OBJ_BALLON_HUT_RED);
	objBallonHutYellow		= _3dsLoadObj(SZ_OBJ_BALLON_HUT_YELLOW);
	objBallonHutGreen		= _3dsLoadObj(SZ_OBJ_BALLON_HUT_GREEN);
	objPrison				= _3dsLoadObj(SZ_OBJ_PRISON);
	objRSPillar				= _3dsLoadObj(SZ_OBJ_RS_PILLAR);
	objRSPillarBlue         = _3dsLoadObj(SZ_OBJ_RS_PILLAR_BLUE);
	objRSPillarRed			= _3dsLoadObj(SZ_OBJ_RS_PILLAR_RED);
	objRSPillarYellow		= _3dsLoadObj(SZ_OBJ_RS_PILLAR_YELLOW);
	objRSPillarGreen		= _3dsLoadObj(SZ_OBJ_RS_PILLAR_GREEN);
	objSpecialA				= _3dsLoadObj(SZ_OBJ_SPECIAL_A);
	objSpecialB				= _3dsLoadObj(SZ_OBJ_SPECIAL_B);
	objSpecialC				= _3dsLoadObj(SZ_OBJ_SPECIAL_C);
	objSpecialD				= _3dsLoadObj(SZ_OBJ_SPECIAL_D);
	objSpecialE				= _3dsLoadObj(SZ_OBJ_SPECIAL_E);
	objSpecialF				= _3dsLoadObj(SZ_OBJ_SPECIAL_F);
	objEagle				= _3dsLoadObj(SZ_OBJ_EAGLE);

	return true;
}


bool EngineDestroyObjects()
{
	OBJ3D		*obj;
	OBJ3D_PART	*part;

	while(Objs3D)
	{
		obj = Objs3D;
		UNLINK(Objs3D, obj);

		while(obj->Parts)
		{
			part = obj->Parts;
			UNLINK(obj->Parts, part);
			if(part->Verts) delete[] part->Verts;
			delete part;
		}

		delete obj;
	}

	return true;
}


long EngineDrawObj3D(OBJ3D *obj, float x, float y, float z, DWORD angle)
{
	if(!obj) return S_OK;

	if(dwEngineFlags & EF_WIREFRAME_OBJS) lpD3DDevice->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_WIREFRAME);

	D3DMATRIX mat = matIdentify;

	switch(angle)
	{
	case ANGLE_45:
		mat._11 =  ANGLE_45_COS;
		mat._33 =  ANGLE_45_COS;
		mat._13 = -ANGLE_45_SIN;
		mat._31 =  ANGLE_45_SIN;
		break;
	case ANGLE_90:
		mat._11 =  ANGLE_90_COS;
		mat._33 =  ANGLE_90_COS;
		mat._13 = -ANGLE_90_SIN;
		mat._31 =  ANGLE_90_SIN;
		break;
	case ANGLE_135:
		mat._11 =  ANGLE_135_COS;
		mat._33 =  ANGLE_135_COS;
		mat._13 = -ANGLE_135_SIN;
		mat._31 =  ANGLE_135_SIN;
		break;
	case ANGLE_180:
		mat._11 =  ANGLE_180_COS;
		mat._33 =  ANGLE_180_COS;
		mat._13 = -ANGLE_180_SIN;
		mat._31 =  ANGLE_180_SIN;
		break;
	case ANGLE_225:
		mat._11 =  ANGLE_225_COS;
		mat._33 =  ANGLE_225_COS;
		mat._13 = -ANGLE_225_SIN;
		mat._31 =  ANGLE_225_SIN;
		break;
	case ANGLE_270:
		mat._11 =  ANGLE_270_COS;
		mat._33 =  ANGLE_270_COS;
		mat._13 = -ANGLE_270_SIN;
		mat._31 =  ANGLE_270_SIN;
		break;
	case ANGLE_315:
		mat._11 =  ANGLE_315_COS;
		mat._33 =  ANGLE_315_COS;
		mat._13 = -ANGLE_315_SIN;
		mat._31 =  ANGLE_315_SIN;
		break;
	default:
		mat._11 =  ANGLE_0_COS;
		mat._33 =  ANGLE_0_COS;
		mat._13 = -ANGLE_0_SIN;
		mat._31 =  ANGLE_0_SIN;
	}

	mat._41 = x;
	mat._42 = y;
	mat._43 = z;

	lpD3DDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &mat);

	OBJ3D_PART *part = obj->Parts;
	if(part) do
	{
		if(part->texture)
			lpD3DDevice->SetTexture(0, part->texture->lpDDSTexture);
		else
			lpD3DDevice->SetTexture(0, 0);

		lpD3DDevice->SetMaterial(&part->Mtrl);

		lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, D3DFVF_VERTEX, part->Verts, part->VertsCount, 0);

#if DRAW_OBJECTS_NORMALS
		for(WORD w = 0; w < part->VertsCount; w++)
		{
			D3DVERTEX v[2];
			memset(v, 0, sizeof(D3DVERTEX) * 2);

			v[0].x = part->Verts[w].x;
			v[0].y = part->Verts[w].y;
			v[0].z = part->Verts[w].z;

			v[1].x = part->Verts[w].x + part->Verts[w].nx;
			v[1].y = part->Verts[w].y + part->Verts[w].ny;
			v[1].z = part->Verts[w].z + part->Verts[w].nz;

			lpD3DDevice->DrawPrimitive(D3DPT_LINELIST, D3DFVF_VERTEX, v, 2, 0);
		}
#endif

		part = part->Next;
	}
	while(part != obj->Parts);

	if(dwEngineFlags & EF_BOUNDINGBOX)
	{
		lpD3DDevice->SetTexture(0, 0);
		lpD3DDevice->DrawIndexedPrimitive(D3DPT_LINELIST, D3DFVF_VERTEX, obj->BoundBox.v, 8, BoundBoxIdxs, 24, 0);
	}

	if(dwEngineFlags & EF_WIREFRAME_OBJS) lpD3DDevice->SetRenderState(D3DRENDERSTATE_FILLMODE, D3DFILL_SOLID);

	return S_OK;
}


void EngineSetTreeType()
{
	switch(leveldat->Header.v2.ObjectsBankNum)
	{
	case 3:
		objTree1 = objTree04;
		objTree2 = objTree05;
		objTree3 = objTree06;
		break;
	case 4:
		objTree1 = objTree07;
		objTree2 = objTree08;
		objTree3 = objTree09;
		break;
	case 5:
		objTree1 = objTree10;
		objTree2 = objTree11;
		objTree3 = objTree12;
		break;
	case 6:
		objTree1 = objTree13;
		objTree2 = objTree01;
		objTree3 = objTree14;
		break;
	default:
		objTree1 = objTree01;
		objTree2 = objTree02;
		objTree3 = objTree03;
	}
}


void EngineNewMap()
{
	memset(&Markers, 0, sizeof(Markers));
	memset(leveldat, 0, sizeof(LEVELDATv3));

	THING *thing;

	while(Things)
	{
		thing = Things;
		UNLINK(Things, thing);
		delete thing;
	}

	ObjectsCount = 0;

	memset(wEngineGround, 0, sizeof(wEngineGround));

	EngineSetTreeType();

	ThingSelected = 0;
	ThingLink = 0;
	DlgObjectUpdateInfo(hDlgObject);
	EngineUpdateView();
	EngineUpdateMiniMap();
	UpdateHeaderDialogs();
	DlgMarkersUpdate(hDlgMarkers);

	szLevel[0] = 0;
	DlgInfoUpdate(hDlgInfo);

    const size_t MAGIC_SIZE = 5;
    const char MAGICDEF[MAGIC_SIZE] = { 'L','E','V','L','3' };
    memcpy(&leveldat->MAGIC, MAGICDEF, MAGIC_SIZE);
}


void EngineLoadConfig()
{
	// tmp map
	char szTempLevelFfilePath[MAX_PATH];
	if (!GetApplicationUserDataFilePath(SZ_ENGINE_TMP_MAP, szTempLevelFfilePath))
	{
		strcpy_s(szTempLevelFfilePath, sizeof(char) * (strlen(SZ_ENGINE_TMP_MAP) + 1), SZ_ENGINE_TMP_MAP);
	}

	EngineLoadLevelV3(szTempLevelFfilePath);
	szLevel[0] = 0;

	// config
	char szEngineConfigFilePath[MAX_PATH];
	if (!GetApplicationUserDataFilePath(SZ_ENGINE_CONFIG, szEngineConfigFilePath))
	{
		strcpy_s(szEngineConfigFilePath, sizeof(char) * (strlen(SZ_ENGINE_CONFIG) + 1), SZ_ENGINE_CONFIG);
	}

	HANDLE h;

	h = CreateFile(szEngineConfigFilePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(h == INVALID_HANDLE_VALUE)
	{
		/*
		sprintf(str, SZ_ERR_CREATEFILE, szEngineConfigFilePath);
		LogWrite(str);
		*/

		//memcpy(&guidDevice, &guid, sizeof(GUID));
		//strcpy(szDeviceName, devname);

		return;
	}

	DWORD dw;
	int   i;

	// engine flags

	dwRW = 0;
	ReadFile(h, &dw, sizeof(DWORD), &dwRW, 0);
	if(dwRW != sizeof(DWORD))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}
	dwEngineFlags = dw;

	// brush size / speed

	dwRW = 0;
	ReadFile(h, &i, sizeof(int), &dwRW, 0);
	if(dwRW != sizeof(int))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}
	if(i < BRUSH_SIZE_MIN) i = BRUSH_SIZE_MIN;
	if(i > BRUSH_SIZE_MAX) i = BRUSH_SIZE_MAX;
	GroundEditBrushSize = i;

	dwRW = 0;
	ReadFile(h, &i, sizeof(int), &dwRW, 0);
	if(dwRW != sizeof(int))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}
	if(i < BRUSH_SPEED_MIN) i = BRUSH_SPEED_MIN;
	if(i > BRUSH_SPEED_MAX) i = BRUSH_SPEED_MAX;
	GroundEditBrushSpeed = i;

	// device / screen mode

	GUID  guid;
	DWORD width, height, depth;
	bool  fullscreen, hw;
	char  devname[sizeof(szDeviceName)];

	dwRW = 0;
	ReadFile(h, &guid, sizeof(GUID), &dwRW, 0);
	if(dwRW != sizeof(GUID))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	dwRW = 0;
	ReadFile(h, devname, sizeof(szDeviceName), &dwRW, 0);
	if(dwRW != sizeof(szDeviceName))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	dwRW = 0;
	ReadFile(h, &hw, sizeof(bool), &dwRW, 0);
    if(dwRW != sizeof(bool))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	dwRW = 0;
	ReadFile(h, &width, sizeof(DWORD), &dwRW, 0);
	if(dwRW != sizeof(DWORD))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	dwRW = 0;
	ReadFile(h, &height, sizeof(DWORD), &dwRW, 0);
	if(dwRW != sizeof(DWORD))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	dwRW = 0;
	ReadFile(h, &depth, sizeof(DWORD), &dwRW, 0);
	if(dwRW != sizeof(DWORD))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	dwRW = 0;
	ReadFile(h, &fullscreen, sizeof(bool), &dwRW, 0);
	if(dwRW != sizeof(bool))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	memcpy(&guidDevice, &guid, sizeof(GUID));
	strcpy(szDeviceName, devname);
	dwScreenWidth  = width;
	dwScreenHeight = height;
	dwScreenDepth  = depth;
	fFullScreen    = fullscreen;
	fHwDevice      = hw;

	//

	CloseHandle(h);
}


void EngineSaveConfig()
{
	SetCurrentDirectory(szStartPath);

	// tmp map

	char szTempLevelFfilePath[MAX_PATH];
	if (!GetApplicationUserDataFilePath(SZ_ENGINE_TMP_MAP, szTempLevelFfilePath))
	{
		strcpy_s(szTempLevelFfilePath, sizeof(char) * (strlen(SZ_ENGINE_TMP_MAP) + 1), SZ_ENGINE_TMP_MAP);
	}

	EngineSaveLevelV3(szTempLevelFfilePath);  //the temp file can always be saved in v3 format, it will guarantee no loss

	// config

	char szEngineConfigFilePath[MAX_PATH];
	if (!GetApplicationUserDataFilePath(SZ_ENGINE_CONFIG, szEngineConfigFilePath))
	{
		strcpy_s(szEngineConfigFilePath, sizeof(char) * (strlen(SZ_ENGINE_CONFIG) + 1), SZ_ENGINE_CONFIG);
	}

	HANDLE h;

	h = CreateFile(szEngineConfigFilePath, GENERIC_WRITE | GENERIC_READ, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(h == INVALID_HANDLE_VALUE)
	{
		sprintf(str, SZ_ERR_CREATEFILE, szEngineConfigFilePath);
		LogWrite(str);
		return;
	}

	// engine flags

	dwRW = 0;
	WriteFile(h, &dwEngineFlags, sizeof(DWORD), &dwRW, 0);
	if(dwRW != sizeof(DWORD))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	// brush size / speed

	dwRW = 0;
	WriteFile(h, &GroundEditBrushSize, sizeof(int), &dwRW, 0);
	if(dwRW != sizeof(int))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	dwRW = 0;
	WriteFile(h, &GroundEditBrushSpeed, sizeof(int), &dwRW, 00);
	if(dwRW != sizeof(int))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	// device / screen mode

	dwRW = 0;
	WriteFile(h, &guidDevice, sizeof(GUID), &dwRW, 0);
	if(dwRW != sizeof(GUID))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	dwRW = 0;
	WriteFile(h, szDeviceName, sizeof(szDeviceName), &dwRW, 0);
	if(dwRW != sizeof(szDeviceName))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	dwRW = 0;
	WriteFile(h, &fHwDevice, sizeof(bool), &dwRW, 0);
	if(dwRW != sizeof(bool))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	dwRW = 0;
	WriteFile(h, &dwScreenWidth, sizeof(DWORD), &dwRW, 0);
	if(dwRW != sizeof(DWORD))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	dwRW = 0;
	WriteFile(h, &dwScreenHeight, sizeof(DWORD), &dwRW, 0);
	if(dwRW != sizeof(DWORD))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	dwRW = 0;
	WriteFile(h, &dwScreenDepth, sizeof(DWORD), &dwRW, 0);
	if(dwRW != sizeof(DWORD))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	dwRW = 0;
	WriteFile(h, &fFullScreen, sizeof(bool), &dwRW, 0);
	if(dwRW != sizeof(bool))
	{
		CloseHandle(h);

		sprintf(str, SZ_ERR_READERROR, szEngineConfigFilePath);
		LogWrite(str);

		return;
	}

	//

	CloseHandle(h);
}


long __stdcall EnumModes(LPDDSURFACEDESC2 ddsd, void*)
{
	if(ddsd->ddpfPixelFormat.dwRGBBitCount <= 8) return D3DENUMRET_OK;

	SCREEN_INFO *si;
	si = new SCREEN_INFO;
	si->Width   = ddsd->dwWidth;
	si->Height  = ddsd->dwHeight;
	si->Depth   = ddsd->ddpfPixelFormat.dwRGBBitCount;

	SCREEN_INFO *a = ScreenInfo;

	if(a)
	{
		do
		{
			if(si->Width < a->Width)
				goto _link_b4;
			else
				if(si->Width > a->Width) goto _next;

			if(si->Height < a->Height)
				goto _link_b4;
			else
				if(si->Height > a->Height) goto _next;

			if(si->Depth < a->Depth)
				goto _link_b4;
			else
				if(si->Depth > a->Depth) goto _next;
_next:
			a = a->Next;
		}
		while(a != ScreenInfo);

		LINK(ScreenInfo, si);
		goto _skip;
_link_b4:
		si->Next = a;
		si->Prev = a->Prev;
		a->Prev->Next = si;
		a->Prev = si;

		if(a == ScreenInfo) ScreenInfo = si;
_skip:;
	}
	else
	{
		LINK(ScreenInfo, si);
	}

	return DDENUMRET_OK;
}


long __stdcall EnumDevices(char *szDescription, char *szDeviceName, LPD3DDEVICEDESC7 lpD3DDeviceDesc, void*)
{
	DEVICE_INFO *di;
	di = new DEVICE_INFO;
	memset(di, 0, sizeof(DEVICE_INFO));

	DWORD dw = strlen(szDeviceName) + 1;
	di->szName = (char*)malloc(dw);
	strcpy(di->szName, szDeviceName);

	memcpy(&di->guid, &lpD3DDeviceDesc->deviceGUID, sizeof(GUID));

	di->fHw = lpD3DDeviceDesc->dwDevCaps & D3DDEVCAPS_HWRASTERIZATION ? true : false;

	LINK(DeviceInfo, di);

	return D3DENUMRET_OK;
}


void EngineMouseSetCapture()
{
	SetCapture(hMainWnd);
	fCaptured = true;
	fMoving = false;

	GetCursorPos(&ptCaptured);
	ScreenToClient(hMainWnd, &ptCaptured);
}


void EngineMouseReleaseCapture()
{
	ReleaseCapture();
	fCaptured = false;
}


void EngineMouseLDown()
{
	if(fEngineEditObjs && Things)
	{
		D3DVECTOR r0, r1;
		EngineGetPick(&r0, &r1);

		OBJ3D *obj;
		DWORD angle;
		D3DVECTOR v[8];
		float sdist, dist;

		ThingSelected = 0;

		THING *t = Things;
		do
		{
			if((t->flags & TF_DRAW_LANDBRIDGE) && (t->Thing.Type == T_EFFECT) && (t->Thing.Model == M_EFFECT_LAND_BRIDGE))
			{
				matObjects._41 = t->LandBridge.ex;
				matObjects._42 = t->LandBridge.ey;
				matObjects._43 = t->LandBridge.ez;

				v[0].x = vObj2Dsmall[0].x;
				v[0].y = vObj2Dsmall[0].y;
				v[0].z = vObj2Dsmall[0].z;
				VectorMatrixMultiply(&v[0], &matObjects);
				v[1].x = vObj2Dsmall[1].x;
				v[1].y = vObj2Dsmall[1].y;
				v[1].z = vObj2Dsmall[1].z;
				VectorMatrixMultiply(&v[1], &matObjects);
				v[2].x = vObj2Dsmall[2].x;
				v[2].y = vObj2Dsmall[2].y;
				v[2].z = vObj2Dsmall[2].z;
				VectorMatrixMultiply(&v[2], &matObjects);
				v[3].x = vObj2Dsmall[3].x;
				v[3].y = vObj2Dsmall[3].y;
				v[3].z = vObj2Dsmall[3].z;
				VectorMatrixMultiply(&v[3], &matObjects);

				if(EngineTriangleIntersect(r0, r1, v[0], v[1], v[2]) || EngineTriangleIntersect(r0, r1, v[3], v[2], v[1]))
				{
					dist = (t->LandBridge.ex - r0.x) + (t->LandBridge.ez - r0.z);
					if(dist < 0) dist *= -1;

					if(!ThingSelected || (dist < sdist))
					{
						ThingSelected = t;
						sdist = dist;
						ThingSelected->flags |= TF_EDIT_LANDBRIDGE;
					}
				}
			}

			if(t->flags & TF_DRAW)
			{
				angle = 0;
				switch(t->Thing.Type)
				{
				case T_PERSON:
					switch(t->Thing.Model)
					{
					case M_PERSON_WILD:
						goto _2d_hit;
					case M_PERSON_BRAVE:
					case M_PERSON_WARRIOR:
					case M_PERSON_PRIEST:
					case M_PERSON_SPY:
					case M_PERSON_FIREWARRIOR:
					case M_PERSON_SHAMAN:
						if((t->Thing.Owner == OWNER_BLUE) || (t->Thing.Owner == OWNER_RED) || (t->Thing.Owner == OWNER_YELLOW) || (t->Thing.Owner == OWNER_GREEN)) goto _2d_hit;
						goto _unknow;
					case M_PERSON_ANGELOFDEATH:
						switch(t->Thing.Owner)
						{
						case OWNER_BLUE:   obj = objAodBlue;   goto _3d_hit;
						case OWNER_RED:    obj = objAodRed;    goto _3d_hit;
						case OWNER_YELLOW: obj = objAodYellow; goto _3d_hit;
						case OWNER_GREEN:  obj = objAodGreen;  goto _3d_hit;
						}
						goto _unknow;
					}
					goto _unknow;
				case T_BUILDING:
					angle = t->Thing.Building.Angle;
					switch(t->Thing.Model)
					{
					case M_BUILDING_HUT1:
						switch(t->Thing.Owner)
						{
						case OWNER_BLUE:   obj = objHut1Blue;   goto _3d_hit;
						case OWNER_RED:	   obj = objHut1Red;    goto _3d_hit;
						case OWNER_YELLOW: obj = objHut1Yellow; goto _3d_hit;
						case OWNER_GREEN:  obj = objHut1Green;  goto _3d_hit;
						}
						goto _unknow;
					case M_BUILDING_HUT2:
						switch(t->Thing.Owner)
						{
						case OWNER_BLUE:   obj = objHut2Blue;   goto _3d_hit;
						case OWNER_RED:	   obj = objHut2Red;    goto _3d_hit;
						case OWNER_YELLOW: obj = objHut2Yellow; goto _3d_hit;
						case OWNER_GREEN:  obj = objHut2Green;  goto _3d_hit;
						}
						goto _unknow;
					case M_BUILDING_HUT3:
						switch(t->Thing.Owner)
						{
						case OWNER_BLUE:   obj = objHut3Blue;   goto _3d_hit;
						case OWNER_RED:	   obj = objHut3Red;    goto _3d_hit;
						case OWNER_YELLOW: obj = objHut3Yellow; goto _3d_hit;
						case OWNER_GREEN:  obj = objHut3Green;  goto _3d_hit;
						}
						goto _unknow;
					case M_BUILDING_TOWER:
						switch(t->Thing.Owner)
						{
						case OWNER_BLUE:   obj = objTowerBlue;   goto _3d_hit;
						case OWNER_RED:    obj = objTowerRed;    goto _3d_hit;
						case OWNER_YELLOW: obj = objTowerYellow; goto _3d_hit;
						case OWNER_GREEN:  obj = objTowerGreen;  goto _3d_hit;
						}
						goto _unknow;
					case M_BUILDING_SPY_TRAIN:
						switch(t->Thing.Owner)
						{
						case OWNER_BLUE:   obj = objSpyTrainBlue;   goto _3d_hit;
						case OWNER_RED:    obj = objSpyTrainRed;    goto _3d_hit;
						case OWNER_YELLOW: obj = objSpyTrainYellow; goto _3d_hit;
						case OWNER_GREEN:  obj = objSpyTrainGreen;  goto _3d_hit;
						}
						goto _unknow;
					case M_BUILDING_WARRIOR_TRAIN:
						switch(t->Thing.Owner)
						{
						case OWNER_BLUE:   obj = objWarriorTrainBlue;   goto _3d_hit;
						case OWNER_RED:    obj = objWarriorTrainRed;    goto _3d_hit;
						case OWNER_YELLOW: obj = objWarriorTrainYellow; goto _3d_hit;
						case OWNER_GREEN:  obj = objWarriorTrainGreen;  goto _3d_hit;
						}
						goto _unknow;
					case M_BUILDING_TEMPLE:
						switch(t->Thing.Owner)
						{
						case OWNER_BLUE:   obj = objTempleBlue;   goto _3d_hit;
						case OWNER_RED:    obj = objTempleRed;    goto _3d_hit;
						case OWNER_YELLOW: obj = objTempleYellow; goto _3d_hit;
						case OWNER_GREEN:  obj = objTempleGreen;  goto _3d_hit;
						}
						goto _unknow;
					case M_BUILDING_FIREWARRIOR_TRAIN:
						switch(t->Thing.Owner)
						{
						case OWNER_BLUE:   obj = objFwTrainBlue;   goto _3d_hit;
						case OWNER_RED:    obj = objFwTrainRed;    goto _3d_hit;
						case OWNER_YELLOW: obj = objFwTrainYellow; goto _3d_hit;
						case OWNER_GREEN:  obj = objFwTrainGreen;  goto _3d_hit;
						}
						goto _unknow;
					case M_BUILDING_BOAT_HUT_1:
					case M_BUILDING_BOAT_HUT_2:
						switch(t->Thing.Owner)
						{
						case OWNER_BLUE:   obj = objBoatHutBlue;   goto _3d_hit;
						case OWNER_RED:    obj = objBoatHutRed;    goto _3d_hit;
						case OWNER_YELLOW: obj = objBoatHutYellow; goto _3d_hit;
						case OWNER_GREEN:  obj = objBoatHutGreen;  goto _3d_hit;
						}
						goto _unknow;
					case M_BUILDING_AIRSHIP_HUT_1:
					case M_BUILDING_AIRSHIP_HUT_2:
						switch(t->Thing.Owner)
						{
						case OWNER_BLUE:   obj = objBallonHutBlue;   goto _3d_hit;
						case OWNER_RED:    obj = objBallonHutRed;    goto _3d_hit;
						case OWNER_YELLOW: obj = objBallonHutYellow; goto _3d_hit;
						case OWNER_GREEN:  obj = objBallonHutGreen;  goto _3d_hit;
						}
						goto _unknow;
					case M_BUILDING_KNOWLEDGE:
						obj = objKnowledge;
						goto _3d_hit;
					case M_BUILDING_PRISON:
						obj = objPrison;
						goto _3d_hit;
					}
					goto _unknow;
				case T_SCENERY:
					angle = t->Thing.Scenery.Angle;
					if(t->Thing.Model == M_SCENERY_TOP_LEVEL_SCENERY || t->Thing.Model == M_SCENERY_SUB_LEVEL_SCENERY)
					{
						switch(t->Thing.Scenery.IslandNum)
						{
						case M_SPECIAL_A: obj = objSpecialA; goto _3d_hit;
						case M_SPECIAL_B: obj = objSpecialB; goto _3d_hit;
						case M_SPECIAL_C: obj = objSpecialC; goto _3d_hit;
						case M_SPECIAL_D: obj = objSpecialD; goto _3d_hit;
						case M_SPECIAL_E: obj = objSpecialE; goto _3d_hit;
						case M_SPECIAL_F: obj = objSpecialF; goto _3d_hit;
						}
						goto _unknow;
					}
					switch(t->Thing.Model)
					{
					case M_SCENERY_PLANT_1:
					case M_SCENERY_PLANT_2:
					case M_SCENERY_WOOD_PILE:
					case M_SCENERY_ROCK:
					case M_SCENERY_FIRE:
						goto _2d_hit_small;
					case M_SCENERY_TREE_1:
					case M_SCENERY_TREE_4:
						obj = objTree1;
						goto _3d_hit;
					case M_SCENERY_TREE_2:
					case M_SCENERY_TREE_5:
						obj = objTree2;
						goto _3d_hit;
					case M_SCENERY_TREE_3:
					case M_SCENERY_TREE_6:
						obj = objTree3;
						goto _3d_hit;
					case M_SCENERY_STONE_HEAD:
						obj = objStoneHead;
						goto _3d_hit;
					case M_SCENERY_RS_PILLAR:
						switch(t->Thing.Owner)
						{
						case OWNER_BLUE:   obj = objRSPillarBlue;   goto _3d_hit;
						case OWNER_RED:    obj = objRSPillarRed;    goto _3d_hit;
						case OWNER_YELLOW: obj = objRSPillarYellow; goto _3d_hit;
						case OWNER_GREEN:  obj = objRSPillarGreen;  goto _3d_hit;
						}
						obj = objRSPillar;
						goto _3d_hit;
					}
					goto _unknow;
				case T_VEHICLE:
					switch(t->Thing.Model)
					{
					case M_VEHICLE_BOAT_1:
					case M_VEHICLE_BOAT_2:
						switch(t->Thing.Owner)
						{
						case OWNER_BLUE:   obj = objBoatBlue;   goto _3d_hit;
						case OWNER_RED:    obj = objBoatRed;    goto _3d_hit;
						case OWNER_YELLOW: obj = objBoatYellow; goto _3d_hit;
						case OWNER_GREEN:  obj = objBoatGreen;  goto _3d_hit;
						}
						obj = objBoat;
						goto _3d_hit;
					case M_VEHICLE_AIRSHIP_1:
					case M_VEHICLE_AIRSHIP_2:
						switch(t->Thing.Owner)
						{
						case OWNER_BLUE:   obj = objBallonBlue;   goto _3d_hit;
						case OWNER_RED:    obj = objBallonRed;    goto _3d_hit;
						case OWNER_YELLOW: obj = objBallonYellow; goto _3d_hit;
						case OWNER_GREEN:  obj = objBallonGreen;  goto _3d_hit;
						}
						obj = objBallon;
						goto _3d_hit;
					}
					goto _unknow;
				case T_GENERAL:
					switch(t->Thing.Model)
					{
					case M_GENERAL_TRIGGER:
					case M_GENERAL_DISCOVERY:
					case M_GENERAL_BUILDING_ADD_ON:
					case M_GENERAL_LIGHT:
						goto _2d_hit_small;
					}
					goto _unknow;
				case T_SPELL:
					goto _2d_hit_small;
				case T_EFFECT:
					goto _2d_hit_small;
                case T_SHOT:
                    goto _2d_hit_small;
				case T_CREATURE:
					if(t->Thing.Model == M_CREATURE_EAGLE)
					{
						obj = objEagle;
						goto _3d_hit;
					}
					goto _unknow;
				}
				goto _unknow;

				goto _continue;
_unknow:;
_2d_hit_small:;
_2d_hit:;
				{
					matObjects._41 = t->ex;
					matObjects._42 = t->ey;
					matObjects._43 = t->ez;

					v[0].x = vObj2Dsmall[0].x;
					v[0].y = vObj2Dsmall[0].y;
					v[0].z = vObj2Dsmall[0].z;
					VectorMatrixMultiply(&v[0], &matObjects);
					v[1].x = vObj2Dsmall[1].x;
					v[1].y = vObj2Dsmall[1].y;
					v[1].z = vObj2Dsmall[1].z;
					VectorMatrixMultiply(&v[1], &matObjects);
					v[2].x = vObj2Dsmall[2].x;
					v[2].y = vObj2Dsmall[2].y;
					v[2].z = vObj2Dsmall[2].z;
					VectorMatrixMultiply(&v[2], &matObjects);
					v[3].x = vObj2Dsmall[3].x;
					v[3].y = vObj2Dsmall[3].y;
					v[3].z = vObj2Dsmall[3].z;
					VectorMatrixMultiply(&v[3], &matObjects);

					if(EngineTriangleIntersect(r0, r1, v[0], v[1], v[2]) || EngineTriangleIntersect(r0, r1, v[3], v[2], v[1])) goto _select;
				}
				goto _continue;
_3d_hit:;
				{
					D3DMATRIX mat = matIdentify;

					switch(angle)
					{
					case ANGLE_45:
						mat._11 =  ANGLE_45_COS;
						mat._33 =  ANGLE_45_COS;
						mat._13 = -ANGLE_45_SIN;
						mat._31 =  ANGLE_45_SIN;
						break;
					case ANGLE_90:
						mat._11 =  ANGLE_90_COS;
						mat._33 =  ANGLE_90_COS;
						mat._13 = -ANGLE_90_SIN;
						mat._31 =  ANGLE_90_SIN;
						break;
					case ANGLE_135:
						mat._11 =  ANGLE_135_COS;
						mat._33 =  ANGLE_135_COS;
						mat._13 = -ANGLE_135_SIN;
						mat._31 =  ANGLE_135_SIN;
						break;
					case ANGLE_180:
						mat._11 =  ANGLE_180_COS;
						mat._33 =  ANGLE_180_COS;
						mat._13 = -ANGLE_180_SIN;
						mat._31 =  ANGLE_180_SIN;
						break;
					case ANGLE_225:
						mat._11 =  ANGLE_225_COS;
						mat._33 =  ANGLE_225_COS;
						mat._13 = -ANGLE_225_SIN;
						mat._31 =  ANGLE_225_SIN;
						break;
					case ANGLE_270:
						mat._11 =  ANGLE_270_COS;
						mat._33 =  ANGLE_270_COS;
						mat._13 = -ANGLE_270_SIN;
						mat._31 =  ANGLE_270_SIN;
						break;
					case ANGLE_315:
						mat._11 =  ANGLE_315_COS;
						mat._33 =  ANGLE_315_COS;
						mat._13 = -ANGLE_315_SIN;
						mat._31 =  ANGLE_315_SIN;
						break;
					default:
						mat._11 =  ANGLE_0_COS;
						mat._33 =  ANGLE_0_COS;
						mat._13 = -ANGLE_0_SIN;
						mat._31 =  ANGLE_0_SIN;
					}

					mat._41 = t->ex;
					mat._42 = t->ey;
					mat._43 = t->ez;

					v[0].x = obj->BoundBox.v[0].x;
					v[0].y = obj->BoundBox.v[0].y;
					v[0].z = obj->BoundBox.v[0].z;
					VectorMatrixMultiply(&v[0], &mat);
					v[1].x = obj->BoundBox.v[1].x;
					v[1].y = obj->BoundBox.v[1].y;
					v[1].z = obj->BoundBox.v[1].z;
					VectorMatrixMultiply(&v[1], &mat);
					v[2].x = obj->BoundBox.v[2].x;
					v[2].y = obj->BoundBox.v[2].y;
					v[2].z = obj->BoundBox.v[2].z;
					VectorMatrixMultiply(&v[2], &mat);
					v[3].x = obj->BoundBox.v[3].x;
					v[3].y = obj->BoundBox.v[3].y;
					v[3].z = obj->BoundBox.v[3].z;
					VectorMatrixMultiply(&v[3], &mat);
					v[4].x = obj->BoundBox.v[4].x;
					v[4].y = obj->BoundBox.v[4].y;
					v[4].z = obj->BoundBox.v[4].z;
					VectorMatrixMultiply(&v[4], &mat);
					v[5].x = obj->BoundBox.v[5].x;
					v[5].y = obj->BoundBox.v[5].y;
					v[5].z = obj->BoundBox.v[5].z;
					VectorMatrixMultiply(&v[5], &mat);
					v[6].x = obj->BoundBox.v[6].x;
					v[6].y = obj->BoundBox.v[6].y;
					v[6].z = obj->BoundBox.v[6].z;
					VectorMatrixMultiply(&v[6], &mat);
					v[7].x = obj->BoundBox.v[7].x;
					v[7].y = obj->BoundBox.v[7].y;
					v[7].z = obj->BoundBox.v[7].z;
					VectorMatrixMultiply(&v[7], &mat);

					if(
						EngineTriangleIntersect(r0, r1, v[0], v[3], v[2]) || EngineTriangleIntersect(r0, r1, v[2], v[1], v[0]) ||
						EngineTriangleIntersect(r0, r1, v[0], v[1], v[5]) || EngineTriangleIntersect(r0, r1, v[5], v[4], v[0]) ||
						EngineTriangleIntersect(r0, r1, v[1], v[2], v[6]) || EngineTriangleIntersect(r0, r1, v[6], v[5], v[1]) ||
						EngineTriangleIntersect(r0, r1, v[2], v[3], v[7]) || EngineTriangleIntersect(r0, r1, v[7], v[6], v[2]) ||
						EngineTriangleIntersect(r0, r1, v[3], v[0], v[4]) || EngineTriangleIntersect(r0, r1, v[4], v[7], v[3])
						)
						goto _select;
				}
				goto _continue;
_select:
				dist = (t->ex - r0.x) + (t->ez - r0.z);
				if(dist < 0) dist *= -1;

				if(!ThingSelected || (dist < sdist))
				{
					ThingSelected = t;
					sdist = dist;
					t->flags &= ~TF_EDIT_LANDBRIDGE;
				}
			}
			
_continue:
			t = t->Prev;
		}
		while(t != Things);

		DlgObjectUpdateInfo(hDlgObject);

		EngineMouseSetCapture();
	}
	else if(fEngineEditLand)
	{
		EngineMouseSetCapture();
		MouseButton = MouseButtonLeft;
	}
	else if(fEngineEditMarkers)
	{
		D3DVECTOR r0, r1;
		EngineGetPick(&r0, &r1);

		D3DVECTOR v[8];
		float sdist, dist;
		MarkerSelected = -1;

		for(int a = 0; a < 256; a++)
		{
			if(Markers[a].flags & TF_DRAW)
			{
				matObjects._41 = Markers[a].ex;
				matObjects._42 = Markers[a].ey;
				matObjects._43 = Markers[a].ez;

				v[0].x = vObj2Dsmall[0].x;
				v[0].y = vObj2Dsmall[0].y;
				v[0].z = vObj2Dsmall[0].z;
				VectorMatrixMultiply(&v[0], &matObjects);
				v[1].x = vObj2Dsmall[1].x;
				v[1].y = vObj2Dsmall[1].y;
				v[1].z = vObj2Dsmall[1].z;
				VectorMatrixMultiply(&v[1], &matObjects);
				v[2].x = vObj2Dsmall[2].x;
				v[2].y = vObj2Dsmall[2].y;
				v[2].z = vObj2Dsmall[2].z;
				VectorMatrixMultiply(&v[2], &matObjects);
				v[3].x = vObj2Dsmall[3].x;
				v[3].y = vObj2Dsmall[3].y;
				v[3].z = vObj2Dsmall[3].z;
				VectorMatrixMultiply(&v[3], &matObjects);

				if(EngineTriangleIntersect(r0, r1, v[0], v[1], v[2]) || EngineTriangleIntersect(r0, r1, v[3], v[2], v[1]))
				{
					dist = (Markers[a].ex - r0.x) + (Markers[a].ez - r0.z);
					if(dist < 0) dist *= -1;

					if((MarkerSelected == -1) || (dist < sdist))
					{
						MarkerSelected = a;
						sdist = dist;
					}
				}
			}
		}


		DlgMarkersUpdate(hDlgMarkers);

		EngineMouseSetCapture();
	}
}


void EngineMouseLUp()
{
	if(fCaptured)
	{
		EngineMouseReleaseCapture();
		if(fEngineEditLand || fEngineEditObjs) EngineUpdateMiniMap();
	}
}


void EngineMouseRDown()
{
	if(fEngineEditLand)
	{
		EngineMouseSetCapture();
		MouseButton = MouseButtonRight;
	}
}


void EngineMouseRUp()
{
	if(fCaptured)
	{
		EngineMouseReleaseCapture();
		if(fEngineEditLand || fEngineEditObjs) EngineUpdateMiniMap();
	}
}


void EngineMouseWheel(int delta)
{
	if(fEngineEditLand)
	{
		if(delta < 0)
		{
			GroundEditBrushSize--;
			if(GroundEditBrushSize < BRUSH_SIZE_MIN) GroundEditBrushSize = BRUSH_SIZE_MIN;
		}
		else
		{
			GroundEditBrushSize++;
			if(GroundEditBrushSize > BRUSH_SIZE_MAX) GroundEditBrushSize = BRUSH_SIZE_MAX;
		}
		DlgBrushUpdate(hDlgBrush);
	}
	else if(fEngineEditObjs && ThingSelected)
	{
		if(ThingSelected->Thing.Type == T_BUILDING)
		{
			if(delta < 0)
			{
				switch(ThingSelected->Thing.Building.Angle)
				{
				case ANGLE_270: ThingSelected->Thing.Building.Angle = ANGLE_180; break;
				case ANGLE_180: ThingSelected->Thing.Building.Angle = ANGLE_90;  break;
				case ANGLE_90:  ThingSelected->Thing.Building.Angle = ANGLE_0;   break;
				default: ThingSelected->Thing.Building.Angle = ANGLE_270;
				}
			}
			else
			{
				switch(ThingSelected->Thing.Building.Angle)
				{
				case ANGLE_0:   ThingSelected->Thing.Building.Angle = ANGLE_90;  break;
				case ANGLE_90:  ThingSelected->Thing.Building.Angle = ANGLE_180; break;
				case ANGLE_180: ThingSelected->Thing.Building.Angle = ANGLE_270; break;
				default: ThingSelected->Thing.Building.Angle = ANGLE_0;
				}
			}
		}
		else if(ThingSelected->Thing.Type == T_SCENERY)
		{
			if(delta < 0)
			{
				switch(ThingSelected->Thing.Scenery.Angle)
				{
				case ANGLE_315: ThingSelected->Thing.Scenery.Angle = ANGLE_270; break;
				case ANGLE_270: ThingSelected->Thing.Scenery.Angle = ANGLE_225; break;
				case ANGLE_225: ThingSelected->Thing.Scenery.Angle = ANGLE_180; break;
				case ANGLE_180: ThingSelected->Thing.Scenery.Angle = ANGLE_135; break;
				case ANGLE_135: ThingSelected->Thing.Scenery.Angle = ANGLE_90;  break;
				case ANGLE_90:  ThingSelected->Thing.Scenery.Angle = ANGLE_45;  break;
				case ANGLE_45:  ThingSelected->Thing.Scenery.Angle = ANGLE_0;   break;
				default: ThingSelected->Thing.Scenery.Angle = ANGLE_315;
				}
			}
			else
			{
				switch(ThingSelected->Thing.Scenery.Angle)
				{
				case ANGLE_0:   ThingSelected->Thing.Scenery.Angle = ANGLE_45;  break;
				case ANGLE_45:  ThingSelected->Thing.Scenery.Angle = ANGLE_90;  break;
				case ANGLE_90:  ThingSelected->Thing.Scenery.Angle = ANGLE_135; break;
				case ANGLE_135: ThingSelected->Thing.Scenery.Angle = ANGLE_180; break;
				case ANGLE_180: ThingSelected->Thing.Scenery.Angle = ANGLE_225; break;
				case ANGLE_225: ThingSelected->Thing.Scenery.Angle = ANGLE_270; break;
				case ANGLE_270: ThingSelected->Thing.Scenery.Angle = ANGLE_315; break;
				default: ThingSelected->Thing.Scenery.Angle = ANGLE_0;
				}
			}
		}
	}
}


bool IsNotNeutral(THING *t)
{
	return (
		((t->Thing.Type == T_PERSON) && (t->Thing.Model != M_PERSON_WILD)) ||
		((t->Thing.Type == T_BUILDING) && (t->Thing.Model != M_BUILDING_KNOWLEDGE)) ||
		((t->Thing.Type == T_SCENERY) && (t->Thing.Model == M_SCENERY_RS_PILLAR)) ||
		((t->Thing.Type == T_GENERAL) && (t->Thing.Model == M_GENERAL_BUILDING_ADD_ON)) ||
		(t->Thing.Type == T_VEHICLE)
	);
}
