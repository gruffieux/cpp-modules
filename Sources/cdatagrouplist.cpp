#include <cdatagrouplist.h>

DataGroupList::DataGroupList()
{
    SelectIndex = -1;
}

DataGroupList::DataGroupList(DataGroupList &model) : List(model)
{
	SelectIndex = -1;
}

DataGroupList::~DataGroupList()
{
    Buffer.RemoveAllElement();
}

DataGroup* DataGroupList::Select(int KeyValue)
{
    int i;

	Buffer.RemoveAllElement();

    for (i = 0; i < ElementCount; i++)
        if (DataGroup::getDataGroupElement(i, this)->GetKey() == KeyValue)
            Buffer.AddElement(DataGroup::getDataGroupElement(i, this));

    return FirstSelect();
}

DataGroup* DataGroupList::Select(Str ValueName, Str Value)
{
    int i;

	Buffer.RemoveAllElement();

    for (i = 0; i < ElementCount; i++)
        if (DataGroup::getDataGroupElement(i, this)->GetValue(ValueName) == Value)
            Buffer.AddElement(DataGroup::getDataGroupElement(i, this));

    return FirstSelect();
}

DataGroup* DataGroupList::FirstSelect()
{
    if (!Buffer.GetElementCount())
        return NULL;

    SelectIndex = 0;

    return DataGroup::getDataGroupElement(SelectIndex, &Buffer);
}

DataGroup* DataGroupList::PreviousSelect()
{
    if (SelectIndex - 1 < 0)
        return NULL;

    SelectIndex--;

    return DataGroup::getDataGroupElement(SelectIndex, &Buffer);
}

DataGroup* DataGroupList::NextSelect()
{
    if (SelectIndex < 0 || SelectIndex + 1 >= Buffer.GetElementCount())
        return NULL;

    SelectIndex++;

    return DataGroup::getDataGroupElement(SelectIndex, &Buffer);
}

DataGroup* DataGroupList::LastSelect()
{
    if (!Buffer.GetElementCount())
        return NULL;

    SelectIndex = Buffer.GetElementCount() - 1;

    return DataGroup::getDataGroupElement(SelectIndex, &Buffer);
}
