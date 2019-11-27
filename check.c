#include "tree.c"

int main() {
	struct Node* head = HeadInit("head");
	strncpy(head -> data, "head", 5);	
	struct Node* left = AddNode("left", head, LEFT);
	struct Node* leftleft = AddNode("leftleft", left, LEFT);

	NodeDump(head);
	NodeDump(left);
	NodeDump(leftleft);
	
	TreeDump (head, stdout);
	fflush(stdout);
	TreeDestroy(head);
}
