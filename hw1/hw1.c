#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LEVEL 4

struct Node {
    char character;
    struct Node *next;
    struct Node *skipNext[MAX_LEVEL];
    int level;
};

int randomLevel() {
    int level = 0;
    while ((rand() % 2 == 0) && (level < MAX_LEVEL - 1)) {
        level++;
    }
    return level;
}

struct Node* createNode(char character, int level) {
    struct Node *node = (struct Node*)malloc(sizeof(struct Node));
    node->character = character;
    node->next = NULL;
    node->level = level;
    int i;
    for (i = 0; i < MAX_LEVEL; i++) {
        node->skipNext[i] = NULL;
    }
    return node;
}

struct Node* findLetterInSkipList(struct Node *head, char character) {
    struct Node *current = head;
    int i;
    for (i = MAX_LEVEL - 1; i >= 0; i--) {
        while (current->skipNext[i] != NULL && current->skipNext[i]->character < character) {
            current = current->skipNext[i];
        }
    }
    current = current->skipNext[0];
    if (current != NULL && current->character == character) {
        return current;
    }
    return NULL;
}

void insertLetterInSkipList(struct Node *head, struct Node *newNode) {
    struct Node *update[MAX_LEVEL];
    struct Node *current = head;
    
    int i;
    for (i = MAX_LEVEL - 1; i >= 0; i--) {
        while (current->skipNext[i] != NULL && current->skipNext[i]->character < newNode->character) {
            current = current->skipNext[i];
        }
        update[i] = current;
    }
    
    for (i = 0; i <= newNode->level; i++) {
        newNode->skipNext[i] = update[i]->skipNext[i];
        update[i]->skipNext[i] = newNode;
    }
}

void appendToList(struct Node **tail, struct Node *newNode) {
    (*tail)->next = newNode;
    *tail = newNode;
}

void freeList(struct Node *head) {
    struct Node *current = head->next;
    while (current != NULL) {
        struct Node *temp = current;
        current = current->next;
        free(temp);
    }
    free(head);
}

int main() {
    FILE *file = fopen("hw1.c", "r");
    if (file == NULL) {
        printf("Cannot open file\n");
        return 1;
    }

    struct Node *listHead = createNode('\0', 0);
    struct Node *listTail = listHead;

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        int level = randomLevel();
        struct Node *newNode = createNode((char)ch, level);
        appendToList(&listTail, newNode);
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
