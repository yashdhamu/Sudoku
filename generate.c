#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include "generate.h"

void fillValuesEasy(board *b){
	int size = (b->size);
	b -> boardptr = malloc(size * size * sizeof(int));
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			*(b -> boardptr + i * size + j) = 0;
		}
	}
	fillDiagonal(b);
	fillSudoku(b, 0, 0);
	savesolution(b);
	removeKDigitsEasy(b);
	savequestion(b);
}

void fillValuesMedium(board *b){
	int size = (b->size);
	b -> boardptr = malloc(size * size * sizeof(int));
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			*(b -> boardptr + i * size + j) = 0;
		}
	}
	fillDiagonal(b);
	fillSudoku(b, 0, 0);
	savesolution(b);
	removeKDigitsMedium(b);
	savequestion(b);
}

void fillValuesHard(board *b){
	int size = (b->size);
	b -> boardptr = malloc(size * size * sizeof(int));
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			*(b -> boardptr + i * size + j) = 0;
		}
	}
	fillDiagonal(b);
	fillSudoku(b, 0, 0);
	savesolution(b);
	removeKDigitsHard(b);
	savequestion(b);
}

void fillDiagonal(board *b){ 
	int temp = squareroot(b->size);
	for (int i = 0; i < (b->size); i=i+temp)
		fillBox(b,i, i);
}

int unUsedInBox(board *b, int rowStart, int colStart, int num){
	int temp = squareroot(b->size);
	for (int i = 0; i<temp; i++)
		for (int j = 0; j<temp; j++)
			if (*(b->boardptr + (i+rowStart)* (b->size) + (j+colStart))==num)
				return 0; 
	return 1;
}
 
void fillBox(board*b, int row,int col){
	srand(time(0));
	int temp = squareroot(b->size);
	int num;
	for (int i=0; i<temp; i++){
		for (int j=0; j<temp; j++){
		do{
			 num = randRange((b->size)) + 1;
		}while (!unUsedInBox(b,row, col, num));
		*(b->boardptr + (i+row)* (b->size) + (j+col)) = num;
		}
	}
}

int randRange(int n){
     int limit;
     int r;
     limit = RAND_MAX - (RAND_MAX % n); 
     while((r = rand()) >= limit);
     	return r % n;
}

int CheckIfSafe(board *b, int i,int j,int num){
		int temp = squareroot(b->size);
		return (unUsedInRow(b,i, num) &&
			unUsedInCol(b,j, num) &&
			unUsedInBox(b,i-i%temp, j-j%temp, num));
}
 
int unUsedInRow(board *b, int i,int num){
	for (int j = 0; j<(b->size); j++)
		if (*(b->boardptr + i* (b->size) + j) == num)
			return 0;
	return 1;
}
 
int unUsedInCol(board *b, int j,int num){
	for (int i = 0; i<(b->size); i++)
		if (*(b->boardptr + i* (b->size) + j) == num)
			return 1;
		return 0;
}

void removeKDigitsEasy(board *b){
	int K;
	int base;
	switch(b->size){
		case 4:
			base = 10;
			break;
		case 9:
			base = 46;
			break;
		case 16:
			base = 155;
			break;
		default:
			break;		
	}
	K = (b->size) * (b->size) - base - randRange(2);
	int count = K;
	while (count != 0){
		int cellId = randRange((b->size)*(b->size)) + 1;
		int i = (cellId/(b->size));
		int j = cellId%(b->size);
		if (*(b->boardptr + i* (b->size) + j) != 0){
			count--;
			*(b->boardptr + i* (b->size) + j) = 0;
		}
	}
}

void removeKDigitsMedium(board *b){
	int K;
	int base;
	switch(b->size){
		case 4:
			base = 9;
			break;
		case 9:
			base = 31;
			break;
		case 16:
			base = 107;
			break;
		default:
			break;		
	}
	K = (b->size) * (b->size) - base - randRange(3);
	int count = K;
	while (count != 0){
		int cellId = randRange((b->size)*(b->size)) + 1;
		int i = (cellId/(b->size));
		int j = cellId%(b->size);
		if (*(b->boardptr + i* (b->size) + j) != 0){
			count--;
		*(b->boardptr + i* (b->size) + j) = 0;
		}
	}
}

void removeKDigitsHard(board *b){
	int K;
	int base;
	switch(b->size){
		case 4:
			base = 8;
			break;
		case 9:
			base = 15;
			break;
		case 16:
			base = 90;
			break;
		default:
			break;		
	}
	K = (b->size) * (b->size) - base - randRange(2);
	int count = K;
	while (count != 0){
		int cellId = randRange((b->size)*(b->size)) + 1;
		int i = (cellId/(b->size));
		int j = cellId%(b->size);
		if (*(b->boardptr + i* (b->size) + j) != 0){
			count--;
		*(b->boardptr + i* (b->size) + j) = 0;
		}
	}
}
 
