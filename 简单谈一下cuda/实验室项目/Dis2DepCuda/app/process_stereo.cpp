#include <iostream>
#include<thread>
#include <condition_variable>
#include <mutex>
#include <vector>


#include"cpptoml.h"
#include <sys/time.h>
#include"stdlib.h"

#include <opencv2/opencv.hpp>



typedef struct dataset
{
    int ids;
    int flag;

    dataset(int m_flag){
        ids = -1;        
        flag = m_flag;
    }
}dataset;

const int threads_num = 20; //队列长度
std::mutex mtx; 
std::condition_variable cond;  //当前队列非空
std::condition_variable cond2; //当前是否消费完毕 如果消费完毕就告诉生产线程
int cur_id = 0;
int consumer_nums = 10;  //消费者线程数
int producer_nums = 5; //生产者线程数
bool is_done = false;
int process_id = 0;


int findPos(const std::vector<dataset> &data_queue,int val){
    for(int id=0;id<threads_num;id++){
        if(data_queue[id].flag == val){return id;}
    }
    return threads_num;
}


void producer(int camera_num,std::vector<dataset> *data_queue,std::string rgb_path,int time_num) {
    while(1){
            std::unique_lock<std::mutex> lock(mtx);  
            int insertPos = findPos(*data_queue,0);
            // if(insertPos == threads_num){
            //     std::this_thread::sleep_for(std::chrono::microseconds(100));
            //     continue;
            // }

            cond2.wait(lock,[data_queue,&insertPos](){
                insertPos = findPos(*data_queue,0);
                return insertPos<threads_num;
            });
        
            if(cur_id<time_num){
                (*data_queue)[insertPos].ids = cur_id; 
                (*data_queue)[insertPos].flag = 1; 
                cur_id++;
                std::cout<<"Load one  data  "<<cur_id<<std::endl;
                cond.notify_all();      
                lock.unlock(); 
            }

            if(is_done){
                cond.notify_all();
                lock.unlock();
                break;
            }
    }
    //std::cout<<"delete producer : "<<std::this_thread::get_id()<<std::endl;
}


void cal_map(std::vector<cv::Mat>&maps,std::string xml_path,std::string xml_out,int width,int height,int camera_nums){

    for(int id=0;id<camera_nums;id++){
        char xml_left_path[255];
        sprintf(xml_left_path,xml_path.c_str(),id*2);
        char xml_right_path[255];
        sprintf(xml_right_path,xml_path.c_str(),id*2+1);
        char xml_stereo_path[255];
        sprintf(xml_stereo_path,xml_out.c_str(),id);

        cv::Mat R1, R2, P1, P2, Q;
        cv::Mat Matrix1, dist1, Matrix2, dist2, R, T;
        cv::Size imageSize;
        //! 输入colmap标定参数
        cv::Mat R_l_mat, R_l_mat_inv, T_l_mat, R_r_mat, T_r_mat;
        imageSize.width = width;
        imageSize.height = height;
        std::cout<<xml_left_path<<" "<<xml_right_path<<std::endl;
        cv::FileStorage xml_left(xml_left_path, cv::FileStorage::READ);
        cv::FileStorage xml_right(xml_right_path, cv::FileStorage::READ);
        cv::FileStorage xml_stereo(xml_stereo_path, cv::FileStorage::WRITE);

        xml_left["IntrinsicCam"] >> Matrix1;
        xml_left["DistortionCam"] >> dist1;
        xml_left["R"] >> R_l_mat;
        xml_left["t"] >> T_l_mat;
        xml_left.release();

        xml_right["IntrinsicCam"] >> Matrix2;
        xml_right["DistortionCam"] >> dist2;
        xml_right["R"] >> R_r_mat;
        xml_right["t"] >> T_r_mat;
        xml_right.release();

        R_l_mat.convertTo(R_l_mat, CV_64F);
        T_l_mat.convertTo(T_l_mat, CV_64F);
        R_r_mat.convertTo(R_r_mat, CV_64F);
        T_r_mat.convertTo(T_r_mat, CV_64F);
        Matrix1.convertTo(Matrix1, CV_64F);
        Matrix2.convertTo(Matrix2, CV_64F);
        dist1.convertTo(dist1, CV_64F);
        dist2.convertTo(dist2, CV_64F);

        cv::invert(R_l_mat, R_l_mat_inv);
        R = R_r_mat * R_l_mat_inv;
        T = T_r_mat - R_r_mat * R_l_mat_inv * T_l_mat;
        /**
         * 输入
         * cameraMatrix1: 第一个相机内参矩阵
         * distCoeffs1: 第一个相机畸变系数
         * cameraMatrix2: 第二个相机内参矩阵
         * distCoeffs2: 第二个相机畸变系数
         * R: 从第一个相机转第二个相机的旋转矩阵
         * t: 从第一个相机转第二个相机的平移向量
         * 输出
         * R1: 从未校正的第一台相机的坐标系转到校正的第一台相机的坐标系的旋转矩阵
         * R2: 从未校正的第二个相机的坐标系转到校正的第二个相机的坐标系的旋转矩阵
         * P1: 第一台相机的校正坐标系中输出 3x4 投影矩阵，即校正后的第一台相机坐标系中给定的点投影到校正后的第一台相机的图像
         * P2: 第二台相机的校正坐标系中输出 3x4 投影矩阵，即校正后的第一台相机坐标系中给定的点投影到校正后的第二台相机的图像
         * Q: 输出 4x4 视差转深度的映射矩阵
         */
        cv::stereoRectify(Matrix1, dist1, Matrix2, dist2, imageSize, R, T, R1, R2, P1, P2, Q, cv::CALIB_ZERO_DISPARITY, 0);

        //! 计算校正映射矩阵CV_16SC2
        cv::Mat map11, map12, map21, map22;
        cv::initUndistortRectifyMap(Matrix1, dist1, R1, P1, imageSize, CV_32F, map11, map12);
        cv::initUndistortRectifyMap(Matrix2, dist2, R2, P2, imageSize, CV_32F, map21, map22);

        maps.emplace_back(map11);
        maps.emplace_back(map12);
        maps.emplace_back(map21);
        maps.emplace_back(map22);

        //! 保存为xml文件
        xml_stereo << "R" << R;
        xml_stereo << "T" << T;
        xml_stereo << "Matrix1" << Matrix1;
        xml_stereo << "Matrix2" << Matrix2;
        xml_stereo << "dist1" << dist1;
        xml_stereo << "dist2" << dist2;
        xml_stereo << "R1" << R1;
        xml_stereo << "R2" << R2;
        xml_stereo << "P1" << P1;
        xml_stereo << "P2" << P2;
        xml_stereo << "Q" << Q;
        xml_stereo.release();
    }
}
void processimg(dataset insertPos,std::string image_path,int camera_nums,std::vector<cv::Mat>maps){
    for(int id = 0;id<camera_nums;id++)
    {

            
            char temp_left[255];
            char temp_right[255];
            sprintf(temp_left,image_path.c_str(),insertPos.ids,id*2);
            sprintf(temp_right,image_path.c_str(),insertPos.ids,id*2+1);

                        
            //! 图像去畸变
            cv::Mat left_image = cv::imread(temp_left,-1);
            cv::Mat right_image = cv::imread(temp_right,-1);
            //std::cout<<temp_left<<std::endl;
            cv::Mat left_image_remap, right_image_remap;
            cv::remap(left_image, left_image_remap, maps[id*4],  maps[id*4+1],cv::INTER_LINEAR);
            cv::remap(right_image, right_image_remap, maps[id*4+2],  maps[id*4+3], cv::INTER_LINEAR);

            //! 去畸变结果保存
            std::string sub = image_path.substr(0, image_path.length() - 8); 
            std::string undis_path = sub + "stereo_%04d.png";
            std::string compare_path = sub + "compare_%04d.png";
            char undis_left[255];
            char undis_right[255];
            char compare[255];
            sprintf(undis_left,undis_path.c_str(),insertPos.ids,id*2);
            sprintf(undis_right,undis_path.c_str(),insertPos.ids,id*2+1);
            sprintf(compare,compare_path.c_str(),insertPos.ids,id);
              

            cv::imwrite(undis_left, left_image_remap);
            cv::imwrite(undis_right, right_image_remap);
            

            //! 对比结果保存
            // cv::Mat result;
            // cv::hconcat(left_image_remap, right_image_remap, result);
            // cv::line(result, cv::Point(-1, result.rows / 4), cv::Point(result.cols, result.rows / 4), cv::Scalar(0, 0, 255), 3);
            // cv::line(result, cv::Point(-1, result.rows / 2), cv::Point(result.cols, result.rows / 2), cv::Scalar(0, 0, 255), 3);
            // cv::line(result, cv::Point(-1, result.rows * 3 / 4), cv::Point(result.cols, result.rows * 3 / 4), cv::Scalar(0, 0, 255), 3);

            // cv::imwrite(compare, result);
            
    }
    std::cout<<"save_undis_img id : "<<insertPos.ids<<std::endl;
}



void consumer(int camera_num,int time_num,std::vector<dataset> *data_queue,
                                std::string rgb_path,std::vector<cv::Mat>maps) {  
  while(!is_done) {
        //std::cout<<"in comsumer"<<std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(100));

        std::unique_lock<std::mutex> lock(mtx);
        int insertPos;

        cond.wait(lock,[data_queue,&insertPos,&is_done](){
                if(is_done){
                    return is_done;
                }
                insertPos = findPos(*data_queue,1);
                return insertPos<threads_num;});
        if(!is_done){
            (*data_queue)[insertPos].flag = 2;  //2表示当前值已经读取 不允许其他线程再重复读取
            lock.unlock();
            processimg((*data_queue)[insertPos],rgb_path,camera_num,maps);
            std::unique_lock<std::mutex> lock(mtx);
            (*data_queue)[insertPos].flag = 0;
            process_id++;
            std::cout << "Consumed: "<< process_id << std::endl;
            lock.unlock();
        }

        if(process_id>=time_num){
            is_done = true;
            cond2.notify_all();
        }
        else{
            cond2.notify_one();
        }
    }
    //std::cout<<"delete consumer : "<<std::this_thread::get_id()<<std::endl;
}

int main() {
    auto toml_config = cpptoml::parse_file("../config/undis.toml");
    int camera_num = *toml_config->get_qualified_as<int>("Input.camera_num");
    int time_num = *toml_config->get_qualified_as<int>("Input.time_num");
    int width = *toml_config->get_qualified_as<int>("Input.width");
    int height = *toml_config->get_qualified_as<int>("Input.height");

    std::string rgb_path = *toml_config->get_qualified_as<std::string>("Input.rgb_path");
    std::string pose_path = *toml_config->get_qualified_as<std::string>("Input.pose_path");
    std::string newPose_path = *toml_config->get_qualified_as<std::string>("Output.xml_path");
    // std::string undis_path = *toml_config->get_qualified_as<std::string>("Output.undis_path");

    dataset init_data(0);
    std::vector<dataset> data_queue(threads_num,init_data);
    std::vector<cv::Mat>maps;

    //creat file 
    std::string sub = newPose_path.substr(0, newPose_path.length() - 9); 
    std::string create ="mkdir -p "+ sub;
    system(create.c_str());


    cal_map(maps,pose_path,newPose_path,width,height,camera_num);
    
    std::vector<std::thread>pro;
    std::vector<std::thread>com;

    for(int id = 0;id<producer_nums;id++){
        std::thread t1 = std::thread(producer,camera_num,&data_queue,rgb_path,time_num);
        pro.emplace_back(std::move(t1));
    }
    for(int id = 0;id<consumer_nums;id++){
        std::thread t2 = std::thread(consumer,camera_num,time_num,&data_queue,rgb_path,maps);
        com.emplace_back(std::move(t2));
    }

    for(int id = 0;id<producer_nums;id++){
        pro[id].join();
    }
    for(int id = 0;id<consumer_nums;id++){
        com[id].join();
    }



  return 0;
}