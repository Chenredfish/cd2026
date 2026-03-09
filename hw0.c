#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEVEL 4

struct Node {
    char character;
    int count;
    struct Node *next[MAX_LEVEL];
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
    for (int i = 0; i <= level; i++) {
        node->next[i] = NULL;
    }
    return node;
}

struct Node* findLetter(struct Node *head, char character) {
    struct Node *current = head;
    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        while (current->next[i] != NULL && current->next[i]->character < character) {
            current = current->next[i];
        }
    }
    current = current->next[0];
    if (current != NULL && current->character == character) {
        return current;
    }
    return NULL;
}

struct Node* insertLetter(struct Node *head, char character) {
    struct Node *update[MAX_LEVEL];
    struct Node *current = head;
    
    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        while (current->next[i] != NULL && current->next[i]->character < character) {
            current = current->next[i];
        }
        update[i] = current;
    }
    
    current = current->next[0];
    
    if (current != NULL && current->character == character) {
        current->count++;
        return current;
    }
    
    int newLevel = randomLevel();
    struct Node *newNode = createNode(character, newLevel);
    
    for (int i = 0; i <= newLevel; i++) {
        newNode->next[i] = update[i]->next[i];
        update[i]->next[i] = newNode;
    }
    
    return newNode;
}

void freeList(struct Node *head) {
    struct Node *current = head->next[0];
    while (current != NULL) {
        struct Node *temp = current;
        current = current->next[0];
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

    struct Node *head = createNode('\0', MAX_LEVEL - 1);
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r') {
            char character = (char)ch;
            insertLetter(head, character);
        }
    }

    fclose(file);

    struct Node *current = head->next[0];
    while (current != NULL) {
        printf("%c: %d\n", current->character, current->count);
        current = current->next[0];
    }

    freeList(head);

    return 0;
}