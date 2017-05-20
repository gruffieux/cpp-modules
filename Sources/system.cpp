#include <system.h>
#include <shlobj.h>

Str LastSysError;

HANDLE Sys_FindFirstFile(Str Root, Str Search, WIN32_FIND_DATA *pData)
{
	int ErrorId;
	LPVOID ErrorMessage;
	HANDLE hFind;

	hFind = FindFirstFile(Root + Search, pData);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		ErrorId = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, ErrorId, 0, (LPTSTR)&ErrorMessage, 0, NULL);
		LastSysError = BuildString("Impossible d'effectuer une recherche dans '%s'. %s", Root.Get(), ErrorMessage);
		return NULL;
	}

	return hFind;
}

bool Sys_FindClose(HANDLE hFind)
{
	int ErrorId;
	LPVOID ErrorMessage;

	if (!FindClose(hFind))
	{
		ErrorId = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, ErrorId, 0, (LPTSTR)&ErrorMessage, 0, NULL);
		LastSysError = BuildString("%s", ErrorMessage);
		return false;
	}

	return true;
}

void Sys_CreateDirectory(Str Root, Str Path, int index)
{
	int LastIndex;
	Str SubFolder;

	LastIndex = 0;
	SubFolder = ExtractString(Path, "\\", index, &LastIndex);
	SubFolder = Sys_CompletePath(SubFolder);

	CreateDirectory(Root + SubFolder, NULL);

	if (!LastIndex)
		Sys_CreateDirectory(Root + SubFolder, Path, index+1);
}

bool Sys_DeleteDirectory(Str Root, Str Folder, bool CheckIfEmpty, bool recursive)
{
	int ErrorId;
	LPVOID ErrorMessage;
	HANDLE hFind;
	WIN32_FIND_DATA FileData;

	// On essaie d'habord de supprimer le dossier d'un coup
	if (!RemoveDirectory(Root + Folder))
	{
		if (!recursive)
		{
			ErrorId = GetLastError();
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, ErrorId, 0, (LPTSTR)&ErrorMessage, 0, NULL);
			LastSysError = BuildString("Impossible de supprimer le dossier '%s'. %s", Folder.Get(), ErrorMessage);
			return false;
		}
	}
	else
		return true;

	hFind = Sys_FindFirstFile(Root + Folder, "*", &FileData);
	if (!hFind) return true;

	// On supprime récursivement dans ce dossier
	do
	{
		if (FileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			// Si on est pas sur ce dossier ou son parent, on va dans le sous-dossier
			if (ExtractString(FileData.cFileName, ".", 0).GetLength())
				Sys_DeleteDirectory(Root + Folder, FileData.cFileName + (Str)"\\", CheckIfEmpty, recursive);
		}
		else
			if (!CheckIfEmpty)
				if (!DeleteFile(Root + Folder + FileData.cFileName))
				{
					ErrorId = GetLastError();
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, ErrorId, 0, (LPTSTR)&ErrorMessage, 0, NULL);
					LastSysError = BuildString("Impossible de supprimer le fichier '%s'. %s", FileData.cFileName, ErrorMessage);
					Sys_FindClose(hFind);
					return false;
				}
	}
	while (FindNextFile(hFind, &FileData));

	Sys_FindClose(hFind);

	// On supprime le dossier
	if (!RemoveDirectory(Root + Folder))
	{
		ErrorId = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, ErrorId, 0, (LPTSTR)&ErrorMessage, 0, NULL);
		LastSysError = BuildString("Impossible de supprimer le dossier '%s'. %s", Folder.Get(), ErrorMessage);
		return false;
	}

	return true;
}

Str Sys_SelectDirectory(HWND hwnd)
{
	char *buffer;
	BROWSEINFO browser;
	ITEMIDLIST *pidl;
	Str Path;

	buffer = new char[255];

	browser.hwndOwner		= hwnd;
	browser.iImage			= NULL;
	browser.lParam			= NULL;
	browser.lpfn			= NULL;
	browser.lpszTitle		= "Sélectionnez le répertoire";
	browser.pidlRoot		= NULL;
	browser.pszDisplayName	= NULL;
	browser.ulFlags			= 0;

	pidl = SHBrowseForFolder(&browser);
	SHGetPathFromIDList(pidl, buffer);
	Path = Str(buffer);

	delete [] buffer;

	return Path;
}

Str Sys_PromptFile(HWND hwnd, char *filter, char *DefaultDir, char *title, bool save)
{
	char buffer[260];
	OPENFILENAME ofn;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = buffer;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(buffer);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = DefaultDir;
	ofn.lpstrTitle = title;
	ofn.Flags = OFN_EXPLORER | OFN_OVERWRITEPROMPT;

	if (save)
		if (GetSaveFileName(&ofn))
			return Str(ofn.lpstrFile);
		else
			return "";

	if (GetOpenFileName(&ofn))
		return Str(ofn.lpstrFile);
	else
		return "";
}

Str Sys_CompletePath(Str Source, bool relative)
{
	int index, LastIndex, counter;
	char *separator, *pch;
	Str SubFolder, NewString;

	if (!Source.GetLength()) return Source;

	index = LastIndex = counter = 0;

	if (relative)
		separator = "/";
	else
		separator = "\\";

	pch = strrchr(Source.Get(),*separator);

	if (pch)
	{
		SubFolder = Str(pch);
		if (SubFolder.GetLength() > 1)
			NewString = Source + separator;
		else
			NewString = Source;
	}
	else
		NewString = Source + separator;

	return NewString;
}

Str Sys_GetSomeDirectory(SYS_DIRECTORY DirType)
{
	char *buffer = new char[255];
	Str Directory;

	switch (DirType)
	{
	case SYS_DIR_CURRENT:
		GetCurrentDirectory(255, buffer);
		break;
	case SYS_DIR_WINDOWS:
		GetWindowsDirectory(buffer, 255);
		break;
	case SYS_DIR_SYS32:
		GetSystemDirectory(buffer, 255);
		break;
	default:
		break;
	}
	
	Directory = Str(buffer);

	delete [] buffer;

	return Sys_CompletePath(Directory);
}

bool Sys_CopyFile(Str SrcFileName, Str DestFileName, bool FailIfExist)
{
	int ErrorId;
	LPVOID ErrorMessage;

	if (!CopyFile(SrcFileName, DestFileName, FailIfExist))
	{
		ErrorId = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, ErrorId, 0, (LPTSTR)&ErrorMessage, 0, NULL);
		LastSysError = BuildString("Le fichier '%s' n'a pas pu être copié. %s", SrcFileName.Get(), ErrorMessage);
		return false;
	}

	return true;
}

char* Sys_GetLastError() {return LastSysError.Get();}
