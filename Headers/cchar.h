// Note: cf table ASCII

#ifndef CCHAR
#define CCHAR

class Char
{
private:
	char data;
public:
	Char() {data = 0;}
	Char(char Char_data) {data = Char_data;}
	void UpperCase() {if (IsLowerCase()) data -= 32;}				// Il y a 32 de diff�rence entre le caract�re en minuscule et celui en majuscule
	void LowerCase() {if (IsUpperCase()) data += 32;}
	bool IsUpperCase() {return ((data >= 65) && (data <= 90));}		// Le code ASCII du caract�re doit �tre entre A et Z
	bool IsLowerCase() {return ((data >= 97) && (data <= 122));}	// Le code ASCII du caract�re doit �tre entre a et z
	char Get() {return data;}
};

#endif
