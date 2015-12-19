#include "binaryTreeNode.h"

BinaryTreeNode::BinaryTreeNode(const int id) {
	this->id = id;
	leftChild = nullptr;
	rightChild = nullptr;
}

int BinaryTreeNode::size() const {
	int count = 1;
	if (leftChild) {
	  count += leftChild->size();
	}
	if (rightChild) {
	  count += rightChild->size();
	}
	return count;
}

void BinaryTreeNode::curtailToLevel(const int lvl) {
	if (lvl==0) {
	  leftChild = nullptr;
	  rightChild = nullptr;
	} else {
	  leftChild->curtailToLevel(lvl-1);
	  rightChild->curtailToLevel(lvl-1);
	}
}

void BinaryTreeNode::extendToLevel(const int size) {
	if (size==0) return;
	if (leftChild && rightChild) {
	  leftChild->extendToLevel(size-1);
	  rightChild->extendToLevel(size-1);
	} else if (!leftChild) {
	  leftChild = new BinaryTreeNode(size);
	  this->extendToLevel(size);
	} else if (leftChild && !rightChild) {
	  rightChild = new BinaryTreeNode(size);
	  this->extendToLevel(size);
	}
}

void BinaryTreeNode::addLeftChild(BinaryTreeNode* leftChild) {
	this->leftChild = leftChild;
}

void BinaryTreeNode::addRightChild(BinaryTreeNode* rightChild) {
	this->rightChild = rightChild;
}

void BinaryTreeNode::finalize() {
	printf("A BinaryTreeNode with field %d has been finalized.\n",id);
}

std::unordered_set<gc_obj*> BinaryTreeNode::getManagedChildren() {
	std::unordered_set<gc_obj*> children;
	if (leftChild)
	  children.insert(leftChild);
	if (rightChild)
	  children.insert(rightChild);
	return children;
}

