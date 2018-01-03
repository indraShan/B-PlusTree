#include "LeafNode.hpp"
#include <iostream>

LeafNode::~LeafNode() {
	if (values != NULL) delete(values);
	if (prev != NULL) delete(prev);
	if (next != NULL) delete(next);	
}

// Splits 'this' node to create a new LeafNode.
std::pair<LeafNode *, float> LeafNode::split() {
	// Break the leaf node into two nodes.
	// Create a new node with the overflow (half of current) keys and values.
	LeafNode *leaf = new LeafNode();
	for (int counter = keys->size()/2; counter < keys->size(); counter++) {
		// Move values along with keys.
		for (int j = 0; j < values->at(counter).size(); j++) {
			leaf->add(keys->at(counter), values->at(counter).at(j));
		}
	}
	
	// Remove copied keys(and values) from current node.
	removeKeyValuePairsStartingIndex(keys->size()/2);
	
	// Make this nodes next, new nodes right.
	leaf->next = this->next;
	if (this->next != NULL) {
		this->next->prev = leaf;
	}
	
	// Make the new node the 'right' node of current node.
	this->next = leaf;
	leaf->prev = this;
	
	return std::make_pair(leaf, leaf->keys->at(0));
}

void LeafNode::removeKeyValuePairsStartingIndex(int index) {
	keys->erase(keys->begin()+index, keys->end());
	values->erase(values->begin()+index, values->end());
}

// Finds the value for a given key in this LeafNode.
// Returns empty vector if the key is not present.
std::vector<std::string> LeafNode::search(float key) {
	// Binary search.
	std::vector<float>::iterator interator = lower_bound(keys->begin(), keys->end(), key);
	int position = interator - keys->begin();
	if (position < keys->size() && keys->at(position) == key) {
		return values->at(position);
	}
	return std::vector<std::string>();
}

// Adds a key-value pair to this node.
// If the key is already present, its values vector is updated to
// inlcude this new value.
void LeafNode::add(float key, std::string value) {
	if (keys == NULL) {
		keys = new std::vector<float>;
		values = new std::vector<std::vector<std::string> >();
	}
	std::vector<std::string> existingValues;
	// First add call?
	if (keys->size() == 0) {
		keys->push_back(key);
		existingValues.push_back(value);
		values->push_back(existingValues);
	}
	else {
		// Check if the key is already contained in the node.
		std::vector<float>::iterator interator = lower_bound(keys->begin(), keys->end(), key);
		int position = interator - keys->begin();
		if (position < keys->size() && keys->at(position) == key) {
			// Update existing value.
			existingValues = values->at(position);
			existingValues.push_back(value);
			values->at(position) = existingValues;
		}
		else {
			// Insert new value.
			keys->insert(keys->begin()+position, key);
			existingValues.push_back(value);
			values->insert(values->begin()+position, existingValues);
		}
	}
}

void LeafNode::print() {
	if (keys == NULL) {
		return;
	}
	std::cout << "Printing leaf node" << std::endl;
	for (int counter = 0; counter < keys->size(); counter++) {
		std::cout << "key = " << keys->at(counter) << std::endl;
		for (int j = 0; j < values->at(counter).size(); j++) {
			std::cout << "Value = " << values->at(counter).at(j) << std::endl;
		}
	}
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	
}