#include <stdio.h>
#include <stdlib.h>

struct Node {
    char character;
    struct Node *next;
};

struct Node* createNode(char character) {
    struct Node *node = (struct Node*)malloc(sizeof(struct Node));
    node->character = character;
    node->next = NULL;
    return node;
}

void appendToList(struct Node **tail, char character) {
    struct Node *newNode = createNode(character);
    (*tail)->next = newNode;
    *tail = newNode;
}

void freeList(struct Node *head) {
    struct Node *current = head;
    while (current != NULL) {
        struct Node *temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    FILE *file = fopen("hw1.c", "r");
    if (file == NULL) {
        printf("Cannot open file\n");
        return 1;
    }

    struct Node *listHead = createNode('\0');
    struct Node *listTail = listHead;

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        appendToList(&listTail, (char)ch);
    }
    fclose(file);

	struct Node *current = listHead->next;
	while (current != NULL) {
    	if (current->character == '\n') {
       		printf("'\\n' ");
    	} else {
        	printf("'%c' ", current->character);
    	}
    	current = current->next;
	}
	printf("\n");

    freeList(listHead);
    return 0;
}
