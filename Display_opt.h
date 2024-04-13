#pragma once

#include <iostream>
#include <string>
#include <stdio.h>

#include "Display.h"

void mainDisplay_print();
void SAT_disp_print();
void help_print();
void header_print(int module);
void Error_print(int module);
void Ending_print();
void colorNumber_print(int val, int cur, int pos);
int Return_Handle(char str[]);
int get_Main_Operater();

void mainDisplay_print() {
    printf("\n\n");
    printf("              ||        ||   || ""       ||    --------     _____________        \n");
    printf("              ||        ||   || ""       ||   //       \\\\  |-----..------|               \n");
    printf("              ||        ||   || ""       ||  ||                  ||       \n");
    printf("              ||        ||   || ""       ||   \\\\                 ||          \n");
    printf("              ||________||   || ""       ||     -------          ||             \n");
    printf("              ||--------||   || ""       ||            \\\\        ||          \n");
    printf("              ||        ||   || ""       ||             ||       ||        \n");
    printf("              ||        ||   || ""       ||             ||       ||      \n");
    printf("              ||        ||   \\\\_______//    \\\\       //        ||           \n");
    printf("              ||        ||     ""               --------         ||       \n");
    printf("\n\n");
    printf("                        --- Sudoku based on SAT Command-Line ---\n");
    printf("                       /                                        \\\n");
    printf("                      /                                          \\\n");
    printf("                     /                 " LIGHT_CYAN"Welcome~" NONE"                   \\\n");
    printf("                    /                                              \\\n");
    printf("                   /                                                \\\n");
    printf("                    Input" LIGHT_PURPLE" < help + Enter >" NONE " if to learn how to start.\n\n\n");
}

void SAT_disp_print() {
    printf("\n\t\t************* SAT *************\n");
    printf("\t\t| 1. New SAT  2. Check Answer |\n");
    printf("\t\t| 0. Exit                     |\n");
    printf("\t\t|_____________________________|\n");
}

void help_print() {
    printf("\n\n                                            --- Helper ---\n\n");
    printf("            1. This project is a solver to SAT as a classic NP-hard problem, based on DPLL-JW.\n");
    printf("            2. As an application, you can play the game of Sudoku. Have fun!\n\n");
    printf("                                           by HUST-CS WB.\n\n\n\n");
    printf("    As a command-line environment, the instructions are...\n\n\n");
    printf("      In " CYAN "<Main>" NONE " Module:\n\n");
    printf("         1.  SAT " LIGHT_PURPLE " <Command>  " NONE "              | Go into <SAT> Module\n");
    printf("         2.  Sudoku " LIGHT_PURPLE " <Command> " NONE "            | Go into <Sudoku> Module\n");
    printf("         3.  help                          | Print the helper page\n");
    printf("         4.  exit                          | Exit this program\n\n");
    printf("      In " CYAN "<SAT>" NONE " Module:\n\n");
    printf("         1.  load" LIGHT_PURPLE"  Filename" NONE "                | Load the given file to SAT solver\n");
    printf("         2.  solve" LIGHT_PURPLE"  <Filename>" NONE "             | Solve the SAT. Filename is NOT necessary if loaded\n");
    printf("         3.  save" LIGHT_PURPLE"  Filename" NONE "                | Save the answer to previous SAT\n");
    printf("         4.  check" LIGHT_PURPLE"  Filename_1 Filename_2" NONE "  | check whether the answer is right or not\n");
    printf("         5.  exit                          | Return to <Main> Module\n\n");
    printf("      In " CYAN "<Sudoku>" NONE " Module:\n\n");
    printf("         1.  play                          | play Sudoku game\n");
    printf("         2.  exit                          | Return to <Main> Module\n\n");
    printf("     In " CYAN " ALL" NONE " Module:\n\n");
    printf("         1.  cls                           | Clear screen.\n\n\n");
    printf("     Notice: In <Main> Module, you can also enter the module name + command instruction.\n\n\n\n");
    // 最后一行暂缓做
}

void header_print(int module) {
    if (module == 0) printf("Project\\Main> ");
    if (module == 1) printf("Project\\Main\\SAT> ");
    if (module == 2) printf("Project\\Main\\Sudoku> ");
}

void Error_print(int module) {
    header_print(module);
    printf("Sorry, we don't know what it means. Please Enter again (press <Enter> to end).\n");
}

void Ending_print() {
    system("cls");
    printf("\n\n\n                               " LIGHT_CYAN"         Thanks for playing ~ " NONE "\n\n");
    printf("                                     By Wang Bin of HUST-CSEE-21 \n\n");
}

int Return_Handle(char str[]) {
    if (*str == '\0') return -2;
    for (int i = 0; i < TOTAL_INSTRUCTION_NUMBER; ++i) {
        bool flag = true;
        int j = 0;
        for (; j < MAX_STR_LENGTH && str[j] != '\0' && Handle_INST[i][j] != '\0'; ++j)
            if (str[j] != Handle_INST[i][j]) {
                flag = false;
                break;
            }
        if (flag && Handle_INST[i][j] == '\0') return i;
    }
    return -1;
}

int get_Main_Operater() {
    header_print(0);
    std::string input_;
    std::getline(std::cin, input_);
    char str[MAX_STR_LENGTH + 1];
    int op = -2;
    for (int i = 0; i < MAX_STR_LENGTH && input_.c_str()[i] != '\0'; ++i)
        if (input_.c_str()[i] != ' ') {
            int j = i;
            for (; input_.c_str()[j] != '\0'; ++j) str[j - i] = input_.c_str()[j];
            str[j] = '\0';
            op = Return_Handle(str);
            break;
        }
    return op;
}


/**
* @param   需打印的数字val，val的序号cur，光标位置 pos
* @return  void
* @brief   显示双数独棋盘显示的框架
*/

void colorNumber_print(int val, int cur, int pos) {
    // 约定【val为正】表示 该数字为棋盘上固有的数字
    // 约定【val为负】表示 该数字为玩家填写的数字
    // 约定【val为0】表示 该数字玩家尚未填写
    bool flag = false;
    if (cur == pos) flag = true;
    // todo
    if (flag) {
        if (val > 0) printf(LIGHT_RED "%d" NONE, val);
        if (val < 0) printf(BROWN "%d" NONE, val);
        if (val == 0) printf(BROWN "_" NONE);
        return;
    }
    if (val > 0) printf(LIGHT_PURPLE "%d" NONE, val);
    if (val < 0) printf(YELLOW "%d" NONE, val);
    if (val == 0) printf(YELLOW "_" NONE);
}

/**
* @param   棋盘1 chess1，棋盘2 chess2，光标位置 pos
* @return  void
* @brief   显示双数独棋盘显示的框架
*/

void Sudoku_print(int chess1[], int chess2[], int pos) {
    // if in chess1, pos = 9*i+j; if in chess 2, pos = 81 + 9*i+j
    // 注意光标位置可能在两个棋盘的交集区域
    system("cls");
    printf("\n");
    int p = 0;
    printf(BLUE "  -");
    for (int i = 0; i < 8; ++i) printf("----");
    printf("---|\n" NONE);
    for (int i = 0; i < 8; ++i) {
        // chess 1
        if (i <= 5) {
            printf(BLUE  "  | " NONE);
            for (int j = 0; j < 8; ++j) {
                colorNumber_print(chess1[i * 9 + j], i * 9 + j, pos);
                if (j % 3 == 2) printf(BLUE);
                printf(" | ");
                if (j % 3 == 2) printf(NONE);
            }       // 绘制数字行
            colorNumber_print(chess1[i * 9 + 8], i * 9 + 8, pos);
            printf(BLUE " |\n" NONE);
        }
        else {
            printf(BLUE  "  | " NONE);
            for (int j = 0; j < 8; ++j) {
                colorNumber_print(chess1[i * 9 + j], i * 9 + j, pos);
                if (j % 3 == 2) printf(BLUE);
                printf(" | ");
                if (j % 3 == 2) printf(NONE);
            }       // 绘制数字行
            colorNumber_print(chess1[i * 9 + 8], i * 9 + 8, pos);
            printf(BLUE " | " NONE);
            //绘制部分chess2
            for (int j = 3; j < 9; ++j) {
                colorNumber_print(chess2[(i - 6) * 9 + j], 81 + (i - 6) * 9 + j, pos);
                if (j % 3 == 2) printf(BLUE);
                printf(" | ");
                if (j % 3 == 2) printf(NONE);
            }
            printf("\n");
        }

        printf(BLUE "  |" NONE);
        if (i % 3 == 2) {
            printf(BLUE);
            for (int j = 0; j < 8; ++j) {
                printf("---");
                printf("|");
            }
            printf("---|");
            printf(NONE);
        }
        else {
            for (int j = 0; j < 8; ++j) {
                printf("---");
                if (j % 3 == 2) printf(BLUE "|" NONE);
                else printf("|");
            }
            printf("---" BLUE "|" NONE);
        }

        // chess 2
        if (i == 5) {
            printf(BLUE);
            for (int i = 0; i < 5; ++i) printf("----");
            printf("---|" NONE);
        }
        if (i > 5) {
            printf("\\");
        }

        printf("\n");
    }
    printf(BLUE "  | " NONE);
    for (int j = 0; j < 8; ++j) {
        colorNumber_print(chess1[8 * 9 + j], 8 * 9 + j, pos);
        if (j % 3 == 2) printf(BLUE);
        printf(" | ");
        if (j % 3 == 2) printf(NONE);
    }
    colorNumber_print(chess1[80], 80, pos);
    printf(BLUE " |\n" NONE);
    printf(BLUE "  |");
    for (int i = 0; i < 8; ++i) printf("----");
    printf("----\n" NONE);

    // 打印说明
    printf("\n   Press <up> <down> <left> <right> to play \\\n");
    printf("   <e> to exit, <s> to submit your answer.\n\n");
}

void Sudoku_print(int chess1[], int pos) {
    // if in chess1, pos = 9*i+j; if in chess 2, pos = 81 + 9*i+j
    // 注意光标位置可能在两个棋盘的交集区域
    system("cls");
    printf("\n");
    int p = 0;
    printf(BLUE "  -");
    for (int i = 0; i < 8; ++i) printf("----");
    printf("---|\n" NONE);
    for (int i = 0; i < 8; ++i) {
        // chess 1
        {
            printf(BLUE  "  | " NONE);
            for (int j = 0; j < 8; ++j) {
                colorNumber_print(chess1[i * 9 + j], i * 9 + j, pos);
                if (j % 3 == 2) printf(BLUE);
                printf(" | ");
                if (j % 3 == 2) printf(NONE);
            }       // 绘制数字行
            colorNumber_print(chess1[i * 9 + 8], i * 9 + 8, pos);
            printf(BLUE " |\n" NONE);
        }


        printf(BLUE "  |" NONE);
        if (i % 3 == 2) {
            printf(BLUE);
            for (int j = 0; j < 8; ++j) {
                printf("---");
                printf("|");
            }
            printf("---|");
            printf(NONE);
        }
        else {
            for (int j = 0; j < 8; ++j) {
                printf("---");
                if (j % 3 == 2) printf(BLUE "|" NONE);
                else printf("|");
            }
            printf("---" BLUE "|" NONE);
        }

        printf("\n");
    }
    printf(BLUE "  | " NONE);
    for (int j = 0; j < 8; ++j) {
        colorNumber_print(chess1[8 * 9 + j], 8 * 9 + j, pos);
        if (j % 3 == 2) printf(BLUE);
        printf(" | ");
        if (j % 3 == 2) printf(NONE);
    }
    colorNumber_print(chess1[80], 80, pos);
    printf(BLUE " |\n" NONE);
    printf(BLUE "  |");
    for (int i = 0; i < 8; ++i) printf("----");
    printf("----\n" NONE);

    // 打印说明
    printf("\n   Press <up> <down> <left> <right> to play \\\n");
    printf("   <e> to exit, <s> to submit your answer.\n\n");
}
