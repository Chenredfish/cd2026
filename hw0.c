#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node {
    char character;
    int count;
    struct Node *next;
};

struct Node* createNode(char character) {
    struct Node *node = (struct Node*)malloc(sizeof(struct Node));
    node->character = character;
    node->count = 1;
    node->next = NULL;
    return node;
}

struct Node* findLetter(struct Node *head, char character) {
    struct Node *current = head;
    while (current != NULL) {
        if (current->character == character) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void appendNode(struct Node *head, struct Node *newNode) {
    struct Node *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
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
    FILE *file = fopen("hw0.c", "r");
    if (file == NULL) {
        printf("Cannot open file\n");
        return 1;
    }

    struct Node *head = NULL;
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r') {
            char character = (char)ch;

            if (head == NULL) {
                head = createNode(character);
            } else {
                struct Node *found = findLetter(head, character);
                if (found != NULL) {
                    found->count++;
                } else {
                    struct Node *newNode = createNode(character);
                    appendNode(head, newNode);
                }
            }
        }
    }

    fclose(file);

    struct Node *current = head;
    while (current != NULL) {
        printf("%c: %d\n", current->character, current->count);
        current = current->next;
    }

    freeList(head);

    return 0;
}