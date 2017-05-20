#include <cstr.h>
#include <cchar.h>
#include <stdio.h>
#include <stdarg.h>

Str::Str()
{
	pData = new char[1];
	*pData = '\0';
	nLength = 0;
}

Str::Str(char *string)
{
	int NewSize = (int)strlen(string) + 1;

	pData = new char[NewSize];

	strcpy_s(pData, NewSize, string);

	nLength = (int)strlen(string);
}

Str::Str(const Str &model)
{
	char *sz = model.Get();
	int n = model.GetLength();
	int NewSize = n + 1;

	pData = new char[NewSize];

	strcpy_s(pData, NewSize, sz);

	nLength = n;
}

void Str::Cpy(char *string)
{
	int n, NewSize;

	n = (int)strlen(string);
	NewSize = n + 1;

	if (nLength != n)
	{
		if (pData)
			delete [] pData;
		pData = new char[NewSize];
		nLength = n;
	}

	strcpy_s(pData, NewSize, string);
}

void Str::Cpy(char *string, int max)
{
	int NewSize = max + 1;

	if (pData)
		delete [] pData;

	pData = new char[NewSize];
	nLength = max;

	strncpy_s(pData, NewSize, string, nLength);
}

void Str::Cpy(int number)
{
	char buffer[9];

	if (sprintf_s(buffer, 9, "%d", number))
		Cpy(buffer);
}

void Str::Cpy(wchar_t *string)
{
	int n, NewSize;

	n = (int)wcslen(string);
	NewSize = n + 1;

	if (nLength != n)
	{
		if (pData)
			delete [] pData;
		pData = new char[NewSize];
		nLength = n;
	}

	wcstombs_s(NULL, pData, nLength + 1, string, nLength + 1);
}

void Str::Cat(char *string)
{
	int n, NewSize;
	char *pTemp;

	n = (int)strlen(string);

	if (n == 0)
		return;

	NewSize = n + nLength + 1;
	pTemp = new char[NewSize];

	if (pData)
	{
		strcpy_s(pTemp, NewSize, pData);
		delete [] pData;
	}

	strcat_s(pTemp, NewSize, string);

	pData = pTemp;
	nLength += n;
}

void Str::Cat(char character)
{
	char buffer[2];

	if (sprintf_s(buffer, sizeof(buffer), "%c", character))
		Cat(buffer);
}

void Str::Cat(int number)
{
	char buffer[9];

	if (sprintf_s(buffer, sizeof(buffer), "%d", number))
		Cat(buffer);
}

void Str::removeAt(int index)
{
	int i, newSize;
	char *str;

	newSize = nLength - 1;
	str = new char[newSize+1];

	memset(str, 0, index+1);

	for (i = 0; i < index; i++)
		str[i] = pData[i];

	for (i = index+1; i < nLength; i++)
		str[i-1] = pData[i];

	str[i-1] = '\0';

	Cpy(str);

	delete [] str;
}

void Str::Trim()
{
	int i, last;
	Str Token, Trimmed;

	i = last = 0;

	while (!last)
	{
		Token = ExtractString(*this, " ", i, &last);
		if (Token.GetLength())
			Trimmed.Cat(Token);
		i++;
	}

	Cpy(Trimmed.Get());
}

Str Str::GetSubString(int start, int end)
{
	int i, j, length;
	char *buffer;
	Str SubString;

	if (end == -1)
		end = nLength;

	length = end - start + 1;
	buffer = new char[length];
	memset(buffer, 0, length);

	j = 0;

	for (i = start; i < end; i++)
	{
		buffer[j] = pData[i];
		j++;
	}

	SubString.Cpy(buffer);

	delete [] buffer;

	return SubString;
}

/*
Retours:
0 -> La chaîne est strictement identique
1 -> La chaîne est plus faible
2 -> La chaîne est plus forte
*/
short Str::Compare(Str String)
{
	int i, n;
	Char c1, c2;

	i = 0;
	n = nLength;
	if (n > String.GetLength()) n = String.GetLength();

	// On compare du caractère 0 au dernier de la chaîne la plus courte
	for (i = 0; i < n; i++)
	{
		c1 = Char(pData[i]);
		c2 = Char(String.Get()[i]);
		c1.LowerCase();
		c2.LowerCase();
		if (c1.Get() < c2.Get())
			return 1;
		if (c1.Get() > c2.Get())
			return 2;
	}

	// Les chaînes sont identiques jusqu'à n...
	if (nLength == String.GetLength()) return 0; // ...strictement identiques
	if (i == nLength) return 1; // ...et la chaîne comparée est plus courte!
	return 2; // ...et la chaîne comparée est plus longue!
}

Str Str::Parse(int number)
{
	Str Value;

	Value.Cpy(number);

	return Value;
}

Str& Str::operator=(const Str &Source)
{
	Cpy(Source.Get());

	return *this;
}

Str& Str::operator=(const int number)
{
	Cpy(number);

	return *this;
}

Str& Str::operator=(wchar_t *string)
{
	Cpy(string);

	return *this;
}

Str::operator wchar_t*()
{
	wchar_t *wc = new wchar_t[(nLength + 1) * sizeof(wchar_t)];

	mbstowcs_s(NULL, wc, nLength + 1, (const char*)pData, nLength + 1);

	return wc;
}

///////////////////////
// Fonctions friends //
///////////////////////

Str BuildString(char *format, ...)
{
	int length;
	char *buffer;
	va_list args;
	Str NewString;

	va_start(args, format);
	length = _vscprintf(format, args) + 1;
	buffer = new char[length*sizeof(char)];

	vsprintf_s(buffer, length, format, args);

	va_end(args);
	NewString = buffer;
	delete [] buffer;

	return NewString;
}

Str ExtractString(Str Source, char *separator, int index, int *last)
{
	int i = 0;
	char *src, *SubString, *next;
	Str Token, Temp;

	Temp = Source;
	src = Source;
	SubString = strtok_s(src, separator, &next);

	if (SubString)
		if (Str(SubString) == Temp)
		{
			if (last) *last = 1;
			return Str(Temp);
		}

	while (SubString)
	{
		Token = SubString;
		if (i == index)
		{
			Source = Temp;
			return Token;
		}
		SubString = strtok_s(NULL, separator, &next);
		if (last)
			if (!strlen(next)) *last = 1;
		i++;
	}

	if (!SubString) return "";

	return Token;
}

Str ReplaceCharByString(Str Source, char *CharList, char *string)
{
	int i = 0;
	int last = 0;
	Str Token, NewString;

	do
	{
		Token = ExtractString(Source, CharList, i, &last);
		if (Token.GetLength() == Source.GetLength()) return Source;
		if (Token.GetLength())
		{
			NewString = NewString + Token;
			if (!last) NewString = NewString + string;
		}
		i++;
	}
	while (Token.GetLength());

	return NewString;
}

Str ReplaceStringByAnother(Str Source, char *OldOne, char *NewOne, char *reserved)
{
	int n;
	char *SubString, *src, *dest, *rest;
	Str NewString, Temp;

	src = Source;
	Temp = Source;

	// On vérifie que la chaîne de remplacement soit différente
	if (Str(NewOne) == Str(OldOne))
		return Temp;

	// On vérifie que le caractère de remplacement résérvé ne soit pas dans la chaîne
	if (ExtractString(Source, reserved, 0).GetLength() != Temp.GetLength())
		return Temp;

	SubString = strstr(src, OldOne);

	// Tant qu'on trouve le mot, on le remplace par le nouveau
	while (SubString)
	{
		n = Temp.GetLength() - (int)strlen(SubString);
		// On copie la partie de la chaîne initiale se trouvant avant le mot à remplacer dans la nouvelle chaîne
		if (n)
		{
			dest = new char[n+1];
			strncpy_s(dest, n+1, src, n);
			NewString = dest;
			delete [] dest;
		}
		// On concatène la nouvelle chaîne avec le nouveau mot
		NewString = NewString + NewOne;
		// On modifie la chaîne "buffer" en remplaçant la partie déjà traitée par le caractère de remplacement
		rest = (char*)memset(src, *reserved, n+strlen(OldOne));
		// On reconstruit la nouvelle chaîne en enlevant le caractère résérvé, subtile!!
		NewString = NewString + ReplaceCharByString(rest, reserved, "");
		// On rétablit la chaîne initiale et rebelote
		Temp = NewString;
		src = Temp;
		SubString = strstr(src, OldOne);
	}

	// Si le mot n'a pas été trouvé on renvoie la chaîne initiale
	if (!NewString.GetLength())
		return Temp;

	return NewString;
}

Str operator+(Str String1, Str String2)							      
{
	Str NewString(String1);

	NewString.Cat(String2.Get());

	return NewString;
}

Str operator+(Str String, char *string)
{
	Str NewString(String);

	NewString.Cat(string);

	return NewString;
}

Str operator+(char *string, Str String)
{
	Str NewString(string);

	NewString.Cat(String.Get());

	return NewString;
}

Str operator+(Str String, int number)
{
	Str NewString(String);

	NewString.Cat(number);

	return NewString;
}

Str operator+(int number, Str String)
{
	Str NewString;

	NewString.Cpy(number);
	NewString.Cat(String.Get());

	return NewString;
}
