#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include "utils.h"
#include "solutions_finder.h"

typedef struct {
	const int m;
	const int n;
	const int* rownums;
	const int* colnums;
	const int shipcounts_length;
	const int** shipcounts;

	int* cur_rownums;
	int* cur_colnums;
	int** cur_shipcounts;
	char** cur_board;
} State;

int count_occupied_cells_in_row(const int i, const State s) {
	int count = 0;
	for (int j = 0; j < s.n; j++) {
		char a = s.cur_board[i][j];
		if (a != '.' && a != 'w') { count++; }
	}
	return count;
}

void update_cur_rownums(State s) {
	for (int i = 0; i < s.m; i++) {
		s.cur_rownums[i] = count_occupied_cells_in_row(i, s);
	}
}

int count_occupied_cells_in_col(const int j, const State s) {
	int count = 0;
	for (int i = 0; i < s.m; i++) {
		char a = s.cur_board[i][j];
		if (a != '.' && a != 'w') { count++; }
	}
	return count;
}

void update_cur_colnums(State s) {
	for (int j = 0; j < s.n; j++) {
		s.cur_colnums[j] = count_occupied_cells_in_col(j, s);
	}
}

void update_cur_shipcounts(State s) {
	s.cur_shipcounts[0] = cpy_int_arr(s.shipcounts[0], s.shipcounts_length);
	for (int i = 0; i < s.m; i++) {
		for (int j = 0; j < s.n; j++) {
		}
	}
}

State initial_sate(ParsedInput in) {
	State s = (State){
		in.m, in.n, in.rownums, in.colnums, in.shipcounts_length, in.shipcounts,
		zero_int_alloc(in.m),
		zero_int_alloc(in.n),
		zero_int_mat(2, in.shipcounts_length),
		cpy_char_mat(in.board, in.m, in.n)
	};

	update_cur_rownums(s);
	update_cur_colnums(s);

	return s;
}

void allowed_moves()

void find_solutions(ParsedInput in) {
	State s = initial_sate(in);
	update_cur_shipcounts(s);
	/* printis(s.cur_shipcounts[0], s.shipcounts_length); */
}

