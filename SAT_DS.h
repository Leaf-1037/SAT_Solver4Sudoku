#pragma once

/***********************************
*文件名称:  satSolver.h
*文件描述:  关于SAT问题的数据结构定义
*日    期:  2022-8-27
*作    者:  计卓2101 王彬
***********************************/

#include <stdio.h>

// MAX_VAR_NUMBER是本程序假定的最大容纳的变元数量，仅在无需运用链表的结构中使用
#define MAX_VAR_NUMBER 5000

// FALSE表示该变元为假，TRUE表示该变元为真，UNVISITED表示该变元未访问
#define FALSE 0
#define TRUE 1
#define UNVISITED -1

// UNSATISFIED表示该子句不满足，UNKNOWN表示已确定的变元不足以推断出该子句的真假状态
#define UNSATISFIED -2
#define UNKNOWN -3

typedef struct literal {
	int data;						// 文字的取值
	struct literal* nxt = NULL;			// 指向下一个子句中下一个文字
} Literal;

typedef struct clause {
	int LiteralCounter = 0;					// 计数子句中总变元个数
	int AssignedLiteralCounter = 0;			// 计数已赋值的变元个数
	int isSatisfied = FALSE;				// 判断是否已被满足
	struct literal* firstarc = NULL;		// 指向子句中第一个文字
	struct clause* nxtClause = NULL;		// 该子句邻接的下一个子句
} Clause;

typedef struct cnf {
	struct clause* root = NULL;
	int numVar = 0;
	int numClause = 0;
	int numClauseSatisfied = 0;				//已满足的子句计数
	bool Satisfied;
} cnfFile;


typedef struct satAnswer {
	int numTotalLiteral = 0;
	int val[MAX_VAR_NUMBER + 1];//记录该变量被赋值为真/假/未访问
	int branchLevel[MAX_VAR_NUMBER + 1];//记录该变元被赋值时所处的层级
} SatAnswer;


typedef struct list {
	Clause* data;
	struct list* nxt;
} List;

// 使用双字面监督法
typedef struct watchedList {
	List* pos_watched[MAX_VAR_NUMBER + 1]; // 正文字监督表
	List* neg_watched[MAX_VAR_NUMBER + 1]; // 负文字监督表
} WatchedList;
