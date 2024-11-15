#include <stdlib.h>
#include <stdio.h>
#define MAX_SIZE 1024

typedef struct Data {
    int age; 
    char name[MAX_SIZE]; 
    float radius;
    float length; 

} Data;

typedef struct Wood {
    Data *data;
    Wood *next;
} Wood;

Wood* createWood(Data *data);
void freeWood(Wood *Wood);

typedef struct Stack {
    Wood *top;
    int size;
} Stack;

Stack *createStack(int size); 
int isEmpty(Stack *stack);
void push(Stack *stack, Data *data);
void pop(Stack *stack);
void freeStack(Stack *stack);

int main(int argc, char **argv) {
    Stack *ware_house = createStack(MAX_SIZE);
    int n;
    scanf("%d", &n);

    for(int i = 0; i < n; i++) {
        Data *data = (Data*)malloc(sizeof(Data));
        printf("Enter the wooden plate %d:\n", i);
        printf("Name: "); fflush(stdin); fgets(data->name, MAX_SIZE, stdin); 
        printf("Age: "); scanf("%d", &data->age);
        printf("Radius: "); scanf("%f", &data->radius);
        printf("Length: "); scanf("%f", &data->length);
        push(ware_house, data);
    }

    int count[MAX_SIZE];
    while (isEmpty(ware_house)) {
        Wood *temp = ware_house->top; 
        count[temp->data->age]++;
        pop(ware_house);
    }

    for (int i = 0; i < MAX_SIZE; i++) {
        if(count[i] > 0) {
            printf("The number of wooden plates with age %d: %d\n", i, count[i]);
        }
    }
}

Wood* createWood(Data *data) {
    Wood *newWood = (Wood*)malloc(sizeof(Wood));
    newWood->data = data;
    newWood->next = NULL;
    return newWood;
}
void freeWood(Wood *Wood) {
    free(Wood->data);
    free(Wood);
}

Stack *createStack(int size) {
    Stack *newStack = (Stack*)malloc(sizeof(Stack));
    newStack->top = NULL;
    newStack->size = size;
    return newStack;
}

void push(Stack *stack, Data *data) {
    if(stack->size == 0) {
        printf("Stack is full. Cannot push data.\n");
        return;
    }
    Wood *newWood = createWood(data);
    newWood->next = stack->top;
    stack->top = newWood;
    stack->size--;
}

int isEmpty(Stack *stack) {
    return stack->top == NULL;
}

void pop(Stack *stack) {
    if(isEmpty(stack)) {
        printf("Stack is empty. Cannot pop data.\n");
        return;
    }
    Wood *temp = stack->top;
    stack->top = stack->top->next;
    freeWood(temp);
    stack->size++;
}

void freeStack(Stack *stack) {
    while(!isEmpty(stack)) {
        pop(stack);
    }
    free(stack);
}