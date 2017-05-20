#ifndef CVISUALLIST
#define CVISUALLIST

#include <cvisual.h>

class VisualList : public List
{
private:
public:
	static const int ORDER_BY_PLAN = 10;
	VisualList() : List() {}
	VisualList(int List_Order) : List(List_Order) {}
	VisualList(VisualList &model) : List(model) {}
	~VisualList() {}
	bool AddElement(Object *pObject, bool CheckElement=false);
	int OrderBy(int NewOrder, Container *pContainer=NULL);
	Visual * GetVisualElement(int index) {return dynamic_cast<Visual*>(pList[index].Content);}
};

#endif