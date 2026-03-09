#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEVEL 4

struct Node {
    char character;
    int count;
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
    node->count = 1;
    node->next = NULL;
    node->level = level;
    for (int i = 0; i < MAX_LEVEL; i++) {
        node->skipNext[i] = NULL;
    }
    return node;
}

struct Node* findLetterInSkipList(struct Node *head, char character) {
    struct Node *current = head;
    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
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
    
    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        while (current->skipNext[i] != NULL && current->skipNext[i]->character < newNode->character) {
            current = current->skipNext[i];
        }
        update[i] = current;
    }
    
    for (int i = 0; i <= newNode->level; i++) {
        newNode->skipNext[i] = update[i]->skipNext[i];
        update[i]->skipNext[i] = newNode;
    }
}

void appendToList(struct Node *tail, struct Node *newNode) {
    tail->next = newNode;
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
    FILE *file = fopen("hw0.c", "r");
    if (file == NULL) {
        printf("Cannot open file\n");
        return 1;
    }

    struct Node *skipListHead = createNode('\0', MAX_LEVEL - 1);
    struct Node *listHead = createNode('\0', 0);
    struct Node *listTail = listHead;
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r') {
            char character = (char)ch;
            
            struct Node *found = findLetterInSkipList(skipListHead, character);
            if (found != NULL) {
                found->count++;
            } else {
                int level = randomLevel();
                struct Node *newNode = createNode(character, level);
                
                insertLetterInSkipList(skipListHead, newNode);
                appendToList(listTail, newNode);
                listTail = newNode;
            }
        }
    }

    fclose(file);

    struct Node *current = listHead->next;
    while (current != NULL) {
        printf("%c: %d\n", current->character, current->count);
        current = current->next;
    }

    freeList(listHead);
    freeList(skipListHead);

    return 0;
}