#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "board.h"
void initboard(board *b){
	b -> boardptr = NULL;
	b -> size = 0;
}
int compareboard(board *b1, board*b2){
	int size = b2 -> size = b1 -> size;
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			if(*(b2 -> boardptr + i * size + j) != *(b1 -> boardptr + i * size + j))
				return 0;
		}
	}
	return 1;
}












