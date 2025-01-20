/*
    This file of code is created by Minkeisme1204 
    Finished at 18/05/2024
*/

#include "decoder.h"

// Encoder principle following Huffman coding algorithm

struct decNode * create_new_decNode (char data, float frequency) {
    struct decNode * temp = (struct decNode *)malloc(sizeof(struct decNode )); // temp is a pseudo decNode 
    temp->data = data;
    temp->frequency = frequency;
    temp->visit = 0;
    temp->pLeft = NULL, temp->pRight = NULL; 
    return temp; 
}

struct Queue* create_new_Queue(int capacity) {
    struct Queue* temp = (struct Queue*)malloc(sizeof(struct Queue)); // temp is a pseudo Queue
    temp->capacity = capacity; 
    temp->array = (struct decNode **)malloc(sizeof(struct decNode ) * temp->capacity);
    return temp; 
}

int isLeaf(struct decNode * root) {
    return  (root->pLeft == NULL && root->pRight == NULL);
}

struct decNode * Build_Huffman_Tree(char data[], float freq[], int size) {
    struct Queue *queue = create_new_Queue(size * size);
    // pseudo_queue = queue; 

    for (int i = 0; i < size; i++) {
        queue->array[i] = create_new_decNode (data[i], freq[i]);
    }

    // Debug scope
    // for (int i = 0; i < size; i++) {
    //     printf("%c ", queue->array[i]->data);
    // }
    // printf("\n");

    queue->size = size;

    struct decNode * default_x = create_new_decNode ('?', 2.);
    struct decNode * default_y = create_new_decNode ('?', 2.);

    int i = 0; 
    while (i < size) { 
        struct decNode  *x = NULL, *y = NULL;
        
        // Set the initial decNode  to x y 
        x = default_x;
        y = default_y;

        // Find the two decNode s with min frequency 
        for (int j = 0; j < size; j++) {
            if (queue->array[j]->visit == 0 && queue->array[j]->frequency <= x->frequency) {
                y = x; 
                x = queue->array[j];
                continue; 
            }
            else if (queue->array[j]->visit == 0 && queue->array[j]->frequency < y->frequency) y = queue->array[j];
        }

        if (x->frequency == 0.) x->ending_sign = 1;

        // Debug scope 
        // printf("X: %f, Y: %f, size: %d,    %d %d\n", x->frequency, y->frequency, size, isLeaf(x), isLeaf(y));

        // Create a new decNode 
        struct decNode  *newdecNode  = create_new_decNode ('$', x->frequency + y->frequency);
        newdecNode ->pLeft = x; 
        newdecNode ->pRight = y; 

        // Insert newdecNode  to the queue
        queue->array[size] = newdecNode ; 
        size++; 

        // Mark the decNode s x y in queue as visited
        x->visit = 1; 
        y->visit = 1; 
        i += 2; 
        
        // printf("%d %d\n", i, size);
        if (i == size - 1) {

            free(default_x);
            free(default_y);
            return newdecNode ;
        } 
    } 

}

// Print array 
void printArr(char arr[], int n) {
    for (int i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
}

// Print the list of code
char** printCodes(FILE *file,struct decNode  *root, char arr[], int size, char **code) {
    if (isLeaf(root)) {
        // printf("%c: %d ", root->data, root->present);
        // printArr(arr, size);
        unsigned char buffer[BUFFER_SIZE];
        arr[size] = '\0';

        sprintf(buffer, "%d %s\n", root->data, arr);
        fprintf(file, buffer);
        return code;
    }
    if (root->pLeft != NULL) {
        arr[size] = '0'; 
        size++;
        printCodes(file, root->pLeft, arr, size, code);
        size--;
    }
    if (root->pRight != NULL) {
        arr[size] = '1'; 
        size++;
        printCodes(file, root->pRight, arr, size, code);
        size--;
    }
}

void free_tree(struct decNode  *root) {
    if (isLeaf(root)) {
        free(root);
        return;
    }
    if (root->pLeft != NULL) {
        free_tree(root->pLeft);
    }
    if (root->pRight != NULL) {
        free_tree(root->pRight);
    }
}

FILE *decode_file_text(FILE *encoded_file, struct decNode *decRoot) {
    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
    char *filename = malloc(sizeof(char) * 256);
    sprintf(filename, "%s%s", RECEIVE_PATH, "Decoded_textFIle.txt");
    FILE *decFile = fopen(filename, "w");
    free(filename);
    
    struct decNode *current_node = decRoot; 

    while (fgets(buffer, BUFFER_SIZE, encoded_file)) {
        for (int i = 0; i < strlen(buffer); i++) {
            if (buffer[i] == '0') {
                current_node = current_node->pLeft;
            }
            else if (buffer[i] == '1') {
                current_node = current_node->pRight;
            } 
            if (isLeaf(current_node)) {
                printf("%c", current_node->data);

                // Check if decoded char is ending sign
                if (current_node->data == 1) {
                    printf("\nGot the ending sign !!!\n");
                    current_node = decRoot;
                    fclose(decFile);
                    return decFile;
                }

                fprintf(decFile, "%c", current_node->data);
                current_node = decRoot; 
            }
        }
    }
    fclose(decFile);
    return decFile;
}

int Extract_Huffman_code(FILE *bin_file, char **ch, float **f) {
    int size; 
    fseek(bin_file, 0, SEEK_SET);
    fread(&size, sizeof(short), 1, bin_file); // Extract the size of the Codes 

    int len = size / (sizeof(float) + sizeof(char)); 

    char *data = malloc(sizeof(char) * len);
    float *freq = malloc(sizeof(float) * len); 

    int i = 0; 
    int check = 0; 

    while (check < size) {
        char ch; 
        float f; 
        fread(&ch, sizeof(char), 1, bin_file); 
        fread(&f, sizeof(float), 1, bin_file);
        // printf("%c: %f\n", ch, f);

        data[i] = ch; 
        freq[i] = f; 

        check += sizeof(char) + sizeof (float);
        i++;
    }
    *ch = data; 
    *f = freq; 
    return len; 
}

void Convert2binary(char data, char **buffer) {
    char bin[9]; // create a string length of 8 char 
    int i = 8 - 1; // set iteration variable 
    unsigned int temp = data; 
    bin[8] = '\0'; // mark the end of the string
    for (int i = 0; i < 8; i++) bin[i] = '0'; // preset the string 

    // File the string with the binary of data 
    while (temp != 0) {
        int r = temp % 2; 
        bin[i] = r + '0';
        i--; 
        temp /= 2; 
    }

    strcpy(*buffer, bin); // Copy string to the buffer (target output) 
    
}

// Test scope 
// int main() {
//     char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
//     float freq[] = {0.0625, 0.0625, 0.25, 0.0625, 0.125, 0.125, 0.25, 0.0625};
//     int *code = malloc(sizeof(int) * 8);

//     printCodes(Build_Huffman_Tree(arr, freq, 8), code, 0);
//     // printf("%f\n", root->pLeft->frequency);

//     free(code);
// }





