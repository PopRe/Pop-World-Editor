#pragma once
#include <tchar.h>

BOOL GetApplicationUserDataStorageDirectory(TCHAR* path);
BOOL GetApplicationUserDataFilePath(const TCHAR* file_name, TCHAR* path);
BOOL GetUserDocumentsDirectory(TCHAR* path);
BOOL InitializeUserFiles();