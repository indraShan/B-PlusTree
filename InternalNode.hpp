#pragma once
#include <cstddef>
#include <vector>
#include "TreeNode.hpp"

struct InternalNode: TreeNode {
public:
	// Number of keys+1
	// 0 is left child of key at index 0. Similarly 1 is the right child.
	std::vector<TreeNode*> *children;
	InternalNode(): children(NULL) {}
	
	// Inserts a given key at its correct position.
	void insert(float key, TreeNode *left, TreeNode *right);
	std::pair<InternalNode *, float> split(); 
	void print();
	~InternalNode();
	TreeNode* childrenNodeToFind(float key, int start, int end);
};