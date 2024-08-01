#include"cudaResource.h"




void cudaResource::init(int width,int height,int camera_num,int seq_num,std::string dis_path,std::string depth_path){
    m_width = width;
    m_height = height;
    m_camera_num  = camera_num;
    m_seq_num = seq_num;
    m_dis_path = dis_path;
    m_depth_path = depth_path;
    // std::cout<<m_camera_num<<std::endl;
    //cuda空间声明
    checkCudaErrors(cudaMalloc((void **)&T_cu, m_camera_num * 3 * sizeof(float)));
    //undis
    checkCudaErrors(cudaMalloc((void **)&P1_cu, m_camera_num * 12 * sizeof(float)));
    checkCudaErrors(cudaMalloc((void **)&P2_cu, m_camera_num * 9 * sizeof(float)));
    checkCudaErrors(cudaMalloc((void **)&R1_cu, m_camera_num * 9 * sizeof(float)));
    checkCudaErrors(cudaMalloc((void **)&dist_cu, m_camera_num * 5 * sizeof(float)));

}


void cudaResource::loadimg(int start,int step){
    if(d_depth){
        cudaFree(d_depth);
    }
    if(d_depth_final){
        cudaFree(d_depth_final);
    }
    if(d_dis){
        cudaFree(d_dis);
    }
    
    checkCudaErrors(cudaMalloc((void **)&d_dis, m_camera_num * step * m_width* m_height * sizeof(float)));

    // std::cout << "m_camera_num: " << m_camera_num << ", m_width: " << m_width << ", m_height" << m_height << ", step: " << step << std::endl;
    
    checkCudaErrors(cudaMalloc((void **)&d_depth, m_camera_num * step* m_width*m_height * sizeof(unsigned short)));
    cuMyMemsetUS(d_depth,0,m_width,m_height,m_camera_num*step);

    checkCudaErrors(cudaMalloc((void **)&d_depth_final, m_camera_num * step* m_width*m_height * sizeof(unsigned short)));
    cuMyMemsetUS(d_depth_final,0,m_width,m_height,m_camera_num*step);
    


    for(int id= start;id<start + step;id++){
        for(int cam_id=0;cam_id<m_camera_num;cam_id++){
            char imgPath[255];
            sprintf(imgPath,m_dis_path.c_str(),id,(cam_id * 2));
            //std::cout<<imgPath<<std::endl;
            cv::Mat disp_img = cv::imread(imgPath,cv::IMREAD_UNCHANGED) ;  
            //存入cuda
            int cur_id = (id - start) * m_camera_num + cam_id;
            checkCudaErrors(cudaMemcpy(d_dis + cur_id*m_width *m_height ,disp_img.data, m_width * m_height* sizeof(float),cudaMemcpyHostToDevice));
        
            //在读完图后 在指定位置创建存储深度的文件夹
            std::string dep_savePath ="mkdir -p " +  m_depth_path + "/" + std::to_string(id);
            system(dep_savePath.c_str());
         
        }
    }
}

void cudaResource::cal_baseline(std::string pose_path){
    for(int id=0;id<m_camera_num;id++){
        char xml_stereo[255];
        sprintf(xml_stereo,pose_path.c_str(),id);
        cv::FileStorage fs(xml_stereo, cv::FileStorage::READ);
        
        cv::Mat P1, P2;
        fs["P1"] >> P1;
        fs["P2"] >> P2;
        
        P1.convertTo(P1, CV_32F);
        P2.convertTo(P2, CV_32F);

        float fx = P2.ptr<float>(0)[0];
        //! colmap：乘以1000
        float val = P2.ptr<float>(0)[3] * 1000;  //俩个相机x的偏移量
        float baseline = abs(val / fx);
        float scale = P2.ptr<float>(0)[2] - P1.ptr<float>(0)[2];
        std::cout << "camera id : "<<id  << " | baseline: " << baseline << " | scale: " << scale << " | fx: " << fx << std::endl;
        //存入cuda
        cv::Mat T_cu_data = (cv::Mat_<float>(3,1)<< fx,baseline,scale);
        checkCudaErrors(cudaMemcpy(T_cu + id*3 ,T_cu_data.data, 3* sizeof(float),cudaMemcpyHostToDevice));
    }
}

void cudaResource::ReadUndisPose(std::string pose_path){
    for(int id=0;id<m_camera_num;id++){
        char xml_stereo[255];
        sprintf(xml_stereo,pose_path.c_str(),id);
        cv::FileStorage fs(xml_stereo, cv::FileStorage::READ);
        
        cv::Mat P1, P2, R, dist,R_inv;

        fs["P1"] >> P1;   //p1是从矫正坐标系到矫正图像的变换 左侧3*3是新内参
        fs["Matrix1"] >> P2;   //p2是标定系下的内参
        fs["R1"] >> R;  //原始相机系到矫正相机系
        fs["dist1"] >> dist;  //标定系下即变系数

        
        P1.convertTo(P1, CV_32F);
        P2.convertTo(P2, CV_32F);
        R.convertTo(R, CV_32F);
        dist.convertTo(dist, CV_32F);
        R_inv = R.inv();

        // std::cout << "P1" << P1 << std::endl;
        // std::cout << "P2" << P2 << std::endl;
        // std::cout << "R_inv" << R_inv << std::endl;
        // std::cout << "dist" << dist << std::endl;
        //存入cuda
        checkCudaErrors(cudaMemcpy(P1_cu + id*12 ,P1.data, 12* sizeof(float),cudaMemcpyHostToDevice));
        checkCudaErrors(cudaMemcpy(P2_cu + id*9 ,P2.data, 9* sizeof(float),cudaMemcpyHostToDevice));
        checkCudaErrors(cudaMemcpy(R1_cu + id*9 ,R_inv.data, 9* sizeof(float),cudaMemcpyHostToDevice));
        checkCudaErrors(cudaMemcpy(dist_cu + id*5 ,dist.data, 5* sizeof(float),cudaMemcpyHostToDevice));
    }
}

void cudaResource::Startdis2dep(int start,int step,std::string save_path){
    for(int id = start;id<start+step;id++){
        for(int camera_id = 0;camera_id<m_camera_num;camera_id++){
            int cur_id = (id - start) * m_camera_num + camera_id;
            dis2dep(d_dis,d_depth,T_cu,m_width,m_height,cur_id,m_camera_num);
            dep2align(d_depth,d_depth_final,P1_cu,P2_cu,R1_cu,dist_cu,m_width,m_height,cur_id,m_camera_num);
        }
    }
}

void cudaResource::SaveDepimg(int start,int step,std::string save_path){
    //save img
    for(int id = start;id<start+step;id++){
        for(int camera_id = 0;camera_id<m_camera_num;camera_id++){
            int cur_id = (id - start) * m_camera_num + camera_id;
            cv::Mat depth_png(m_height,m_width,CV_16UC1);
            cudaMemcpy(depth_png.data, d_depth_final+cur_id*m_width*m_height, m_width * m_height * sizeof(unsigned short), cudaMemcpyDeviceToHost);
            char temp[255];
            std::string save_img = save_path + "/" + std::to_string(id) + "/%04d.png";
            sprintf(temp,save_img.c_str(),camera_id);
            cv::imwrite(temp,depth_png);
        }
    }
}

cudaResource::cudaResource(/* args */)
{
    
}


cudaResource::~cudaResource()
{
    if(d_depth){
        cudaFree(d_depth);
    }
    if(d_depth_final){
        cudaFree(d_depth_final);
    }
    if(d_dis){
        cudaFree(d_dis);
    }
    if(T_cu){
        cudaFree(T_cu);
    }
    if(P1_cu){
        cudaFree(P1_cu);
    }
    if(P2_cu){
        cudaFree(P2_cu);
    }
    if(R1_cu){
        cudaFree(R1_cu);
    }
    if(dist_cu){
        cudaFree(dist_cu);
    }
}