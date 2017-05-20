/*
Identifiants pour les types de données:
REG_NONE				=0
REG_SZ					=1
REG_EXPAND_SZ			=2
REG_BINARY				=3
REG_DWORD				=4
REG_DWORD_BIG_ENDIAN	=5
REG_LINK				=6
REG_MULTI_SZ			=7
REG_RESOURCE_LIST		=8
*/

#ifndef REG_HEADER
#define REG_HEADER

#include <windows.h>
#include <cstr.h>

bool Reg_OpenKey(HKEY basekey, Str subkey_name, PHKEY subkey);
bool Reg_CreateKey(HKEY basekey, Str entry_name, PHKEY subkey);
bool Reg_DeleteKey(HKEY basekey, Str entry_name);
bool Reg_SetValue(HKEY subkey, int type_id, Str data, Str entry_name);
bool Reg_SetValue(HKEY basekey, int type_id, int *data, Str entry_name);
Str Reg_GetValue(HKEY subkey, Str entry_name);
Str Reg_GetValueName(HKEY subkey, int index);
bool Reg_DeleteValue(HKEY basekey, Str entry_name);
bool Reg_AddDsn(Str Name, Str Path);
bool Reg_RemoveDsn(Str Name);
char* Reg_GetLastError();

#endif