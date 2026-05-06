#include <iostream>
#include <omp.h>
using namespace std;

#define THRESHOLD 1000

void merge(int a[], int l, int mid, int r) {
    int n = r - l + 1;
    int* temp = new int[n];

    int i = l, j = mid + 1, k = 0;

    while (i <= mid && j <= r) {
        if (a[i] <= a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }

    while (i <= mid) temp[k++] = a[i++];
    while (j <= r)   temp[k++] = a[j++];

    for (int t = 0; t < n; t++)
        a[l + t] = temp[t];

    delete[] temp;
}

void mergeSortParallel(int a[], int l, int r) {
    if (l >= r) return;

    if (r - l < THRESHOLD) {
        int mid = (l + r) / 2;
        mergeSortParallel(a, l, mid);
        mergeSortParallel(a, mid + 1, r);
        merge(a, l, mid, r);
        return;
    }

    int mid = (l + r) / 2;

    #pragma omp task
    mergeSortParallel(a, l, mid);

    #pragma omp task
    mergeSortParallel(a, mid + 1, r);

    #pragma omp taskwait
    merge(a, l, mid, r);
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    int* a = new int[n];

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) cin >> a[i];

    #pragma omp parallel
    {
        #pragma omp single
        mergeSortParallel(a, 0, n - 1);
    }

    cout << "\nSorted array:\n";
    for (int i = 0; i < n; i++) cout << a[i] << " ";

    delete[] a;
    return 0;
}