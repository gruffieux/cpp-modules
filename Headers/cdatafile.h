#ifndef CDATAFILE
#define CDATAFILE

#include <clist.h>
#include <cdatavalue.h>
#include <stdio.h>

class DataFile : public Object
{
private:
	int NumIndex;
	fpos_t position;
	Str Index;
	DataValue Data;
	List SubIndex;
public:
	DataFile();
	DataFile(fpos_t DataFile_position, Str DataFile_Index, DataValue DataFile_DataValue);
	DataFile(int DataFile_NumIndex, fpos_t DataFile_position, DataValue DataFile_DataValue);
	DataFile(Str Object_Name, Str DataFile_Index, DataValue DataFile_DataValue);
	DataFile(DataFile &model);
	~DataFile() {SubIndex.RemoveAllElement(0, true);}
	bool compare(Object *pObject);
	int GetNumIndex() {return NumIndex;}
	fpos_t* getPosition() {return &position;}
	Str GetIndex() {return Index;}
	DataValue* GetData() {return &Data;}
	List* GetSubIndex() {return &SubIndex;}
	static DataFile * getDataFileElement(int index, List *pList) {return dynamic_cast<DataFile*>(pList->GetElement(index));}
};

#endif