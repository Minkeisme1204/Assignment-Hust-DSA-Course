#include <stdio.h>
#include <stdlib.h>
// #define DEBUG
typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    Node *tail;
    int size;
} List; 

// Function to create a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory error!\n");
        exit(0);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void freeNode(Node* node) {
    free(node);
}

List* createList() {
    List* list = (List*)malloc(sizeof(List));
    if (!list) {
        printf("Memory error!\n");
        exit(0);
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void freeList(List* list) {
    Node* current = list->head;
    Node* next;

    while (current!= NULL) {
        next = current->next;
        freeNode(current);
        current = next;
    }
    free(list);
}

void addNode(Node* node, List *list) {
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    list->size++;
} 

void deleteNode(Node* node, List *list) {
    if (list->head == NULL) {
        printf("List is empty\n");
        return;
    }
    if (list->head == node) {
        list->head = list->head->next;
        if (list->head == NULL) {
            list->tail = NULL;
        }
    } else {
        Node* current = list->head;
        while (current->next!= node && current!= NULL) {
            current = current->next;
        }
        if (current == NULL) {
            printf("Node not found in the list\n");
            return;
        }
        current->next = node->next;
        if (node->next == NULL) {
            list->tail = current;
        }
    }
    freeNode(node);
    list->size--;
}

int main(int argc, char **argv) {
    List *list = createList();
    int n, m; 
    printf("Enter N: ");
    scanf("%d", &n); 
    printf("Enter M: ");  // m is the number of skips in each round
    scanf("%d", &m);
    for (int i = 1; i <= n; i++) {
        Node* temp = createNode(i);
        addNode(temp, list);
    }
    printf("List size: %d\n", list->size);
    Node *pCurrent = list->head;
    while (list->size > 1) {
        for (int i = 0; i < m; i++) {
            pCurrent = (pCurrent->next != NULL) ? pCurrent->next:list->head;
        }
        Node *pDelete = pCurrent; 
        pCurrent = pCurrent->next;
        #ifdef DEBUG
        printf("%d\n", pDelete->data);
        #endif
        deleteNode(pDelete, list);
    }
    printf("%d\n", list->head->data);
}

