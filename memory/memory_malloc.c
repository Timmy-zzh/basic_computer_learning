//
// Created by user on 2021/3/27.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
//    long page_size = sysconf(_SC_PAGESIZE);
//    printf("page_size:%ld\n", page_size);

    printf("memeory malloc study...\n");

//    brk(4);
    void *p = sbrk(0);
    printf("sbrk(0) : 0x%08x\n", p);
    p = sbrk(4);
    printf("sbrk(4) : 0x%08x\n", p);
    p = sbrk(4);
    printf("sbrk(4) : 0x%08x\n", p);

    void *p_m = malloc(16);
    int *p_int = malloc(4);
    printf("     p_m: 0x%08x\n", p_m);
    printf("   p_int: 0x%08x\n", p_int);

    p = sbrk(0);
    printf("sbrk(0) : 0x%08x\n", p);

    return 0;
}
