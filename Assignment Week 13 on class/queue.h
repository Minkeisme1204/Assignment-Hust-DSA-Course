#ifndef QUEUE
#define QUEUE

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

typedef struct Queue {
    Node* front;
    Node* back;
    int size;
} Queue; 

Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->back = NULL;
    queue->size = 0;
    return queue;
}

void freeQueue(Queue* queue) {
    if (queue!= NULL) {
        Node* temp = queue->front;
        while (temp!= NULL) {
            Node* nextTemp = temp->next;
            freeNode(temp);
            temp = nextTemp;
        }
        free(queue);
    }
}

int isEmptyQueue(Queue *queue) {
    return queue->size == 0;
} 

void enQueue(Queue* queue, Data* data) {
    Node* newNode = createNode();
    newNode->data = data;

    if (queue->front == NULL) {
        queue->front = newNode;
    } else {
        queue->back->next = newNode;
    }
    queue->back = newNode;
    queue->size++;
}

void deQueue(Queue* queue) {
    if (isEmptyQueue(queue)) {
        printf("Queue is empty.\n");
        return;
    }

    Node* temp = queue->front;
    queue->front = temp->next;

    if (queue->front == NULL) {
        queue->back = NULL;
    }

    freeNode(temp);

    queue->size--;
}

void printQueue(Queue* queue) {
    if (isEmptyQueue(queue)) {
        printf("Queue is empty.\n");
        return;
    }

    Node* temp = queue->front;
    int i = 1; 
    while (temp!= NULL) {
        printf("%d) Owner: %s, Item: %s, Money: %d VND\n", i, temp->data->owner_name, temp->data->item_name, temp->data->money);
        temp = temp->next;
        i++;
    }
    printf("\n");
}



#endif
