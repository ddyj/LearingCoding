#ifndef __CU_UTILS_H__
#define __CU_UTILS_H__


#include <typeinfo>
#include <iostream>
#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include "helper_cuda.h"

#include <Eigen/Dense>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/SVD>


__device__ void distorted(float *dist_cu, const float x, const float y, 
                                                    float *dx, float *dy,int camera_id);
__global__ void cudep2align(unsigned short* d_depth,
                                unsigned short* d_depth_final,
                                float* P1_cu, float *P2_cu,
                                float *R1_cu, float *dist_cu,
                                int width,int height,
                                int cur_id,int camera_num);

void dep2align(unsigned short* d_depth,
                                unsigned short* d_depth_final,
                                float* P1_cu,float *P2_cu,
                                float *R1_cu, float *dist_cu,
                                int m_width,int m_height,
                                int cur_id,int m_camera_num);

__global__ void cudis2dep(float * dis_cu,unsigned short* dep_cu,float *T_cu,
                                                            int width,int height,
                                                            int cur_id,int camera_num);

void dis2dep(float * dis_cu,unsigned short* dep_cu,float *T_cu,
                            int width,int height,
                            int cur_id,int camera_num);




__global__ void kernelMyMemsetDouble(double *pdData,
                                    double value,
                                    unsigned int width,
                                    unsigned int height,
                                    unsigned int camera_num);
void cuMyMemsetDouble(double *pdData,
                                double value,
                                unsigned int width,
                                unsigned int height,
                                unsigned int camera_num);
__global__ void kernelMyMemsetUC(unsigned char *pdData,
                                 unsigned char value,
                                 unsigned int width,
                                 unsigned int height,
                                 unsigned int camera_num);
void cuMyMemsetUC(unsigned char *pdData,
                             unsigned char value,
                             unsigned int width,
                             unsigned int height,
                             unsigned int camera_num);

__global__ void kernelMyMemsetUS(unsigned short *pdData,
                                 unsigned short value,
                                 unsigned int width,
                                 unsigned int height,
                                 unsigned int camera_num);
void cuMyMemsetUS(unsigned short *pdData,
                             unsigned short value,
                             unsigned int width,
                             unsigned int height,
                             unsigned int camera_num);

void cuMyMemsetFloat(float *pdData,
                                float value,
                                unsigned int width,
                                unsigned int height,
                                unsigned int camera_num);
__global__ void kernelMyMemsetFloat(float *pdData,
                                    float value,
                                    unsigned int width,
                                    unsigned int height,
                                    unsigned int camera_num);



#endif