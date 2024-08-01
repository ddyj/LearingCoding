#ifndef __CUDARESOURCE_H__
#define __CUDARESOURCE_H__

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include <Eigen/Dense>
#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>

#include <cuda_runtime_api.h>
#include <driver_functions.h>
#include "helper_cuda.h"

#include"stdlib.h"
#include "cu_utils.h"

class cudaResource
{
private:
    int m_width;
    int m_height;
    int m_seq_num;
    int m_camera_num;
    std::string m_dis_path;
    std::string m_depth_path;

    //cuda
    float *d_dis;
    unsigned short *d_depth;
    unsigned short *d_depth_final;
    float *T_cu; //两相机之间相对T

    float *P1_cu;
    float *P2_cu;
    float *R1_cu;
    float *dist_cu;

public:


    using Ptr = std::shared_ptr<cudaResource>;
    static Ptr GetInstance(){
        static Ptr ptr = std::make_shared<cudaResource>();
        return ptr;
    } 

    void init(int width,int height,int camera_num,int seq_num,std::string dis_path,std::string depth_path);
    void loadimg(int start,int step);
    void cal_baseline(std::string pose_path);

    void Startdis2dep(int start,int step,std::string save_path);
    void SaveDepimg(int start,int step,std::string save_path);

    void ReadUndisPose(std::string pose_path);


    cudaResource(/* args */);
    ~cudaResource();
};












#endif