#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "loadgame.h"
int savegame(board *b){
	int fd = 0;
	fd = open("Sudokugamefile", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR  | O_TRUNC ); 
	if(fd == -1) {
		perror("open failed: ");
		return errno;		
	}
	write(fd,&(b->size),sizeof(int));
	for(int i = 0; i < b->size; i++){
		for(int j = 0; j < b->size; j++){
			write(fd,(b->boardptr + i * b->size + j),sizeof(int));
		}
	}
	free(b->boardptr);
	close(fd);
	return 0;
}
int savequestion(board *question){
	int fd = 0;
	fd = open("SudokugameQuestion", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR  | O_TRUNC ); 
	if(fd == -1) {
		perror("open failed: ");
		return errno;		
	}
	write(fd,&(question->size),sizeof(int));
	for(int i = 0; i < question->size; i++){
		for(int j = 0; j < question->size; j++){
			write(fd,(question->boardptr + i * question->size + j),sizeof(int));
		}
	}
	close(fd);
	return 0;
}

int savesolution(board *solution){
	int fd = 0;
	fd = open("SudokugameSolution", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR  | O_TRUNC ); 
	if(fd == -1) {
		perror("open failed: ");
		return errno;		
	}
	write(fd,&(solution->size),sizeof(int));
	for(int i = 0; i < solution->size; i++){
		for(int j = 0; j < solution->size; j++){
			write(fd,(solution->boardptr + i * solution->size + j),sizeof(int));
		}
	}
	close(fd);
	return 0;
}

board loadgame(){
	board b;
	initboard(&b);
	int fd = 0;
	fd = open("Sudokugamefile", O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR  | O_TRUNC ); 
	if(fd == -1) {
		printf("open failed: ");	
	}
	read(fd,&(b.size),sizeof(int));
	int size = b.size;
	b.boardptr = malloc(sizeof(int) * size * size);
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			read(fd,(b.boardptr + i * b.size + j),sizeof(int));
		}
	}
	close(fd);
	return b;
}

board loadquestion(){
	board b;
	initboard(&b);
	int fd = 0;
	fd = open("SudokugameQuestion", O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR  | O_TRUNC ); 
	if(fd == -1) {
		printf("open failed: ");	
	}
	read(fd,&(b.size),sizeof(int));
	int size = b.size;
	b.boardptr = malloc(sizeof(int) * size * size);
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			read(fd,(b.boardptr + i * b.size + j),sizeof(int));
		}
	}
	close(fd);
	return b;
}

board loadsolution(){
	board b;
	initboard(&b);
	int fd = 0;
	fd = open("SudokugameSolution", O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR  | O_TRUNC ); 
	if(fd == -1) {
		printf("open failed: ");	
	}
	read(fd,&(b.size),sizeof(int));
	int size = b.size;
	b.boardptr = malloc(sizeof(int) * size * size);
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			read(fd,(b.boardptr + i * b.size + j),sizeof(int));
		}
	}
	close(fd);
	return b;
}
