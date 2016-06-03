
/*

Alacn
alacn.uhahaa@gmail.com
http://alacn.dnsalias.org:8080/

*/

#include "std.h"
#include "popedt.h"
#include "log.h"
#include "commctrl.h"
#include "pop.h"
#include "engine.h"
#include "dialogs.h"
#include "user_storage.h"



HWND				hMainWnd;
HINSTANCE			hInst;
RECT				rcClient;
bool				fActive,
					fEngineActive,
					fFullScreen;
BYTE				bKeys[256];
char				str[STR_SIZE],
					str2[STR_SIZE],
					str3[STR_SIZE],
					szStartPath[STR_SIZE];
long				rs;
DWORD				dwRW;


long __stdcall WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
bool RegisterCreateMainWindow();
void ProcessCommandLine(char *cmdline);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, char *szCmdLine, int)
{
	hInst = hInstance;

	dwScreenWidth	= 640;
	dwScreenHeight	= 480;
	dwScreenDepth	= 16;
	//guidDevice		= IID_IDirect3DHALDevice;
	//strcpy(szDeviceName, "Direct3D HAL");
	guidDevice		= IID_IDirect3DTnLHalDevice;
	strcpy(szDeviceName, "Direct3D TnLHal");
	fHwDevice		= true;
	fFullScreen		= true;

	if (!InitializeUserFiles())
	{
		MessageBox(NULL, _T("Failed to initialize user files."), _T("Initialization Error"), MB_ICONERROR | MB_OK);
		return -1;
	}

	GetCurrentDirectory(sizeof(szStartPath), szStartPath);

	if(!RegisterCreateMainWindow()) return -1;

	LogCreate();

	EnginePrepare();
	dwEngineFlags = EF_DITHER | EF_TEXTURE_FILTER | EF_TRANSPARENT_MINIMAP | EF_SHOW_MINIMAP;
	EngineLoadConfig();

	ProcessCommandLine(szCmdLine);

	ShowWindow(hMainWnd, SW_SHOW);
	UpdateWindow(hMainWnd);
	SetFocus(hMainWnd);
	fActive = true;
	InitializeDialogs();

	HACCEL hAcell = LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_ACCELERATOR));

_retry:
	if FAILED(EngineCreate())
	{
		EngineDestroy();

		if(guidDevice == IID_IDirect3DTnLHalDevice)
		{
			guidDevice = IID_IDirect3DHALDevice;
			strcpy(szDeviceName, "Direct3D HAL");
			goto _retry;
		}

		ModalMsg(SZ_ENGINE_FAILED, APPNAME, MB_ICONHAND);
		if(!hDlgDevice) DlgDeviceToggle();
		CenterDialog(hDlgDevice);
	}

	MenuBarToggle();
	if(fEngineEditObjs) DlgObjectToggle();
	SetFocus(hMainWnd);
	if(hDlgDevice) SetFocus(hDlgDevice);

	MSG Msg;
	while(true)
	{
		if(PeekMessage(&Msg, 0, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage(&Msg, 0, 0, 0)) return Msg.wParam;

			HWND hWnd = GetActiveWindow();

			if(!TranslateAccelerator(hMainWnd, hAcell, &Msg))
			{
				if(hWnd == 0 || hWnd == hMainWnd || !IsDialogMessage(hWnd, &Msg))
				{
					TranslateMessage(&Msg);
					DispatchMessage(&Msg);
				}
			}
		}
		else
		{
			if(fActive && fEngineActive)
			{
				if FAILED(rs = EngineUpdateFrame())
				{
					EngineDestroy();
					ModalMsg(SZ_ENGINE_FAILED, APPNAME, MB_ICONHAND);
					if(!hDlgDevice) DlgDeviceToggle();
					CenterDialog(hDlgDevice);
				}
			}
			else WaitMessage();
		}
	}
}


bool RegisterCreateMainWindow()
{
	WNDCLASS wc;

	wc.cbClsExtra		=
    wc.cbWndExtra		= 0;
	wc.hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);
	wc.hCursor			= LoadCursor(0, IDC_ARROW);
	wc.hIcon			= LoadIcon(hInst, MAKEINTRESOURCE(IDI_CN));
	wc.hInstance		= hInst;
	wc.lpfnWndProc		= WndProc;
	wc.lpszClassName	= APPNAME;
	wc.lpszMenuName		= 0;
	wc.style			= CS_DBLCLKS;
	
	if(!RegisterClass(&wc))
	{
		MessageBox(0, SZ_CANNOT_REGISTER_CLASS, APPNAME, MB_ICONHAND);
		return false;
	}

	hMainWnd = CreateWindowEx(0, APPNAME, APPNAME, WS_POPUP, 0, 0, 0, 0, 0, 0, hInst, 0);
	if(!hMainWnd)
	{
		MessageBox(0, SZ_CANNOT_CREATE_WINDOW, APPNAME, MB_ICONHAND);
		return false;
	}

	return true;
}


long __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CLOSE:
		FinishDialogs();
		EngineDestroy();
		EngineSaveConfig();
		EngineFinish();
		LogClose();
		DestroyWindow(hMainWnd);
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		if(wParam == SIZE_MAXHIDE || wParam == SIZE_MINIMIZED)
			fActive = false;
		else
			fActive = true;
		break;

	case WM_MOVE:
		if(!fFullScreen)
		{
			GetClientRect(hWnd, &rcClient);

			POINT pt = {0, 0};
			ClientToScreen(hWnd, &pt);
			rcClient.left   += pt.x;
			rcClient.right  += pt.x;
			rcClient.top    += pt.y;
			rcClient.bottom += pt.y;
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_ACCEL_MENUBAR:   MenuBarToggle();      break;
		case ID_ACCEL_DLGBRUSH:  DlgBrushToggle();     break;
		case ID_ACCEL_DLGLIST:   DlgListToggle();      break;
		case ID_ACCEL_DLGDEVICE: DlgDeviceToggle();    break;
		case ID_ACCEL_LAND:      SetEditLand();        break;
		case ID_ACCEL_OBJECTS:   SetEditObjects();     break;
		case ID_ACCEL_MARKERS:   SetEditMarkers();     break;
		case ID_ACCEL_HELP:      ShowHelp();           break;
		case ID_ACCEL_NEW:       LevelNew();           break;
		case ID_ACCEL_OPEN:      LevelOpen();          break;
		case ID_ACCEL_SAVE:      LevelSave();          break;
		case ID_ACCEL_SAVE_AS:   LevelSaveAs();        break;
		}
		break;

	case WM_KILLFOCUS:
		memset(bKeys, 0, sizeof(bKeys));
		break;

	case WM_LBUTTONDOWN:
		EngineMouseLDown();
		break;

	case WM_LBUTTONUP:
		EngineMouseLUp();
		break;

	case WM_RBUTTONDOWN:
		EngineMouseRDown();
		break;

	case WM_RBUTTONUP:
		EngineMouseRUp();
		break;

	case WM_MOUSEWHEEL:
		EngineMouseWheel((short)HIWORD(wParam));
		break;

	case WM_KEYDOWN:
		if(wParam < sizeof(bKeys)) bKeys[LOWORD(wParam)] = 1;
		switch(wParam)
		{
		case VK_ESCAPE:
			PostMessage(hMainWnd, WM_CLOSE, 0, 0);
			return 0;
		}
		break;

	case WM_KEYUP:
		if(wParam < sizeof(bKeys)) bKeys[LOWORD(wParam)] = 0;
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void ProcessCommandLine(char *cmdline)
{
	char *str, *strLimit;

	str = cmdline;
	strLimit = str + strlen(str);

	while(str < strLimit)
	{
		char *strWord;
		strWord = str;

		while(str[0] != '\0' && str[0] != ' ') str++;
		str[0] = '\0'; str++;

		if(!strcmpi(strWord, "Win")) fFullScreen = false;
		else if(!strcmpi(strWord, "fs")) fFullScreen = true;

		else if(!strcmpi(strWord, "640x400"))  { dwScreenWidth = 640;  dwScreenHeight = 400; }
		else if(!strcmpi(strWord, "640x480"))  { dwScreenWidth = 640;  dwScreenHeight = 480; }
		else if(!strcmpi(strWord, "800x600"))  { dwScreenWidth = 800;  dwScreenHeight = 600; }
		else if(!strcmpi(strWord, "1024x768")) { dwScreenWidth = 1024; dwScreenHeight = 768; }

		else if(!strcmpi(strWord, "depth16")) dwScreenDepth = 16;
		else if(!strcmpi(strWord, "depth32")) dwScreenDepth = 32;

		else if(!strcmpi(strWord, "TnL")) { guidDevice = IID_IDirect3DTnLHalDevice; strcpy(szDeviceName, "TnL Hal"); fHwDevice = true;  }
		else if(!strcmpi(strWord, "HAL")) { guidDevice = IID_IDirect3DHALDevice;    strcpy(szDeviceName, "Hal");     fHwDevice = true;  }
		else if(!strcmpi(strWord, "RGB")) { guidDevice = IID_IDirect3DRGBDevice;    strcpy(szDeviceName, "RGB");     fHwDevice = false; }
		else if(!strcmpi(strWord, "Ref")) { guidDevice = IID_IDirect3DRefDevice;    strcpy(szDeviceName, "Ref");     fHwDevice = false; }

		while(str < strLimit && str[0] == ' ') str++;
	}
}
