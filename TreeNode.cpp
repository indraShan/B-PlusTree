#include "TreeNode.hpp"
#include <iostream>

TreeNode::~TreeNode() {
	if (keys != NULL) delete(keys);
}

void TreeNode::print(){
	std::cout << "Printing keys" << std::endl;
	for (int counter = 0; counter < keys->size(); counter++) {
		std::cout << "Key = " << keys->at(counter) << " at index = " << counter << std::endl;
	}
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}