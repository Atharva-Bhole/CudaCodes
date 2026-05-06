#include<iostream>
#include<omp.h>
using namespace std;


int binarySearchRange(int *a, int low, int high, int key){
    while(low<=high){
        int mid = low + (high - low) / 2;

        if(a[mid] == key){
            return mid;
        }
        else if(a[mid] < key){
            low = mid + 1;
        }
        else{
            high = mid - 1;
        }
    }
    return -1;
}

int parallel_binary_search(int *a, int n, int key){
    int mid = n/2;
    int loc = -1;

    #pragma omp parallel sections shared(loc)
    {
        #pragma omp section
        {
            int res = binarySearchRange(a, 0, mid, key);
            if(res != -1){
                #pragma omp critical
                loc = res;
            }
        }

        #pragma omp section
        {
            int res = binarySearchRange(a, mid + 1, n-1, key);
            if(res != -1){
                #pragma omp critical
                loc = res;
            }
        }
    }

    return loc;
}

int main(){
    int n, key;
    cout<<"\nEnter total number of elements:";
    cin>>n;

    int *a = new int[n];

    cout<<"Enter sorted elements: \n";
    for(int i = 0; i < n; i++){
        cin>>a[i];
    }

    cout<<"Enter key to find:";
    cin>>key;

    int loc = parallel_binary_search(a, n, key);

    if(loc == -1) cout<<"Key not found";
    else cout<<"Key found at position: "<<loc+1<<"\n";

    delete[] a;
    return 0;
}