#ifndef CDATAGROUP
#define CDATAGROUP

#include <clist.h>

class DataGroup : public Object
{
private:
    int key;
    List Values;
public:
    DataGroup();
    DataGroup(int DG_key);
    DataGroup(DataGroup &model);
    ~DataGroup();
	void SetKey(int key) {this->key = key;}
	bool AddValue(Str ValueName, int value);
    bool AddValue(Str ValueName, Str Value, int MaxLength);
	bool AddValue(Str ValueName, int type, Str Value, int MaxLength);
    int GetKey() {return key;}
    Str GetValue(Str ValueName);
    List* GetValues() {return &Values;}
    static DataGroup* getDataGroupElement(int index, List *pList) {return dynamic_cast<DataGroup*>(pList->GetElement(index));}
};

#endif