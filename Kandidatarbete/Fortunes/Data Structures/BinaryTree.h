#pragma once
#include "../Event/Event.h"
class BinaryTree
{
private:
	class Node
	{
	public:
		Event* currEvent;
		Node *left;
		Node *right;
		Node(Event* newEvent) {
			currEvent = newEvent; this->left = nullptr; this->right = nullptr;
		}
		~Node() {}
	};
public:
	BinaryTree();
	~BinaryTree();
};

