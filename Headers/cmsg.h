#ifndef CMSG
#define CMSG

#include <clist.h>

class Msg : public Object
{
protected:
	int color;
public:
	Msg(void);
	Msg(int Msg_color, Str Object_Name);
	int GetColor() {return color;}
	static Msg * getMsgElement(int index, List *pList) {return dynamic_cast<Msg*>(pList->GetElement(index));}
};

#endif