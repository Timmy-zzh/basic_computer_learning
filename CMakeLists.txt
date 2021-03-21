cmake_minimum_required(VERSION 3.16)
project(basic_computer_learning C)

set(CMAKE_C_STANDARD 99)

add_executable(basic_computer_learning main.c)
add_executable(elf elf/elf_parse.c elf/elf_type.h elf/data_offset.h)

# 添加子项目的 文件目录
add_subdirectory("memory")