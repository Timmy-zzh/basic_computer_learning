//
// Created by timmy on 2021/4/18.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <arpa//inet.h>

const int PORT = 8090;
const int LISTENQ = 6666;
const int BUFF_SIZE = 6666;