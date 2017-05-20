#ifndef CLIST
#define CLIST

#include <cobject.h>

class List
{
public:
	static const int ORDER_BY_RANK = 0;
	static const int ORDER_BY_NAME = 1;
	static const int ORDER_BY_NUMINDEX = 2;
	static const int ORDER_BY_ALPHAINDEX = 3;
	enum ORDER_TYPE
	{
		ORDER_NUM,
		ORDER_ALPHA
	};
	enum ORDER_RESULT
	{
		ORDER_FAILED,
		ORDER_OK,
		ORDER_UNKNOWN,
		LIST_CHANGED
	};
	struct Container
	{
		Object *Content;
		Container() {Content = NULL;}
	};
	List();
	List(int List_Order);
	List(int size, int List_Order);
	List(List &model);
	virtual ~List() {if (pList) delete [] pList;}
	void SetOrder(int List_order) {order = List_order;}
	void SetLastToFirst(bool List_LastToFirst) {LastToFirst = List_LastToFirst;}
	void SetElement(int index, Object *pObject) {pList[index].Content = pObject;}
	void OrderMe() {IsOrdered = false;}
	void Initialize();
	void Initialize(int size, int List_Order=ORDER_BY_RANK);
	void MapName(Str Object_Name, int MapIndex);
	virtual bool AddElement(Object *pObject, bool CheckElement=false);
	bool AddElement(int index, Object *pObject);
	bool RemoveElement(Object *pObject, bool DeleteObject=false);
	bool RemoveElement(int StartIndex, int n, bool DeleteObject=false);
	bool RemoveAllElement(int StartIndex=0, bool DeleteObject=false) {return RemoveElement(StartIndex, ElementCount - StartIndex, DeleteObject);}
	int SearchElement(Object *pObject, bool compare=false);
	int SearchElement(int Object_id);
	int SearchElementByNumIndex(int index);
    int SearchElementByAlphaIndex(Str Index);
	int SearchElementByName(Str Name, bool exact);
	void PermuteElement(int index1, int index2);
	int OrderElements(ORDER_TYPE type);
	virtual int OrderBy(int NewOrder, Container *pContainer=NULL);
	void ReverseOrder();
	bool ElementExist(int index) {return ElementCount > index && index >= 0 && pList[index].Content != NULL;}
	int GetElementCount() {return ElementCount;}
	int GetOrder() {return order;}
	bool GetLastToFirst() {return LastToFirst;}
	bool GetIsOrdered() {return IsOrdered;}
	Object * GetElement(int index) {return pList[index].Content;}
private:
	bool LastToFirst, IsReversed;
protected:
	int ElementCount, order;
	bool IsOrdered;
	Container *pList;
	void FinalizeOrder(int NewOrder);
};

#endif
