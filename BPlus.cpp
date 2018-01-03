#include <iostream>
#include <stack>
#include <fstream>
#include "InternalNode.hpp"
#include "LeafNode.hpp"

// main class
class BPlusTree {
public:
	// Inserts a give key, value pair into the tree
	void Insert(float key, std::string value);
	// Initializes a tree with given order. (supported values > 2)
	void Initialize(int m);
	// Returns a vector of values for the given key. Empty vector if the key is 
	// not present in the tree.
	std::vector<std::string> Search(float key);
	// Returns a vector of values for the given key range (key>=key1 && key<=key2). 
	// Empty vector if the key is not present in the tree. 
	std::vector<std::pair<float, std::string> > Search(float key1, float key2);
	~BPlusTree() {
		if (root != NULL) delete(root);
	}
private:
	// Tree order.
	int m;
	// The root node.
	TreeNode *root;
	
	void Search(TreeNode *node, float key, std::stack<TreeNode*> *stack);
	bool isLeafNode(TreeNode *node);
	void print();
	void print(TreeNode *node);
};


void BPlusTree::Initialize(int m) {
	this->m = m;
	root = NULL;
}

// Returns true if the given TreeNode is of type LeafNode. False otherwise.
bool BPlusTree::isLeafNode(TreeNode *node) {
	return dynamic_cast<LeafNode*>(node) != NULL;
}

// Recursive search till the leaf.
void BPlusTree::Search(TreeNode *node, float key, std::stack<TreeNode*> *stack) {
	// Add the found node to the stack.
	stack->push(node);
	// Stop and return if we have found the leaf node for the given key.
	if (isLeafNode(node)) {
		return;
	}
	// If internal node, get the 'next' childrent node to follow down.
	InternalNode *internalNode = (InternalNode *)node;
	TreeNode* children = internalNode->childrenNodeToFind(key, 0, internalNode->keys->size()-1);
	// Recursive call.
	Search(children, key, stack);
}	
	
std::vector<std::string> BPlusTree::Search(float key) {
	if (root == NULL) {
		return std::vector<std::string>();
	}
	std::stack<TreeNode*> *stack = new std::stack<TreeNode *>;
	Search(root, key, stack);
	// Get the leaf node - which would be always at the top of the stack.
	LeafNode *leaf = (LeafNode *)stack->top();
	delete(stack);
	// Leaf search
	return leaf->search(key);
}

// Range search.
std::vector<std::pair<float, std::string> > BPlusTree::Search(float key1, float key2) {
 	std::stack<TreeNode*> *stack = new std::stack<TreeNode *>;
	// Exact search for key1.
	Search(root, key1, stack);
	LeafNode *leaf = (LeafNode *)stack->top();
	delete(stack);
	
	int position = 0;
	std::vector<std::pair<float, std::string> > result;
	LeafNode *currentLeaf = leaf;
	float value = currentLeaf->keys->at(position);
	// Iterate over 'next' leaf nodes till we find a key greater than key2
	while (value <= key2) {
		if (value >= key1) {
			// Found key greater than key1 and lesser than key2
			// Interate over all values and add them to result
			for (int counter = 0; counter <  currentLeaf->values->at(position).size(); counter++) {
				result.push_back(std::make_pair(value, currentLeaf->values->at(position).at(counter)));
			}
		}
		// Are we at the end of this node?
		if (position < currentLeaf->keys->size() - 1) {
			position++;
		}
		else {
			// Move to next leaf.
			currentLeaf = currentLeaf->next;
			position = 0;
		}
		if (currentLeaf == NULL) {
			break;
		}
		value = currentLeaf->keys->at(position);
	}
	
	return result;
}

void BPlusTree::print(TreeNode *node) {
	if (isLeafNode(node)) {
		((LeafNode *)node)->print();
		return;
	}
	((InternalNode *)node)->print();
	InternalNode *internalNode = (InternalNode *)node;
	if (internalNode->children == NULL) {
		return;
	}
	for (int counter = 0; counter < internalNode->children->size(); counter++) {
		print(internalNode->children->at(counter));
	}
}

void BPlusTree::print() {
	print(root);
}

void BPlusTree::Insert(float key, std::string value) {
	if (root == NULL) {
		LeafNode *leaf = new LeafNode();
		leaf->add(key, value);
		root = leaf;
		return;
	}
	// Get the leaf node where this key should be inserted.
	std::stack<TreeNode*> *stack = new std::stack<TreeNode *>;
	Search(root, key, stack);
	LeafNode *leaf = (LeafNode *)stack->top();
	std::vector<std::string> existingValues = leaf->search(key);
	
	// Leaf node has enough space or this is a duplicate key.
	if (leaf->keys->size()+1 <= m-1 || existingValues.size() > 0) {
		// Insert into the leaf and we are done.
		leaf->add(key, value);
		delete(stack);
		return;
	}
		
	// Temporarily add the key into already full node.
	leaf->add(key, value);
	// Split the leaf
	std::pair<TreeNode*, float> result = leaf->split();

	stack->pop();
	
	// Root is being split.
	if (leaf == root) {
		InternalNode *node = new InternalNode();
		// Smaller of the new node becomes the new root.
		node->insert(result.second, root, result.first);
		// Change the root
		root = node;
		delete(stack);
		return;
	}

	InternalNode *node = (InternalNode *)stack->top();
	// Iterate over the stack till we find a node with enough capacity or till
	// we split the root.
	while(stack->empty() == false) {
		// Add the new 'split' node to parent with its 'right' children
		// node.
		node->insert(result.second, NULL, result.first);
		// After addition if the node is not overflowing, we are done.
		if (node->keys->size() <= m-1) {
			break;
		}
		stack->pop();
		// Split the next parent.
		result = node->split();
		// If we are spliting the root, re-construct the root and stop.
		if (node == root) {
			InternalNode *newRoot = new InternalNode();
			newRoot->insert(result.second, node, result.first);
			root = newRoot;
			break;
		}
		if (stack->empty() == false) {
			node = (InternalNode *)stack->top();
		}
	}
	delete(stack);
}

// IO method to read input and create output_file.txt with the results.
// Method to test the class.
int readInput(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "First argument missing. Path to input file should be the fist argument to run." << std::endl;
        return -1;
    }
    std::string input = argv[1];
    std::ifstream inputstream(input);
    if (!inputstream) {
        std::cout << "Input file not found." << std::endl;
        return -1;
    }

    // Open outputfile.
    std::ofstream outputstream("output_file.txt");

    if (!outputstream) {
        std::cout << "Failed to open output_file.txt" << std::endl;
        return -1;
    }

    BPlusTree* tree = new BPlusTree();
    std::string m;

    // Read order of the tree
    getline(inputstream, m);

    int order = atoi(m.c_str());

    // Initialize the tree.
    tree->Initialize(order);

    while (inputstream) {
        std::string input;
        inputstream >> input;

        std::string searchMethod = "Search";
        std::string comma = ",";
        std::string openingBracket = "(";
        std::string closingBracket = ")";

        size_t position = input.find(openingBracket);

        // Get the method name.
        std::string methodName = input.substr(0, position);
        input.erase(0, position + openingBracket.length());

        std::string param1 = "";
        std::string param2 = "";

        if (methodName == searchMethod) {
			// Do we have a comma in the method paramters?
            if (input.find(comma) > (size_t)1000) {
				// Exact search
                position = input.find(closingBracket);
                param1 = input.substr(0, position);
                input.erase(0, position + closingBracket.length());
            }
            else {
				// Found a comma. Range search
                position = input.find(comma);
                param1 = input.substr(0, position);
                input.erase(0, position + comma.length());
                position = input.find(closingBracket);
                param2 = input.substr(0, position);
                input.erase(0, position + closingBracket.length());
            }

            float startKey = atof(param1.c_str());
			// Exact search if only one parameter.
            if (param2 == "") {
                std::vector<std::string> result = tree->Search(startKey);
                if (result.size() == 0) {
                    outputstream << "Null" << std::endl;
                }
                else {
                    for (std::vector<std::string>::const_iterator iterator = result.begin(); iterator < result.end(); ++iterator) {
                        outputstream << *iterator;
                        if (iterator < result.end() - 1)
                            outputstream << ",";
                        else {
                       	 	outputstream << std::endl;
                        }
                    }
                }
            }
            else {
				// Range search.
                float key2 = atof(param2.c_str());
                std::vector<std::pair<float, std::string> > result = tree->Search(startKey, key2);
                if (result.size() == 0) {
                    outputstream << "Null" << std::endl;
                }
                else {
                    for (std::vector<std::pair<float, std::string> >::const_iterator iterator = result.begin(); iterator < result.end(); ++iterator) {
                        outputstream << "(" << result[iterator - result.begin()].first << "," << result[iterator - result.begin()].second << ")";
                        if (iterator < result.end() - 1)
                            outputstream << ",";
                        else
                            outputstream << std::endl;
                    }
                }
            }
        }
        else {
			// Insert method
            position = input.find(comma);
            param1 = input.substr(0, position);
            input.erase(0, position + comma.length());
            position = input.find(closingBracket);
            param2 = input.substr(0, position);
            input.erase(0, position + closingBracket.length());

            float key = atof(param1.c_str());
            tree->Insert(key, param2);
        }
    }
	std::cout << "Success" << std::endl;

    delete (tree);

    return 0;
}

int main(int argc, char **argv) {
	return readInput(argc, argv);
}