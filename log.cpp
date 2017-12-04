
/*

Alacn
alacn.uhahaa@gmail.com
http://alacn.dnsalias.org:8080/

*/

#include "std.h"
#include "popedt.h"
#include "user_storage.h"
#include "log.h"



HANDLE hLog = INVALID_HANDLE_VALUE;
char szLogFilePath[MAX_PATH];


long LogCreate()
{
	if(hLog != INVALID_HANDLE_VALUE) return -1;

	if(!GetApplicationUserDataFilePath(LOG_FILE, szLogFilePath)) return -1;

	hLog = CreateFile(szLogFilePath, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(hLog == INVALID_HANDLE_VALUE) return -1;

	LogWrite(LOG_TITLE);

	sprintf(str, LOG_INFO, __DATE__, __TIME__, POPEDT_VERSION);
	LogWrite(str);

	SYSTEMTIME time;
	GetSystemTime(&time);
	sprintf(str, LOG_TIME, time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond);
	LogWrite(str);

	return S_OK;
}


long LogClose()
{
	if(hLog == INVALID_HANDLE_VALUE) return -1;
	LogWrite(LOG_END);
	if(!CloseHandle(hLog)) return -1;
	return S_OK;
}


long LogWrite(char *str)
{
#ifdef _DEBUG
	OutputDebugString(str);
	OutputDebugString("\n");
#endif
	if(hLog == INVALID_HANDLE_VALUE) return -1;

	int len = strlen(str);

	dwRW = 0;
	WriteFile(hLog, str, len, &dwRW, 0);
	if(dwRW != len) return -1;

	dwRW = 0;
	WriteFile(hLog, "\r\n", 2, &dwRW, 0);
	if(dwRW != 2) return -1;

	return S_OK;
}
