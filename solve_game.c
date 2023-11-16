#include "solve_game.h"
#include <stdio.h>
#include <math.h>

int squareroot(int a){
	int temp;
	temp = sqrt(a);
	return temp;
}
int isAvailable(board *b, int row, int col, int num){
    int temp = squareroot(b->size);
    int rowStart = (row/ temp) * temp;
    int colStart = (col/ temp) * temp;
    int i;
    for(i = 0; i < (b->size); ++i){
        if (*(b->boardptr + row * (b->size) + i) == num) 
        	return 0;
        if (*(b->boardptr + (b->size) * i + col) == num) 
        	return 0;
        if (*(b->boardptr + (rowStart + (i % temp)) * (b->size) + (colStart + (i / temp))) == num) 
        	return 0;
    }
    return 1;
}

int fillSudoku(board *b, int row, int col){
	int i;
	if(row < (b->size) && col < (b->size)){
		if(*(b->boardptr + row * (b->size) + col) != 0){
			if((col + 1) < (b->size))
				return fillSudoku(b, row, col+1);
			else if((row + 1) < (b->size)) 
				return fillSudoku(b, row+1, 0);
			else 
				return 1;
		}
		else{
			for(i = 0; i < (b->size); ++i){
				if(isAvailable(b, row, col, i+1)){
					*(b->boardptr + row * (b->size) + col) = i+1;
					if((col + 1) < (b->size)){
						if(fillSudoku(b, row, col +1)) 
							return 1;
						else 
						*(b->boardptr + row * (b->size) + col) = 0;
					}
					else if((row + 1) < (b->size)){
						if(fillSudoku(b, row+1, 0))
							return 1;
						else 
							*(b->boardptr + row * (b->size) + col) = 0;
					}
					else 
						return 1;
				}
			}
		}
		return 0;
	}
	else 
	return 1;
}

