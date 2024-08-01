#include"cudaResource.h"
#include"cpptoml.h"
#include <sys/time.h>
#include"stdlib.h"

int main(){
    //读取参数表
    timeval start, end;
    gettimeofday(&start, nullptr);
    auto toml_config = cpptoml::parse_file("../config/dis2dep.toml");
    int camera_num = *toml_config->get_qualified_as<int>("Input.camera_num");
    int time_num = *toml_config->get_qualified_as<int>("Input.time_num");
    int width = *toml_config->get_qualified_as<int>("Input.width");
    int height = *toml_config->get_qualified_as<int>("Input.height");
    int block_size = *toml_config->get_qualified_as<int>("Input.block_size");

    std::string dis_path = *toml_config->get_qualified_as<std::string>("Input.dis_path");
    std::string pose_path = *toml_config->get_qualified_as<std::string>("Input.pose_path");

    std::string depth_path = *toml_config->get_qualified_as<std::string>("Output.depth_path");



    cudaResource::Ptr ptr = cudaResource::GetInstance();
    ptr->init(width,height,camera_num,time_num,dis_path,depth_path);
    ptr->cal_baseline(pose_path);
    ptr->ReadUndisPose(pose_path);
    
    int add_step = block_size;
    int start_id = 0;
    while(start_id<time_num){
        add_step = start_id+block_size > time_num ? time_num-start_id : block_size;
        // std::cout << "start_id: " << start_id << std::endl;
        ptr->loadimg(start_id,add_step);
        // std::cout << "start_id: " << start_id << std::endl;
        ptr->Startdis2dep(start_id,add_step,depth_path);
        ptr->SaveDepimg(start_id,add_step,depth_path);
        start_id = start_id + add_step;
        std::cout<<"finish steps "<<add_step<<std::endl;
    }

    gettimeofday(&end, nullptr);
    double time_elapsed = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
    printf("Total Time %.3fs\n", time_elapsed / 1000.0);


    return 0;

}