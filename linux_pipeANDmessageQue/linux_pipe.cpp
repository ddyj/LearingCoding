#include <iostream>
#include <unistd.h>
#include <cstring>

int main() {
    int pipefd[2];
    char buffer[128];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // 子进程
        close(pipefd[1]); // 关闭写端
        read(pipefd[0], buffer, sizeof(buffer)); // 从管道读取数据
        std::cout << "Child process read: " << buffer << std::endl;
        close(pipefd[0]);
    } else { // 父进程
        close(pipefd[0]); // 关闭读端
        const char *msg = "Hello from parent!";
        write(pipefd[1], msg, strlen(msg) + 1); // 向管道写入数据
        close(pipefd[1]);
    }

    return 0;
}
