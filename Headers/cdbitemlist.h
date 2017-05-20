#ifndef CDBITEMLIST
#define CDBITEMLIST

class DBItemList : public List
{
private:
protected:
	Str KeyName, TableName;
	SQL *Data;
public:
	static const int ORDER_BY_NO = 10;
	DBItemList() {Data = NULL;}
	DBItemList(int List_size, int List_Order) : List(List_size, List_Order) {Data = NULL;}
	DBItemList(int List_Order) : List(List_Order) {Data = NULL;}
	DBItemList(Str DBItemList_KeyName, Str DBItemList_TableName, SQL *DBItemList_Data, int List_Order);
	DBItemList(DBItemList &model);
	~DBItemList() {}
	bool CreateList();
	int OrderBy(int NewOrder, Container *pContainer);
	int SearchElementByNo(int no);
	Str GetKeyName() {return KeyName;}
	Str GetTableName() {return TableName;}
	SQL * GetData() {return Data;}
	virtual DBItem * GetNewDBItem() {return new DBItem(Data, KeyName, TableName);}
	DBItem * GetDBItemElement(int index) {return dynamic_cast<DBItem*>(GetElement(index));}
};

#endif