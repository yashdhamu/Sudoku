#ifndef __GENERATE_H
#define __GENERATE_H
#include "board.h"
#include "solve_game.h"
#include "loadgame.h"

void fillValuesEasy(board *b);
void fillValuesMedium(board *b);
void fillValuesHard(board *b);
void fillDiagonal(board *b);
int randRange(int n);
void fillBox(board*b, int row,int col);
int unUsedInBox(board *b, int rowStart, int colStart, int num);
int unUsedInCol(board *b, int j,int num);
int unUsedInRow(board *b, int i,int num);
int CheckIfSafe(board *b, int i,int j,int num);
void removeKDigitsEasy(board *b);
void removeKDigitsMedium(board *b);
void removeKDigitsHard(board *b);
#endif
