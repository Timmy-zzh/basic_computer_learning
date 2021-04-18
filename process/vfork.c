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
    printf("ipc vfork \n");
    int count = 0;

    /**
     * vfork:先执行子进程，子进程退出后，才会执行父进程
     */
    pid_t pid = vfork();
    if (pid == 0) {
        printf("child process pid:%d \n", getpid());
        count++;
        printf("count:%d \n", count);
        _exit(1);
    } else {
        printf("father process pid:%d \n", getpid());
        count += 2;
        printf("count:%d \n", count);
    }

    return 0;
}

