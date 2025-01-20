/*
    This file of code is created by Minkeisme1204 
    Finished at 18/05/2024
*/

#include "encoder.h"

// Encoder principle following Huffman coding algorithm

struct Node* create_new_Node(char data, float frequency) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node)); // temp is a pseudo Node
    temp->data = data;
    temp->frequency = frequency;
    temp->visit = 0;
    temp->pLeft = NULL, temp->pRight = NULL; 
    return temp; 
}

struct Queue* create_new_Queue(int capacity) {
    struct Queue* temp = (struct Queue*)malloc(sizeof(struct Queue)); // temp is a pseudo Queue
    temp->capacity = capacity; 
    temp->array = (struct Node**)malloc(sizeof(struct Node) * temp->capacity);
    return temp; 
}

int isLeaf(struct Node* root) {
    return  (root->pLeft == NULL && root->pRight == NULL);
}

struct Node* Build_Huffman_Tree(char data[], float freq[], int size) {
    struct Queue *queue = create_new_Queue(size * size); // Initialize the queue

    for (int i = 0; i < size; i++) {
        queue->array[i] = create_new_Node(data[i], freq[i]); // Parse each character and its frequency to the queue
    }

    queue->size = size;

    // Initialize first nodes for each turn pop 2 nodes from queue
    struct Node* default_x = create_new_Node('?', 2.); 
    struct Node* default_y = create_new_Node('?', 2.);

    int i = 0; 
    while (i < size) { 
        struct Node *x = NULL, *y = NULL;
        
        // Set the initial node to x y 
        x = default_x;
        y = default_y;

        // Find the two Nodes with min frequency 
        for (int j = 0; j < size; j++) {
            if (queue->array[j]->visit == 0 && queue->array[j]->frequency <= x->frequency) {
                y = x; 
                x = queue->array[j];
                continue; 
            }
            else if (queue->array[j]->visit == 0 && queue->array[j]->frequency < y->frequency) y = queue->array[j];
        }

        if (x->frequency == 0.) x->ending_sign = 1; // Assign as ending signal. For continuous transmit bits for decoding

        // Create a new node
        struct Node *newNode = create_new_Node('$', x->frequency + y->frequency);
        newNode->pLeft = x; 
        newNode->pRight = y; 

        // Insert new Node to the queue
        queue->array[size] = newNode; 
        size++; 

        // Mark the nodes x y in queue as visited
        x->visit = 1; 
        y->visit = 1; 
        i += 2; 
        
        // printf("%d %d\n", i, size);
        if (i == size - 1) {
            free(default_x);
            free(default_y);
            return newNode; // The tree is the adddress of the root node
        } 
    } 
}

// Print array 
void printArr(FILE *stat, char arr[], int n) {
    for (int i = 0; i < n; ++i) {
        printf("%c", arr[i]);  
        fprintf(stat, "%c", arr[i]);  
    }
    printf("\n");
}

// Print the list of code
int printCodes(FILE *file, FILE *stat, struct Node *root, char arr[], int size, char **code, int *len) {
    if (isLeaf(root)) {
        // printf("%c: %d ", root->data, root->present);
        if (root->data == '\n') fprintf(stat, "ENTER\t\t");
        else if (root->data == PADDING_CHAR) {fprintf(stat, "END SIGN\t");}
        else if (root->data == ' ') {fprintf(stat, "SPACE\t\t");}
        else fprintf(stat, "%c\t\t\t", root->data);
        fprintf(stat, "%f\t\t\t\t", root->frequency);
        printf("%c: ", root->data);
        printArr(stat, arr, size);
        fprintf(stat, "\t\t\t%-6d\n", size);
        unsigned char buffer[BUFFER_SIZE];
        arr[size] = '\0';
        // *length += size; 
        if ((int)root->data != PADDING_CHAR) {
            *len += size;
        } else printf("\n");
        strcpy(code[root->data], arr); 

        sprintf(buffer, "%c %s\n", root->data, arr);
        fprintf(file, buffer);
        return size;
    }
    if (root->pLeft != NULL) {
        arr[size] = '0'; 
        size++;
        printCodes(file, stat, root->pLeft, arr, size, code, len);
        size--;
    }
    if (root->pRight != NULL) {
        arr[size] = '1'; 
        size++;
        printCodes(file, stat, root->pRight, arr, size, code, len);
        size--;
    }
}

int Insert_Huffman_code(FILE *target_file, char *data, float *freq, int size) {// Insert the Code to in heading of the compressed file
    int temp = (sizeof(float) + sizeof(char)) * size; 
    fwrite(&temp, sizeof(short), 1, target_file);
    int sum = 0; 
    for (int i = 0; i < size; i++) {
        fwrite(&data[i], sizeof(char), 1, target_file);
        fwrite(&freq[i], sizeof(float), 1, target_file); 
        sum += sizeof(data[i]) + sizeof(freq[i]); 
    }
    printf("HUFFMAN COST: %d bytes %d\n", sum, temp); 
    return sum; 
}

FILE *encode_file_text(FILE *input_file, char **code, char*data, float *freq, int size) {
    char *buffer = malloc(sizeof(char) * BUFFER_SIZE);
    char *filename = malloc(sizeof(char) * 256);

    int *converter = malloc(sizeof(int) * 8);
    int current = 0; // to track the index of converter

    sprintf(filename, "%s%s", SEND_PATH, "Binary_Compressed.txt");
    FILE *compressed_file = fopen(filename, "w");

    sprintf(filename, "%s%s", SEND_PATH, "Real_Compressed.bin");
    FILE *bin_compressed_file = fopen(filename, "wb");

    int huffman_len = Insert_Huffman_code(bin_compressed_file, data, freq, size); 

    int sum = 0; 
    
    // Read the binary content txt file and convert to a file.bin
    while (fgets(buffer, BUFFER_SIZE, input_file)) { // Buffer now is a line of text 
        for (int i = 0; i < strlen(buffer); i++) {
            fprintf(compressed_file, "%s", code[buffer[i]]);
            for (int j = 0; j < strlen(code[buffer[i]]); j++) {
                if(current == 8) {
                    unsigned char c = 0;
                    for (int k = 0; k < current; k++) c += pow(2, 8 - k - 1) * converter[k]; 
                    for (int k = 0; k < current; k++) converter[k] = 0; 
                    current = 0; 

                    printf("%c", (int)c);
                    // fprintf(bin_compressed_file, "%c", (int)c);

                    size_t element_write = fwrite(&c, sizeof(char), 1, bin_compressed_file);
                    if (element_write != 1) printf("Error writing Bin file\n");
                    sum += 8;
                }
                converter[current] = code[buffer[i]][j] - '0';
                current ++; 
            }
            // sum += strlen(code[buffer[i]]);  
        }
    }

    // printf("\n%s", code['u']);

    printf("\n");
    for (int k = 0; k < current; k++) printf("%d", converter[k]); 
    printf("\n");

    // Padding the ending sign char and fill up to be full of 8 bits
    for (int i = 0; i < strlen(code[data[size - 1]]) || current % 8 != 0; i++) { 
        converter[current] = (i < strlen(code[data[size - 1]])) ? code[data[size - 1]][i] - '0' : '0' - '0'; 
        printf("\n Convert bit %d is %d", current, converter[current]);
        current++;

        if (current == 8) {// Convert binary 8 bits into a character
            printf("\n good ");
            for (int k = 0; k < current; k++) printf("%d", converter[k]); 
            printf("\n");

            unsigned char c = 0;
            for (int k = 0; k < current; k++) c += pow(2, 8 - k - 1) * converter[k]; 
            for (int k = 0; k < current; k++) converter[k] = 0; 
            current = 0; 

            printf("%c", (int)c);
            // fprintf(bin_compressed_file, "%c", (int)c);

            size_t element_write = fwrite(&c, sizeof(char), 1, bin_compressed_file);

            if (element_write != 1) printf("Error writing Bin file\n");
            sum += 8; 
        }
    }

    printf("\n%s %c", code[data[size - 1]], data[size - 1]);

    printf("\nCheck compressed %d bits\n", sum);
    fclose(compressed_file);
    fclose(bin_compressed_file);

}

void free_tree(struct Node *root) {
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

// Test scope 
// int main() {
//     char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
//     float freq[] = {0.0625, 0.0625, 0.25, 0.0625, 0.125, 0.125, 0.25, 0.0625};
//     int *code = malloc(sizeof(int) * 8);

//     printCodes(Build_Huffman_Tree(arr, freq, 8), code, 0);
//     // printf("%f\n", root->pLeft->frequency);

//     free(code);
// }





