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
int stringToInt(char *s); 


// Node APIs
Node *createNode();
void freeNode(Node *node);
void initNode(Node *node, char *student, char *name, int sex, char *address, char *Class, char *major, int day, int month, int year);
void printNode (Node *node);
void inputNode(Node *node);

// List APIs
List *createList();
void readCSVtoList(List *list, char *filename); 
void addNewNode(List *list, Node *node);
void printList(List *list);
void sortListByStudentID(List *list);
void freeList(List *list);
void filterSameBirthDayStudent(List *list);
void findSameBirthDayStudent(List *list);



int main(int argc, char **argv) {
    List *list = createList();

    readCSVtoList(list, "StudentList.csv");

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
    // printf("Size of student data %d\n", sizeof(*newNode->studentData));
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

int stringToInt(char *s) {
    int i = 0, num = 0;
    while (s[i]!= '\0') {
        num = num * 10 + (s[i] - '0');
        i++;
    }
    return num;
}

void initNode(Node *node, char *student, char *name, int sex, char *address, char *Class, char *major, int day, int month, int year) {
    stringCopy(node->studentData->studentID, student);
    stringCopy(node->studentData->name, name);
    node->studentData->sex = sex;
    node->studentData->dateOfBirth.day = day;
    node->studentData->dateOfBirth.month = month;
    node->studentData->dateOfBirth.year = year;
    stringCopy(node->studentData->address, address);
    stringCopy(node->studentData->Class, Class);
    stringCopy(node->studentData->major, major);
}

void addNewNode(List *list, Node *node) {
    Node *pTemp = list->pFirst;
    if (list->pFirst == NULL) {
        list->pFirst = node;
        list->pLast = node;
    }
    else {
        list->pLast->pNext = node;
        list->pLast = node;
    }  
    list->len++;
}

void inputNode(Node *node) {
    if (node == NULL) {
        node = createNode();
    }

    printf("Enter student ID: ");
    fgets(node->studentData->studentID, 10, stdin);

    printf("Enter name: ");
    // getchar();
    fgets(node->studentData->name, 50, stdin); 

    printf("Enter sex (1: Male, 0: Female): ");
    scanf("%d", &node->studentData->sex);
    getchar();

    printf("Enter date of birth (DD MM YYYY): ");
    scanf("%d %d %d", &node->studentData->dateOfBirth.day, &node->studentData->dateOfBirth.month, &node->studentData->dateOfBirth.year);
    getchar(); 

    printf("Enter address: ");
    fgets(node->studentData->address, 100, stdin);

    printf("Enter class: ");
    fgets(node->studentData->Class, 12, stdin);

    printf("Enter major: ");
    fgets(node->studentData->major, 7, stdin);
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
    printf("Finish Creating List\n");
    return list;
}

void printList(List *list) {
    Node *pTemp = list->pFirst;
    int i = 1;
    while (pTemp!= NULL) {
        printf("Student %d: \n", i);
        printNode(pTemp);
        pTemp = pTemp->pNext;
        i++;
    }
}

void sortListByStudentID(List *list) {
    int i = 0;
    int j = 0;
    Node *pCurrent = list->pFirst;
    while (pCurrent != NULL) {
        Node *pPre = pCurrent; 
        Node *pTemp = pCurrent->pNext;
        while (pTemp != NULL) {
            if (stringToInt(pCurrent->studentData->studentID) < stringToInt(pTemp->studentData->studentID)) {
               pCurrent->pNext = pTemp;
               pPre->pNext = pCurrent;
            } 
            else {
                pPre = pTemp;
                pTemp = pTemp->pNext;
            }
        }
        pCurrent = pCurrent->pNext;
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

void filterSameBirthDayStudent(List *list) {
    Node *pCurrent = list->pFirst;
    while (pCurrent!= NULL) {
        Node *pTemp = pCurrent->pNext;
        while (pTemp!= NULL) {
            if (pCurrent->studentData->dateOfBirth.day == pTemp->studentData->dateOfBirth.day && 
                pCurrent->studentData->dateOfBirth.month == pTemp->studentData->dateOfBirth.month && 
                pCurrent->studentData->dateOfBirth.year == pTemp->studentData->dateOfBirth.year) {
                Node *pDelete = pTemp;
                pTemp = pTemp->pNext;
                pCurrent->pNext = pTemp;
                freeNode(pDelete);
                list->len--;
            }
            else {
                pTemp = pTemp->pNext;
            }
        }
        Node *pDelete = pCurrent; 
        pCurrent = pCurrent->pNext;
        freeNode(pDelete);
        list->len--;
    }
}

void findSameBirthDayStudent(List *list) {
    Node *pCurrent = list->pFirst;
    while (pCurrent!= NULL) {
        Node *pTemp = pCurrent->pNext;
        while (pTemp!= NULL) {
            if (pCurrent->studentData->dateOfBirth.day == pTemp->studentData->dateOfBirth.day && 
                pCurrent->studentData->dateOfBirth.month == pTemp->studentData->dateOfBirth.month && 
                pCurrent->studentData->dateOfBirth.year == pTemp->studentData->dateOfBirth.year) {
                printf("Student with same birth day: \n");
                printNode(pCurrent);
                printNode(pTemp);
                break;
            }
            pTemp = pTemp->pNext;
        }
        pCurrent = pCurrent->pNext;
    }
}

void readCSVtoList(List *list, char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return;
    }

    char line[1024];
    while (!feof(fp)) {
        Node *node = createNode();
        fscanf(fp, "%s , %s , %d , %d , %d , %d , %s , %s , %s", 
        node->studentData->studentID, 
        node->studentData->name, 
        &node->studentData->sex, 
        &node->studentData->dateOfBirth.day, 
        &node->studentData->dateOfBirth.month,
        &node->studentData->dateOfBirth.year, 
        node->studentData->address, 
        node->studentData->Class, 
        node->studentData->major);

        addNewNode(list, node);
    }    
}