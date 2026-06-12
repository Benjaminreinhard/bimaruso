#include <stdio.h>
#include <stdbool.h>

#include "utils.h"
#include "renderer.h"

void render_cur_board(State s) {
	for (int i = 0; i < s.m; i++) {
		for (int j = 0; j < s.n; j++) {
			switch (s.cur_board[i][j]) {
			case D:
				printc(D);
				break;
			case O:
				printf("%s ", "\u223F");
				break;
			case X: case M:
				printf("%s ", "\u25CF");
				break;
			}
		}
		printf("\n");
	}
	printf("\n");
}

void render(State s) {
	if (s.solvable) {
		printf("Bimaru is solvable.\nA solution is:\n\n");
		render_cur_board(s);
	} else {
		printf("Bimaru is not solvable.\n");
	}
}