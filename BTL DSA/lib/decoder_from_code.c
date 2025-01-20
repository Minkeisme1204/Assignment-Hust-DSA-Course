#include "decode from_code.h"



// Hàm tạo một nút mới
struct Node* createNewNode() {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->character = '\0';  // Đặt ký tự mặc định là null
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Hàm xây dựng cây Huffman từ danh sách mã
void insertCode(struct Node* root, char* code, char character) {
    struct Node* current = root;
    for (int i = 0; code[i] != '\0'; i++) {
        if (code[i] == '0') {
            if (current->left == NULL) {
                current->left = createNewNode();
            }
            current = current->left;
        } else if (code[i] == '1') {
            if (current->right == NULL) {
                current->right = createNewNode();
            }
            current = current->right;
        }
    }
    // Gán ký tự tại nút lá
    current->character = character;
}

// Hàm duyệt cây Huffman để kiểm tra
void printHuffmanTree(struct Node* root, char* code, int depth) {
    if (root == NULL) return;
    if (root->character != '\0') {
        code[depth] = '\0';
        printf("Ký tự: %c, Mã Huffman: %s\n", root->character, code);
    }
    code[depth] = '0';
    printHuffmanTree(root->left, code, depth + 1);
    code[depth] = '1';
    printHuffmanTree(root->right, code, depth + 1);
}

// Hàm giải phóng bộ nhớ của cây Huffman
void freeHuffmanTree(struct Node* root) {
    if (root == NULL) return;
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}

