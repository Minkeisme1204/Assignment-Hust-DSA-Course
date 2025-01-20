#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "config.h"

struct decNode  {
    char data; 
    float frequency;  
    int visit;
    int ending_sign; 
    // visit is used to check if the decNode  is visited in Queue
    // present is the number of character data in the text
    struct decNode  *pLeft, *pRight; 
};

struct Queue {
    struct decNode  **array; 
    int size, capacity, visit;  
};

// struct Queue *pseudo_queue;
/*
data is character 
freq is probability of occurrence
size is number of characters 
*/
struct decNode * create_new_decNode (char data, float frequency);
struct Queue* create_new_Queue(int capacity);
int isLeaf(struct decNode * root);
struct decNode *Build_Huffman_Tree(char data[], float freq[], int size);
void printArr(char arr[], int n);
char** printCodes(FILE *file,struct decNode  *root, char arr[], int size, char **code);
void free_tree(struct decNode  *root);
FILE *decode_file_text(FILE *encoded_file, struct decNode *decRoot);
int Extract_Huffman_code(FILE *bin_file, char **ch, float **f);
void Convert2binary(char data, char **buffer);