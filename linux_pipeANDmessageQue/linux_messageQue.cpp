#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <unistd.h>

struct message {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key = ftok("progfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    if (msgid == -1) {
        perror("msgget");
        return 1;
    }

    message msg;
    msg.msg_type = 1;
        pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // 子进程
        strcpy(msg.msg_text, "Hello from parent process");
        if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
            perror("msgsnd");
            return 1;
        }
            std::cout << "Message sent: " << msg.msg_text << std::endl;
    } else { // 父进程
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
            perror("msgrcv");
            return 1;
        }
        std::cout << "Message received: " << msg.msg_text << std::endl;
    }
    msgctl(msgid, IPC_RMID, NULL); // 删除消息队列
    return 0;
}


/*
创建消息队列或者获取消息队列
int msgget(key_t key, int msgflg);
发送消息
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
接收消息
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
删除消息
int msgctl(int msqid, int cmd, struct msqid_ds *buf);
*/