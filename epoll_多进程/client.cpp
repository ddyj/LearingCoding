#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // 创建socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 将地址转换为二进制形式
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported" << std::endl;
        return -1;
    }

    // 连接到服务器
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    std::cout << "Connected to the server. Enter a message to send: " << std::endl;
    
    while (true) {
        std::string message;
        std::cout << "You: ";
        std::getline(std::cin, message);

        // 检查是否输入"exit"来断开连接
        if (message == "exit") {
            std::cout << "Disconnecting..." << std::endl;
            break;
        }

        // 发送消息到服务器
        send(sock, message.c_str(), message.length(), 0);

        // 接收服务器的响应
        int valread = read(sock, buffer, sizeof(buffer) - 1);
        if (valread > 0) {
            buffer[valread] = '\0';  // 添加字符串结束符
            std::cout << "Server: " << buffer << std::endl;
        } else {
            std::cerr << "Failed to receive message from server" << std::endl;
        }
    }

    // 关闭socket
    close(sock);

    return 0;
}
