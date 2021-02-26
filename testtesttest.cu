#include<stdio.h>

__global__ void test();

__syncthreads();
cudaFree(variable);
cudaMemcpy(dest, src, bytes, cudaMemcpyHostToDevice);
cudaMalloc((void**)&variable, bytes);
cudaMallocManaged((void**)&variable, bytes);
__syncthreads();



int main(){
}