#ifndef CINTEGER
#define CINTEGER

#include <clist.h>

class Integer : public Object
{
private:
	int value;
public:
	Integer();
	Integer(int Int_value);
	Integer(int Int_value, Str Object_Name);
	Integer(Integer &model);
	~Integer() {}
	bool IsDivisable(int diviser);
	void SetValue(int Int_value) {value = Int_value;}
	int GetValue() const {return value;}
	operator int() {return GetValue();}
	operator const int() {return GetValue();}
	static Integer * getIntegerElement(int index, List *pList) {return dynamic_cast<Integer*>(pList->GetElement(index));}
};

#endif