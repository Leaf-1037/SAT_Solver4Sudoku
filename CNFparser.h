#pragma once

/***********************************
*文件名称:  satSolver.h
*文件描述:  关于SAT问题的求解函数
*日    期:  2022-8-27
*作    者:  计卓2101 王彬
***********************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SAT_DS.h"

SatAnswer Ans;
int cntAnswerNum = 0;
double progress_cnf = 0.0;

double power2(int x);
void init_Sat(cnfFile* S, SatAnswer* A, WatchedList* W);
void pre_init_Watched_List(WatchedList* W);
bool CNF_Load(cnfFile* S, FILE* fp, WatchedList* W, SatAnswer* A);
bool DPLL(cnfFile* S, SatAnswer* A, WatchedList* W, int branchLevel_, int op);
int SimplifySAT(cnfFile* S, int value, WatchedList* W, SatAnswer* A, int branchLevel_);
Clause* singleClause(cnfFile* S, SatAnswer* A);
int selectVar(cnfFile S, SatAnswer A);
int selectVar_Optimized(cnfFile* S, SatAnswer* A, WatchedList* W);
bool isUnitClause(Clause* x);
bool isClauseSatisfied(cnfFile* S, Clause* x, SatAnswer* A);
bool isCnfSatisfied(cnfFile* S, SatAnswer* A);
void printAnswer(SatAnswer A);
void printProgress(int x);
bool CheckResultsIsCorrect(cnfFile* S, SatAnswer* A);

/* 优化警示：
1.检验空子句居然使用的是全文通检，慢！
2.选择变元策略不好。*/

double power2(int x) {
	// 快速幂求2^(-x)
	if (x == 0) return 1.0;
	if (x == 1) return 0.5;
	if (x % 2) return 0.5 * (power2(x / 2) * power2(x / 2));
	return (power2(x / 2) * power2(x / 2));
}

void TEST_CNF_IS_RIGHT(cnfFile S, SatAnswer* A) {
	printf("\nnumVar=%d\nnumClause=%d\n", S.numVar, S.numClause);
	Clause* p = S.root;
	int cnt = 0;
	while (p) {
		printf("Clause #%d: Assigned %d, Total %d, Satisfied %d, Satisfied Flag %d\n", ++cnt, p->AssignedLiteralCounter, p->LiteralCounter, isClauseSatisfied(&S, p, A), p->isSatisfied ? 1 : 0);

		p = p->nxtClause;
	}
	printf("\nSatisfiedClause=%d\n", S.numClauseSatisfied);
	printf("\n");
}


/* 1. 初始化模组  */

void pre_init_Watched_List(WatchedList* W) {
	for (int i = 1; i <= MAX_VAR_NUMBER; ++i) W->pos_watched[i] = NULL;
	for (int i = 1; i <= MAX_VAR_NUMBER; ++i) W->neg_watched[i] = NULL;
}

void init_Sat(cnfFile* S, SatAnswer* A, WatchedList* W) {
	/* 初始化cnf算例[S]，算例解[A] */
	// 回收子句空间
	//printf("1\n");
	cntAnswerNum = 0;
	S->numClause = 0;
	S->numVar = 0;
	Clause* chead = S->root, * ct;
	Literal* lhead, * lt;
	while (chead) {
		ct = chead->nxtClause;
		lhead = chead->firstarc;
		while (lhead) {
			lt = lhead->nxt;
			free(lhead);
			lhead = lt;
		}
		free(chead);
		chead = ct;
	}
	// 初始化A
	A->numTotalLiteral = 0;
	for (int i = 1; i <= MAX_VAR_NUMBER; ++i)
		A->val[i] = UNVISITED;
	for (int i = 1; i <= MAX_VAR_NUMBER; ++i)
		A->branchLevel[i] = 0;
	// 初始化Ans
	Ans.numTotalLiteral = 0;
	for (int i = 1; i <= MAX_VAR_NUMBER; ++i)
		Ans.val[i] = UNVISITED;
	for (int i = 1; i <= MAX_VAR_NUMBER; ++i)
		Ans.branchLevel[i] = 0;
	// 初始化索引表W
	for (int i = 1; i <= MAX_VAR_NUMBER; ++i) {
		List* phead = W->pos_watched[i], * pt;
		while (phead) {
			pt = phead->nxt;
			free(phead);
			phead = pt;
		}
		W->pos_watched[i] = NULL;
	}
	for (int i = 1; i <= MAX_VAR_NUMBER; ++i) {
		List* phead = W->neg_watched[i], * pt;
		while (phead) {
			pt = phead->nxt;
			free(phead);
			phead = pt;
		}
		W->neg_watched[i] = NULL;
	}
	progress_cnf = 0.0;
}

bool CNF_Load(cnfFile* S, FILE* fp, WatchedList* W, SatAnswer* A) {
	char c;
	int t;
	int numVar = 0, numClause = 0;
	Clause* tmp;
	tmp = (Clause*)malloc(sizeof(struct clause));// head pointer
	fscanf_s(fp, "%c", &c, 1);
	while (c == 'c') {
		while (c != '\n') fscanf_s(fp, "%c", &c, 1);
		while (c == '\n') fscanf_s(fp, "%c", &c, 1);		// 跳过空行
	}
	fscanf_s(fp, " cnf ");
	fscanf_s(fp, "%d%d", &numVar, &numClause);
	(S)->numVar = numVar;
	A->numTotalLiteral = numVar;
	(S)->numClause = numClause;
	S->numClauseSatisfied = 0;
	S->root = tmp;
	for (int i = 0; i < numClause; ++i) {
		fscanf_s(fp, "%d", &t);
		literal* p = NULL;							// p用于建立文字链表
		bool flag = true;							// 标记正在读取的子句是否为算例的第一个子句
		tmp->AssignedLiteralCounter = tmp->LiteralCounter = 0;
		tmp->isSatisfied = FALSE;
		while (t != 0) {
			if (flag) {
				tmp->firstarc = (Literal*)malloc(sizeof(struct literal));
				tmp->firstarc->data = t;
				p = tmp->firstarc;
				flag = false;
				// 建立索引表
				if (t > 0) {
					List* pt = W->pos_watched[t];
					W->pos_watched[t] = (List*)malloc(sizeof(List));
					W->pos_watched[t]->nxt = pt;
					W->pos_watched[t]->data = tmp;
				}
				else {
					List* pt = W->neg_watched[-t];
					W->neg_watched[-t] = (List*)malloc(sizeof(List));
					W->neg_watched[-t]->nxt = pt;
					W->neg_watched[-t]->data = tmp;
				}
				++(tmp->LiteralCounter);
				fscanf_s(fp, "%d", &t);
				continue;
			}
			++(tmp->LiteralCounter);
			p->nxt = (Literal*)malloc(sizeof(struct literal));
			p = p->nxt;
			p->data = t;
			// 建立索引表
			if (t > 0) {
				List* pt = W->pos_watched[t];
				W->pos_watched[t] = (List*)malloc(sizeof(List));
				W->pos_watched[t]->nxt = pt;
				W->pos_watched[t]->data = tmp;
			}
			else {
				List* pt = W->neg_watched[-t];
				W->neg_watched[-t] = (List*)malloc(sizeof(List));
				W->neg_watched[-t]->nxt = pt;
				W->neg_watched[-t]->data = tmp;
			}
			fscanf_s(fp, "%d", &t);
		}
		p->nxt = NULL;
		if (i < numClause - 1) {
			tmp->nxtClause = (Clause*)malloc(sizeof(struct clause));
			tmp = tmp->nxtClause;
		}
	}
	Ans.numTotalLiteral = A->numTotalLiteral;
	tmp->nxtClause = NULL;
	S->Satisfied = false;
	return true;
}


/* 2. DPLL算法模组  */

bool DPLL(cnfFile* S, SatAnswer* A, WatchedList* W, int branchLevel_, int op) {
	// op选择模式
	//if (S->Satisfied) return TRUE;//如果已经有解 就不必进行下一步计算了
	printProgress(branchLevel_);
	Clause* t;
	while ((t = singleClause(S, A)) != NULL) {
		// 取单子句中的变元value
		Literal* p = t->firstarc;
		while (p) {
			if (A->val[abs(p->data)] == UNVISITED) {// 需要该变元未被访问
				break;
			}
			p = p->nxt;
		}
		int value = p->data;// 单子句中包含的变元,可能为abs(val)或 非abs(val)
		// 利用value为真的性质进行化简
		int res = SimplifySAT(S, value, W, A, branchLevel_);
		A->branchLevel[abs(value)] = branchLevel_;
		if (res == -1) return FALSE;
		if (res == 1) {
			S->Satisfied = true;
			++cntAnswerNum;
			printAnswer(*A);
			return TRUE;
		}
	}

	// 选择变元v
	int v = ((op == 2) ? selectVar_Optimized(S, A, W) : selectVar(*S, *A));

	// 回溯时“解铃还须系铃人”，对于比branchLevel层级高的变元对其pos/neg监督表进行依次地复原
	// 留意：需要连带修改satAnswer里的val和branchLevel，Clause中的AssignedLiteralCounter
	// 至于该子句是否仍满足，应当手工判断

	int res = SimplifySAT(S, v, W, A, branchLevel_);// 将v代入化简
	if (res == -1) return FALSE;
	if (res == 1) return TRUE;
	if (DPLL(S, A, W, branchLevel_ + 1, op)) {
		S->Satisfied = true;
		++cntAnswerNum;
		printAnswer(*A);
		return TRUE;
	}
	//选择变元非v，需进行回溯操作
	for (int i = 1; i <= S->numVar; ++i) {
		if (A->branchLevel[i] > branchLevel_) {
			List* x = W->pos_watched[i];
			while (x) {
				Clause* y = x->data;
				--(y->AssignedLiteralCounter);
				isClauseSatisfied(S, y, A);//修正计数值
				x = x->nxt;
			}
			x = W->neg_watched[i];
			while (x) {
				Clause* y = x->data;
				--(y->AssignedLiteralCounter);
				isClauseSatisfied(S, y, A);
				x = x->nxt;
			}
			A->val[i] = UNVISITED;
			A->branchLevel[i] = 0;
		}
	}

	List* x = W->pos_watched[abs(v)];
	while (x) {
		Clause* y = x->data;
		--(y->AssignedLiteralCounter);
		isClauseSatisfied(S, y, A);
		x = x->nxt;
	}
	x = W->neg_watched[abs(v)];
	while (x) {
		Clause* y = x->data;
		--(y->AssignedLiteralCounter);
		isClauseSatisfied(S, y, A);
		x = x->nxt;
	}
	A->val[abs(v)] = UNVISITED;
	A->branchLevel[abs(v)] = 0;

	res = SimplifySAT(S, -v, W, A, branchLevel_);//选择变元-v
	if (res == -1) return FALSE;
	if (res == 1) {
		S->Satisfied = true;
		++cntAnswerNum;
		printAnswer(*A);
		return TRUE;
	}
	return DPLL(S, A, W, branchLevel_ + 1, op);
}

int SimplifySAT(cnfFile* S, int value, WatchedList* W, SatAnswer* A, int branchLevel_) {
	// 运用变量val化简S,若使S中有空子句返回-1,若使其成为空集返回1，否则返回0
	// 这里的value变元被标记“未被访问”
	int res = 0;
	if (value > 0) {
		A->branchLevel[value] = branchLevel_;
		A->val[value] = TRUE;
		List* p = W->pos_watched[value];
		while (p) {
			Clause* x = p->data;
			++(x->AssignedLiteralCounter);
			if (!isClauseSatisfied(S, x, A) && x->AssignedLiteralCounter == x->LiteralCounter) {
				res = -1;
			}
			p = p->nxt;
		}
		List* q = W->neg_watched[value];
		while (q) {
			Clause* x = q->data;
			++(x->AssignedLiteralCounter);
			if (!isClauseSatisfied(S, x, A) && x->AssignedLiteralCounter == x->LiteralCounter) {
				res = -1;
			}
			q = q->nxt;
		}
	}
	if (value < 0) {
		A->branchLevel[abs(value)] = branchLevel_;
		A->val[abs(value)] = FALSE;
		List* p = W->neg_watched[abs(value)];
		while (p) {
			Clause* x = p->data;
			++(x->AssignedLiteralCounter);
			if (!isClauseSatisfied(S, x, A) && x->AssignedLiteralCounter == x->LiteralCounter) {
				res = -1;
			}
			p = p->nxt;
		}
		List* q = W->pos_watched[abs(value)];
		while (q) {
			Clause* x = q->data;
			++(x->AssignedLiteralCounter);
			if (!isClauseSatisfied(S, x, A) && x->AssignedLiteralCounter == x->LiteralCounter) {
				res = -1;
			}
			q = q->nxt;
		}
	}
	if (res == -1) return res;
	if (isCnfSatisfied(S, A)) res = 1;
	return res;
}

Clause* singleClause(cnfFile* S, SatAnswer* A) {
	// 如果存在单子句，返回该单子句；如果不存在，返回NULL
	//clock_t be, ed;
	//be = clock();
	Clause* x = S->root;
	while (x) {
		//if (!isClauseSatisfied(S, x, A) && x->AssignedLiteralCounter == x->LiteralCounter - 1)
		if (isUnitClause(x))
			return x;
		x = x->nxtClause;
	}
	//ed = clock();
	//tot += ed - be;
	return NULL;
}

bool isUnitClause(Clause* x) {
	return (!x->isSatisfied) && (x->AssignedLiteralCounter == x->LiteralCounter - 1);
}

bool isClauseSatisfied(cnfFile* S, Clause* x, SatAnswer* A) {
	Literal* p = x->firstarc;
	while (p) {
		if (p->data > 0 && A->val[p->data] == TRUE) {
			if (x->isSatisfied == FALSE) {
				x->isSatisfied = TRUE;
				(S->numClauseSatisfied)++;			// 子句x的真假状态更改，对S的已满足子句计数贡献为+-1，下同
			}
			return true;
		}
		if (p->data < 0 && A->val[abs(p->data)] == FALSE) {
			if (x->isSatisfied == FALSE) {
				x->isSatisfied = TRUE;
				(S->numClauseSatisfied)++;
			}
			return true;
		}
		p = p->nxt;
	}
	if (x->isSatisfied == TRUE) {
		x->isSatisfied = FALSE;
		(S->numClauseSatisfied)--;
	}
	return false;
}

bool isCnfSatisfied(cnfFile* S, SatAnswer* A) {
	//if (S->Satisfied == true) return false;				//S已满足，故不再产生新解
	return (S->numClause == S->numClauseSatisfied);
}

int selectVar(cnfFile S, SatAnswer A) {
	// 依据策略选取变元 待优化
	// 若所有变元已赋值 返回0
	// 应当返回自然数
	Clause* p = S.root;
	Clause* q = p;
	int minn = MAX_VAR_NUMBER;
	while (p) {
		if (p->LiteralCounter - p->AssignedLiteralCounter > 0 && p->LiteralCounter - p->AssignedLiteralCounter < minn) {
			q = p;
			minn = p->LiteralCounter - p->AssignedLiteralCounter;
		}
		p = p->nxtClause;
	}
	Literal* t = q->firstarc;
	while (t) {
		if (A.val[abs(t->data)] == UNVISITED) return abs(t->data);
		t = t->nxt;
	}
	//return 0;
}

int selectVar_Optimized(cnfFile* S, SatAnswer* A, WatchedList* W) {
	int loc = 0;
	double maxx = -1.0;
	for (int i = 1; i <= S->numVar; ++i)
		if (A->val[i] == UNVISITED) {
			double t = 0.0, r = 0.0;
			List* x = W->pos_watched[i];
			while (x) {
				if (x->data->isSatisfied == FALSE)
					t += power2(x->data->LiteralCounter - x->data->AssignedLiteralCounter);
				x = x->nxt;
			}
			x = W->neg_watched[i];
			while (x) {
				if (x->data->isSatisfied == FALSE)
					r += power2(x->data->LiteralCounter - x->data->AssignedLiteralCounter);
				x = x->nxt;
			}
			if (t + r > maxx) {
				loc = i;
				maxx = t + r;
			}
		}
	return loc;
}


/* 3. 打印及检查模组  */

void printAnswer(SatAnswer A) {
	for (int i = 1; i <= A.numTotalLiteral; ++i)
		Ans.val[i] = A.val[i];
	Ans.numTotalLiteral = A.numTotalLiteral;
}

void printProgress(int branchlevel_) {
	if (1.0 * (branchlevel_ - 1) / Ans.numTotalLiteral > progress_cnf + 1e-3) {
		progress_cnf = 1.0 * (branchlevel_ - 1) / Ans.numTotalLiteral;
		system("cls");
		printf("Progress: %.2lf%%\n", 100.0 * progress_cnf);
	}
}

bool CheckResultsIsCorrect(cnfFile* S, SatAnswer* A) {
	Clause* x = S->root;
	while (x) {
		if (!isClauseSatisfied(S, x, A)) return false;
		x = x->nxtClause;
	}
	return true;
}