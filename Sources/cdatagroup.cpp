#include <cdatagroup.h>
#include <cdatavalue.h>

DataGroup::DataGroup()
{
    key = 0;
}

DataGroup::DataGroup(int DG_key) : Object()
{
    key = DG_key;
}

DataGroup::DataGroup(DataGroup &model) : Object(model)
{
    key = model.GetKey();
    Values = *model.GetValues();
}

DataGroup::~DataGroup()
{
    Values.RemoveAllElement(0, true);
}

bool DataGroup::AddValue(Str ValueName, int value)
{
	DataValue *Element;

	Element = new DataValue(value);
	Element->getIndexData()->alpha = ValueName;

	return Values.AddElement(Element);
}

bool DataGroup::AddValue(Str ValueName, Str Value, int MaxLength)
{
	DataValue *Element;

	Element = new DataValue(Value, MaxLength);
	Element->getIndexData()->alpha = ValueName;

	return Values.AddElement(Element);
}

bool DataGroup::AddValue(Str ValueName, int type, Str Value, int MaxLength)
{
	DataValue *Element;

	Element = new DataValue(Value, MaxLength, type);
	Element->getIndexData()->alpha = ValueName;

	return Values.AddElement(Element);
}

Str DataGroup::GetValue(Str ValueName)
{
    int index;

    index = Values.SearchElementByAlphaIndex(ValueName);

    if (index != -1)
        return *Values.GetElement(index)->getName();

    return "";
}
