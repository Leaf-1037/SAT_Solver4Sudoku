#pragma once

#define MAX_STR_LENGTH 256

#define NONE          "\033[m"
#define RED           "\033[0;32;31m"
#define LIGHT_RED     "\033[1;31m"
#define GREEN         "\033[0;32;32m"
#define LIGHT_GREEN   "\033[1;32m"
#define BLUE          "\033[0;32;34m"
#define LIGHT_BLUE    "\033[1;34m"
#define DARY_GRAY     "\033[1;30m"
#define CYAN          "\033[0;36m"
#define LIGHT_CYAN    "\033[1;36m"
#define PURPLE        "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN         "\033[0;33m"
#define YELLOW        "\033[1;33m"
#define LIGHT_GRAY    "\033[0;37m"
#define WHITE         "\033[1;37m"

// COLUMN表示数独的行列数
#define COLUMN 9

// TOTAL_INSTRUCTION_NUMBER表示总指令之个数
#define TOTAL_INSTRUCTION_NUMBER 10

char Handle_INST[TOTAL_INSTRUCTION_NUMBER][MAX_STR_LENGTH + 1]
= { "exit", "SAT", "Sudoku", "help", "load", "solve", "save", "check", "play", "cls" };
//  该指令集对应的句柄展示如下
//    0       1        2       3        4       5        6       7        8      9

