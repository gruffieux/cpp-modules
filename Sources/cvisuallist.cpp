#include <cvisuallist.h>

bool VisualList::AddElement(Object *pObject, bool CheckElement)
{
	Visual *pVisual = dynamic_cast<Visual*>(pObject);

	if (!pVisual) return false;

	if (List::AddElement(pVisual, CheckElement))
	{
		pVisual->setPlanGenerator(this);
		return true;
	}

	return false;
}

int VisualList::OrderBy(int NewOrder, Container *pContainer)
{
	int i, result;

	result = List::OrderBy(NewOrder, pContainer);

	if (result == ORDER_UNKNOWN)
	{
		switch (NewOrder)
		{
		case ORDER_BY_PLAN:
			for (i = 0; i < ElementCount; i++)
				pList[i].Content->getSortData()->num = GetVisualElement(i)->getPlan();
			result = OrderElements(ORDER_NUM);
			if (!result) return result;
			break;
		default:
			return ORDER_UNKNOWN;
		}
	}
	else
		return result;

	FinalizeOrder(NewOrder);

	return result;
}
