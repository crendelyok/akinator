#ifndef _TREE_C
#define _TREE_C

#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

//does smth for each node in subtree

struct Visitor{
	struct Node* node;
	FILE* file;
};

//FIRST FUNC(LEFT) FUNC(RIGHT) THEN FUNC(NODE)
int VisitorLRN(struct Node* node, int (*func)(struct Node*)) {
	assert(func != NULL);
	assert(node != NULL);

	if (node -> left != NULL) {
		if (VisitorLRN(node -> left, func) == 0) 
			return 0;
	}
	
	if (node -> right != NULL) {
		if (VisitorLRN(node -> right, func) == 0)
			return 0;
	}
	return func(node);
			
}

//returns 0 on success
//otherwise 1
int TreeDump(struct Node* node, FILE* file) {
	if (node == NULL)
		return 1;	
	else {
		fprintf(file, "{\n");
		if ((node -> data)[strlen(node -> data) - 1] != '\n') {
			fprintf(file, "%s\n", node -> data);
		}	
		else
		       	fprintf(file, "%s", node -> data); 

		if (node -> left != NULL)
			TreeDump(node -> left, file);
		if (node -> right != NULL)
			TreeDump(node -> right, file);
		fprintf(file, "}\n");
		return 0;
	}
	
	return 1;
}

//returns 0 on success
//otherwise 1
int NodeDump(struct Node* node) {
	printf("\n---------------\n");
	if (node == NULL) {
		printf("*node = NULL\n");
		return 1;
	}
	printf("*(node): %p\n",  node);
	printf("node -> data: %s\n",  node -> data);
	printf("*(node -> parent): %p\n",  node -> parent);
	printf("*(node -> left): %p\n",  node -> left);
	printf("*(node -> right): %p\n",  node -> right);
	printf("\n---------------\n");
	return 0;
	
}


int SubTreeDestroy(struct Node* node) {
	free(node);
	return 1;
}

//returns 1 on success
//otherwise 0
int TreeDestroy(struct Node* node) {
	assert(node != NULL);
	int ret = VisitorLRN(node, SubTreeDestroy);  
	return ret;
}


enum Flags{
	NONE, // 0
	LEFT, // 1
	RIGHT // 2
};

//retruns Node* on success
//otherwise returns NULL
struct Node* HeadInit(const char* data) {
	struct Node* new = calloc(1, sizeof(struct Node));
	if (new == NULL) 
		return NULL;
	
	strncpy(new -> data, data, STR_LEN);
	
	return new;	
}	

//If flag == NONE, copies string from *data to node -> data 
//and returns strutc Node* node.
//If *node == NULL returns HeadInit(data).
//Otherwise returns Node* on succes and NULL if error appeared.
//If flag == LEFT, creates Node* new: new -> parent = node
//and node -> left = new.  
//If flag == RIGHT, creates Node* new: new -> parent = node
//and node -> right = new.
struct Node* AddNode(const char* data, struct Node* node, int flag) {
	if (node == NULL) {
		return HeadInit(data);
	}
	
	if (flag == NONE) {
		strncpy(node -> data, data, STR_LEN);
		return node;
	}

	struct Node* new = calloc(1, sizeof(struct Node));
	if (new == NULL)
		return NULL;

	if (flag == LEFT) {
		(node) -> left = new;
	}
	if (flag == RIGHT) {
		(node) -> right = new;
	}

	strncpy(new -> data, data, STR_LEN);
	new -> left = NULL;
	new -> right = NULL;
	new -> parent = node;
		
	return new;
}

//Recursively reads file, pointed by FILE* in and creates a tree by nodes.
//*node rewrites: it now points at the head of subtree.
//returns *node on success and NULL otherwise.
struct Node* FileAddNode(struct Node** node, FILE* in) {
	assert(in != NULL);
	assert(node != NULL);
	
	char* buffer = calloc(STR_LEN, sizeof(char));
	fgets(buffer, STR_LEN, in); //node -> data value
	AddNode(buffer, *node, NONE);	


	fgets(buffer, STR_LEN, in); // expect {
	if (buffer[0] == '}') {
		free(buffer);
		return NULL;
	}
		
	struct Node* left = calloc(1, sizeof(struct Node));
	(*node) -> left = left;
	left -> parent = (*node);
	FileAddNode(&left, in);

	fgets(buffer, STR_LEN, in); // expect {
	if (buffer[0] == '}') {
		free(buffer);
		return NULL;
	}

	struct Node* right = calloc(1, sizeof(struct Node));
	right -> parent = (*node);
	(*node) -> right = right;
	FileAddNode(&right, in);

	fgets(buffer, STR_LEN, in); // expect }

	free(buffer);

	return *node;
}

//Reads file, pointed by FILE* in and creates a tree.
//Creates and returns head of this tree.
//If error appeared returns NULL.
struct Node* TreeInit(FILE* in) {
	struct Node* head = calloc(1, sizeof(struct Node));
	char* buffer = calloc(STR_LEN, 1);

	fgets(buffer, STR_LEN, in); //expect {
	if (buffer[0] != '{') {
		free(buffer);
		free(head);
		return NULL;
	}

	FileAddNode(&head, in);
	printf("%p\n", head);
	fflush(stdout);

	free(buffer);

	return head;
}

//*node points at the leaf that needs to be separated
//Creates 2 nodes: separeting and second leaf
//separating -> left = second leaf
//separating -> right = separated(aka node)
//Returns NULL if error appeared
//Returns Node* sep of the separating node
struct Node* SplitNode(struct Node* node, const char* separating,
		       const char* data) {
	assert(node != NULL);
	
	struct Node* sep = HeadInit(separating);
	struct Node* new = HeadInit(data);
	if (new == NULL || sep == NULL)
		return NULL;
	sep -> parent = node -> parent;
	sep -> left = new;
	sep -> right = node;
	new -> parent = sep;
	if (node -> parent -> left == node) {
		node -> parent -> left = sep; 
	}
	else {
		node -> parent -> right = sep;
	}
	node -> parent = sep;
	return sep;
}

#endif //_TREE_C
