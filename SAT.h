#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <time.h>

#include "SAT_DS.h"
#include "CNFparser.h"
#include "Display_opt.h"


void SAT() {
    cnfFile S;
    SatAnswer A;
    WatchedList W;
    FILE* fp;
    int op;
    char str_[MAX_STR_LENGTH];
    pre_init_Watched_List(&W);
    SAT_disp_print();
    printf("\n\t\tInput your operation[0..2]:\t");
    scanf_s("%d", &op);
    while (op) {
        if (op == 1) {
            system("cls");
            printf("\nTest SAT Filename (*.cnf):\t");
            scanf_s("%s", str_, MAX_STR_LENGTH - 1);
            errno_t err = fopen_s(&fp, str_, "r");
            if (err) {
                printf("Error: Invalid Filename.\n");
                return;
            }
            init_Sat(&S, &A, &W);
            CNF_Load(&S, fp, &W, &A);
            //TEST_WATCHLIST_IS_RIGHT_(W);
            clock_t st, ed;
            st = clock();
            DPLL(&S, &A, &W, 1, 2);
            ed = clock();
            system("cls");
            printf("Progress: %.2lf%%\n", 100.0);
            if (S.Satisfied) {
                printf("One solution of SAT is like:\n");
                for (int i = 1; i <= Ans.numTotalLiteral; ++i)
                    if (Ans.val[i] == TRUE || Ans.val[i] == UNVISITED) printf("%d ", i);
                    else if (Ans.val[i] == FALSE) printf("%d ", -i);
                    else printf("0 ");
                printf("\n");
                int flag = CheckResultsIsCorrect(&S, &Ans);
                if (flag) printf("Correct! Total num = %d\n\t", cntAnswerNum);
            }
            else {
                printf("This SAT has no solution.\n");
            }
            printf("\tRuntime: %dms\n", (int)((double)(ed - st) * 1000 / CLOCKS_PER_SEC));
            //printf("\tSingleClauseRuntime: %dms\n", (int)((double)(tot) * 1000 / CLOCKS_PER_SEC));
        }
        else if (op == 2) {

        }
        else {
            printf("\t\tPlease choose the operation again[0..2]:\t");
            scanf_s("%d", &op);
            continue;
        }
        Sleep(1000);
        printf("\n\n\t\t\tPress any key continue..");
        getchar();
        getchar();
        system("cls");
        SAT_disp_print();
        printf("\n\t\tInput your operation[0..2]:\t");
        scanf_s("%d", &op);
    }
}