#ifndef CDBITEM
#define CDBITEM

class DBItem : public Object
{
private:
	Str NoName;
protected:
	int no;
	SQL *Data;
public:
	DBItem();
	DBItem(SQL *DBItem_Data, Str DBItem_NoName, Str Object_Name);
	DBItem(DBItem &model);
	~DBItem() {}
	virtual void SetValues(SQLHSTMT hstmt) {no = GetNumData(1, hstmt);}
	bool DataToElement();
	bool CreateInnerJoin(List *pList, Str TableName, Str NoName, Str NoExName, Str Condition, List *pSrcList=NULL);
	bool Update(Str ValueSet, Str Condition="");
	int GetNumData(int col, SQLHSTMT hstmt);
	Str GetStringData(int col, SQLHSTMT hstmt);
	int GetNo() {return no;}
	Str GetNoName() {return NoName;}
	SQL * GetData() {return Data;}
};

#endif
