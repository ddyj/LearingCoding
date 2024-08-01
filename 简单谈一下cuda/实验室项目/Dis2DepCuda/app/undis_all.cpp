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


void ReadPamra(std::vector<cv::Mat>&IntrinsicCams,std::vector<cv::Mat>&DistortionCams,
                                    std::string pose_path,int width,int height,int camera_num){

    for(int id=0;id<camera_num;id++){
        char xml_path[255];
        sprintf(xml_path,pose_path.c_str(),id*2);
        cv::Mat IntrinsicCam,DistortionCam;
        cv::FileStorage xml(xml_path, cv::FileStorage::READ);
        xml["IntrinsicCam"] >> IntrinsicCam;
        xml["DistortionCam"] >> DistortionCam;
        IntrinsicCams.emplace_back(IntrinsicCam);
        DistortionCams.emplace_back(DistortionCam);
        xml.release();
    }
}
void processimg(dataset insertPos,std::string image_path,std::string depth_path,
                                    int camera_num,cv::Size img_size,
                                    std::vector<cv::Mat> IntrinsicCams,
                                    std::vector<cv::Mat> DistortionCams){
    for(int id = 0;id<camera_num;id++)
    {
            char temp[255];
            sprintf(temp,image_path.c_str(),insertPos.ids,id*2);

            char dep_temp[255];
            sprintf(dep_temp,depth_path.c_str(),insertPos.ids,id);

                        
            //! 图像去畸变
            cv::Mat color = cv::imread(temp,-1);
            cv::Mat depth = cv::imread(dep_temp,cv::IMREAD_UNCHANGED);
            cv::Mat K = IntrinsicCams[id];
            cv::Mat dist = DistortionCams[id];


            //! 去畸变结果保存
            std::string subimg = image_path.substr(0, image_path.length() - 8); 
            std::string subdep = depth_path.substr(0, depth_path.length() - 8); 
            std::string imgundis_path = subimg + "undis_%04d.png";
            std::string depundis_path = subdep + "undis_%04d.png";
            cv::Mat map1, map2,color_res,depth_res;
            
            cv::initUndistortRectifyMap(K, dist, cv::Mat(),
                                    K, img_size, CV_32FC1, map1, map2);
            cv::remap(color, color_res, map1, map2, cv::INTER_NEAREST);
            cv::remap(depth, depth_res, map1, map2, cv::INTER_NEAREST);
           
            char temp_res[255];
            sprintf(temp_res,imgundis_path.c_str(),insertPos.ids,id);
            char dep_temp_res[255];
            sprintf(dep_temp_res,depundis_path.c_str(),insertPos.ids,id);
            imwrite(temp_res, color_res);
            imwrite(dep_temp_res, depth_res);    
    }
    std::cout<<"save_undis_img id : "<<insertPos.ids<<std::endl;
}


void consumer(int camera_num,int time_num,std::vector<dataset> *data_queue,
                                std::string rgb_path,std::string depth_path,int width,int height,
                                std::vector<cv::Mat>IntrinsicCams,std::vector<cv::Mat>DistortionCams
                               ) {  
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

        cv::Size img_size;
        img_size.width = width;
        img_size.height = height;
        if(!is_done){  
            (*data_queue)[insertPos].flag = 2;  //2表示当前值已经读取 不允许其他线程再重复读取
            lock.unlock();
            processimg((*data_queue)[insertPos],rgb_path,depth_path,
                                                    camera_num,img_size,
                                                    IntrinsicCams,DistortionCams);
            std::unique_lock<std::mutex> lock(mtx);
            (*data_queue)[insertPos].flag = 0;
            process_id++;
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
    std::string depth_path = *toml_config->get_qualified_as<std::string>("Input.depth_path");
    std::string newPose_path = *toml_config->get_qualified_as<std::string>("Output.xml_path");
    std::string output_path = *toml_config->get_qualified_as<std::string>("Output.output_path");

    dataset init_data(0);
    std::vector<dataset> data_queue(threads_num,init_data);
    std::vector<cv::Mat>IntrinsicCams;
    std::vector<cv::Mat>DistortionCams;

    //creat file 
    std::string sub = newPose_path.substr(0, newPose_path.length() - 9); 
    std::string create ="mkdir -p "+ sub;
    system(create.c_str());


    ReadPamra(IntrinsicCams,DistortionCams,pose_path,width,height,camera_num);
    
    std::vector<std::thread>pro;
    std::vector<std::thread>com;

    for(int id = 0;id<producer_nums;id++){
        std::thread t1 = std::thread(producer,camera_num,&data_queue,rgb_path,time_num);
        pro.emplace_back(std::move(t1));
    }

    for(int id = 0;id<consumer_nums;id++){
        std::thread t2 = std::thread(consumer,camera_num,time_num,&data_queue,
                                                                                        rgb_path,depth_path,width,height,
                                                                                        IntrinsicCams,DistortionCams);
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