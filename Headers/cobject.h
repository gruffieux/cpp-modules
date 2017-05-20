#ifndef COBJECT
#define COBJECT

#include <cstr.h>

class Object
{
public:
	struct Data
	{
		int num;
		Str alpha;
		Data() {num = 0;}
	};
	Object();
	Object(Str name);
	Object(Object &model);
	virtual ~Object() {}
	void setName(Str name) {this->name = name;}
	virtual bool compare(Object *pObject) {return false;}
	int getId() {return id;}
	static int getNextId() {return nextId;}
	Data* getSortData() {return &sortData;}
    Data* getIndexData() {return &indexData;}
	Str* getName() {return &name;}
private:
	static int nextId;
    Data indexData;
protected:
	int id;
	Data sortData;
	Str name;
};

#endif