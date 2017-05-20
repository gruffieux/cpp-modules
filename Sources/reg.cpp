#include <reg.h>
#include <system.h>
#include <shlwapi.h>

/*
Valeurs possibles pour le paramètre HKEY basekey:
HKEY_CLASSES_ROOT
HKEY_CURRENT_CONFIG
HKEY_CURRENT_USER
HKEY_DYN_DATA
HKEY_LOCAL_MACHINE
HKEY_PERFORMANCE_DATA
HKEY_USERS
*/

Str LastRegError;

bool Reg_OpenKey(HKEY basekey, Str subkey_name, PHKEY subkey)
{
	int long error;
	LPVOID ErrorMessage;

	error = RegOpenKeyEx(basekey, subkey_name, 0, KEY_ALL_ACCESS, subkey);

	if (error != ERROR_SUCCESS)
	{
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, (LPTSTR)&ErrorMessage, 0, NULL);
		LastRegError = BuildString("Impossible d'ouvrir la clé '%s'. %s", subkey_name.Get(), ErrorMessage);
		return false;
	}

	return true;
}

bool Reg_CreateKey(HKEY basekey, Str entry_name, PHKEY subkey)
{
	int long error;
	LPVOID ErrorMessage;

	error = RegCreateKeyEx(basekey, entry_name, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, subkey, NULL);

	if (error != ERROR_SUCCESS)
	{
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, (LPTSTR)&ErrorMessage, 0, NULL);
		LastRegError = BuildString("Impossible de créer la clé '%s'. %s", entry_name.Get(), ErrorMessage);
		return false;
	}

	return true;
}

bool Reg_DeleteKey(HKEY basekey, Str entry_name)
{
	int long error;
	LPVOID ErrorMessage;

	error = SHDeleteKey(basekey, entry_name);

	if (error != ERROR_SUCCESS)
	{
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, (LPTSTR)&ErrorMessage, 0, NULL);
		LastRegError = BuildString("Impossible de supprimer la clé '%s'. %s", entry_name.Get(), ErrorMessage);
		return false;
	}

	return true;
}

bool Reg_SetValue(HKEY subkey, int type_id, Str data, Str entry_name)
{
	int long error;
	LPTSTR str_data;
	LPVOID ErrorMessage;

	str_data = data;
	error = RegSetValueEx(subkey, entry_name, 0, type_id, (LPBYTE)str_data, (DWORD)(lstrlen(data)+1)*sizeof(TCHAR));

	if (error != ERROR_SUCCESS)
	{
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, (LPTSTR)&ErrorMessage, 0, NULL);
		LastRegError = BuildString("Impossible de de définir la valeur '%s'. %s", entry_name.Get(), ErrorMessage);
		return false;
	}

	return true;
}

bool Reg_SetValue(HKEY subkey, int type_id, int *data, Str entry_name)
{
	int long error;
	LPVOID ErrorMessage;

	error = RegSetValueEx(subkey, entry_name, 0, type_id, (LPBYTE)data, sizeof(data));

	if (error != ERROR_SUCCESS)
	{
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, (LPTSTR)&ErrorMessage, 0, NULL);
		LastRegError = BuildString("Impossible de définir la valeur '%s'. %s", entry_name.Get(), ErrorMessage);
		return false;
	}

	return true;
}

Str Reg_GetValue(HKEY subkey, Str entry_name)
{
	int long error;
	char *buffer = new char[255];
	DWORD size, type;
	LPVOID ErrorMessage;

	error = RegQueryValueEx(subkey, entry_name, 0, &type, (LPBYTE)buffer, &size);

	if (error != ERROR_SUCCESS)
	{
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, (LPTSTR)&ErrorMessage, 0, NULL);
		LastRegError = BuildString("Impossible récupérer la valeur '%s'. %s", entry_name.Get(), ErrorMessage);
		return "";
	}

	return Str(buffer);
}

Str Reg_GetValueName(HKEY subkey, int index)
{
	int long error;
	char *buffer = new char[255];
	DWORD size, type;
	LPVOID ErrorMessage;

	error = RegEnumValue(subkey, index, buffer, &size, NULL, &type, NULL, NULL);

	if (error != ERROR_SUCCESS)
	{
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, (LPTSTR)&ErrorMessage, 0, NULL);
		LastRegError = BuildString("Impossible de récupérer la valeur %d. %s", index, ErrorMessage);
		return "";
	}

	return Str(buffer);
}

bool Reg_DeleteValue(HKEY subkey, Str entry_name)
{
	int long error;
	LPVOID ErrorMessage;

	error = RegDeleteValue(subkey, entry_name);

	if (error != ERROR_SUCCESS)
	{
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error, 0, (LPTSTR)&ErrorMessage, 0, NULL);
		LastRegError = BuildString("Impossible de supprimer la valeur '%s'. %s", entry_name.Get(), ErrorMessage);
		return false;
	}

	return true;
}

bool Reg_AddDsn(Str Name, Str Path)
{
	int data;
	Str str_data;
	HKEY subkey, sourcekey;
	Str WinDir;

	// On ouvre la clé correspondant à l'enregistrement de la source
	if (!Reg_OpenKey(HKEY_CURRENT_USER, "SOFTWARE", &subkey)) return false;
	if (!Reg_CreateKey(subkey, "ODBC", &subkey)) return false;
	if (!Reg_CreateKey(subkey, "ODBC.INI", &subkey)) return false;

	// On enregistre la source de données
	if (!Reg_CreateKey(subkey, Name, &sourcekey)) return false;
	if (!Reg_SetValue(sourcekey, REG_SZ, Path, "DBQ")) return false;
	if (!Reg_SetValue(sourcekey, REG_SZ, Name, "Description")) return false;
	if (!Reg_SetValue(sourcekey, REG_SZ, Sys_GetSomeDirectory(SYS_DIR_SYS32) + (Str)"odbcjt32.dll", "Driver")) return false;
	data = 25;
	if (!Reg_SetValue(sourcekey, REG_DWORD, &data, "DriverId")) return false;
	if (!Reg_SetValue(sourcekey, REG_SZ, (Str)"MS Access;", "FIL")) return false;
	data = 0;
	if (!Reg_SetValue(sourcekey, REG_DWORD, &data, "SafeTransactions")) return false;
	if (!Reg_SetValue(sourcekey, REG_SZ, (Str)"", "UID")) return false;

	RegCloseKey(subkey);
	RegCloseKey(sourcekey);

	return true;
}

bool Reg_RemoveDsn(Str Name)
{
	HKEY subkey;

	// On ouvre la clé correspondant à l'enregistrement de la source
	if (!Reg_OpenKey(HKEY_CURRENT_USER, "SOFTWARE", &subkey)) return false;
	if (!Reg_OpenKey(subkey, "ODBC", &subkey)) return false;
	if (!Reg_OpenKey(subkey, "ODBC.INI", &subkey)) return false;

	// On supprime et on ferme
	Reg_DeleteKey(subkey, Name);
	RegCloseKey(subkey);

	return true;
}

char* Reg_GetLastError() {return LastRegError.Get();}
