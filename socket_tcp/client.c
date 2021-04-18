//
// Created by timmy on 2021/4/18.
//

#include "config.h"

//从fd中不断读取数据,数据都存放在vprt中，并返回数据的长度
ssize_t readline(int fd, char *vptr, size_t size) {
    int n, rc;
    char c, *ptr;

    ptr = vptr;
    for (n = 1; n < size; ++n) {
        // 一个字节的读
        if ((rc = read(fd, &c, 1)) == 1) {
            //指针赋值
            *ptr++ = c;
            if (c == '\n') {
                break;//下一行
            }
        } else if (rc == 0) {
            //没有数据了
            *ptr = 0;
            return n - 1;
        } else {
            //出错
            return -1;
        }
    }
//    printf("client get msg:%s\n", &vptr);
    *ptr = 0;
    return n;
}

/**
 * socket 客户端tcp实现
 * 1。sockt配置项，ip，port
 * 2。与服务器进行连接-connect
 * 3。与服务端通信
 */
int main() {
    printf("socket tcp client pid:%d\n", getpid());

    //1。创建套接字socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("socket error");
        _exit(1);
    }

    //2.配资信息
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    //端口
    server_addr.sin_port = htons(PORT);
    int inet_p = inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    if (inet_p < 0) {
        perror("inet_pton error for \n");
        _exit(1);
    }

    //3.客户端发起请求链接
    int connect_res = connect(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (connect_res < 0) {
        perror("connect error");
        _exit(1);
    }

    //4.消息处理,从控制台获取输入的数据，作为发送给服务端的数据
    char send_line[BUFF_SIZE], recv_line[BUFF_SIZE];
    while (fgets(send_line, BUFF_SIZE, stdin) != NULL) {
        //写入
        write(socket_fd, send_line, strlen(send_line));

        //读取
        if (readline(socket_fd, recv_line, BUFF_SIZE) < 0) {
            perror("server terminated prematurely");
            exit(1);
        }

        // 输出到控制台
        printf("client get msg:%s\n", recv_line);
        if (fputs(recv_line, stdout) == EOF) {
            perror("fputs error");
            exit(1);
        }
    }

    //5.关闭套接字
    close(socket_fd);

    return 0;
}


