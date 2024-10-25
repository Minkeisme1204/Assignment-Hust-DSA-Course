#include <stdio.h>
#include <stdlib.h>

typedef struct Day {
    int day, month, year;
} Day;  

typedef struct Student {
    char studentID[8]; 
    char name[50]; 
    int sex;
    Day dateOfBirth;
    char address[100]; 
    char Class[12]; 
    char major[7]; 
} Student; 

typedef struct Node {
    Student *studentData; 
    struct Node *pNext;
} Node;

typedef struct List {
    Node *pFirst; 
    Node *pLast; 
    int len;
} List; 

void stringCopy(char *src, char *dest);

// Node APIs
Node *createNode();
void freeNode(Node *node);
void initNode(Node *node, char *student, char *name, int sex, char *address, char *Class, char *major, int day, int month, int year);
void printNode (Node *node);
void inputNode(Node *node);

// List APIs
List *createList();
void addNewNode(List *list, Node *node);
void printList(List *list);
void sortList(List *list);
void freeList(List *list);




int main(int argc, char **argv) {
    List *list = createList();

    for (int i = 1; i < 5; i++) {
        Node *newNode = createNode();
        inputNode(newNode);
        addNewNode(list, newNode);
    }

    printList(list);

    return 0;

}

Node *createNode() {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->studentData = (Student*)malloc(sizeof(Student));
    if (newNode->studentData == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->pNext = NULL;
    printf("Size of student data %d", sizeof(*newNode->studentData));
    return newNode;
}

void stringCopy(char *src, char *dest) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void initNode(Node *node, char *student, char *name, int sex, char *address, char *Class, char *major, int day, int month, int year) {
    stringCopy(node->studentData->studentID, student);
    // stringCopy(node->studentData->name, name);
    node->studentData->sex = sex;
    node->studentData->dateOfBirth.day = day;
    node->studentData->dateOfBirth.month = month;
    node->studentData->dateOfBirth.year = year;
    // stringCopy(node->studentData->address, address);
    // stringCopy(node->studentData->Class, Class);
    // stringCopy(node->studentData->major, major);
}

void addNewNode(List *list, Node *node) {
    Node *pTemp = list->pFirst;
    while (pTemp->pNext != NULL) pTemp = pTemp->pNext; 
    pTemp->pNext = node;
    list->len++;
}

void input(Node *node) {
    char studentID[8];
    char name[50];
    int sex;
    int day, month, year;
    char address[100];
    char Class[12];
    char major[7];

    printf("Enter student ID: ");
    scanf("%s", studentID);

    printf("Enter name: ");
    fflush(stdin); 
    fgets(name, 50, stdin); 

    printf("Enter sex (1: Male, 0: Female): ");
    scanf("%d", &sex);

    printf("Enter date of birth (DD MM YYYY): ");
    scanf("%d %d %d", &day, &month, &year);

    printf("Enter address: ");
    fflush(stdin); 
    fgets(address, 100, stdin);

    printf("Enter class: ");
    fflush(stdin); 
    fgets(Class, 12, stdin);

    printf("Enter major: ");
    fflush(stdin); 
    fgets(major, 7, stdin);

    initNode(node, studentID, name, sex, address, Class, major, day, month, year);
}

void printNode (Node *node) {
    printf("Student ID: %s\n", node->studentData->studentID);
    printf("Name: %s\n", node->studentData->name);
    printf("Sex: %d\n", node->studentData->sex);
    printf("Date of Birth: %d/%d/%d\n", node->studentData->dateOfBirth.day, node->studentData->dateOfBirth.month, node->studentData->dateOfBirth.year);
    printf("Address: %s\n", node->studentData->address);
    printf("Class: %s\n", node->studentData->Class);
    printf("Major: %s\n", node->studentData->major);
}

List *createList() {
    List *list = (List*)malloc(sizeof(List));
    if (list == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    list->pFirst = NULL;
    list->pLast = NULL;
    list->len = 0;  // Initialize length to 0  // not necessary if list is initially empty  // but good practice for future reference  // and to make the code more robust  // if the list becomes very large, we can add a counter to track the length and use it in the sorting function  // instead of traversing the entire list each time we want to check the length  // for example, if we want to add a new node, we first increment
    return list;
}

void printList(List *list) {
    Node *pTemp = list->pFirst;
    while (pTemp!= NULL) {
        printNode(pTemp);
        pTemp = pTemp->pNext;
    }
}

void sortList(List *list) {
    int i = 0;
    int j = 0;
    Node *pCurrent = list->pFirst;
    while (pCurrent != NULL) {
        Node *pPre = pCurrent; 
        Node *pTemp = pCurrent->pNext;
        while (pTemp != NULL) {
            if (pCurrent->studentData->studentID < pTemp->studentData->studentID) {
               pCurrent->pNext = pTemp;
               pPre->pNext = pCurrent;
            } 
            else {
                pPre = pTemp;
                pTemp = pTemp->pNext;
            }
        }
    }
}

void freeNode(Node *node) {
    free(node->studentData);
    free(node);
}

void freeList(List *list) {
    Node *pTemp = list->pFirst;
    while (pTemp!= NULL) {
        Node *pCurrent = pTemp;
        pTemp = pTemp->pNext;
        freeNode(pCurrent);
    }
    free(list);
}