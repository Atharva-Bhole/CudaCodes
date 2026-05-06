#include<iostream>
#include<omp.h>
using namespace std;

inline void swap_val(int &a, int &b){
    int temp = a;
    a = b;
    b = temp;
}

void sequentialBubbleSort(int *a, int n) {
    for (int i = 0; i < n; i++) {
        bool swapped = false;

        for (int j = 0; j < n - 1; j++) {
            if (a[j] > a[j + 1]) {
                swap_val(a[j], a[j + 1]);
                swapped = true;
            }
        }

        if (!swapped) break;
    }
}

void parallelBubbleSort(int *a, int n){
    for(int i = 0; i < n; i++){
        int first = i % 2;
        int swapped = 0;

        #pragma omp parallel for reduction(|:swapped)
        for(int j = first; j < n-1; j+=2){
            if(a[j] > a[j+1]){
                swap_val(a[j], a[j+1]);
                swapped = 1;
            }
        }

        if(!swapped) break;
    }
}

int main() {
    int n;
    cout << "\nEnter total number of elements: ";
    cin >> n;

    int *a = new int[n];
    int *b = new int[n];  // copy for parallel version

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        b[i] = a[i];  // copy original array
    }
    sequentialBubbleSort(a, n);
    parallelBubbleSort(b, n);

    cout << "\nSorted array (Parallel):\n";
    for (int i = 0; i < n; i++) cout << b[i] << " ";

    delete[] a;
    delete[] b;

    return 0;
}