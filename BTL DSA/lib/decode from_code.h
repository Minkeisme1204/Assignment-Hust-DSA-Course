#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cấu trúc nút trong cây Huffman
struct Node {
    char character; // Ký tự tại nút lá (nếu có)
    struct Node *left, *right; // Con trỏ đến hai nhánh trái và phải
};
struct Node* createNewNode();
void insertCode(struct Node* root, char* code, char character);
void printHuffmanTree(struct Node* root, char* code, int depth);
void freeHuffmanTree(struct Node* root);