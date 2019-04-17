#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <assert.h>
template <typename T>
class LinkedList
{

private:
	class Node
	{
	public:
		T element;
		Node *next;
		Node *prev;
		Node(T element) { this->element = element; this->next = nullptr; this->prev; }
		~Node(){}
	};
	int nrOfNodes;
	Node *first = nullptr;
	Node *currentSearchNode = nullptr;
	//Node *last;
public:
	LinkedList();
	~LinkedList();
	void push_front(const T& element);
	T findAt(int index);
	T iterator(bool firstStep);
	//T dequeue();
	T peek() const;
	int size() const;
};

template <typename T>
LinkedList<T>::LinkedList()
{
	this->nrOfNodes = 0;
	this->first = nullptr;
}
template <typename T>
LinkedList<T>::~LinkedList()
{
	Node *toDelete = this->first;
	while (this->first != nullptr)
	{
		this->first = this->first->next;
		delete toDelete;
		toDelete = this->first;
	}
}
template <typename T>
void LinkedList<T>::push_front(const T& element)
{
	Node *e = new Node(element);
	if (this->first != nullptr)
	{
		this->first->prev = e;
		e->next = this->first;
		this->first = e;
	}
	else
	{
		this->first = e;
	}
	this->nrOfNodes++;
}

//template <typename T>
//T LinkedList<T>::dequeue()
//{
//	if (this->first == nullptr)
//	{
//		return NULL;
//	}
//	else
//	{
//		Node *toDelete = this->first;
//		this->first = this->first->next;
//		T result = toDelete->element;
//		delete toDelete;
//		this->nrOfNodes--;
//		return result;
//	}
//}

template <typename T>
T LinkedList<T>::peek() const
{
	return this->first->element;
}

template <typename T>
int LinkedList<T>::size()const
{
	return this->nrOfNodes;
}
template <typename T>
T LinkedList<T>::findAt(int index)
{
	Node* walker = this->first;
	if (index >= this->nrOfNodes)
		return NULL;
	for (int i = 0; i < index; i++)
	{
		this->walker = this->walker->next;
	}

	return walker;
}
template <typename T>
T LinkedList<T>::iterator(bool firstStep)
{
	
	this->currentSearchNode = this->currentSearchNode->next;
	
	if (firstStep)
		this->currentSearchNode = this->first;

	return this->currentSearchNode;
}

#endif