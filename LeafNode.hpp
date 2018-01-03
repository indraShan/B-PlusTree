#pragma once
#include <cstddef>
#include <string>
#include <vector>
#include "TreeNode.hpp"

struct LeafNode: TreeNode {
public:
	// Values associated with a given key. Values are stored as a
	// vector of string vectors.
	std::vector<std::vector<std::string> > *values;
	// Doubly linked list.
	struct LeafNode *prev;
	struct LeafNode *next;

	// Adds a given key and value to this leaf node.
	void add(float key, std::string value);
	// Search value for a given node.
	std::vector<std::string> search(float key);
	// Split this node to create a new leaf node.
	std::pair<LeafNode *, float> split();
	void print();	
	LeafNode(): values(NULL), prev(NULL), next(NULL) {}
	~LeafNode();
private:
	void removeKeyValuePairsStartingIndex(int index);
	
};