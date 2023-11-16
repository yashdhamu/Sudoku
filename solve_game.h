#ifndef __SOLVE_GAME_H
#define __SOLVE_GAME_H
#include "board.h"

int isAvailable(board *b, int row, int col, int num);

int fillSudoku(board *b, int row, int col);

int squareroot(int a);

#endif
