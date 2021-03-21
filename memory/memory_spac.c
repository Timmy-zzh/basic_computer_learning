//
// Created by user on 2021/3/27.
//

/**
 * 验证地址空间分布
 * -查看当程序运行时，当前进程的虚拟地址空间的各个数据的地址分布
 * 1.堆
 * 2.栈
 * 3.mmap file
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int global_num = 0;  //全局变量-未初始化
char global_str_arr[65535] = {'a'};  //全局变量-已初始化

int main() {
    printf("process address space....... \n");

    //局部变量-未初始化
    char *heap_var = NULL;
    int local_var = 0;
//    printf("Address of function main 0x%lx\n", main);
    printf(" Address of function main: 0x%08x\n", main);
    //求全局变量在虚拟地址空间中的地址
    printf("    Address of global_num: 0x%08x\n", &global_num);
    // 已初始化变量地址（.data） 在 未初始化变量地址（.bss）下面
    printf("Address of global_str_arr: 0x%08x\n", global_str_arr);

    //栈空间，局部变量 --高地址
    printf("          Top of stack is: 0x%08x\n", &local_var);
    //堆空间首地址
    printf("           Top of Heap is: 0x%08x\n", heap_var);
    heap_var = malloc(sizeof(char) * 4);
    printf("           Top of Heap is: 0x%08x\n", heap_var);

    return 0;
}
