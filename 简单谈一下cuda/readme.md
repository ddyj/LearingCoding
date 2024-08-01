[toc]

# 什么是cuda编程

在CUDA中，host和device是两个重要的概念，我们用host指代CPU及其内存，而用device指代GPU及其内存。CUDA程序中既包含host程序，又包含device程序，它们分别在CPU和GPU上运行。

一个简单的CUDA编程执行流程如下
1.分配host内存，并进行数据初始化；
2.分配device内存，并从host将数据拷贝到device上；
3.调用CUDA的核函数在device上完成指定的运算；
4.将device上的运算结果拷贝到host上；
5.释放device和host上分配的内存。

__global__：在device上执行，从host中调用（一些特定的GPU也可以从device上调用），返回类型必须是void，不支持可变参数参数，不能成为类成员函数。注意用__global__定义的kernel是异步的，这意味着host不会等待kernel执行完就执行下一步。
__device__：在device上执行，单仅可以从device中调用，不可以和__global__同时用。
__host__：在host上执行，仅可以从host上调用，一般省略不写，不可以和__global__同时用，但可和__device__，此时函数会在device和host都编译。

# 深度理解kernel

kernel在device上执行时实际上是启动很多线程，一个kernel所启动的所有线程称为一个网格（grid），同一个网格上的线程共享相同的全局内存空间，grid是线程结构的第一层次，而网格又可以分为很多线程块（block），一个线程块里面包含很多线程，这是第二个层次


如下图所示 是一个grid和block都是2-dim的线程组织
<img src="https://pic1.zhimg.com/80/v2-aa6aa453ff39aa7078dde59b59b512d8_720w.webp">

核心难点在于如何确认到线程ID
例如在上图中 Thread(1,1) 其满足
```c++
threadIdx.x = 1
threadIdx.y = 1
blockIdx.x = 1
blockIdx.y = 1
```

而对于三维的block (Dx,Dy,Dz),线程xyz的ID值为 x + y*Dx + z*Dx*Dy

kernel的这种线程组织结构天然适合vector,matrix等运算

# 案例 矩阵乘法

设输入矩阵为A、B，要得到C = A  x  B  
实现思路是每一个线程计算C的一个元素Cij，对于矩阵运算，应该选用grid和block为2-D的


注意这里是如何获取到threadid的  是通过二维的获取方法 这种方式也常常被使用于图像的加速
```c++
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
```