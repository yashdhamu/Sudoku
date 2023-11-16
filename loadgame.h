#ifndef __LOADGAME_H
#define __LOADGAME_H
#include "board.h"
#include "solve_game.h"
int savegame(board *b);
int savequestion(board *question);
int savesolution(board *solution);
board loadquestion();
board loadsolution();
board loadgame();
#endif
