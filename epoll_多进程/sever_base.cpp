#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <fcntl.h>
#include <csignal>
#include <sys/wait.h>

#define MAX_EVENTS 10
#define PORT 8080

// 设置文件描述符为非阻塞模式
int setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl get");
        return -1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        perror("fcntl set");
        return -1;
    }
    return 0;
}

// 子进程大小写转换函数
void toUpperCase(char* str, int len) {
    for (int i = 0; i < len; ++i) {
        if (islower(str[i])) {
            str[i] = toupper(str[i]);
        }
    }
}

// 处理SIGCHLD信号，避免僵尸进程
void sigchld_handler(int sig) {
    // 使用非阻塞的 waitpid 处理所有子进程
    while (waitpid(0, nullptr, WNOHANG) > 0);
}

int main() {
    int server_fd, new_socket, epoll_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    struct epoll_event ev, events[MAX_EVENTS];

    // 注册SIGCHLD信号，防止僵尸进程
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;  // 确保信号处理后继续中断的系统调用
    if (sigaction(SIGCHLD, &sa, nullptr) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // 创建监听socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置socket地址重用
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // 设置监听地址和端口
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听socket，允许最大队列数为10
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // 创建epoll实例
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // 设置监听socket为非阻塞
    setNonBlocking(server_fd);

    // 将监听socket添加到epoll
    ev.events = EPOLLIN;  // 监听读事件
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        perror("epoll_ctl: server_fd");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running and waiting for connections..." << std::endl;

    while (true) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            if (errno == EINTR) {  // 如果是信号打断则继续
                continue;
            }
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == server_fd) {
                // 新的客户端连接
                new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                if (new_socket == -1) {
                    perror("accept");
                    continue;
                }

                // 设置新socket为非阻塞
                setNonBlocking(new_socket);

                // 将新socket添加到epoll实例
                ev.events = EPOLLIN | EPOLLET;  // 边缘触发
                ev.data.fd = new_socket;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &ev) == -1) {
                    perror("epoll_ctl: new_socket");
                    exit(EXIT_FAILURE);
                }

                std::cout << "New connection accepted!" << std::endl;

            } else {
                // 已连接客户端发送数据
                char buffer[1024] = {0};
                int count = read(events[i].data.fd, buffer, sizeof(buffer));
                if (count == -1) {
                    if (errno != EAGAIN) {
                        perror("read");
                        close(events[i].data.fd);
                    }
                } else if (count == 0) {
                    // 客户端断开连接
                    std::cout << "Client disconnected!" << std::endl;
                    close(events[i].data.fd);
                } else {
                    // 创建子进程处理大小写转换
                    pid_t pid = fork();
                    if (pid == 0) {
                         //这里不需要监听epoll事件
                        close(server_fd);
                        // 子进程
                        toUpperCase(buffer, count);
                        write(events[i].data.fd, buffer, count);  // Echo 回客户端
                        close(events[i].data.fd);  // 关闭子进程中的socket
                        exit(0);  // 子进程退出
                    } else if (pid > 0) {
                        // 父进程继续  
                        //这里不需要监听这个事件
                        int status;
                        waitpid(pid,&status,0);
                        //close(events[i].data.fd);
                        std::cout << "Forked child process to handle data." << std::endl;
                    } else {
                        perror("fork");
                    }
                }
            }
        }
    }

    close(server_fd);
    close(epoll_fd);
    return 0;
}
