//
// Created by user on 2021/3/27.
//
#include <stdio.h>
#include <unistd.h>

int main(){

    printf("memeory ...");
    sysconf(_SC_PAGESIZE);

    return 0;
}
