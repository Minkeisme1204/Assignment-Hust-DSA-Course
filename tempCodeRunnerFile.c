
void heapSort(int *a, int n) {
    int i = n - 1; 
    while (i >= 0) {
        buildMaxHeap(a, n, i);
        int temp = a[0];
        a[0] = a[i];
        a[i] = temp;
    }
}