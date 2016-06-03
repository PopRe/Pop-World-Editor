#include "shlobj.h"
#include "user_storage.h"


BOOL GetApplicationUserDataStorageDirectory(TCHAR* path)
{
	// RETRIEVE THE PATH TO THE USER'S LOCAL APPDATA FOLDER.
	// The SHGetFolderPath function is deprecated, but is being used to maintain backwards compatibility
	// with older operating systems, such as Windows XP and earlier.
	const HWND NO_WINDOW_HANDLE_OWNER = nullptr;
	const HANDLE NO_ACCESS_TOKEN = nullptr;
	const DWORD NO_ADDITIONAL_OPTIONS = 0;
	if (!SUCCEEDED(SHGetFolderPath(
		NO_WINDOW_HANDLE_OWNER,
		CSIDL_LOCAL_APPDATA,
		NO_ACCESS_TOKEN,
		NO_ADDITIONAL_OPTIONS,
		path)))
	{
		// Return indicating that the path could not be retrieved.
		return FALSE;
	}

	const char* const POPULOUS_EDITOR_USER_DIRECTORY = "\\PopEdt\\";
	const size_t POPULOUS_EDITOR_USER_DIRECTORY_LENGTH = strlen(POPULOUS_EDITOR_USER_DIRECTORY);
	size_t full_path_length = strlen(path) + POPULOUS_EDITOR_USER_DIRECTORY_LENGTH + 1;
	if (full_path_length > MAX_PATH)
	{
		return FALSE;
	}

	TCHAR* path_pos = path + strlen(path);
	strcpy_s(path_pos, strlen(POPULOUS_EDITOR_USER_DIRECTORY) + 1, POPULOUS_EDITOR_USER_DIRECTORY);
	return TRUE;
}

BOOL GetApplicationUserDataFilePath(const TCHAR* file_name, TCHAR* path)
{
	if (!GetApplicationUserDataStorageDirectory(path))
	{
		return FALSE;
	}

	// Add two characters for the delimiting \ and null-terminator.
	size_t full_path_length = strlen(path) + strlen(file_name) + 2;
	if (full_path_length > MAX_PATH)
	{
		return FALSE;
	}

	strcat_s(path, sizeof(TCHAR) * MAX_PATH, file_name);
	return TRUE;
}

BOOL GetUserDocumentsDirectory(TCHAR* path)
{
	// RETRIEVE THE PATH TO THE USER'S LOCAL APPDATA FOLDER.
	// The SHGetFolderPath function is deprecated, but is being used to maintain backwards compatibility
	// with older operating systems, such as Windows XP and earlier.
	const HWND NO_WINDOW_HANDLE_OWNER = nullptr;
	const HANDLE NO_ACCESS_TOKEN = nullptr;
	const DWORD NO_ADDITIONAL_OPTIONS = 0;
	if (!SUCCEEDED(SHGetFolderPath(
		NO_WINDOW_HANDLE_OWNER,
		CSIDL_PERSONAL,
		NO_ACCESS_TOKEN,
		NO_ADDITIONAL_OPTIONS,
		path)))
	{
		// Return indicating that the path could not be retrieved.
		return FALSE;
	}

	return TRUE;
}

BOOL InitializeUserFiles()
{
	TCHAR user_files_path[MAX_PATH];
	if (!GetApplicationUserDataStorageDirectory(user_files_path))
	{
		return FALSE;
	}

	TCHAR folder[MAX_PATH];
	TCHAR *end;
	ZeroMemory(folder, MAX_PATH * sizeof(TCHAR));

	end = strchr(user_files_path, _T('\\'));

	while (end != NULL)
	{
		strncpy(folder, user_files_path, end - user_files_path + 1);
		
		const size_t DRIVE_LETTER_LENGTH = 3;
		bool folder_is_drive_letter = (
			DRIVE_LETTER_LENGTH == strlen(folder) &&
			(0 == strcmp(folder + 1, _T(":\\"))));
		if (folder_is_drive_letter)
		{
			end = strchr(++end, L'\\');
			continue;
		}
		
		if (!CreateDirectory(folder, NULL))
		{
			DWORD err = GetLastError();

			if (err != ERROR_ALREADY_EXISTS)
			{
				return FALSE;
			}
		}
		end = strchr(++end, L'\\');
	}

	return TRUE;
}