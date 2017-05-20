#include <cmsg.h>

Msg::Msg()
{
	color = 0;
}

Msg::Msg(int Msg_color, Str Object_Name) : Object(Object_Name)
{
	color = Msg_color;
}
