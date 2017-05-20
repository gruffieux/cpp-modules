#ifndef CKEY
#define CKEY

#include <clist.h>

class Key : public Object
{
private:
	int dik, def;
public:
	Key() {dik = def = 0;}
	Key(int dik);
	Key(int dik, int def, Str name);
	char character(bool upperCase=false);
	int getDik() {return dik;}
	int getDefault() {return def;}
	Str label();
	void setDefault(int Key_def) {def = Key_def;}
	void setDefaultDik() {dik = def;}
	void setDik(int dik) {this->dik = dik;}
	static Key * getKeyElement(int index, List *pList) {return dynamic_cast<Key*>(pList->GetElement(index));}
};

#endif