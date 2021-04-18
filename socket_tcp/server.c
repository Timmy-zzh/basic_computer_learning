//
// Created by timmy on 2021/4/18.
//

#include "config.h"

/**
 * socket编程，实现服务端
 * 1。socket配置信息--ip地址与端口号
 * 2。等待客户端连接请求：
 * -bind
 * -listen
 * 3。获取客户端发送过来的数据，并返回
 */
int main() {
    printf("socket tcp server pid:%d\n", getpid());

    // 声明服务端与客户端链接地址
    struct sockaddr_in server_addr, client_addr;

    //1。初始化监听套接字,返回该套接字的fd
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        perror("socket error");
        _exit(1);
    }

    //2.设置服务器 server_addr 结构
    bzero(&server_addr, sizeof server_addr);
    server_addr.sin_family = AF_INET;
    //表明可以接收任意ip地址
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //端口
    server_addr.sin_port = htons(PORT);

    //3. 绑定套接字和端口
    int bind_res = bind(listen_fd, (struct sockaddr *) &server_addr, sizeof server_addr);
    if (bind_res < 0) {
        perror("bind error");
        _exit(1);
    }

    //4。监听客户端请求
    int listen_res = listen(listen_fd, LISTENQ);
    if (listen_res < 0) {
        perror("listen error");
        _exit(1);
    }

    //5。客户端请求
    socklen_t client_size;
    //客户端请求socket通道
    int connect_fd;

    for (;;) {
        client_size = sizeof(client_addr);
        connect_fd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_size);
        if (connect_fd < 0) {
            perror("accent error");
            _exit(1);
        }

        //6.io
        //新建子进程处理请求链接
        pid_t child_pid = fork();
        if (child_pid == 0) {
            //子进程中 套接字fd关闭
            close(listen_fd);

            //读取客户端发送过来的数据,并写回去
            ssize_t n;
            char buff[BUFF_SIZE];
            char *msg = "hello client ";
            while ((n = read(connect_fd, buff, BUFF_SIZE)) > 0) {
                printf("server,get msg: %s\n", buff);
                write(connect_fd, buff, n);

//                msg = strcat(msg, n);
//                write(connect_fd, msg, strlen(msg));
            }
            _exit(0);
        }
        close(connect_fd);
    }
    //7.关闭监听套接字
    close(listen_fd);

    return 0;
}