#include <stdio.h>

typedef struct Data {
    int decimal;
    float coefficient;  
} Data; 

typedef struct Node {
    Data *data; 
    struct Node *pNext;
} Node;

typedef struct List {
    Node *pFirst; 
    Node *pLast; 
    int len;
} List; 


Node *createNode();
void freeNode(Node *node);
List *createList();
void freeList(List *list);
void listAddAfter(List *list, Node *node, Data data);
void listPopBack(List *list);
void listRemove(List *list, Node *node);

int main(int argc, char **argv) {

}

Node *createNode(Data data) { 
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        printf("Error: Out of memory\n");
        return NULL;
    }
    node->data = (Data *)malloc(sizeof(Data));
    if (node->data == NULL) {
        printf("Error: Out of memory\n");
        free(node);
        return NULL;
    }
    node->data->decimal = data.decimal;  // Assign the data to the node
    node->data->coefficient = data.coefficient;  // Assign the data to the node
    node->pNext = NULL;
    return node;
}
void freeNode(Node *node) {
    if (node->data!= NULL) {
        free(node->data);
    }
    free(node);
}
List *createList() {
    List *list = (List *)malloc(sizeof(List));
    if (list == NULL) {
        printf("Error: Out of memory\n");
        return NULL;
    }
    list->pFirst = NULL;
    list->pLast = NULL;
    list->len = 0;
    return list;
}
void freeList(List *list) {
    Node *node = list->pFirst;
    while (node!= NULL) {
        Node *temp = node;
        node = node->pNext;
        freeNode(temp);
    }
    free(list);
}

void listAddAfter(List *list, Node *node, Data data) {
    Node *temp = list->pFirst;
    if (temp == NULL) {
        printf("List is empty\n");
        return; 
    }

    while (temp != node) temp = temp->pNext;
    Node *new = createNode(data);
    new->pNext = temp->pNext;
    temp->pNext = new;
}

void listPopBack(List *list) {
    if (list->pFirst == NULL) {
        printf("List is empty\n");
        return;
    }
    if (list->pFirst == list->pLast) {
        freeNode(list->pFirst);
        list->pFirst = NULL;
        list->pLast = NULL;
    }
    else {
        Node *temp = list->pFirst;
        while (temp->pNext != list->pLast) temp = temp->pNext;
        freeNode(list->pLast);
        list->pLast = temp;
        temp->pNext = NULL;
    }
}

void listRemove(List *list, Node *node) {
    if (list->pFirst == NULL) {
        printf("List is empty\n");
        return;
    }
    if (list->pFirst == node) {
        list->pFirst = node->pNext;
        if (list->pFirst == NULL) list->pLast = NULL;
        freeNode(node);
    }
    else {
        Node *temp = list->pFirst;
        while (temp->pNext!= node) temp = temp->pNext;
        temp->pNext = node->pNext;
        if (node == list->pLast) list->pLast = temp;
        freeNode(node);
    }
}