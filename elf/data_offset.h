//
// Created by user on 2021/3/21.
//

/**
 * 创建 DataOffset结构体，
 * 该结构体用于保存，elf文件解析过程中，获取到的一些重要数据
 * 比如：程序头部偏移地址，section个数等
 */
#include <stdint.h>

//todo 1.定义elf基本数据类型
typedef uint32_t Elf32_Addr; // Program address
typedef uint32_t Elf32_Off;  // File offset
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Word;
typedef int32_t Elf32_Sword;


typedef struct {
    // 程序头部偏移地址
    Elf32_Off progream_header_off;
    // 程序头部数量
    Elf32_Half progream_size;
    // 节头部偏移地址
    Elf32_Off section_header_off;
    // 节个数
    Elf32_Half section_size;
    // 动态链接的 ic 的偏移地址
    Elf32_Off dynameic_off;

    Elf32_Word dynameic_size;
    // 字符节偏移地址
    Elf32_Off str_off;
    // 字符 大小
    Elf32_Word str_size;
    // 字符串1 偏移地址
    Elf32_Off str1_off;

    Elf32_Word str1_size;
    Elf32_Off str2_off;
    Elf32_Word str2_size;
    // Section header str table index
    Elf32_Half sh_strtab_index;
    // 字符表偏移地址
    Elf32_Off symtab_off;
    Elf32_Word symtab_size;
} DataOffset;
