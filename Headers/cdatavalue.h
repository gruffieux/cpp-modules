#ifndef CDATAVALUE
#define CDATAVALUE

#include <clist.h>

class DataValue : public Object
{
private:
	int type, MaxLength;
public:
	enum DATA_TYPE
	{
		NONE,
		INTEGER,
		STRING
	};
	DataValue();
	DataValue(int value);
	DataValue(Str Value, int MaxLength);
	DataValue(Str Value, int MaxLength, int type);
	DataValue(DataValue &model);
	~DataValue();
	void SetValue(int value) {setName(Str::Parse(value));}
	void SetValue(Str Value) {setName(Value);}
	int GetIntegerValue() {return Str::ToInt(name);}
	Str GetStringValue() {return name;}
	int GetType() {return type;}
	int GetMaxLength() {return MaxLength;}
	static DataValue* getDataValueElement(int index, List *pList) {return dynamic_cast<DataValue*>(pList->GetElement(index));}
};

#endif
