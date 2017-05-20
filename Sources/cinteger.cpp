#include <cinteger.h>

Integer::Integer()
{
	value = 0;
}

Integer::Integer(int Int_value)
{
	value = Int_value;
}

Integer::Integer(int Int_value, Str Object_Name) : Object(Object_Name)
{
	value = Int_value;
}

Integer::Integer(Integer &model)
{
	value = model.GetValue();
}

bool Integer::IsDivisable(int diviser)
{
	int n = value;

	if (value == 0)
		return false;

	if (value > 0)
	{
		while (n > 0)
			n -= diviser;
	}
	else
		while (n < 0)
			n += diviser;

	return n == 0;
}
