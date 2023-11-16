#ifndef __BOARD_H
#define __BOARD_H
typedef struct board{
	int size;
	int *boardptr;
}board;
void initboard(board *b);
int compareboard(board *b1, board*b2);
#endif
