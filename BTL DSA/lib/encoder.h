#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "config.h"

struct Node {
    char data; // Character 
    float frequency;  // Frequency
    int visit;
    int ending_sign; 
    // visit is used to check if the node is visited in Queue
    struct Node *pLeft, *pRight; 
};

struct Queue {
    struct Node **array; 
    int size, capacity, visit;  
};

// struct Queue *pseudo_queue;

struct Node* create_new_Node(char data, float frequency);
struct Queue* create_new_Queue(int capacity); 
int isLeaf(struct Node* root);
struct Node* Build_Huffman_Tree(char data[], float freq[], int size);
/*
data is character 
freq is probability of occurrence
present[i] is the number of character data[i]
size is number of characters 
*/

int printCodes(FILE *file, FILE *stat, struct Node *root, char arr[], int size, char **code, int *len);

void free_tree(struct Node *root);

FILE *encode_file_text(FILE *file, char **code, char*data, float *freq, int size);