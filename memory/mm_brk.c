//
// Created by timmy on 2021/4/4.
//

#include <stdio.h>
#include <unistd.h>

int main() {
    void *p = sbrk(0);      //sbrk(0) 获取堆内存空间中break指针位置
    printf("sbrk(0) : 0x%08x\n", p);
    int *p1 = p;
    // brk是将break指针设置为某个固定指针
    brk(p1 + 4);
    int *p2 = sbrk(4);
    printf("sbrk(4) : 0x%08x\n", p2);

    p = sbrk(4);
    printf("-- sbrk(4) : 0x%08x\n", p);
    p = sbrk(4);
    printf("sbrk(4) : 0x%08x\n", p);
    p = sbrk(4);
    printf("sbrk(4) : 0x%08x\n", p);
    p = sbrk(4);
    printf("sbrk(4) : 0x%08x\n", p);
    // 获取未分配空间的开始位置
    p = sbrk(0);
    printf("sbrk(0) : 0x%08x\n", p);

    return 0;
}
