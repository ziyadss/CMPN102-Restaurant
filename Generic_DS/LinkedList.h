#pragma once
#include "Node.h"

template <typename T>
class LinkedList
{
private:
	Node<T>* frontPtr;
	int count;

public:


	LinkedList()
	{
		frontPtr = nullptr;
		count = 0;
	}


	~LinkedList()
	{

	}

	bool isEmpty() const
	{
		return !count;
	}

	void enqueue(const T& data)
	{
		Node<T>* p = frontPtr;

		if (frontPtr)
		{
			while (p->getNext())
			{
				p = p->getNext();
			}
			Node<T>* R = new Node<T>(data);
			p->setNext(R);
		}
		else
		{
			Node<T>* R = new Node<T>(data);
			frontPtr = R;
		}
		count++;
	}


	bool dequeue(T& itm)
	{
		if (isEmpty())
			return false;

		Node<T>* p = frontPtr;
		itm = frontPtr->getItem();
		frontPtr = frontPtr->getNext();

		delete p;

		count--;
		return true;

	}


	bool remove(const int key, T& itm)
	{
		Node<T>* p = frontPtr;
		Node<T>* m;
		if (p && p->getItem()->getID() == key)
		{
			return dequeue(itm);
		}

		while (p)
		{

			if (p->getNext() && p->getNext()->getItem()->getID() == key)
			{
				m = p->getNext();
				p->setNext(p->getNext()->getNext());
				itm = m->getItem();
				delete m;
				count--;
				return true;
			}
			else
				p = p->getNext();
		}
		return false;
	}

	int getCount()
	{
		return count;
	}

	bool peekFront(T& frntEntry) const
	{
		if (isEmpty())
			return false;

		frntEntry = frontPtr->getItem();
		return true;

	}

	T* toArray()
	{
		if (!frontPtr)
			return nullptr;

		T* Arr = new T[count];
		Node<T>* p = frontPtr;
		for (int i = 0; i < count; i++)
		{
			Arr[i] = p->getItem();
			p = p->getNext();
		}
		return Arr;
	}
};