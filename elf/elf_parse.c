#include <stdio.h>
#include "elf_type.h"
#include "data_offset.h"

DataOffset parseElfHeader(FILE *pIobuf, DataOffset param);

DataOffset parseElfSectionHeader(FILE *fp, DataOffset off);

DataOffset parseElfPargramHeader(FILE *fp, DataOffset off);

void parseSoDynamicSection(FILE *fp, DataOffset off);

void parseStrSection(FILE *fp, DataOffset off, int i);

/**
 * ELF 文件解析入口类
 * 1.先定义elf文件的数据结构，包括：
 * -elf header
 * -program header table
 * -section header table
 * -dynamic symbol table : 动态链接字符表
 * 2.elf.h 文件来源于 android系统源码
 * -路径：  /art/runtime/elf.h
 * 3.对elf文件的解析
 * -先加载.so文件，fopen
 * -拿到文件头的字节数组，然后进行解析
 * -。。。其他的数据结构都类似
 *
 * @return
 */
int main() {
    printf("------>elf file parse start---------\n");

    //打开文件
    FILE *fp;
//    char *fileName = "E:\\project\\CLion\\basic_computer_learning\\elf\\libnative-lib.so";
    char *fileName = "/Users/timmy1/study/basic_computer/basic_computer_learning/elf/libnative-lib.so";
    fp = fopen(fileName, "rb");

    DataOffset off = {0};
    off = parseElfHeader(fp, off);
    printf("-----check--------Program Header Offset :0x%08x", off.progream_header_off);

    //Program Header
    off = parseElfPargramHeader(fp, off);
    //继续解析 Section Header
    off = parseElfSectionHeader(fp, off);

    parseSoDynamicSection(fp, off);
    parseStrSection(fp, off, 1);
    parseStrSection(fp, off, 2);
    parseStrSection(fp, off, 3);

    fclose(fp);
    return 0;
}

void parseStrSection(FILE *fp, DataOffset off, int flag) {
    int total = 0;
    int i;
    int ch;
    int mark;
    Elf32_Off init;
    Elf32_Off addr;
    Elf32_Word count;

    mark = 1;


    if (flag == 1) {
        count = off.str_size;
        init = off.str_off;
    } else if (flag == 2) {
        count = off.str1_size;
        init = off.str1_off;
    } else {
        count = off.str2_size;
        init = off.str2_off;
    }

    printf("String Address==>0x%x\n", init);
    printf("String List %d:\n\t[1]==>", flag);

    for (i = 0; i < count; i++) {

        addr = init + (i * 1);

        fseek(fp, addr, SEEK_SET);
        fread(&ch, 1, 1, fp);

        if (i == 0 && ch == 0) {
            continue;
        } else if (ch != 0) {
            printf("%c", ch);
        } else if (ch == 0 && i != 0) {
            printf("\n\t[%d]==>", (++mark));
        }
    }
    printf("\n");
}

void parseSoDynamicSection(FILE *fp, DataOffset off) {
    int dynamicnum;
    Elf32_Off init;
    Elf32_Off addr;
    Elf32_Dyn dynamicData;
    int i;

    init = off.dynameic_off;
    dynamicnum = (off.dynameic_size / 8);

    printf("Dynamic:\n");
    printf("\t\tTag\t\t\tType\t\t\tName/Value\n");

    for (i = 0; i < dynamicnum; i++) {
        addr = init + (i * 8);
        fseek(fp, addr, SEEK_SET);
        fread(&dynamicData, 1, 8, fp);
        printf("\t\t0x%08x\t\tNOPRINTF\t\t0x%x\n", dynamicData.d_tag, dynamicData.d_un);
    }

}

/**
 * 1.解析elf节头部，
 * -因为elf的节表也是有多个，所以也需要根据elf文件头中获取的e_shnum(节数量)，进行遍历获取
 * 同样的方式也需要对每个section 解析，都需要进行偏移
 *
 * @param fp
 * @param off
 * @return
 */
DataOffset parseElfSectionHeader(FILE *fp, DataOffset off) {
    // 开始解析位置
    Elf32_Half start = off.progream_header_off;
    Elf32_Half addr;//当前程序头的偏移位置
    int i, id;
    int k = 0;
    //程序头-接收者
    Elf32_Shdr sectionHeader;
    for (i = 0; i < off.section_size; ++i) {
        //TODO 0x20是每个Program header 的大小，32个字节
        addr = start + (i * 0x20);
        //文件定位到该位置，并将解析到的数据保存到Elf32_Phdr 对象中
        //todo 1.先文件偏移
        fseek(fp, addr, SEEK_SET);
        // 接着从偏移位置开始读取数据
        fread(&sectionHeader, 1, 40, fp);
        //类型
        switch (sectionHeader.sh_type) {
            case 3:
                if (k == 0) {
                    off.str_off = sectionHeader.sh_offset;
                    off.str_size = sectionHeader.sh_size;
                    k++;
                } else if (k == 1) {
                    off.str1_off = sectionHeader.sh_offset;
                    off.str1_size = sectionHeader.sh_size;
                    k++;
                } else {
                    off.str2_off = sectionHeader.sh_offset;
                    off.str2_size = sectionHeader.sh_size;
                    k++;
                }
                break;
            default:
                break;
        }
        // Section Name
        id = sectionHeader.sh_name;
        printf("\n\nSection Header %d\n", (i + 1));
        printf("    Section Name Index: 0x%x\n", id);

        printf("    Section Type: 0x%08x\n", sectionHeader.sh_type);
        printf("    Section Flag: 0x%08x\n", sectionHeader.sh_flags);
        printf("    Address where section is to be loaded: 0x%08x\n", sectionHeader.sh_addr);
        printf("    Offset: 0x%x\n", sectionHeader.sh_offset);
        printf("    Size of section, in bytes: 0x%08x\n", sectionHeader.sh_size);
        printf("    Section type-specific header table index link: 0x%08x\n", sectionHeader.sh_link);
        printf("    Section type-specific extra information: 0x%08x\n", sectionHeader.sh_info);
        printf("    Section address alignment: 0x%08x\n", sectionHeader.sh_addralign);
        printf("    Size of records contained within the section: 0x%08x\n", sectionHeader.sh_entsize);
    }
    return off;
}

/**
 * 1.解析 ELF 程序头文件
 * -根据入参off，获取到elf文件头中定义好的 program header offset
 * --在根据偏移地址，通过fseek 让fp定位到该位置，然后从该位置进行解析
 * -解析后的结果数据保存到Elf32_Phdr 数据结构中
 * 2.因为程序头有多个，在解析的时候，需要获取所有的程序头，然后根据每个程序头计算当前程序头的偏移位置
 * -使用for循环，解析出每个程序头的数据
 *
 * @param fp
 * @param off
 * @return
 */
DataOffset parseElfPargramHeader(FILE *fp, DataOffset off) {
    // 开始解析位置
    Elf32_Half start = off.progream_header_off;
    Elf32_Half addr;//当前程序头的偏移位置
    int i;
    //程序头-接收者
    Elf32_Phdr programHeader;

    for (i = 0; i < off.progream_size; ++i) {
        //TODO 0x20是每个Program header 的大小，32个字节
        addr = start + (i * 0x20);
        //文件定位到该位置，并将解析到的数据保存到Elf32_Phdr 对象中
        //todo 1.先文件偏移
        fseek(fp, addr, SEEK_SET);
        // 接着从偏移位置开始读取数据
        fread(&programHeader, 1, 32, fp);
        switch (programHeader.p_type) {
            case 2:
                off.dynameic_off = programHeader.p_offset;
                off.dynameic_size = programHeader.p_filesz;
                break;
            default:
                break;
        }
        //其他数据的解析
        printf("\n\nSegment Header %d:\n", (i + 1));
        printf("    Type of segment: 0x%08x\n", programHeader.p_type);
        // 当前程序头 偏移地址
        printf("    Segment Offset: 0x%08x\n", programHeader.p_offset);
        // 虚拟地址
        printf("    Virtual address of beginning of segment: 0x%08x\n", programHeader.p_vaddr);
        // 物理地址
        printf("    Physical address of beginning of segment: 0x%08x\n", programHeader.p_paddr);
        // 文件大小
        printf("    Num. of bytes in file image of segment: 0x%08x\n", programHeader.p_filesz);
        printf("    Num. of bytes in mem image of segment (may be zero): 0x%08x\n", programHeader.p_memsz);
        // flag 权限
        printf("    Segment flags: 0x%08x\n", programHeader.p_flags);
        // 程序头 对其大小
        printf("    Segment alignment constraint: 0x%08x\n", programHeader.p_align);
    }
    return off;
}

/**
 * 解析ELF文件的头部
 * @param pIobuf
 */
DataOffset parseElfHeader(FILE *fp, DataOffset off) {
    // 定义elf文件头变量 header
    Elf32_Ehdr header;
    int i;

    fseek(fp, 0, SEEK_SET);
    fread(&header, 1, sizeof(header), fp);
    printf("ELF Header: \n");

    //todo 1.魔数
    printf("Header Magic :");
    for (i = 0; i < 16; ++i) {
        printf("%02x ", header.e_ident[i]);
    }

    //todo 2.elf文件类型
    printf("\n");
    printf("So file type: 0x%02x", header.e_type);
    //TODO 如何直到elf 文件的type类型有哪些数据，---》主要是协议指定，查看elf文档
    switch (header.e_type) {
        case 0x00:
            printf("(No file type)\n");
            break;
        case 0x01:
            printf("(Relocatable file)\n");
            break;
        case 0x02:
            printf("(Executable file)\n");
            break;
        case 0x03:
            printf("(Shared object file)\n");
            break;
        case 0xff00:
            printf("(Beginning of Processor -specific)\n");
            break;
        case 0xffff:
            printf("(Processor -specific)\n");
            break;
        default:
            printf("\n");
            break;
    }

    //todo 3.Machine
    printf("Required Architecture: 0x%04x", header.e_machine);
    if (header.e_machine == 0x0028) {
        printf("(ARM)\n");
    } else {
        printf("\n");
    }

    //todo 4.Version
    printf("Version: 0x%02x\n", header.e_version);
    //todo 5.程序进入地址
    printf("Start Program Address: 0x%08x\n", header.e_entry);
    //todo 6.程序头部偏移地址
    printf("Program Header Offset: 0x%08x\n", header.e_phoff);
    off.progream_header_off = header.e_phoff;
    //todo 7.节头部偏移地址
    printf("Section Header Offset: 0x%08x\n", header.e_shoff);
    off.section_header_off = header.e_shoff;

    //todo 8.文件权限flag，可读，可写，可执行
    printf("Process-specific Flags: 0x%08x\n", header.e_flags);
    //todo 9.ELF 头大小
    printf("ELF Header Size: 0x%04x\n", header.e_ehsize);
    //todo 10.程序头部表
    printf("Size of entry in program header table: 0x%04x\n", header.e_phentsize);
    //todo 11.程序头大小
    printf("Program Header Size: 0x%04x\n", header.e_phnum);
    off.progream_size = header.e_phnum;
    //todo 12.节头部
    printf("Size of an entry in the section header table: 0x%04x\n", header.e_shentsize);
    //todo 13.节头部大小
    printf("Section Header Size: 0x%04x\n", header.e_shnum);
    off.section_size = header.e_shnum;
    //todo 14.字符节位置
    printf("String Section Index: 0x%04x\n", header.e_shstrndx);
    off.sh_strtab_index = header.e_shstrndx;
    return off;
}
