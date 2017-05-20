#ifndef CLINKEDLIST
#define CLINKEDLIST

template <class T> class LinkedList
{
	struct LinkedElement
	{
		T data;
		LinkedElement *next, *previous;
		LinkedElement()
		{
			next = previous = 0;
		}
	};

private:
	LinkedElement *first, *last;

public:
	class Iterator
	{
	private:
		LinkedElement *current;
		LinkedList<T> *list;
	public:
		Iterator()
		{
			current = 0;
			list = 0;
		}
		Iterator(LinkedList<T> *list)
		{
			current = 0;
			this->list = list;
		}
		~Iterator()
		{
		}
		T currentData() {return current->data;}
		bool find(T data)
		{
			current = list->getFirst();
			while (current)
			{
				if (current->data == data)
					return true;
				current = current->next;
			}
			return false;
		}
		T first()
		{
			current = list->getFirst();
			if (current)
				return current->data;
			else
				return 0;
		}
		bool hasCurrent() {return current != 0;}
		bool hasNext() {return current != 0 && current->next != 0;}
		bool hasPrevious() {return current != 0 && current->previous != 0;}
		bool insert(T data, T after, bool changeCurrent=false)
		{
			LinkedElement *element;
			element = new LinkedElement;
			element->data = data;
			if (!find(after))
				return false;
			element->previous = current;
			if (current->next)
			{
				element->next = current->next;
				current->next->previous = element;
			}
			current->next = element;
			if (changeCurrent)
				current = element;
			return true;
		}
		T last()
		{
			current = list->getLast();
			if (current)
				return current->data;
			else
				return 0;
		}
		T next()
		{
			current = current->next;
			if (current)
				return current->data;
			else
				return 0;
		}
		T previous()
		{
			current = current->previous;
			if (current)
				return current->data;
			else
				return 0;
		}
		bool remove(T data, bool changeCurrent=false)
		{
			LinkedElement *element;
			if (!find(data))
				return false;
			element = current;
			if (element->previous)
				element->previous->next = current->next;
			if (element->next)
				element->next->previous = current->previous;
			if (changeCurrent)
				current = element->previous;
			else
				current = NULL;
			delete element;
			return true;
		}
		void setList(LinkedList<T> *list) {this->list = list;}
	};

	LinkedList()
	{
		first = last = 0;
	}

	~LinkedList()
	{
	}

	void append(T data)
	{
		LinkedElement *element;
		element = new LinkedElement;
		element->data = data;
		if (!first)
			first = element;
		if (last)
		{
			last->next = element;
			element->previous = last;
		}
		last = element;
	}

	void clear()
	{
		LinkedElement *element, *nextElement;
		element = first;
		while (element)
		{
			nextElement = element->next;
			delete element;
			element = nextElement;
		}
		first = last = 0;
	}

	int elementCount()
	{
		int count = 0;
		LinkedElement *element = first;
		while (element)
		{
			element = element->next;
			count++;
		}
		return count;
	}

	LinkedElement* getFirst() {return first;}
	LinkedElement* getLast() {return last;}

	bool isEmpty() {return elementCount() == 0;}
};

#endif
