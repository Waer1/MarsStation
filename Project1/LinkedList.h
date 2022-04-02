#pragma once

#include <iostream>
#include "Node.h"
using namespace std;


template <typename T>
class LinkedList
{
private:
	Node<T>* Head;

public:

	LinkedList()
	{
		Head = nullptr;
	}

	~LinkedList()
	{
		DeleteAll();
	}

	Node<T>* getHead()
	{
		return Head;
	}

	void DeleteLast() {
		Node<T>* temp = Head;
		Node<T>* last;
		while ((temp->getNext())->getNext()) {
			temp = temp->getNext();
		}

		last = temp->getNext();
		temp->setNext(nullptr);
		delete last;
	};


	void ReadList()
	{
		int val;
		cin >> val;
		while (val != -1)
		{
			this->InsertEnd(val);
			cin >> val;
		}
	}

	void InsertEnd(const T& data)
	{
		Node<T>* R = new Node<T>(data);
		if (!Head)
		{
			Head = R;
			return;
		}
		Node<T>* p = Head;
		while (p->getNext())
			p = p->getNext();
		p->setNext(R);
	}

	void DeleteAll()
	{
		Node<T>* P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
	}

	int count() {
		Node<T>* cur = Head;
		int count{ 0 };
		while (cur)
		{
			count++;
			cur = cur->getNext();
		}
		return count;
	}

	void InsertBeg(T  data)
	{

		Node<T>* R = new Node<T>(data);
		R->setNext(Head);
		Head = R;

	}

	void PrintList() const
	{
		Node<T>* p = Head;
		while (p)
		{
			cout << "{ "; p->getItem()->print(); cout << " ,";
			p = p->getNext();
		}
		cout << "}" << endl;;
	}

	void DeleteFirst() {
		Node<T>* temp = Head;
		Head = Head->getNext();
		delete temp;
	};



}; 
