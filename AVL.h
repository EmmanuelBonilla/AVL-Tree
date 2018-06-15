/**
* @ author: Emmanuel Bonilla (emmanuel.bonilla@wsu.edu)
* @ description: This program is designed to create an AVL tree
* 
* compiling instructions: g++ -std=c++11 -Wall AVL.cpp -o AVL
*/

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>

template <class T>
class Node
{
public:
	T data;
	int height;
	Node * left;
	Node * right;
	Node * parent;

	/**
	* This method is designed to calculate the height of the node from the node given up the tree
	* @param[in] none
	*/
	void calcHeight()
	{
		int leftHeight = (left != NULL) ? left->height : 0;
		int rightHeight = (right != NULL) ? right->height : 0;
		height = (std::max(leftHeight, rightHeight)) + 1;
	}

	/**
	* This method is designed to give the balance factor of the node
	* recommend to calculate the height before calling this method
	* @param[in] nothing
	* @return int
	*/
	int balFac()
	{
		Node *temp = this;
		int leftHeight = (temp->left != NULL) ? temp->left->height : 0;
		int rightHeight = (temp->right != NULL) ? temp->right->height : 0;
		int bFac = rightHeight - leftHeight;
		return bFac;
	}

	/**
	* This method is designed to print out values and height for the AVL tree
	* @return nothing
	*/
	void print()
	{
		std::cout << data << " (height: " << height << ")";
		if (left)
		{
			std::cout << " L ";
			left->print();
		}
		if (right)
		{
			std::cout << " R ";
			right->print();
		}
		std::cout << "U ";
	}

	Node(T aData)
		: data(aData),
		height(1),
		left(NULL),
		right(NULL),
		parent(NULL)
	{}
	
	~Node()
	{
		delete left;
		delete right;
	}
};


template <class T>
class AvlTree
{
private:
	Node<T> * root;

	/**
	* This method is designed to search the tree based on the item passed in the argument
	* @return node pointer to the location of the item
	*/
	Node <T> *exists(T item)
	{
		Node<T> *temp = root;

		while (temp != NULL)
		{
			if (temp->data == item)
				break;
			if (item > temp->data)
				temp = temp->right;
			else
				temp = temp->left;
		}
		return temp;
	}

public:
	/**
	* This method is designed to rotate left from the given node
	* @param[in] node pointer
	*/
	void rotateLeft(Node <T> *node)
	{
		Node <T> *temp = node->right;
		Node <T> * parent = node->parent;

		node->right = temp->left;
		temp->left = node;
		temp->parent = node->parent;
		node->parent = temp;
		if (node->right != NULL)
			node->right->parent = node;

		if (parent == NULL)
			root = temp;
		else
		{
			if (parent->left == node)
				parent->left = temp;
			else 
				parent->right = temp;			
		}
	}
	
	/**
	* This method is designed to rotate right from the given node
	* @param[in] node pointer
	*/
	void rotateRight (Node <T> *node)
	{
		Node <T> *temp = node->left;
		Node <T> *parent = node->parent; 

		node->left = temp->right;
		temp->right = node;
		temp->parent = node->parent;
		node->parent = temp;
		if (node->left != NULL)
			node->left->parent = node;
		
		if (parent == NULL)
			root = temp;
		else
		{
			if (parent->right == node)
				parent->right = temp;
			else
				parent->left = temp;
		}
	}
	
	/**
	* This method is designed to do a left right rotation
	* @param[in] node pointer
	*/
	void insideRight (Node <T> *node)
	{
		rotateRight(node->right);
		rotateLeft(node);
	}

	/**
	* This method is designed to do a right left rotaion
	* @param[in] node pointer
	*/
	void insideLeft (Node <T> *node)
	{
		rotateLeft(node->left);
		rotateRight(node);
	}

	/**
	* This method is designed to balance the tree base off the balance factor (-1 > 0 < 1)
	* @param[in] node pointer
	*/
	void balanceTree(Node <T> *node)
	{
		bool leftsubtree = false;
		bool leftchild = false;

		while (node != NULL)
		{
			Node <T> *parent = node->parent;
			int bal = node->balFac();

			if (bal < -1 )
			{
				if (leftsubtree)
				{
					if (leftchild)
						rotateRight(node);
					else
						insideLeft(node);
				}
				else
				{
					if (leftchild)
						insideLeft(node);
					else
						rotateLeft(node);
				}

				parent = node->parent;
				parent->left->calcHeight();
				parent->right->calcHeight();
				parent->calcHeight();
				break;
			}

			if (bal > 1)
			{
				if (leftsubtree)
				{
					if (leftchild)
						rotateLeft(node);
					else
						insideRight(node);
				}
				else
				{
					if (leftchild)
						insideLeft(node);
					else
						rotateLeft(node);
				}

				parent = node->parent;
				parent->left->calcHeight();
				parent->right->calcHeight();
				parent->calcHeight();
				break;
			}

			//else
			{
				node->calcHeight();
				if (parent == NULL)
					break;
				leftsubtree = leftchild;
				leftchild = parent->left == node;
			}

			node = parent;
		}
	}
	
	/**
	* This method is designed to insert a node to the tree
	* @return nothing
	*/
	void insert (T data)
	{
		Node<T> *node = new Node<T>(data);

		if (root == NULL)
		{
			root = node;
		}
		else
		{
			Node<T> * temp = root;

			while (true)
			{
				if (data == temp->data)
				{
					std::cout << "Tree does not accept items of the same value " << data <<std::endl;
					break;
				}

				if (data > temp->data) // insert right 
				{
					if (temp->right == NULL)
					{
						node->parent = temp;
						temp->right = node;
						break;
					}
					temp = temp->right;
				}
				else
				{
					if (temp->left == NULL)
					{
						node->parent = temp;
						temp->left = node;
						break;
					}
					temp = temp->left;
				}
			}

			balanceTree(node);
		}
	}

	/**
	* This method is designed to remove the node based on the value of the data
	* it will use the search method to locate the data if its avaliable 
	* it will balance after remove
	* @return nothing
	*/		
	void remove(T value)
	{
		Node <T> * temp = exists(value);

		if (temp == NULL)
			return;

		if (temp->left == NULL && temp->right == NULL)
		{
			if (temp == root)
				root = NULL;
			else
			{
				if (temp->parent->left == temp)
					temp->parent->left = NULL;
				else
				{
					temp->parent->right = NULL;
				}

				temp->parent->calcHeight();
				balanceTree(temp->parent);
			}
		}

		else if (temp->right == NULL)
		{
			if (temp == root)
				root = temp->left;
			else
			{
				if (temp->parent->left == temp)
					temp->parent->left = temp->left;
				else
					temp->parent->right = temp->left;
			}
			temp->left->parent = temp->parent;
			balanceTree(temp->left);
		}

		else if (temp->left == NULL)
		{
			if (temp == root)
				root = temp->right;
			else
			{
				if (temp->parent->left == temp)
					temp->parent->left = temp->right;
				else
					temp->parent->right = temp->right;
			}
			temp->right->parent = temp->parent;
			balanceTree(temp->right);
		}

		else
		{
			Node <T> *temp2 = temp->right;

			while (temp2->left != NULL)
			{
				temp2 = temp2->left;
			}
			
			Node <T> *balNode = temp2->parent;
			if (temp->right != temp2)
			{  //removing left most child of the right child
				temp2->parent->left = temp2->right;
				if (temp2->right != NULL)
					temp2->right->parent = temp2->parent;
				temp2->parent->calcHeight();

				//start of moving temp 2 at the top
				temp2->right = temp->right;
				temp2->right->parent = temp2;
			}
			
			temp2->parent = temp->parent; //1
			temp2->left = temp->left;//2
			temp2->left->parent = temp2;//3
			
			if (temp == root)
				root = temp2;			
			else
			{
				if (temp->parent->left == temp)
					temp->parent->left = temp2;
				else
					temp->parent->right = temp2;
			}
			balNode->calcHeight();
			balanceTree(balNode);
		}
		temp->right = NULL;
		temp->left = NULL;
		delete temp;
	}

	/**
	* This method is designed to read in a file and add to AVL tree
	* you must change the type to match the contents in the file
	* file must be in the same directory as avl.cpp and avl.h
	* @param[in] file name
	*/
	void readFile(std::string fileName)
	{
		std::ifstream userFile (fileName);
		if (userFile.is_open())
		{
			while (userFile.good())
			{
				T data;
				if (userFile >> data)
				{
					insert(data);
				}				
			}
		}
		else
		{
			std::cout << "Unable to open file" << std::endl;
		}
		userFile.close();
	}

	AvlTree ()
		: root(NULL)
	{}

	~AvlTree ()
	{
		delete root;
	}
	
	T search (T val)
	{
		Node<T> *temp = root;

		while (temp != NULL)
		{
			if (temp->data == item)
				break;
			if (item > temp->data)
				temp = temp->right;
			else
				temp = temp->left;
		}
		return temp->data;
	}

	void print()
	{
		root->print();
		std::cout << std::endl;
	}
};