
#include <stdlib.h>
#include <stdio.h>

typedef struct Data {
    char owner_name[50]; 
    int money; 
    char item_name[50]; 
} Data;

typedef struct Node {
    Data *data;
    struct Node* next;
} Node;

Node* createNode() {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = NULL;
    newNode->next = NULL;
    return newNode;
}

void freeNode(Node* node) {
    if (node!= NULL) {
        free(node->data);
        free(node);
    }
}

typedef struct Tree {
    Node *root; 
    int size;

} Tree;