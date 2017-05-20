#include <clist.h>

List::List()
{
	ElementCount = 0;
	order = ORDER_BY_RANK;
	LastToFirst = IsReversed = IsOrdered = false;
	pList = NULL;
}

List::List(int List_Order)
{
	ElementCount = 0;
	order = List_Order;
	LastToFirst = IsReversed = IsOrdered = false;
	pList = NULL;
}

List::List(int size, int List_Order)
{
	ElementCount = size;
	order = List_Order;
	LastToFirst = IsReversed = IsOrdered = false;

	if (ElementCount)
		pList = new Container[ElementCount];
	else
		pList = NULL;
}

List::List(List &model)
{
	int i;

	ElementCount = model.GetElementCount();
	order = model.GetOrder();
	LastToFirst = model.GetLastToFirst();
	IsReversed = IsOrdered = false;

	if (ElementCount)
	{
		pList = new Container[ElementCount];
		for (i = 0; i < ElementCount; i++)
			pList[i].Content = model.GetElement(i);
	}
	else
		pList = NULL;
}

void List::Initialize()
{
	ElementCount = 0;
	if (pList) delete [] pList;
	pList = NULL;
}

void List::Initialize(int size, int List_Order)
{
	ElementCount = size;
	order = List_Order;

	if (pList) delete [] pList;

	if (ElementCount)
		pList = new Container[ElementCount];
	else
		pList = NULL;
}

void List::MapName(Str Object_Name, int MapIndex)
{
	int index = SearchElementByName(Object_Name, true);

	if (index != -1)
		GetElement(index)->getIndexData()->num = MapIndex;
}

bool List::AddElement(Object *pObject, bool CheckElement)
{
	int i;
	Container *TempList;

	if (CheckElement)
		if (SearchElement(pObject) != -1) return false;

	TempList = new Container[ElementCount+1];

	// On inverse directement la TempList si c'est trié par rang
	// Pour tout autre tri, la liste sera inversée après le triage
	if ((LastToFirst == true) && (order == ORDER_BY_RANK))
	{
		for (i = 0; i < ElementCount; i++)
			TempList[i+1].Content = pList[i].Content;
		TempList[0].Content = pObject;
		IsReversed = true;
	}
	else
	{
		for (i = 0; i < ElementCount; i++)
			TempList[i].Content = pList[i].Content;
		TempList[ElementCount].Content = pObject;
		IsReversed = false;
	}

	ElementCount++;
	OrderBy(order, TempList);

	delete [] TempList;

	return true;
}

bool List::AddElement(int index, Object *pObject)
{
	if ((index < 0) | (index >= ElementCount))
		return false;
	
	pList[index].Content = pObject;

	return true;
}

bool List::RemoveElement(Object *pObject, bool DeleteObject)
{
	int i = SearchElement(pObject);

	if (i != -1) return RemoveElement(i, 1, DeleteObject);

	return false;
}

bool List::RemoveElement(int StartIndex, int n, bool DeleteObject)
{
	int i;
	Container *TempList;

	if ((StartIndex < 0) | (StartIndex >= ElementCount))
		return false;

	if (n > ElementCount)
		n = ElementCount - StartIndex;

	TempList = new Container[ElementCount];
	
	for (i = 0; i < ElementCount; i++)
		TempList[i].Content = pList[i].Content;

	for (i = StartIndex; i < StartIndex+n; i++)
		if ((i+n) < ElementCount)
			TempList[i].Content = TempList[i+n].Content;
		else
			if (DeleteObject)
				delete TempList[i].Content;
			else
				TempList[i].Content = NULL;

	for (i; i < ElementCount; i++)
		TempList[i].Content = TempList[i+1].Content;

	ElementCount-=n;

	if (ElementCount)
	{
		if (pList) delete [] pList;
		pList = new Container[ElementCount];
		for (i = 0; i < ElementCount; i++)
			pList[i].Content = TempList[i].Content;
	}
	else
		pList = NULL;

	delete [] TempList;

	return true;
}

int List::SearchElement(Object *pObject, bool compare)
{
	int i;

	if (!pObject) return -1;

	// On cherche l'objet ou l'id correspondant (apparemmment ça trouve toujours par l'id)
	for (i = 0; i < ElementCount; i++)
	{
		if ((pList[i].Content == pObject) | (pList[i].Content->getId() == pObject->getId()))
			return i;
		if ((compare == true) && (pList[i].Content->compare(pObject) == true))
			return i;
	}

	return -1;
}

int List::SearchElement(int Object_id)
{
	int i;

	for (i = 0; i < ElementCount; i++)
		if (pList[i].Content->getId() == Object_id)
			return i;

	return -1;
}

int List::SearchElementByNumIndex(int index)
{
    int i;

    for (i = 0; i < ElementCount; i++)
        if (pList[i].Content->getIndexData()->num == index)
            return i;

    return -1;
}

int List::SearchElementByAlphaIndex(Str Index)
{
    int i;

    for (i = 0; i < ElementCount; i++)
        if (pList[i].Content->getIndexData()->alpha == Index)
            return i;

    return -1;
}

int List::SearchElementByName(Str Name, bool exact)
{
	int i;
	char *SubString;

	for (i = 0; i < ElementCount; i++)
		if (!exact)
		{
			SubString = strstr(pList[i].Content->getName()->Get(), Name.Get());
			if (SubString) return i;
		}
		else
			if (*pList[i].Content->getName() == Name)
                return i;

	return -1;
}

void List::PermuteElement(int index1, int index2)
{
	Object *Temp;

	Temp = pList[index1].Content;
	pList[index1].Content = pList[index2].Content;
	pList[index2].Content = Temp;
}

int List::OrderElements(ORDER_TYPE type)
{
	int i, next, min;
	ORDER_RESULT result = ORDER_OK;

	// On parcours chaque élément de la liste
	for (i = 0; i < ElementCount-1; i++)
	{
		min = i;
		// On parcours chaque élément suivant l'élément en cours
		for (next = i+1; next < ElementCount; next++)
		{
			// Si l'élément suivant est le plus petit on le mémorise
			switch (type)
			{
			case ORDER_NUM:
				if (pList[next].Content->getSortData()->num < pList[min].Content->getSortData()->num)
				{
					min = next;
					result = LIST_CHANGED;
				}
				break;
			case ORDER_ALPHA:
				if (pList[next].Content->getSortData()->alpha < pList[min].Content->getSortData()->alpha)
				{
					min = next;
					result = LIST_CHANGED;
				}
				break;
			default:
				return ORDER_FAILED;
			}
		}
		// On permute l'élément en cours avec l'élément le plus petit
		PermuteElement(i, min);
	}

	return result;
}

int List::OrderBy(int NewOrder, Container *pContainer)
{
	int i, result;

	if (pContainer)
	{
		if (pList) delete [] pList;
		pList = new Container[ElementCount];
		for (i = 0; i < ElementCount; i++)
			pList[i].Content = pContainer[i].Content;
	}

	switch (NewOrder)
	{
	case ORDER_BY_RANK:
		result = ORDER_OK;
		break;
	case ORDER_BY_NAME:
		for (i = 0; i < ElementCount; i++)
			pList[i].Content->getSortData()->alpha = *pList[i].Content->getName();
		result = OrderElements(ORDER_ALPHA);
		if (!result) return result;
		break;
	case ORDER_BY_NUMINDEX:
		for (i = 0; i < ElementCount; i++)
			pList[i].Content->getSortData()->num = pList[i].Content->getIndexData()->num;
		result = OrderElements(ORDER_NUM);
		if (!result) return result;
		break;
	case ORDER_BY_ALPHAINDEX:
		for (i = 0; i < ElementCount; i++)
			pList[i].Content->getSortData()->alpha = pList[i].Content->getIndexData()->alpha;
		result = OrderElements(ORDER_ALPHA);
		if (!result) return result;
		break;
	default:
		return ORDER_UNKNOWN; // Ce type de tri est inconnu dans cette classe
	}

	FinalizeOrder(NewOrder);

	return result;
}

void List::FinalizeOrder(int NewOrder)
{
	order = NewOrder;
	IsOrdered = true;

	if (LastToFirst && !IsReversed)
		ReverseOrder();
}

void List::ReverseOrder()
{
	int i, j;
	Container *TempList;

	TempList = new Container[ElementCount];

	for (i = 0; i < ElementCount; i++)
		TempList[i].Content = pList[i].Content;

	for (i = 0; i < ElementCount; i++)
	{
		j = ElementCount - i - 1;
		pList[i].Content = TempList[j].Content;
	}

	delete [] TempList;
}
