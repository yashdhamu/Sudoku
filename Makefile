project: so.o board.o generate.o loadgame.o solve_game.o 
	cc -Wall -o project so.o board.o generate.o loadgame.o solve_game.o -lm `pkg-config --libs --cflags gtk+-2.0` 
so.o: so.c board.h generate.h loadgame.h solve_game.h
	cc -Wall -lm `pkg-config --libs --cflags gtk+-2.0` -c so.c
board.o: board.c board.h
	cc -Wall -c board.c
generate.o: generate.c generate.h board.h solve_game.h loadgame.h
	cc -Wall -lm -c generate.c
loadgame.o: loadgame.c loadgame.h board.h solve_game.h
	cc -Wall -lm -c loadgame.c
solve_game.o: solve_game.h board.h
	cc -Wall -lm -c solve_game.c
