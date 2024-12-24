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

void strcpy(char *s, char* des);
Stack *createStack(int size); 
int isEmpty(Stack *stack);
void push(Stack *stack, Data *data);
void pop(Stack *stack);
void freeStack(Stack *stack);
void printStack(Stack *stack);
void addToBottom(Stack *stack, Data *data);

int main(int argc, char **argv) {
    Stack *ware_house = createStack(MAX_SIZE);
    int n;
    scanf("%d", &n);

    for(int i = 0; i < n; i++) {
        Data *data = (Data*)malloc(sizeof(Data));
        printf("Enter the wooden plate %d:\n", i);
        printf("Name: "); getchar(); fgets(data->name, MAX_SIZE, stdin); 
        printf("Age: "); scanf("%d", &data->age);
        printf("Radius: "); scanf("%f", &data->radius);
        printf("Length: "); scanf("%f", &data->length);
        push(ware_house, data);
    }
    printStack(ware_house);

    printf("Stack of wood after adding Dog Brain wood: \n");
    Data *data = (Data*)malloc(sizeof(Data));
    data->age = 5;
    strcpy(data->name, "oc cho");
    data->length = 10;
    data->radius = 5;


    // addToBottom(ware_house, data);



    printStack(ware_house);

    // Push the new data to the top of the stack
}

Wood* createWood(Data *data) {
    Wood *newWood = (Wood*)malloc(sizeof(Wood));
    newWood->data = data;
    newWood->next = NULL;
    return newWood;
}
void freeWood(Wood *Wood) {
    if (Wood == NULL) return;
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
    if (stack == NULL) return;
    while(!isEmpty(stack)) {
        pop(stack);
    }
    free(stack);
}

void printStack(Stack *stack) {
    if (stack->top == NULL) return;
    Stack *tempStack = createStack(stack->size);
    int i = 1;
    while (!isEmpty(stack)) {
        Wood *temp = stack->top;
        printf("Wood %d\n", i);
        printf("\tName: %s", temp->data->name);
        printf("\tAge: %d\n", temp->data->age);
        printf("\tRadius: %.2f\n", temp->data->radius);
        printf("\tLength: %.2f\n", temp->data->length);

        Data *data = (Data*)malloc(sizeof(Data));
        data->age = temp->data->age;
        strcpy(data->name, temp->data->name);
        data->radius = temp->data->radius;
        data->length = temp->data->length;

        push(tempStack, data);
        pop(stack);
        i++;
    }

    while (!isEmpty(tempStack)) {
        Data *data = (Data*)malloc(sizeof(Data));
        data->age = tempStack->top->data->age;
        strcpy(data->name, tempStack->top->data->name);
        data->radius = tempStack->top->data->radius;
        data->length = tempStack->top->data->length;
        
        push(stack, data);
        pop(tempStack);
    }

    freeStack(tempStack);
}

void addToBottom(Stack *stack, Data *data) {
    if(stack->size == 0) {
        printf("Stack is full. Cannot add data to the bottom.\n");
        return;
    }
    if (isEmpty(stack)) {
        push(stack, data);
        return;
    }

    Stack *tempStack = createStack(stack->size);

    // Copy the data from ware_house to tempStack
    while (!isEmpty(stack)) {
        Data *data = (Data*)malloc(sizeof(Data));
        data->age = stack->top->data->age;
        strcpy(data->name, stack->top->data->name);
        data->radius = stack->top->data->radius;
        data->length = stack->top->data->length;
        
        push(tempStack, data);
        pop(stack);
    }
    printf("Stack temp: \n");
    printStack(tempStack);
    // Push the new data to the bottom of the stack
    push(stack, data);

    // Copy the data from tempStack to stack
    while (!isEmpty(tempStack)) {
        Data *data = (Data*)malloc(sizeof(Data));
        data->age = tempStack->top->data->age;
        strcpy(data->name, tempStack->top->data->name);
        data->radius = tempStack->top->data->radius;
        data->length = tempStack->top->data->length;

        push(stack, data);
        pop(tempStack);
    }

    freeStack(tempStack);
}

void strcpy(char *s, char* des) {
    while (*des) {
        *s = *des;
        s++;
        des++;
    }
    *s = '\0';
}