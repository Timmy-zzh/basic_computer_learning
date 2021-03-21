//
// Created by user on 2021/3/21.
//

#ifndef BASIC_COMPUTER_LEARNING_ELF_TYPE_H
#define BASIC_COMPUTER_LEARNING_ELF_TYPE_H

/**
 * 1.elf内容解析，需要用到的数据结构：包括：
 * -elf header
 * -program header table
 * -section header table
 * -dynamic symbol table : 动态链接字符表
 *
 * 2.该文件中的struct复制与 elf。c文件，当前只考虑32位情况
 */

#include <stdint.h>

//todo 1.定义elf基本数据类型
typedef uint32_t Elf32_Addr; // Program address
typedef uint32_t Elf32_Off;  // File offset
typedef uint16_t Elf32_Half;
typedef uint32_t Elf32_Word;
typedef int32_t Elf32_Sword;

typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;
typedef uint16_t Elf64_Half;
typedef uint32_t Elf64_Word;
typedef int32_t Elf64_Sword;
typedef uint64_t Elf64_Xword;
typedef int64_t Elf64_Sxword;

//static int EI_NIDENT  = 16
//todo 2.定义elf header，elf文件头数据结构体
typedef struct {
    unsigned char e_ident[16]; // ELF Identification bytes
    Elf32_Half e_type;      // Type of file (see ET_* below)
    Elf32_Half e_machine;   // Required architecture for this file (see EM_*)
    Elf32_Word e_version;   // Must be equal to 1
    Elf32_Addr e_entry;     // Address to jump to in order to start program
    Elf32_Off e_phoff;     // Program header table's file offset, in bytes
    Elf32_Off e_shoff;     // Section header table's file offset, in bytes
    Elf32_Word e_flags;     // Processor-specific flags
    Elf32_Half e_ehsize;    // Size of ELF header, in bytes
    Elf32_Half e_phentsize; // Size of an entry in the program header table
    Elf32_Half e_phnum;     // Number of entries in the program header table
    Elf32_Half e_shentsize; // Size of an entry in the section header table
    Elf32_Half e_shnum;     // Number of entries in the section header table
    Elf32_Half e_shstrndx;  // Sect hdr table index of sect name string table
} Elf32_Ehdr;

// todo 3.Program header for ELF32.
typedef struct {
    Elf32_Word p_type;   // Type of segment
    Elf32_Off p_offset; // File offset where segment is located, in bytes
    Elf32_Addr p_vaddr;  // Virtual address of beginning of segment
    Elf32_Addr p_paddr;  // Physical address of beginning of segment (OS-specific)
    Elf32_Word p_filesz; // Num. of bytes in file image of segment (may be zero)
    Elf32_Word p_memsz;  // Num. of bytes in mem image of segment (may be zero)
    Elf32_Word p_flags;  // Segment flags
    Elf32_Word p_align;  // Segment alignment constraint
} Elf32_Phdr;

// todo 4.Section header.
typedef struct {
    Elf32_Word sh_name;      // Section name (index into string table)
    Elf32_Word sh_type;      // Section type (SHT_*)
    Elf32_Word sh_flags;     // Section flags (SHF_*)
    Elf32_Addr sh_addr;      // Address where section is to be loaded
    Elf32_Off sh_offset;    // File offset of section data, in bytes
    Elf32_Word sh_size;      // Size of section, in bytes
    Elf32_Word sh_link;      // Section type-specific header table index link
    Elf32_Word sh_info;      // Section type-specific extra information
    Elf32_Word sh_addralign; // Section address alignment
    Elf32_Word sh_entsize;   // Size of records contained within the section
} Elf32_Shdr;

// todo 5.Dynamic table entry for ELF32.
typedef struct {
    Elf32_Sword d_tag;            // Type of dynamic table entry.
    union {
        Elf32_Word d_val;         // Integer value of entry.
        Elf32_Addr d_ptr;         // Pointer value of entry.
    } d_un;
} Elf32_Dyn;

//todo 6. Symbol table entries for ELF32.
struct Elf32_Sym {
    Elf32_Word st_name;  // Symbol name (index into string table)
    Elf32_Addr st_value; // Value or address associated with the symbol
    Elf32_Word st_size;  // Size of the symbol
    unsigned char st_info;  // Symbol's type and binding attributes
    unsigned char st_other; // Must be zero; reserved
    Elf32_Half st_shndx; // Which section (header table index) it's defined in
};

#endif //BASIC_COMPUTER_LEARNING_ELF_TYPE_H
