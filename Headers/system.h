#ifndef SYSTEM_HEADER
#define SYSTEM_HEADER

#include <windows.h>
#include <cstr.h>

enum SYS_DIRECTORY
{
	SYS_DIR_CURRENT,
	SYS_DIR_WINDOWS,
	SYS_DIR_SYS32
};

HANDLE Sys_FindFirstFile(Str Root, Str Search, WIN32_FIND_DATA *pData);
bool Sys_FindClose(HANDLE hFind);
void Sys_CreateDirectory(Str Root, Str Path, int index);
bool Sys_DeleteDirectory(Str Root, Str Folder, bool CheckIfEmpty, bool recursive);
Str Sys_SelectDirectory(HWND hwnd);
Str Sys_PromptFile(HWND hwnd, char *filter, char *DefaultDir, char *title, bool save);
Str Sys_CompletePath(Str Source, bool relative=false);
Str Sys_GetSomeDirectory(SYS_DIRECTORY DirType);
bool Sys_CopyFile(Str SrcFileName, Str DestFileName, bool FailIfExist=false);
char* Sys_GetLastError();

#endif