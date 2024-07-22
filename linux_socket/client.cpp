//
// Created by dai on 2024/7/15.
//

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>


#include <vector>


#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // 创建 socket 文件描述符
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 将地址转换为二进制形式
    // 如果此时客户端不在一个同一个ip下 首先需要设置在一个子网下 修改下面的ip
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // 连接服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    // 发送消息到服务器
    const char *message = "Hello from client";
    send(sock, message, strlen(message), 0);
    std::cout << "Message sent to server" << std::endl;

    // 读取服务器的回复 valread表示读取到的实际字节数
    int valread = read(sock, buffer, BUFFER_SIZE);
    std::cout << "Reply from server: " << buffer << std::endl;


    char bufferVec[BUFFER_SIZE];
    int valread_Vec = read(sock, bufferVec, BUFFER_SIZE);
    std::vector<int>vec(valread_Vec/sizeof(int));
    memcpy(vec.data(), bufferVec,valread_Vec);
    for(int i:vec){
        std::cout << i << " ";
    }


    close(sock);
    return 0;
}
