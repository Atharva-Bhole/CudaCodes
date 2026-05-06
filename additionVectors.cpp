#include <iostream>
#include <cuda_runtime.h>
using namespace std;

__global__ void addVectors(int *A, int *B, int *C, int n){
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(i < n){
        C[i] = A[i] + B[i];
    }
}

int main()
{
    int n = 1000000;
    int *A, *B, *C;
    int size = n * sizeof(int);

    // Pinned host memory
    cudaMallocHost(&A, size);
    cudaMallocHost(&B, size);
    cudaMallocHost(&C, size);

    for(int i = 0; i < n; i++){
        A[i] = i;
        B[i] = i * 2;
    }

    int *devA, *devB, *devC;

    cudaMalloc(&devA, size);
    cudaMalloc(&devB, size);
    cudaMalloc(&devC, size);

    cudaMemcpy(devA, A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(devB, B, size, cudaMemcpyHostToDevice);

    int blockSize = 256;
    int numBlocks = (n + blockSize - 1) / blockSize;

    // FIXED kernel launch
    addVectors<<<numBlocks, blockSize>>>(devA, devB, devC, n);

    cudaMemcpy(C, devC, size, cudaMemcpyDeviceToHost);

    for(int i = 0; i < 10; i++){
        cout << C[i] << " ";
    }
    cout << endl;

    cudaFree(devA);
    cudaFree(devB);
    cudaFree(devC);

    cudaFreeHost(A);
    cudaFreeHost(B);
    cudaFreeHost(C);

    return 0;
}