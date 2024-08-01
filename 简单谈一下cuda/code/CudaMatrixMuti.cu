#include <iostream>

#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>


using namespace std;




struct Martix
{
    int width;
    int height;
    float* elements;
};

// 获取矩阵的(row, col)元素
__device__ float getElement(Martix* A,int row,int col){
    return A->elements[row*A->width + col];
}
// 获取矩阵的(row, col)元素
__device__ void setElement(Martix* A,int row,int col,float value){
    A->elements[row*A->width + col] = value;
}

//矩阵相乘
__global__ void matMulKernel(Martix* A,Martix* B,Martix* C){
    float Cvalue = 0.0;
    int row = threadIdx.y + blockIdx.y * blockDim.y;
    int col = threadIdx.x + blockIdx.x * blockDim.x;
    if(col >= A->width || row >= A->height|| col >= B->width || row >= B->height){
        return;
    }
    for(int i=0;i<A->width;++i){
        Cvalue += getElement(A,row,i) * getElement(B,i,col);
    }
    setElement(C,row,col,Cvalue); 
}

int main(){

    int width;
    int height;
    cin >> width;
    cin >> height;

    Martix* A;
    Martix* B;
    Martix* C;

    //申请托管内存
    cudaMallocManaged((void**)&A,sizeof(Martix));
    cudaMallocManaged((void**)&B,sizeof(Martix));
    cudaMallocManaged((void**)&C,sizeof(Martix));
    int nBytes = width * height * sizeof(float);
    cudaMallocManaged((void**)&A->elements,nBytes);
    cudaMallocManaged((void**)&B->elements,nBytes);
    cudaMallocManaged((void**)&C->elements,nBytes);

    //初始化数据
    A->height = height;
    A->width = width;
    B->height = height;
    B->width = width;
    C->height = height;
    C->width = width;

    for(int i=0;i<width*height;++i){
        A->elements[i] = 1.0;
        B->elements[i] = 2.0;
    }
    //定义kernel的执行配置
    dim3 blockSize(32,32);
    dim3 gridSize((width + blockSize.x-1)/blockSize.x,(height + blockSize.y-1)/blockSize.y);

    matMulKernel<< <gridSize,blockSize >> >(A,B,C);
    // 同步device 保证结果能正确访问
    cudaDeviceSynchronize();
    //检查执行结果
    for(int i=0;i<width*height;++i){
        std::cout << C->elements[i]<< " ";
        if( (i+1) % width == 0){
            std::cout << "\b" << std::endl;
        }
    } 
    return 0;
}