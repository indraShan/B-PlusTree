#include "InternalNode.hpp"
#include <iostream>

InternalNode::~InternalNode() {
	if (children != NULL) delete(children);	
}

void InternalNode::print(){
	std::cout << "Printing internal node" << std::endl;
	for (int counter = 0; counter < keys->size(); counter++) {
		std::cout << "Key = " << keys->at(counter) << " at index = " << counter << std::endl;
	}
	std::cout << "Printing children" << std::endl;
	for (int counter = 0; counter < children->size(); counter++) {
		std::cout << "Child = " << children->at(counter) << " at index = " << counter << std::endl;
	}
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}

// Splits 'this' node and creates a new InternalNode by
// moving half the elements (keys and children) to the new node.
std::pair<InternalNode *, float>  InternalNode::split() {
	// Split at center.
	float splitValue = keys->at((keys->size()/2));
	InternalNode *node = new InternalNode();
	node->keys = new std::vector<float>;
	node->children = new std::vector<TreeNode*>;
	
	int slice = (keys->size()/2)+1;
	// Move values to the right of center to the new node.
	for (int counter = slice; counter < keys->size(); counter++) {
		node->keys->push_back(keys->at(counter));
		node->children->push_back(children->at(counter));
	}
	// Copy over the last one.
	node->children->push_back(children->at(keys->size()));
	
	// Remove copied keys from current node.
	keys->erase(keys->begin()+slice-1, keys->end());
	children->erase(children->begin()+slice, children->end());
	return std::make_pair(node, splitValue);
}

// Inserts a key at its correct location along with its children nodes.
void InternalNode::insert(float key, TreeNode *left, TreeNode *right) {
	if (keys == NULL) {
		keys = new std::vector<float>;
		children = new std::vector<TreeNode*>;
	}
	// Find the right index to insert the new key (this takes logn time)
	std::vector<float>::iterator interator = lower_bound(keys->begin(), keys->end(), key);
	int position = interator - keys->begin();
	
	// Insert key
	keys->insert(keys->begin()+position, key);

	// Insert left child
	if (left != NULL) {
		children->insert(children->begin()+position+0, left);
	}
	
	// Insert right child to the right.
	if (right != NULL) {
		children->insert(children->begin()+position+1, right);
	}
	
}

// Does a binary search to find the correct child node to pointer to
// follow while doing a key search.
TreeNode* InternalNode::childrenNodeToFind(float key, int start, int end) {
	if (keys->size() == 0) {
		return NULL;
	}
	// If the node contains just a single key, then its either the left 
	// or the right child to follow.
	if (keys->size() == 1) {
		return (key < keys->at(0)) ? children->at(0) : children->at(1);
	}
	// Binary search.
	int mid = (start+end)/2;
	if (key == keys->at(mid)) {
		return children->at(mid+1);
	}
	// Is smaller?
	if (key < keys->at(mid)) {
		// Do we have a smaller key in this node to compare against?
		int prev = mid-1;
		if (prev < 0) {
			// No smaller key available. Return the fist child node we have.
			return children->at(0);
		}
		// Is the previous key larger than the key we want to find?
		if (key >= keys->at(prev)) {
			// Found the correct child node.
			return children->at(mid);
		}
		
		// Left key is still smaller than  the key we are looking for.
		return 	childrenNodeToFind(key, start, mid-1);
	}
	
	int next = mid+1;
	if (next >= keys->size()) {
		// No larger key available. Return the last child node we have.
		return children->at(children->size()-1);
	}
	// Is the key we want to find still larger than the next available key in the node?
	if (key >= keys->at(next)) {
		// Keep searching to the right.
		return childrenNodeToFind(key, mid+1, end);
	}
		
	// Found the child node we are looking for
	return children->at(mid+1);
}