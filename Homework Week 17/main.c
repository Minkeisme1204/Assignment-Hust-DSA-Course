#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node* pLeft;
    struct Node* pRight;
    int height;
} Node;

Node* createNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->pLeft = NULL;
    node->pRight = NULL;
    node->height = 1; 
    return node;
}

int getHeight(Node* node) {
    return node ? node->height : 0;
}

int getBalance(Node* node) {
    return node ? getHeight(node->pLeft) - getHeight(node->pRight) : 0;
}

void updateHeight(Node* node) {
    if (node) {
        node->height = 1 + (getHeight(node->pLeft) > getHeight(node->pRight) ? getHeight(node->pLeft) : getHeight(node->pRight));
    }
}

Node* rotatepRight(Node* y) {
    Node* x = y->pLeft;
    Node* temp = x->pRight;

    x->pRight = y;
    y->pLeft = temp;

    updateHeight(y);
    updateHeight(x);

    return x;
}

Node* rotatepLeft(Node* x) {
    Node* y = x->pRight;
    Node* temp = y->pLeft;

    y->pLeft = x;
    x->pRight = temp;

    updateHeight(x);
    updateHeight(y);

    return y;
}

Node* balanceTree(Node* node) {
    updateHeight(node);
    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->pLeft) >= 0)
        return rotatepRight(node);

    if (balance < -1 && getBalance(node->pRight) <= 0)
        return rotatepLeft(node);

    if (balance > 1 && getBalance(node->pLeft) < 0) {
        node->pLeft = rotatepLeft(node->pLeft);
        return rotatepRight(node);
    }

    if (balance < -1 && getBalance(node->pRight) > 0) {
        node->pRight = rotatepRight(node->pRight);
        return rotatepLeft(node);
    }

    return node;
}

Node* insertNode(Node* node, int key) {
    if (!node)
        return createNode(key);

    if (key < node->key)
        node->pLeft = insertNode(node->pLeft, key);
    else if (key > node->key)
        node->pRight = insertNode(node->pRight, key);
    else
        return node;

    return balanceTree(node);
}

void inorderTraversal(Node* root) {
    if (root) {
        inorderTraversal(root->pLeft);
        printf("%d ", root->key);
        inorderTraversal(root->pRight);
    }
}

void preorderTraversal(Node* root) {
    if (root) {
        printf("%d ", root->key);
        preorderTraversal(root->pLeft);
        preorderTraversal(root->pRight);
    }
}

void postorderTraversal(Node* root) {
    if (root) {
        postorderTraversal(root->pLeft);
        postorderTraversal(root->pRight);
        printf("%d ", root->key);
    }
}

int main() {
    Node* root = NULL;
    int arr[] = {10, 20, 30, 40, 50, 25};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Add nodes of tree and print\n");
    for (int i = 0; i < n; i++) {
        root = insertNode(root, arr[i]);
        printf("AVL tree after adding %d (Inorder): ", arr[i]);
        inorderTraversal(root);
        printf("\n");
    }

    printf("\nPre-order iteration:\n");
    preorderTraversal(root);
    printf("\n");

    printf("\nInorder iteration:\n");
    inorderTraversal(root);
    printf("\n");

    printf("\nPostorder iteration:\n");
    postorderTraversal(root);
    printf("\n");

    return 0;
}
