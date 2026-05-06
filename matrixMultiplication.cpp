#include<cuda_runtime.h>
#include<iostream>
using namespace std;

__global__ void matmul(const int *a, const int *b, int *c, int n){
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    if(row < n && col < n){
        int value = 0;
        for(int k = 0; k < n; k++){
            value += a[row * n + k] * b[k * n + col];
        }
        c[row * n + col] = value;
    }
}

int main(){
    int n = 512;
    size_t size = n * n * sizeof(int);

    int *a, *b, *c;
    int *devA, *devB, *devC;

    cudaMallocHost(&a, size);
    cudaMallocHost(&b, size);
    cudaMallocHost(&c, size);

    cudaMalloc(&devA, size);
    cudaMalloc(&devB, size);
    cudaMalloc(&devC, size);

    for(int i =0 ; i < n; i++){
        for(int j = 0; j < n; j++){
            a[i*n+j] = i*n+j;
            b[i*n+j] = j*n+i;
        }
    }

    cudaMemcpy(devA, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(devB, b, size, cudaMemcpyHostToDevice);
    dim3 block(16, 16);
    dim3 grid((n + block.x - 1) / block.x,
              (n + block.y - 1) / block.y);

    matmul<<<grid, block>>>(devA, devB, devC, n);
    cudaDeviceSynchronize();
    cudaMemcpy(c, devC, size, cudaMemcpyDeviceToHost);
     cout << "Top-left 10x10 result:\n";
    for (int i = 0; i < min(10, n); i++) {
        for (int j = 0; j < min(10, n); j++) {
            cout << c[i * n + j] << " ";
        }
        cout << endl;
    }

    cudaFree(devA);
    cudaFree(devB);
    cudaFree(devC);

    cudaFreeHost(a);
    cudaFreeHost(b);
    cudaFreeHost(c);

    return 0;

}