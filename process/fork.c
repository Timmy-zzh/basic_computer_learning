//
// Created by timmy on 2021/4/10.
//

/**
 * fork函数练习：
 * 创建子进程
 */
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("ipc fork \n");
    int count = 0;

    pid_t pid = fork();
    if (pid == 0) {
        printf("child process pid:%d \n", getpid());
        count++;
        printf("count:%d \n", count);
    } else {
        printf("father process pid:%d \n", getpid());
        count += 2;
        printf("count:%d \n", count);
    }

    return 0;
}

