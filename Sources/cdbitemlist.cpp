#include <cstr.h>
#include <cobject.h>
#include <clist.h>
#include <csql.h>
#include <cdbitem.h>
#include <cdbitemlist.h>

DBItemList::DBItemList(Str DBItemList_KeyName, Str DBItemList_TableName, SQL *DBItemList_Data, int List_Order) : List(List_Order)
{
	KeyName = DBItemList_KeyName;
	TableName = DBItemList_TableName;
	Data = DBItemList_Data;
}

DBItemList::DBItemList(DBItemList &model)
{
	KeyName = model.GetKeyName();
	TableName = model.GetTableName();
	Data = model.GetData();
}

bool DBItemList::CreateList()
{
	SQLRETURN result;
	Str RequestString;
	DBItem *NewDBItem;

	if (!Data) return false;

	RequestString = BuildString("SELECT * FROM %s", TableName.Get());
	result = SQLExecDirect(Data->Gethstmt(0), (SQLCHAR *)RequestString.Get(), SQL_NTS);
	if (result != SQL_SUCCESS)
	{
		Data->SetLastSQLError(result, Data->Gethstmt(0));
		SQLFreeStmt(Data->Gethstmt(0), NULL);
		return false;
	}

	while (SQLFetch(Data->Gethstmt(0)) == SQL_SUCCESS)
	{
		NewDBItem = GetNewDBItem();
		NewDBItem->SetValues(Data->Gethstmt(0));
		AddElement(NewDBItem);
	}

	SQLFreeStmt(Data->Gethstmt(0), NULL);

	return true;
}

int DBItemList::OrderBy(int NewOrder, Container *pContainer)
{
	int i, result;

	result = List::OrderBy(NewOrder, pContainer);

	if (result == ORDER_UNKNOWN)
	{
		switch (NewOrder)
		{
		case ORDER_BY_NO:
			for (i = 0; i < ElementCount; i++)
				pList[i].Content->GetSortData()->num = GetDBItemElement(i)->GetNo();
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

int DBItemList::SearchElementByNo(int no)
{
	int i;

	for (i = 0; i < ElementCount; i++)
		if (GetDBItemElement(i)->GetNo() == no)
			return i;

	return -1;
}
