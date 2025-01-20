#include "decode from_code.h"
// Hàm chính để kiểm tra xây dựng cây Huffman
int main() {
    // Danh sách các ký tự và mã Huffman tương ứng
    char characters[] = {'A', 'B', 'C', 'D'};
    char* codes[] = {"0", "10", "110", "111"};

    // Số lượng ký tự trong bảng mã
    int n = sizeof(characters) / sizeof(characters[0]);

    // Tạo cây Huffman rỗng
    struct Node* root = createNewNode();

    // Duyệt danh sách mã để xây dựng cây Huffman
    for (int i = 0; i < n; i++) {
        insertCode(root, codes[i], characters[i]);
    }

    // Kiểm tra bằng cách in các mã Huffman từ cây
    char code[100];
    printf("Cây Huffman được xây dựng:\n");
    printHuffmanTree(root, code, 0);

    // Giải phóng bộ nhớ của cây
    freeHuffmanTree(root);

    return 0;
}