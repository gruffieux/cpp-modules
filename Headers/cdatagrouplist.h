#ifndef CDATAGROUPLIST
#define CDATAGROUPLIST

#include <cdatagroup.h>

class DataGroupList : public List
{
private:
    int SelectIndex;
	Str Index;
    List Buffer;
public:
    DataGroupList();
	DataGroupList(DataGroupList &model);
     ~DataGroupList();
	 void SetIndex(Str Index) {this->Index = Index;}
     DataGroup* Select(int KeyValue);
     DataGroup* Select(Str ValueName, Str Value);
     DataGroup* Select(Str ValueName, int value) {return Select(ValueName, Str::Parse(value));}
     DataGroup* FirstSelect();
     DataGroup* PreviousSelect();
     DataGroup* NextSelect();
     DataGroup* LastSelect();
	 Str GetIndex() {return Index;}
};

#endif
