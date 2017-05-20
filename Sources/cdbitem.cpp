#include <cstr.h>
#include <cobject.h>
#include <clist.h>
#include <csql.h>
#include <cdbitem.h>
#include <cdbitemlist.h>

DBItem::DBItem()
{
	no = 0;
	Data = NULL;
}

DBItem::DBItem(SQL *DBItem_Data, Str DBItem_NoName, Str Object_Name) : Object(Object_Name)
{
	no = 0;
	NoName = DBItem_NoName;
	Data = DBItem_Data;
}

DBItem::DBItem(DBItem &model)
{
	no = model.GetNo();
	NoName = model.GetNoName();
	Data = model.GetData();
}

bool DBItem::DataToElement()
{
	SQLRETURN result;
	Str RequestString;

	if (!Data) return false;

	RequestString = BuildString("SELECT * FROM %s WHERE %s = %d", Name.Get(), NoName.Get(), no);
	result = SQLExecDirect(Data->Gethstmt(0), (SQLCHAR *)RequestString.Get(), SQL_NTS);
	if (result != SQL_SUCCESS)
	{
		Data->SetLastSQLError(result, Data->Gethstmt(0));
		SQLFreeStmt(Data->Gethstmt(0), NULL);
		return false;
	}

	result = SQLFetch(Data->Gethstmt(0));
	if (result != SQL_SUCCESS)
	{
		Data->SetLastSQLError(result, Data->Gethstmt(0));
		SQLFreeStmt(Data->Gethstmt(0), NULL);
		return false;
	}
	
	SetValues(Data->Gethstmt(0));
	SQLFreeStmt(Data->Gethstmt(0), NULL);

	return true;
}

bool DBItem::CreateInnerJoin(List *pList, Str TableName, Str NoName, Str NoExName, Str Condition, List *pSrcList)
{
	int i;
	SQLRETURN result;
	Str RequestString;
	DBItem *NewDBItem;
	DBItemList *pDBItemList = dynamic_cast<DBItemList*>(pList);
	DBItemList *pDBItemSrcList = dynamic_cast<DBItemList*>(pSrcList);

	if (!Data) return false;
	if (!pDBItemList) return false;

	RequestString = BuildString("SELECT * FROM %s INNER JOIN %s ON %s = %s WHERE %s", pDBItemList->GetTableName().Get(), TableName.Get(), NoName.Get(), NoExName.Get(), Condition.Get());
	result = SQLExecDirect(Data->Gethstmt(0), (SQLCHAR *)RequestString.Get(), SQL_NTS);
	if (result != SQL_SUCCESS)
	{
		Data->SetLastSQLError(result, Data->Gethstmt(0));
		SQLFreeStmt(Data->Gethstmt(0), NULL);
		return false;
	}

	while (SQLFetch(Data->Gethstmt(0)) == SQL_SUCCESS)
	{
		NewDBItem = pDBItemList->GetNewDBItem();
		NewDBItem->SetValues(Data->Gethstmt(0));
		if (pDBItemSrcList)
		{
			i = pDBItemSrcList->SearchElementByNo(NewDBItem->GetNo());
			if (i == -1) return false;
			delete NewDBItem;
			NewDBItem = pDBItemSrcList->GetDBItemElement(i);
		}
		pDBItemList->AddElement(NewDBItem);
	}

	SQLFreeStmt(Data->Gethstmt(0), NULL);

	return true;
}

bool DBItem::Update(Str ValueSet, Str Condition)
{
	SQLRETURN result;
	Str RequestString;

	if (!Data) return false;
	
	if (!Condition.GetLength()) Condition = BuildString("%s = %d", NoName.Get(), no);
	RequestString = BuildString("UPDATE %s SET %s WHERE %s", Name.Get(), ValueSet.Get(), Condition.Get());
	result = SQLExecDirect(Data->Gethstmt(0), (SQLCHAR *)RequestString.Get(), SQL_NTS);
	if (result != SQL_SUCCESS)
	{
		Data->SetLastSQLError(result, Data->Gethstmt(0));
		SQLFreeStmt(Data->Gethstmt(0), NULL);
		return false;
	}
	
	SQLFreeStmt(Data->Gethstmt(0), NULL);
	
	return true;
}

int DBItem::GetNumData(int col, SQLHSTMT hstmt)
{
	int value;
	SQLRETURN result;

	result = SQLGetData(hstmt, col, SQL_C_SLONG, &value, 0, NULL);

	if (result != SQL_SUCCESS)
	{
		if (Data) Data->SetLastSQLError(result, hstmt);
		SQLFreeStmt(hstmt, NULL);
	}

	return value;
}

Str DBItem::GetStringData(int col, SQLHSTMT hstmt)
{
	char *buffer = new char[255];
	SQLRETURN result;
	Str Value;

	result = SQLGetData(hstmt, col, SQL_C_CHAR, buffer, 255, NULL);

	if (result != SQL_SUCCESS)
	{
		if (Data) Data->SetLastSQLError(result, hstmt);
		SQLFreeStmt(hstmt, NULL);
	}

	Value = Str(buffer);
	delete [] buffer;

	return Value;
}
