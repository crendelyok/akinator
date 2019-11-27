#ifndef _TREE_H
#define _TREE_H

#define STR_LEN 256

struct Node {
	char data[STR_LEN];
	struct Node* parent;
	struct Node* left;
	struct Node* right;
};

#endif //_TREE_H
