//
// Created by timmy on 2021/4/10.
//

/**
 * 进程通信：管道练习
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//宏定义
#define MAX_LINE 1024

void pipe_test1();

void pipe_test2();

int main() {
//    pipe_test1();
    pipe_test2();

    return 0;
}

/**
 * 1.创建管道，并获取管道两端通信的fd
 * 2。fork创建子进程，并在父子进程之间进行通信，多次读写（3）
 */
void pipe_test2() {
    printf("ipc - pipe2 \n");
    //创建pipe
    int fd[2] = {0, 0};
    /**
     * 建立管道得到一对文件描述符，其中
     * fd[0] 为read
     * fd[1] 为write
     */
    if (pipe(fd) < 0) {
        exit(0);
    }

    //创建一个子进程,返回当前进程的pid
    pid_t pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (pid > 0) {  //pid大于0，在父进程读
        printf("this is parent process ====== \n");
        sleep(2);
        close(fd[1]);//先关闭写
        char buf[MAX_LINE];
        int count = 3;
        while (--count) {
            size_t len = read(fd[0], buf, MAX_LINE);
            printf("Father is reading ...\n");
            if (len > 0) {
                buf[len] = '\0';
                printf("child say : %s\n", buf);
            } else if (len == 0) {
                printf("Read the end of pipe\n");
                break;
            } else {
                perror("read");
                exit(1);
            }
        }
    } else if (pid == 0) { //pid等于0，在子进程写
        printf("this is child process ------ \n");
        sleep(2);
        const char *msg = "Hellp pipe\n";
        close(fd[0]);//关闭读
        int count = 3;
        while (1) {
            ssize_t size = write(fd[1], msg, strlen(msg));
            printf("size:%d\n", size);
            sleep(1);
            printf("child is writing ... \n");
        }
    }
    close(fd[0]);//关闭读

    //再获取子进程的状态
    int status = 0;
    pid_t _pid = waitpid(pid, &status, 0);
    if (_pid == pid) {
        printf("wait succes for child\n");
        // single:13 -- pipe管道破裂
        printf("Exit code:%d , Exit signal:%d\n", (status >> 8) & 0xff, status & 0xff);
    } else {
        perror("wait");
    }
    exit(0);
}

void pipe_test1() {
    printf("ipc - pipe1 \n");
    //创建pipe
    int fd[2] = {0, 0};

    /**
     * 建立管道得到一对文件描述符，其中
     * fd[0] 为read
     * fd[1] 为write
     */
    if (pipe(fd) < 0) {
        exit(0);
    }

    //创建一个子进程,返回当前进程的pid
    pid_t pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (pid > 0) {  //pid大于0，在父进程--父进程写数据
        printf("this is parent process ====== \n");
        close(fd[0]);//先关闭读
        char data[] = "Hello parent pid write";
        write(fd[1], data, sizeof(data));
    } else if (pid == 0) { //pid等于0，在子进程 -- 子进程读取数据
        printf("this is child process ------ \n");
        //先关闭写
        close(fd[1]);
        char line[MAX_LINE];
        int n = read(fd[0], line, MAX_LINE);
        printf("child read : %s\n", line);
        fflush(stdout);
    }
    while (1) {}
}
