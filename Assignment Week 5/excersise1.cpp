#include <iostream>
using namespace std; 
#define RED 0
#define GREEN 1
#define BLUE 2

void quick_sort(int a[], int l, int r) {
    if (l >= r) return; 
    int pivot = a[l];
    int i = l, j = r;
    while (i <= j) {
        while (i < r && a[i] < pivot) i++;
        while (j > l && a[j] > pivot) j--;
        if (i <= j) {
            swap(a[i], a[j]);
            i++;
            j--;
        }
    }
    quick_sort(a, l, j);
    quick_sort(a, i, r);
}

int main() {
    int n; 
    scanf("%d", &n);
    int *a = new int[n]; 
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    quick_sort(a, 0, n - 1); 

    for (int i = 0; i < n; i++) std::cout << a[i] << std::endl;
}
