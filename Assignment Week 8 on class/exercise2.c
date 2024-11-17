#include <stdio.h>
#include <stdlib.h>

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
typedef List Function; 
Function *add2Function(Function *f, Function *g); 
void rearrangeFunction(Function *f);
void input(Function *f);
void printFunction(Function *f);

int main(int argc, char **argv) {
    Function *f = createList();
    Function *g = createList();
    input(f);
    input(g);
    printf("Function f: ");
    printFunction(f);
    printf("Function g: ");
    printFunction(g);

    Function *h = add2Function(f, g);
    printf("Function sum of f and g: ");
    printFunction(h);

    freeList(f);
    freeList(g);
    freeList(h);
    return 0;
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
        list->pFirst = createNode(data);
        list->pLast = list->pFirst;
        // printf("List is empty\n");
        return; 
    }

    while (temp != node) temp = temp->pNext;
    Node *new = createNode(data);
    new->pNext = temp->pNext;
    temp->pNext = new;
    if(node == list->pLast) list->pLast = new; 
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

Function *add2Function(Function *f, Function *g) {
    Function *result = createList();

    Node *p1 = f->pFirst;
    Node *p2 = g->pFirst;

    while (p1 != NULL && p2 != NULL) {
        if (p1->data->decimal == p2->data->decimal) {
            Data data = {p1->data->decimal, p1->data->coefficient + p2->data->coefficient};
            if (data.coefficient != 0) listAddAfter(result, result->pLast, data);
            p1 = p1->pNext;
            p2 = p2->pNext;
        }
        else {
            // printf("hello\n");
            if (p1->data->decimal < p2->data->decimal) {
                // printf("hello f\n");
                while (p2->data->decimal > p1->data->decimal) {
                    listAddAfter(result, result->pLast, *(p2->data));
                    p2 = p2->pNext;
                }
            }
            else {
                // printf("hello g\n");
                while (p2->data->decimal < p1->data->decimal) {
                    listAddAfter(result, result->pLast, *(p1->data));
                    p1 = p1->pNext;
                }
            }
        }
    }
    if (p1 != NULL) {
        while (p1!= NULL) {
            listAddAfter(result, result->pLast, *(p1->data));
            p1 = p1->pNext;
        }
    }
    if (p2 != NULL) {
        while (p2!= NULL) {
            listAddAfter(result, result->pLast, *(p2->data));
            p2 = p2->pNext;
        }
    }
    return result;
}

void printFunction(Function *f) {
    if (f == NULL) printf("0"); 
    Node *pTemp = f->pFirst;
    if (pTemp->data->decimal != 0) {
        if (pTemp->data->coefficient > 0) {
            printf("%.2f*x^%d ", pTemp->data->coefficient, pTemp->data->decimal);
        }
        else {
            printf("- %.2f*x^%d ", -pTemp->data->coefficient, pTemp->data->decimal);
        }
    }
    else {
            if (pTemp->data->coefficient > 0) {
            printf("%.2f", pTemp->data->coefficient);
        }
        else {
            printf("- %.2f", -pTemp->data->coefficient);
        }
    }
    pTemp = pTemp->pNext;  // Skip the first term to print the rest of the function
    while (pTemp!= NULL) {
        if (pTemp->data->decimal != 0) {
            if (pTemp->data->coefficient > 0) {
                printf("+ %.2f*x^%d ", pTemp->data->coefficient, pTemp->data->decimal);
            }
            else {
                printf("- %.2f*x^%d ", -pTemp->data->coefficient, pTemp->data->decimal);
            }
        }
        else {
             if (pTemp->data->coefficient > 0) {
                printf("+ %.2f", pTemp->data->coefficient);
            }
            else {
                printf("- %.2f", -pTemp->data->coefficient);
            }
        }
        pTemp = pTemp->pNext;
    }
    printf("\b\n");
}

void input(Function *f) {
    printf("Enter the number of terms in the function\n");
    int n; 
    scanf("%d", &n);
    int d[n]; 
    float e[n]; 
    for (int i = 0; i < n; i++) {
        printf("Enter the coefficient and the decimal\n"); 
        scanf("%f %d", &e[i], &d[i]); 
    }
    for (int i = 0; i < n; i++) {
        Data data = {d[i], e[i]};
        if(e[i] == 0) continue; 
        listAddAfter(f, f->pLast, data);
    }
}