#pragma once
#include <cstddef>
#include <vector>

// Base struct from which both Internal and Leaf nodes extend from.
struct TreeNode {
public:
	// Keys present in the tree.
	std::vector<float> *keys;
	TreeNode(): keys(NULL) {}
	virtual ~TreeNode();
private:
	void print();
};