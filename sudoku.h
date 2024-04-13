#pragma once

#include <time.h>
#include <conio.h>

#include "SAT_DS.h"
#include "CNFparser.h"
#include "Display.h"
#include "Display_opt.h"

int chess1[COLUMN * COLUMN + 1], chess2[COLUMN * COLUMN + 1];
double progress = 0.0; // ��������������

void Sudoku_play();
void generateSudoku();
bool build(int, int ch[]);
bool build2(int val, int ch[]);
void swap(int* a, int* b);
bool isLegal(int pos, int val, int* chess);

void Sudoku() {
	Sudoku_play();
}

void Sudoku_play() {
	generateSudoku();
	//for (int i = 0; i < 9; ++i) {
	//	for (int j = 0; j < 9; ++j) printf("%d ", chess1[9 * i + j]);
	//	printf("\n");
	//}
	//printf("\n");
	//for (int i = 0; i < 9; ++i) {
	//	for (int j = 0; j < 9; ++j) printf("%d ", chess2[9 * i + j]);
	//	printf("\n");
	//}
	//getchar();
	//getchar();
	Sudoku_print(chess1, chess2, 0);//
	int pos_i = 0, pos_j = 0;
	int ch = _getch();
	while (ch != 's' && ch != 'e') {
		switch (ch) {
		case 72:
			// up
			if (pos_i >= 1 && pos_i <= 8) --pos_i;
			break;
		case 80:
			// down
			if (pos_i >= 0 && pos_i <= 7) ++pos_i;
			break;
		case 75:
			//left
			if (pos_j >= 1 && pos_j <= 8) --pos_j;
			break;
		case 77:
			//right
			if (pos_j >= 0 && pos_j <= 7) ++pos_j;
			break;
		default:
			if (ch >= '1' && ch <= '9') {
				if (chess1[pos_i * 9 + pos_j] <= 0)
					chess1[pos_i * 9 + pos_j] = -(ch - '0');
			}
		}
		Sudoku_print(chess1, chess2, pos_i * 9 + pos_j);
		ch = _getch();
	}

}


/**
  * ������ӳ��Ϊһ����ֵ, ��2������
  * @return ��
  */
int f(int i, int j) {
	return 9 * (i - 1) + (j - 1);
}
int f(int i, int j, int k) {
	return 81 * (i - 1) + 9 * (j - 1) + k - 1;
}

/**
  * �����������
  * @return ����һ���������������
  */
void generateSudoku() {
	progress = 0.0;
	for (int i = 0; i < 81; ++i) chess1[i] = chess2[i] = 0;
	build(0, chess1);
	printf("\n\n");
	int xh, yh;
	xh = yh = 6;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j) {
			chess2[9 * i + j] = chess1[(xh + i) * 9 + yh + j];
		}
	build(0, chess2);
	return;
}

/**
  * �ݹ��������λ��i��ֵ
  * @param ����λ��i
  * @return λ��i�Ƿ��������ֵ
  */
bool build(int val, int ch[]) {
	//progress = max(progress, 1.0 * (val + 1) / 81);
	//printf("%d # \n", val);
	if (val == 81) return true; // �������д���񣬷���true
	else if (ch[val] != 0) {
		return build(val + 1, ch);
	}// ��λ��i������ʵ����֣��ݹ�����һ�����
	else {
		// ����1..9���������randArray
		int randArray[9];
		for (int i = 0; i < 9; ++i) {
			randArray[i] = (5 * i) % 9;
		}
		srand((int)time(0));
		for (int i = 0; i < 9; ++i) {
			int t = (rand() % 9 + 9) % 9;
			swap(&randArray[i], &randArray[t]);
		}
		for (int i = 0; i < 9; ++i) ++randArray[i];

		// ��λ��val������д����
		for (int i = 0; i < 9; ++i) {
			if (isLegal(val, randArray[i], ch)) {
				ch[val] = randArray[i];
				if (build(val + 1, ch)) {
					//printf("%d %d\n", val, ch[val]);
					return true;
				}
			}
		}
		ch[val] = 0;
		return false;
	}
}

/**
  * �ݹ��������2λ��i��ֵ
  * @param ����λ��i
  * @return λ��i�Ƿ��������ֵ
  */
bool build2(int val, int ch[]) {
	//progress = max(progress, 1.0 * (val + 1) / 81);
	printf("%d ?\n", val);
	if (val == 81) return true; // �������д���񣬷���true
	else if (ch[val] != 0) build(val + 1, ch); // ��λ��i������ʵ����֣��ݹ�����һ�����
	else {
		// ����1..9���������randArray
		int randArray[9];
		for (int i = 0; i < 9; ++i) {
			randArray[i] = (5 * i) % 9;
		}
		srand((int)time(0));
		for (int i = 0; i < 9; ++i) {
			int t = (rand() % 9 + 9) % 9;
			swap(&randArray[i], &randArray[t]);
		}
		for (int i = 0; i < 9; ++i) ++randArray[i];

		// ��λ��val������д����
		for (int i = 0; i < 9; ++i) {
			if (isLegal(val, randArray[i], ch)) {
				ch[val] = randArray[i];
				if (build(val + 1, ch)) {

					return true;
				}
			}
		}
	}
	ch[val] = 0;
	return false;
}

void swap(int* a, int* b) {
	int t = *b;
	*b = *a;
	*a = t;
}

bool isLegal(int pos, int val, int* chess) {
	int row = pos / 9;		// ������
	int col = pos % 9;		// ������
	int xh = row / 3 * 3;	// �Ź��������Ͻǵĺ�����
	int yh = col / 3 * 3;	// �Ź��������Ͻǵ�������
	// �жϸ����Ƿ�Ϸ�
	for (int i = 0; i < 9; ++i)
		if (val == chess[row * 9 + i])
			return false;
	// �жϸ����Ƿ�Ϸ�
	for (int i = 0; i < 9; ++i)
		if (val == chess[i * 9 + col])
			return false;
	// �жϸþŹ������Ƿ�Ϸ�
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (val == chess[(xh + i) * 9 + yh + j])
				return false;
	return true;
}