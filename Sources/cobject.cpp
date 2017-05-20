#include <cobject.h>

int Object::nextId = 0;

Object::Object()
{
	id = nextId;

	nextId++;
}

Object::Object(Str name)
{
	id = nextId;
	this->name = name;

	nextId++;
}

Object::Object(Object &model)
{
	id = model.getId();
	sortData = *model.getSortData();
    indexData = *model.getIndexData();
	name = *model.getName();
}
