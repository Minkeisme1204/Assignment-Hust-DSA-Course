#include <stdio.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024

void buildMaxHeap(int *a, int n, int i) {
    if(i == 0) return; 
    int leftChild = 2 * i + 1;
    int rightChild = 2 * i + 2;
    int largest = i;

    if (leftChild < n && a[leftChild] > a[largest]) {
        largest = leftChild;
    }

    if (rightChild < n && a[rightChild] > a[largest]) {
        largest = rightChild;
    }

    if (largest!= i) {
        int temp = a[i];
        a[i] = a[largest];
        a[largest] = temp;
        buildMaxHeap(a, n, largest/2);
    }
    i++;
}

void heapSort(int *a, int n) {
    int i = n - 1; 
    while (i >= 0) {
        buildMaxHeap(a, n, i);
        int temp = a[0];
        a[0] = a[i];
        a[i] = temp;
    }
}

int main(int argc, char **argv) {
    int n; 
    int a[BUFFER_SIZE];

    scanf("%d", &n); 
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    heapSort(a, n);

    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
}