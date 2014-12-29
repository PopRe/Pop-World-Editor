
/*

Alacn
alacn.uhahaa@gmail.com
http://alacn.dnsalias.org:8080/

*/

#define SZ_ENGINE_FAILED				"Engine Failed"
#define SZ_ENGINE_FAILED_RESTORE		"Engine Failed\nrestore?"
#define SZ_CANNOT_REGISTER_CLASS		"Cannot Register Window Class"
#define SZ_CANNOT_CREATE_WINDOW			"Cannot Create Window"



extern HINSTANCE hInst;
extern HWND hMainWnd;
extern RECT rcClient;
extern BYTE bKeys[256];
extern char str[STR_SIZE], str2[STR_SIZE], str3[STR_SIZE], szStartPath[STR_SIZE];
extern long rs;
extern bool fEngineActive, fFullScreen;
extern DWORD dwRW;
