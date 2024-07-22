#include "ShareMemory.h"
//写标志位
void ShareMemory::WriteFlag2Share(sharedata value)
{
    buffer_head->flag = value.flag;
    for(int i=0;i<sizeof(value.is_start)/sizeof(value.is_start[0]);i++){
        buffer_head->is_start[i] = value.is_start[i];
    }
    for(int i=0;i<sizeof(value._re)/ sizeof(value._re[0]);++i){
        buffer_head->_re[i] = value._re[i];
    }
}
void ShareMemory::WriteFlag2Share(int value,time_ms* res,size_t t_size,size_t r_size)
{
    buffer_head->flag = value;
    for(size_t i=0;i<t_size;i++){
        buffer_head->is_start[i] = res[i];
    }
    for(size_t i=0;i<r_size;++i){
        buffer_head->_re[i] = value;
    }
}

//读标志位
sharedata ShareMemory::ReadFlagFromShare(sharedata &value)
{
    memcpy(&value, buffer_head, sizeof(value));
}
sharedata ShareMemory::ReadFlagFromShare()
{
    time_ms res[stand] = {0};
    sharedata temp = {0,10,10};
    memcpy(&temp, buffer_head, sizeof(temp));
    return temp;
}

void ShareMemory::getBufferHead(){
    key_t key = ftok(".",_key);
    shm_id = shmget(key, sizeof(sharedata),IPC_CREAT|0666);

    if(shm_id == -1)
    {
        cout<<"shmget error"<<endl;
        exit(1);
    }
    //cout << "read shem id is  "<<shm_id<<endl;
    // char* buffer_head;
    buffer_head = (sharedata*)shmat(shm_id, NULL, 0);
    if(buffer_head == (sharedata*)-1)
    {
        perror("Share memary can't get pointer\n");
        exit(1);
    }
}
