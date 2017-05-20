#include <cdatavalue.h>

DataValue::DataValue() : Object()
{
	type = NONE;
	MaxLength = -1;
}

DataValue::DataValue(int value) : Object(Str::Parse(value))
{
	type = INTEGER;
	MaxLength = -1;
}

DataValue::DataValue(Str Value, int MaxLength) : Object(Value)
{
	type = STRING;
	this->MaxLength = MaxLength;
}

DataValue::DataValue(Str Value, int MaxLength, int type) : Object(Value)
{
	this->type = type;
	this->MaxLength = MaxLength;
}

DataValue::DataValue(DataValue &model) : Object(model)
{
	type = model.GetType();
	MaxLength = model.GetMaxLength();
}

DataValue::~DataValue()
{
}
