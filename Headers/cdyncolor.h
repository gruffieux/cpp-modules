#ifndef CDYNCOLOR
#define CDYNCOLOR

#include <clist.h>

class DynColor : public Object
{
private:
	int OldColor, NewColor;
public:
	DynColor() {OldColor = NewColor = 0;}
	DynColor(int DC_OldColor, int DC_NewColor);
	~DynColor() {}
	bool compare(Object *pObject);
	int GetOldColor() {return OldColor;}
	int GetNewColor() {return NewColor;}
	static DynColor * getDynColorElement(int index, List *pList) {return dynamic_cast<DynColor*>(pList->GetElement(index));}
};

#endif
