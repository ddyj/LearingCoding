#include"cu_utils.h"

__device__ void distorted(float *dist_cu, const float x, const float y, 
                                                    float *dx, float *dy,int camera_id) {
    const float k1 = dist_cu[camera_id*5];
    const float k2 = dist_cu[camera_id*5+1];
    const float p1 = dist_cu[camera_id*5+2];
    const float p2 = dist_cu[camera_id*5+3];
    
    const float u2 = x * x;
    const float uv = x * y;
    const float v2 = y * y;
    const float r2 = u2 + v2;
    const float r4 = r2 * r2;
    const float r6 = r4 * r2;

    const float radial = (float(1) + k1 * r2 + k2 * r4);
    (*dx) = x * radial + float(2) * p1 * uv + p2 * (r2 + float(2) * u2) - x;
    (*dy) = y * radial + float(2) * p2 * uv + p1 * (r2 + float(2) * v2) - y;
    //printf("dx:%f - dy: %f\n",(*dx),(*dy));
}

__global__ void cudep2align(unsigned short* d_depth,
                                unsigned short* d_depth_final,
                                float* P1_cu, float *P2_cu,
                                float *R1_cu, float *dist_cu,
                                int width,int height,
                                int cur_id,int camera_num){
    unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;  //u
    unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;  //v

    if(x>=width || y>=height){return;}

    float dep = static_cast<float>(d_depth[cur_id*width*height+y*width+x])/1000.0;
    if(dep<=0){return ;}
    int camera_id = cur_id % camera_num;    
    
    float x1,y1,z1; //矫正相机坐标系    
    float x2,y2,z2;//原始相机系

    //真实相机坐标系->增加畸变
    float x3 = 0;
    float y3 = 0;

    float x4,y4;//加畸变后转到像素坐标系

    x1 = dep * (x-P1_cu[camera_id*12+2])/(P1_cu[camera_id*12]+1e-20);
    y1 = dep * (y-P1_cu[camera_id*12+6])/(P1_cu[camera_id*12+5]+1e-20);
    z1 = dep ;

    x2 = R1_cu[camera_id*9]*x1 + R1_cu[camera_id*9+1] *y1 + R1_cu[camera_id*9+2]*z1 ;
    y2 = R1_cu[camera_id*9+3]*x1 + R1_cu[camera_id*9+4] *y1 + R1_cu[camera_id*9+5]*z1;
    z2 = R1_cu[camera_id*9+6]*x1 + R1_cu[camera_id*9+7] *y1 + R1_cu[camera_id*9+8]*z1 ;
    float x_ = x2 / (z2 + 1e-20);
    float y_ = y2 / (z2 + 1e-20);
    distorted(dist_cu, x_, y_, &x3, &y3,camera_id);    
    x3 += x_;
    y3 += y_;

    x4 = P2_cu[camera_id*9] * x3 +  P2_cu[camera_id*9 + 2]; 
    y4 = P2_cu[camera_id*9+4] * y3 +  P2_cu[camera_id*9 + 5]; 

    if(x4>0 && x4<width && y4>0 && y4<height)
    {
        int u = x4;
        int v = y4;
        d_depth_final[cur_id*width*height+v*width+u] = static_cast<unsigned short>(z2*1000.0);
    }
}
                                

void dep2align(unsigned short* d_depth,
                                unsigned short* d_depth_final,
                                float* P1_cu,float *P2_cu,
                                float *R1_cu, float *dist_cu,
                                int m_width,int m_height,
                                int cur_id,int m_camera_num){

    //use 2*2的cuda核  通过xy二维控制图像
    dim3 threads(32, 32);
    unsigned int blockX = (threads.x + m_width - 1) / threads.x;
    unsigned int blockY = (threads.y + m_height - 1) / threads.y;
    dim3 blocks(blockX, blockY);

    cudep2align<<<blocks, threads>>>(d_depth,
                                            d_depth_final,
                                            P1_cu,P2_cu,R1_cu,dist_cu,
                                             m_width,m_height,
                                             cur_id,m_camera_num);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());

}







__global__ void cudis2dep(float * dis_cu,unsigned short* dep_cu,float *T_cu,
                                                            int width,int height,
                                                            int cur_id,int camera_num){
    unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;  //u
    unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;  //v

    if(x>=width || y>=height){return;}

    float dis = static_cast<float>(dis_cu[cur_id*width*height+y*width+x]);
    int camera_id = cur_id % camera_num;
    //printf("%f - %f - %f \n",T_cu[camera_id*3],T_cu[camera_id*3+1],T_cu[camera_id*3+2]);
    float dep =  T_cu[camera_id*3] * T_cu[camera_id*3+1] / (dis + T_cu[camera_id*3+2]);
    //printf("%f - %f \n",dis,dep);
    if(dep<=5000 && dep>0){
        dep_cu[cur_id*width*height+y*width+x] = static_cast<unsigned short>(dep);
    }
}


void dis2dep(float * dis_cu,unsigned short* dep_cu,float *T_cu,
                            int width,int height,
                            int cur_id,int camera_num){
    //use 2*2的cuda核  通过xy二维控制图像
    dim3 threads(32, 32);
    unsigned int blockX = (threads.x + width - 1) / threads.x;
    unsigned int blockY = (threads.y + height - 1) / threads.y;
    dim3 blocks(blockX, blockY);

    cudis2dep<<<blocks, threads>>>(dis_cu,
                                             dep_cu,T_cu,
                                             width,height,
                                             cur_id,camera_num);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());
}



__global__ void kernelMyMemsetFloat(float *pdData,
                                    float value,
                                    unsigned int width,
                                    unsigned int height,
                                    unsigned int camera_num)
{
    unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= width || y >= height)
    {
        return;
    }
    for (int  i = 0; i < camera_num; i++)
    {
        unsigned int offset = i*width*height +y * width + x;
        pdData[offset] = value;
    }
}
void cuMyMemsetFloat(float *pdData,
                                float value,
                                unsigned int width,
                                unsigned int height,
                                unsigned int camera_num)
{
    //use 2*2的cuda核  通过xy二维控制图像
    dim3 threads(32, 32);
    unsigned int blockX = (threads.x + width - 1) / threads.x;
    unsigned int blockY = (threads.y + height - 1) / threads.y;
    dim3 blocks(blockX, blockY);

    kernelMyMemsetFloat<<<blocks, threads>>>(pdData,
                                             value,
                                             width,
                                             height,
                                             camera_num);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());
};

__global__ void kernelMyMemsetDouble(double *pdData,
                                    double value,
                                    unsigned int width,
                                    unsigned int height,
                                    unsigned int camera_num)
{
    unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= width || y >= height)
    {
        return;
    }
    for (int  i = 0; i < camera_num; i++)
    {
        unsigned int offset = i*width*height +y * width + x;
        pdData[offset] = value;
    }
}
void cuMyMemsetDouble(double *pdData,
                                double value,
                                unsigned int width,
                                unsigned int height,
                                unsigned int camera_num)
{
    //use 2*2的cuda核  通过xy二维控制图像
    dim3 threads(32, 32);
    unsigned int blockX = (threads.x + width - 1) / threads.x;
    unsigned int blockY = (threads.y + height - 1) / threads.y;
    dim3 blocks(blockX, blockY);

    kernelMyMemsetDouble<<<blocks, threads>>>(pdData,
                                             value,
                                             width,
                                             height,
                                             camera_num);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());
};

__global__ void kernelMyMemsetUC(unsigned char *pdData,
                                 unsigned char value,
                                 unsigned int width,
                                 unsigned int height,
                                 unsigned int camera_num)
{
    unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;
    if (x >= width || y >= height)
    {
        return;
    }
    for (int  i = 0; i < camera_num; i++)
    {
        unsigned int offset = i*width*height +y * width + x;
        pdData[offset] = value;
    }
}
void cuMyMemsetUC(unsigned char *pdData,
                             unsigned char value,
                             unsigned int width,
                             unsigned int height,
                             unsigned int camera_num)
{
    dim3 threads(32, 32);
    unsigned int blockX = (threads.x + width - 1) / threads.x;
    unsigned int blockY = (threads.y + height - 1) / threads.y;
    dim3 blocks(blockX, blockY);

    kernelMyMemsetUC<<<blocks, threads>>>(pdData,
                                          value,
                                          width,
                                          height,
                                          camera_num );
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());
}
__global__ void kernelMyMemsetUS(unsigned short *pdData,
                                 unsigned short value,
                                 unsigned int width,
                                 unsigned int height,
                                 unsigned int camera_num)
{
    unsigned int u = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int v = blockIdx.y * blockDim.y + threadIdx.y;
    if (u >= width || v >= height)
    {
        return;
    }
    for (int  i = 0; i < camera_num; i++)
    {
        unsigned int offset = i*width*height +v * width + u;
        pdData[offset] = value;
    }
}
void cuMyMemsetUS(unsigned short *pdData,
                             unsigned short value,
                             unsigned int width,
                             unsigned int height,
                             unsigned int camera_num)
{
    
    dim3 threads(32, 32);
    
    unsigned int blockX = (threads.x + width - 1) / threads.x;
    unsigned int blockY = (threads.y + height - 1) / threads.y;
    dim3 blocks(blockX, blockY);
    kernelMyMemsetUS<<<blocks, threads>>>(pdData,
                                          value,
                                          width,
                                          height,
                                          camera_num );
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());
}