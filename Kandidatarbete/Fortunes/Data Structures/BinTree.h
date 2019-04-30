#pragma once
#include <time.h>
#include <limits>
#include <iomanip>
#include <vector>
#include <random>
#include <cassert>

#include "../Math/Parabola.h"
#include "DCEL.h"

class Event; 



template<typename T>
class BinTree
{
public:
	struct Node
	{
		//Height of the tree
		int height;
		//Pointer to the position of the sweepline 
		double* sweepline; 
		//Pointer to a vector of input points.
		std::vector<Point2D>* points; 

		//Indices of the points
		std::pair<int, int> indices; 

		T element;
		Node* left;
		Node* right;
		Node* parent; 

		//Pointer to a circle event for a leaf node, 
		//or halfEdge for an internal node
		std::shared_ptr<Event> circleEvent; 
		std::shared_ptr<HALF_EDGE::HE_Edge*> edge; 

		//Pointer to next and previous arc node:
		Node* next, prev; 

		Node(const std::pair<int,int>& _indices, 
			double* sweepline = nullptr, 
			const std::vector<Point2D>* _points,
			Node* _left, Node* right, Node* parent,
			int height = 1)
		{
		}
		~Node()
		{
		}

		//This works since we define a leaf with 
		//that the first and second elements are equal.
		inline bool isLeaf()
		{
			return indices.first == indices.second; 
		}

		inline bool hasIndices(int a, int b)
		{
			return indices.first == a && indices.second == b; 
		}

		inline hasIndices(const std::pair<int, int> &p)
		{
			return indices.first == p.first &&
				indices.second == p.second; 
		}

		//Return x-coordinate of:
		// - in case leaf node - corresponding focus of parabola
		// - in case of internal node - breakpoint
		double value()
		{
			if (points == nullptr)
			{
				return std::numeric_limits<double>::infinity();
			}
			else if (isLeaf())
			{
				return (*points)[indices.first].x; 
			}
			else
			{
				Point2D p1 = (*points)[indices.first], p2 = (*points)[indices.second]; 
				std::vector<Point2D> ips = findIntersectionPoints(p1, p2, *sweepline); 

				//This means its a breakpoint
				if (ips.size() == 2)
				{
					if (p1.y < p2.y)
					{
						return ips[0].x;
					}
					else
					{
						return ips[1].x;
					}
				}
				//This means its a leaf node
				else
				{
					return ips[0].x;
				}
			}
		}
	};

	//Connect as a list
	void connect(Node* prev, Node* next)
	{
		prev->next = next; 
		next->prev = prev; 
	}

	//Check if node is a root node
	bool isRoot(Node* node)
	{
		return node->parent == nullptr; 
	}

	//Get height of node
	int getHeight(Node* node)
	{
		if (node == nullptr)
			return; 
		return node->height; 
	}

	//update height of node
	void updateHeight(Node* node)
	{
		if (node == nullptr)
			return; 
		node->height = std::max(getHeight(node->left, node->right)) + 1; 
	}

	//Get balance of the node (difference between the heights of left and right subtrees. 
	int GetBalance()
	{
		return GetHeight(node->left) - getHeight(node->right); 
	}

	/*Think these are fixed already*/

	////Performs rotation of a tree around "node" such that it goes to the right subtree´. 
	//Node* rotateRight(Node* node)
	//{

	//}

	////Performs rotation of a tree around "node" such that it goes to the left subtree´. 
	//Node* rotateLeft(Node* node)
	//{

	//}



	/*Find a leaf in a tree such that x is under the parabolic 
	arc, which corresponds to this leaf*/
	Node* find(Node*& root, double x)
	{
		if (root == nullptr) {
			return nullptr;
		}
		Node* node = root;
		while (!node->is_leaf()) {
			if (node->value() < x) {
				node = node->right;
			}
			else {
				node = node->left;
			}
		}
		return node;
	}

	///*Replace a leaf "node" with a new subtree, which has a root,
	//"newNode". The function rebalances the tree and return the pointer to a 
	//new root node.*/

	///*Remove a dissapearing arc*/ //(Use already existing?)
	//Node* remove(Node* leaf)
	//{

	//}

	/*Returns breakpoints for a given arc*/
	std::pair<Node*, Node*> breakpoints(Node* leaf)
	{

	}

	Node* makeSimpleSubTree(int index, int index_behind, double *sweepline,
		const std::vector<Point2D> *points,
		std::vector<HalfEdgePtr> &edges) // Check if neccesary
	{

	}


	bool validate(Node* node)
	{

	}

	bool checkBalance(Node* node)
	{

	}
private:
	Node* m_root; 
public:

	BinTree()
	{
		m_root = nullptr; 
	}
	~BinTree()
	{
		//Travers through tree and delete.
		DeleteRecursively(m_root); 
	}

	void AddNode(Node*& root, T element)
	{
		//Empty tree
		if (root == nullptr)
		{
			root = GetNewNode(element); 
		}
		else if (element <= root->element)
		{
			AddNode(root->left, element);
			
			if(root->left->parent == nullptr)
			root->left->parent = root; 
		}
		else
		{
			AddNode(root->right, element);
			
			if(root->right->parent == nullptr)
			root->right->parent = root; 
		}
		updateHeight(root); 
	}

	bool Search(Node*& root, T element)
	{
		if (root == nullptr)
		{
			return false;
		}
		else if (root->element == element)
		{
			return true;
		}
		else if (element <= root->element)
		{
			return Search(root->left, element);
		}
		else
		{
			return Search(root->right, element);
		}
	}

	Node* FindMin(Node* root)
	{ 
		bool minFound = false; 
		while (!minFound)
		{
			if (root->left != nullptr)
			{
				root = root->left; 
			}
			else
			{
				minFound = true; 
			}
		}
		return root; 
	}

	Node* GetNewNode(T element)
	{
		Node* newNode = new Node();
		newNode->element = element;
		newNode->left = nullptr;
		newNode->right = nullptr;
		return newNode;
	}

	Node*& GetRoot()
	{
		return m_root;
	}

	void Delete(Node*& root, T element)
	{
		if (root == nullptr)
		{
			std::cout << "Tree / Subtree is empty!" << std::endl; 
		}
		else if (element < root->element)
		{
			Delete(root->left, element);
		}
		else if(element > root->element)
		{
			Delete(root->right, element); 
		}
		else
		{
			//Node to delete has been found

			//Case 1: No child node
			if (root->left == nullptr && root->right == nullptr)
			{
				delete root;
				root = nullptr; 
			}
			//Case 2: One child node
			else if (root->left == nullptr)
			{
				Node* temp = root; 
				root = root->right; 
				delete temp; 
			}
			else if (root->right == nullptr)
			{
				Node* temp = root; 
				root = root->left; 
				delete temp; 
			}
			//Case 3: 2 child nodes
			else
			{
				Node* temp = FindMin(root->right); 
				root->element = temp->element; 
				Delete(root->right, temp->element); 
			}
		} 
	}

	//Pre order traversal
	void DrawTree(Node* root)
	{
		if (root != nullptr)
		{
			std::cout << root->element << std::endl;
			DrawTree(root->left);
			DrawTree(root->right);
		}
		else
		{
			std::cout << "nullptr" << std::endl; 
		}
	}

	void DeleteRecursively(Node*& root)
	{
		DeleteRecursively(root->left); 
		DeleteRecursively(root->right); 
		delete root; 
	}
};
