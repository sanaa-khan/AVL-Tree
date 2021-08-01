#pragma once

#pragma once

#include <iostream>
using namespace std;

template <class DT>
class AVLTree {

	template <typename DT>
	struct treeNode {
		DT value;
		treeNode* left;
		treeNode* right;
		int height;
	};

	treeNode<DT>* root;

	int max(int a, int b) {
		if (a >= b)
			return a;
		else
			return b;
	}

	int getHeight(treeNode<DT>* node) {

		if (node == NULL)
			return 0;

		else
			return 1 + max(height(node->rightChild), height(node->leftChild));
	}

	void destroySubTree(treeNode<DT>* node) {

		if (node != NULL) {
			destroySubTree(node->left);
			destroySubTree(node->right);

			delete node;

			node = NULL;
		}
	}

	int getBalance(treeNode<DT>* node) {
		if (node == NULL)
			return 0;

		return getHeight(node->left) -	getHeight(node->right);
	}

	treeNode<DT>* minValueNode(treeNode<DT>* node) {
		treeNode<DT>* current = node;

		/* loop down to find the leftmost leaf */
		while (current->left != NULL)
			current = current->left;

		return current;
	}


	void deleteNode(int num, treeNode<DT>*& nodePtr) {
		if (nodePtr == NULL)
			cout << num << " not found.\n";
		else if (num < nodePtr->value)
			deleteNode(num, nodePtr->left);
		else if (num > nodePtr->value)
			deleteNode(num, nodePtr->right);
		else
			makeDeletion(nodePtr);
	}

	void makeDeletion(treeNode<DT>*& nodePtr) {
		treeNode* tempNodePtr;

		if (nodePtr->right == NULL) {
			tempNodePtr = nodePtr;
			nodePtr = nodePtr->left;
			delete tempNodePtr;
		}

		else if (nodePtr->left == NULL) {
			tempNodePtr = nodePtr;
			nodePtr = nodePtr->right;
			delete tempNodePtr;
		}

		else {
			tempNodePtr = nodePtr->right;

			while (tempNodePtr->left)
				tempNodePtr = tempNodePtr->left;

			tempNodePtr->left = nodePtr->left;
			tempNodePtr = nodePtr;
			nodePtr = nodePtr->right;
			delete tempNodePtr;
		}

		nodePtr->height = max(getHeight(nodePtr->left), getHeight(nodePtr->right)) + 1;
		int balance = getBalance(nodePtr);

		if (balance > 1) {

			if (getBalance(nodePtr->left) >= 0)
				nodePtr = SingleRotateWithLeft(nodePtr);

			if (getBalance(nodePtr->left) < 0) {
				nodePtr->left = SingleRotateWithRight(nodePtr->left);
				nodePtr = SingleRotateWithLeft(nodePtr);
			}
		}

		else if (balance < -1) {

			if (getBalance(nodePtr->right) <= 0)
				nodePtr = SingleRotateWithRight(nodePtr);

			if (getBalance(nodePtr->right) > 0) {
				nodePtr->right = SingleRotateWithLeft(nodePtr->right);
				nodePtr = SingleRotateWithRight(nodePtr);
			}
		}
	}

	void displayInOrder(treeNode<DT>* node) {

		if (node != NULL) {
			displayInOrder(node->left);
			cout << node->value << " ";
			displayInOrder(node->right);
		}
	}
	void displayPreOrder(treeNode<DT>* node) {

		if (node != NULL) {
			cout << node->value << " ";
			displayPreOrder(node->left);
			displayPreOrder(node->right);
		}
	}

	void displayPostOrder(treeNode<DT>* node) {

		if (node != NULL) {
			displayPostOrder(node->left);
			displayPostOrder(node->right);
			cout << node->value << " ";
		}
	}

	treeNode<DT>* SingleRotateWithRight(treeNode<DT>* violatedNode) {
		treeNode<DT>* temp = violatedNode->right;
		treeNode<DT>* temp2 = temp->left;
 
		// left rotation

		temp->left = violatedNode;
		violatedNode->right = temp2;

		violatedNode->height = max(getHeight(violatedNode->left), getHeight(violatedNode->right)) + 1;
		temp->height = max(getHeight(temp->left), getHeight(temp->right)) + 1;
		
		return temp;
	}

	treeNode<DT>* SingleRotateWithLeft(treeNode<DT>* violatedNode) {
		treeNode<DT>* temp = violatedNode->left;
		treeNode<DT>* temp2 = temp->right;

		// right rotation  

		temp->right = violatedNode;
		violatedNode->left = temp2;

		// Update heights  
		violatedNode->height = max(getHeight(violatedNode->left), getHeight(violatedNode->right)) + 1;
		temp->height = max(getHeight(temp->left), getHeight(temp->right)) + 1;

		// Return new root  
		return temp;
	}

	treeNode<DT>* DoubleRotateWithRight(treeNode<DT>* violatedNode) {
		violatedNode->right = SingleRotateWithLeft(violatedNode->right);
		return SingleRotateWithRight(violatedNode);
	}

	treeNode<DT>* DoubleRotateWithLeft(treeNode<DT>* violatedNode) {
		violatedNode->rleft = SingleRotateWithRight(violatedNode->left);
		return SingleRotateWithLeft(violatedNode);
	}

public:
	AVLTree() {
		root = NULL;
		root->height = 0;
	}

	~AVLTree() {
		destroySubTree(root);
	}

	AVLTree insert(DT val, AVLTree tree) {
		
		if (tree == NULL) {
			tree = new treeNode;
			tree->value = val;
			tree->left = tree->right = NULL;
		}

		else if (val < tree->value) {

			tree->left = insert(val, tree->left);

			if (getHeight(tree->left) - getHeight(tree->right) == 2) {

				if (val < tree->left->value)
					tree = SingleRotateWithLeft(tree); // RR rotation
				else
					tree = DoubleRotateWithLeft(tree); // RL rotation
			}
		}

		else if (val > tree->value) {

			tree->right = insert(val, tree->right);

			if (getHeight(tree->right) - getHeight(tree->left) == 2) {

				if (val > tree->right->value)
					tree = SingleRotateWithRight(tree); // LL rotation
				else
					tree = DoubleRotateWithRight(tree); // LR rotation
			}
		} 

		tree->height = max(getHeight(tree->left), getHeight(tree->right)) + 1;
		return tree;
	}

	bool find(DT num) {
		treeNode<DT>* node = root;

		while (node) {
			if (node->value == num)
				return true;
			else if (num < node->value)
				node = node->left;
			else
				node = node->right;
		}

		return false;
	}

	void remove(DT num) {
		deleteNode(num, root);
	}

	void showNodesInOrder() {
		displayInOrder(root);
	}

	void showNodesPreOrder() {
		displayPreOrder(root);
	}

	void showNodesPostOrder() {
		displayPostOrder(root);
	}
};