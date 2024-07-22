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
    //服务器 socket 文件描述符 和 客户端连接的 socket 文件描述符
    int server_fd, new_socket;
    //存储服务器地址信息的结构体
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    //存储从客户端接受的数据
    char buffer[BUFFER_SIZE] = {0};

    // 创建 socket 文件描述符
    // AF_INET 表示使用 IPv4 地址，SOCK_STREAM 表示使用面向连接的 TCP 协议
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 绑定 socket 到端口
    // 允许端口在程序关闭后立即被重用。
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    // 接受任何IP地址
    address.sin_addr.s_addr = INADDR_ANY;
    // 指定端口号
    address.sin_port = htons(PORT);
    // 将socket 绑定到指定的 IP 地址和端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听端口 最多监听三个端口(就使用一个缓存队列长度为三的缓冲区)
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // 接受客户端连接
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // 读取客户端发送的消息
    int valread = read(new_socket, buffer, BUFFER_SIZE);
    std::cout << "Message from client: " << buffer << std::endl;

    // 发送回复给客户端
    const char *reply = "Message received";
    send(new_socket, reply, strlen(reply), 0);
    //test
    std::vector<int>messages_vec = {1,2,3,4,5,6,7,8,9,10,11,12,13};
    size_t size = messages_vec.size() * sizeof(int);
    char Vecbuffer[BUFFER_SIZE] = {0};
    memcpy(Vecbuffer,messages_vec.data(),size);
    send(new_socket,Vecbuffer,size,0);


    std::cout << "Reply sent to client" << std::endl;

    close(new_socket);
    close(server_fd);
    return 0;
}
