#ifndef CSTR
#define CSTR

#include <string.h>
#include <stdlib.h>

class Str
{
private:
	int nLength;
	char *pData;
public:
	Str();
	Str(char *string);
	Str(const Str &model);
	~Str() {delete [] pData;}
	void Cpy(char *string);
	void Cpy(char *string, int max);
	void Cpy(int number);
	void Cpy(wchar_t *string);
	void Cat(char *string);
	void Cat(char character);
	void Cat(int number);
	void removeAt(int index);
	void Trim();
	short Compare(Str String);
	static Str Parse(int number);
	static int ToInt(Str String, int base=10) {return String.Get(base);}
	int GetLength(void) const {return nLength;}
	char * Get(void) const {return pData;}
	int Get(int base) {return strtol(pData, NULL, base);}
	char GetChar(int index) {return pData[index];}
	Str GetSubString(int start, int end=-1);
	Str& operator=(const Str &Source);																// Fontion de l'op�rateur d'assignation
	Str& operator=(const int number);																// Assignation � un entier
	Str& operator=(wchar_t *string);																// Assignation � une cha�ne unicode (wide string)
	operator char*() {return Get();}																// Convertit un objet en type char*
	operator const char*() {return Get();}															// Convertit une constante
	operator wchar_t*();																			// Convertit en type unicode (wide string)
	friend Str BuildString(char *format, ...);														// Construit une cha�ne en lui passant toute la cha�ne format�e avec des param�tres
	friend Str ExtractString(Str Source, char *separator, int index, int *last=NULL);				// Extraie une ou plusieurs sous-cha�nes s�par�es par un caract�re
	friend Str ReplaceCharByString(Str Source, char *CharList, char *string);						// Remplace une s�rie de caract�re par une cha�ne
	friend Str ReplaceStringByAnother(Str Source, char *OldOne, char *NewOne, char *reserved="�");	// Remplace toutes les sous-cha�ne cherch�e dans la cha�ne par une autre
	friend Str operator+(Str String1, Str String2);													// Addition de deux cha�nes
	friend Str operator+(Str String, char *string);													//
	friend Str operator+(char *string, Str String);													//
	friend Str operator+(Str String, int number);													// Addition d'une cha�ne et d'un entier
	friend Str operator+(int number, Str String);													// Addition d'un entier et d'une cha�ne
	friend bool operator==(Str String1, Str String2) {return String1.Compare(String2) == 0;}
	friend bool operator!=(Str String1, Str String2) {return String1.Compare(String2) != 0;}
	friend bool operator<(Str String1, Str String2) {return String1.Compare(String2) == 1;}
	friend bool operator>(Str String1, Str String2) {return String1.Compare(String2) == 2;}
	friend bool operator<=(Str String1, Str String2) {return ((String1.Compare(String2) == 1) | (String1.Compare(String2) == 0));}
	friend bool operator>=(Str String1, Str String2) {return ((String1.Compare(String2) == 2) | (String1.Compare(String2) == 0));}
};

#endif
