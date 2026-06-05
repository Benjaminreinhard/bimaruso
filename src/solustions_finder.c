#include <stdio.h>
#include <stdbool.h>

#include "utils.h"
#include "solutions_finder.h"

void find_solutions(State s) {
	printf("\n");
	printcmat(s.cur_board, s.m, s.n);
}
