//
// Created by timmy on 2021/3/31.
//
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

/**
 * mmap 内存映射学习
 * 1。打开一个文件，
 * 2。将该文件通过mmap函数进行文件映射
 * 3。往文件中写入数据
 * --进程间通信
 */
int main() {
    printf(" mmap study \n");
    char *file_path = "/Users/timmy1/study/basic_computer/basic_computer_learning/mmap/mmap_test.text";
    //1.打开文件
//    int fp = fopen(file_path, "rb");
//    FILE *fp = fopen(file_path, "r");
    FILE *fp = fopen(file_path, "r+");
    if (fp == NULL) {
        printf("fopen fail\n");
        return 0;
    }
    printf("fopen succes fp:0x%08x\n", fp);

    //2.获取linux地址空间的一页大小
    long page_size = sysconf(_SC_PAGESIZE);
    printf("page_size:%ld \n", page_size);

    //3.调用mmap对fp文件进行文件映射，并返回映射地址
//    void *mmap_addr = mmap(0, 4096 * 4, PROT_READ, MAP_SHARED, fp, 0);
    void *mmap_addr = mmap(0, 4096 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fp, 0);
    if (mmap_addr == MAP_FAILED) {
//        perror("error");
        printf("map_fail \n");
        _exit(1);
    }
    printf(" mmap_addr: 0x%08x\n", mmap_addr);
    printf(" mmap_addr: 0x%s\n", (char *) mmap_addr);

    return 0;
}


