# B-PlusTree
C++ based implementation of a B+ Tree.

This code implements a memory resident B+ Tree that supports duplicate keys.  In addition to insertion and search, the implementation also supports range queries.  
(Implemented as a part of an assignment at UFL, Gainesville)  


#### Implemented methods:  
	- Insert(key,value): Inserts a given key, value pair into the tree  
	- Initialize(m): Initializes a tree with given order (supported values > 2)  
	- Search(key): Returns a vector of values for the given key. Empty vector if the key is 
	not present in the tree.  
	- Search(key1, key2): Returns a vector of values for the given key range (key>=key1 && key<=key2). 
	Empty vector if the key is not present in the tree.  


#### Code organisation
	- TreeNode is the base struct from which both InternalNode and LeafNode inherit. TreeNode.cpp 
	on its own just contains a vector of keys inserted into the tree.
	- LeafNode.cpp stores values and hence maintains a vector of values for the associated 
	keys (which are inherited from TreeNode). LeafNode also maintains a doubly linked-list in-order 
	to support range queries.
	- InternalNode stores a vector of children nodes.
	- BPlus.cpp contains the main function - hence the starting point of the program. Using 
	the APIs provided by LeafNode and InternalNode, this class implements insert, initialise 
	and search methods.



