
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

void appendToList(struct Node *tail, struct Node *newNode) {
    tail->next = newNode;
}

void printLettersOnly(struct Node *head) {
    struct Node *current = head->next;
    while (current != NULL) {
        if (current->character < '0' || current->character > '9') {
            printf("%c, ", current->character);
        }
        current = current->next;
    }
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

void processFile(const char *filename, struct Node *skipListHead, struct Node **listHead, struct Node **listTail) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file\n");
        return;
    }
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
                appendToList(*listTail, newNode);
                *listTail = newNode;
            }
        }
    }
    fclose(file);
}

int main() {
    struct Node *skipListHead = createNode('\0', MAX_LEVEL - 1);
    struct Node *listHead = createNode('\0', 0);
    struct Node *listTail = listHead;

    processFile("lab1.c", skipListHead, &listHead, &listTail);

    printLettersOnly(listHead);

    freeList(listHead);
    freeList(skipListHead);
    return 0;
}
