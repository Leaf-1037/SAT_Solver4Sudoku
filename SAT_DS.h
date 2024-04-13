#pragma once

/***********************************
*�ļ�����:  satSolver.h
*�ļ�����:  ����SAT��������ݽṹ����
*��    ��:  2022-8-27
*��    ��:  ��׿2101 ����
***********************************/

#include <stdio.h>

// MAX_VAR_NUMBER�Ǳ�����ٶ���������ɵı�Ԫ����������������������Ľṹ��ʹ��
#define MAX_VAR_NUMBER 5000

// FALSE��ʾ�ñ�ԪΪ�٣�TRUE��ʾ�ñ�ԪΪ�棬UNVISITED��ʾ�ñ�Ԫδ����
#define FALSE 0
#define TRUE 1
#define UNVISITED -1

// UNSATISFIED��ʾ���Ӿ䲻���㣬UNKNOWN��ʾ��ȷ���ı�Ԫ�������ƶϳ����Ӿ�����״̬
#define UNSATISFIED -2
#define UNKNOWN -3

typedef struct literal {
	int data;						// ���ֵ�ȡֵ
	struct literal* nxt = NULL;			// ָ����һ���Ӿ�����һ������
} Literal;

typedef struct clause {
	int LiteralCounter = 0;					// �����Ӿ����ܱ�Ԫ����
	int AssignedLiteralCounter = 0;			// �����Ѹ�ֵ�ı�Ԫ����
	int isSatisfied = FALSE;				// �ж��Ƿ��ѱ�����
	struct literal* firstarc = NULL;		// ָ���Ӿ��е�һ������
	struct clause* nxtClause = NULL;		// ���Ӿ��ڽӵ���һ���Ӿ�
} Clause;

typedef struct cnf {
	struct clause* root = NULL;
	int numVar = 0;
	int numClause = 0;
	int numClauseSatisfied = 0;				//��������Ӿ����
	bool Satisfied;
} cnfFile;


typedef struct satAnswer {
	int numTotalLiteral = 0;
	int val[MAX_VAR_NUMBER + 1];//��¼�ñ�������ֵΪ��/��/δ����
	int branchLevel[MAX_VAR_NUMBER + 1];//��¼�ñ�Ԫ����ֵʱ�����Ĳ㼶
} SatAnswer;


typedef struct list {
	Clause* data;
	struct list* nxt;
} List;

// ʹ��˫����ල��
typedef struct watchedList {
	List* pos_watched[MAX_VAR_NUMBER + 1]; // �����ּල��
	List* neg_watched[MAX_VAR_NUMBER + 1]; // �����ּල��
} WatchedList;
