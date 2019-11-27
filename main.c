#include <stdio.h>
#include "tree.c"

#define DATA "in"

struct Node* AkinatorInit();
void AkinatorRefresh(struct Node*);
int Akinator(struct Node*);
int Right();
int Wrong(struct Node*);
int GetName(char**);
int GetQues(char**);

int main() {	
	struct Node* akinator = AkinatorInit();
	if (akinator == NULL) {
		return -1;
	}

	if (Akinator(akinator)) {
		return -1;
	}

	AkinatorRefresh(akinator);

	printf("----------------------\n\n");
	TreeDump(akinator, stdout);

	TreeDestroy(akinator);
	return 0;
}

struct Node* AkinatorInit() {
	FILE* in = fopen(DATA, "r");
	if (in == NULL) {
		printf("cannot read input file");
		return NULL;
	}

	struct Node *new = TreeInit(in);
	if (new == NULL) {
		printf("cannot create tree\n");
		fflush(stdout);
		TreeDestroy(new);
		fclose(in);
		return NULL;
	}

	fclose(in);
	
	return new;
}

void AkinatorRefresh(struct Node* akinator) {
	FILE* out = fopen(DATA, "w");
	TreeDump(akinator, out);
	fclose(out);
}

int Akinator(struct Node* akinator) {
	printf("Welcome to akinator, think of someone and I will name it\n");
	printf("Enter 'y' if I am right and anything else if I am wrong\n");
	printf("Enter any symbol when you are ready\n");

	char tmp = 0;
	scanf("%c", &tmp);
	while (fgetc(stdin) != '\n');

	int leaf = 0;
	struct Node* cur = akinator;
	while(!leaf) {
		char temp = 0;
		printf("%s\n", cur -> data);
		scanf("%c", &temp);
		while (fgetc(stdin) != '\n');

		if (temp == 'y') {
			if (cur -> left == NULL) { 
				leaf = 1;
				return Right();
			}
			cur = cur -> left;
		}
		else {
			if (cur -> right == NULL) {
				leaf = 1;
				return Wrong(cur); 
			}
			cur = cur -> right;
		}
	}
	
	return 1;
}

int Right() {
	printf("I been knew pf!\n");
	printf("Goodbye!\n");
	return 0;
}

int Wrong(struct Node* node) {
	printf("Oops, I don't know what you have thought of\n");

	char* inputObj = calloc(STR_LEN, 1);
	int name = 0;
	while(name == 0) 
		name = GetName(&inputObj);
	if (name == -1) {
		printf("Sorry something went wrong\n");
		free(inputObj);
		return 1;
	}

	char* inputQue = calloc(STR_LEN, 1);
	int ques = 0;
	while(ques == 0)
		ques = GetQues(&inputQue);
	if (ques == -1) {
		printf("Sorry something went wrong\n");
		free(inputObj);
		free(inputQue);
		return 1;
	}

	if (SplitNode(node, inputQue, inputObj) == NULL) {
		printf("Error appeared\n");
		return 1;
	}
	free(inputObj);
	free(inputQue);

	return 0;
}

int GetName(char** input) {
 	printf("Please name who have you thought of? (less than 255 symbols)\n");
	if (fgets(*input, STR_LEN, stdin) == NULL) {
		printf("Sorry something went wrong =(");
		return -1;
	}	

	if (strlen(*input) >= STR_LEN) {
		printf("Too many symbols\n");
		return 0;
	}
	
	(*input)[strlen(*input) - 1] = '?';
	return 1;
}

int GetQues(char** input) {
 	printf("Please name what differs it from my last guess?\n");
	if (fgets(*input, STR_LEN, stdin) == NULL) {
		printf("Sorry something went wrong =(");
		return 1;
	}	
	
	char* invalid = NULL;

	invalid = strstr(*input, "?");
       	if (invalid == NULL) {
		printf("Type a question with\"?\"\n");
		return 0;
	}

	invalid = strstr(*input, "not");
       	if (invalid != NULL) {
		printf("Type a question without \"not\"\n");
		return 0;
	}

	invalid = strstr(*input, "Not");
       	if (invalid != NULL) {
		printf("Type a question without \"Not\"\n");
		return 0;
	}

	invalid = strstr(*input, "No");
       	if (invalid != NULL) {
		printf("Type a question without \"No\"\n");
		return 0;
	}

	invalid = strstr(*input, "no");
       	if (invalid != NULL) {
		printf("Type a question without \"no\"\n");
		return 0;
	}

	return 1;	

}
