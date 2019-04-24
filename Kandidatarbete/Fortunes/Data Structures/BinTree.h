#pragma once
template<typename T>
class BinTree
{
public:
	struct Node
	{
		T element;
		Node* left;
		Node* right;

		Node()
		{
		}
		~Node()
		{

		}
	};
private:
	int m_nrOfNodes;
	int m_nrOfLevels;
	Node* m_root = nullptr;
	bool m_nodeInserted;
public:

	BinTree()
	{
		m_nrOfNodes = 0;
		m_nrOfLevels = 0;
		m_nodeInserted = false;
	}
	~BinTree()
	{
		//Travers through tree and delete.
		delete m_root; 
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
		}
		else
		{
			AddNode(root->right, element);
		}
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

	void DrawTree()
	{

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
};
