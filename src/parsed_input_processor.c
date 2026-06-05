#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "utils.h"
#include "parsed_input_processor.h"

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

void fill_o_err(const char a, const char b, const char* err_msg) {
	bool cond = (a != D) && (a != W);
	cond = cond || (b == X) || (b == M);
	cond_err(cond, err_msg);
}

void side_err(const char a, const char b, const char good_char, const char* err_msg) {
	bool cond = (a != D) && (a != M) && (a != good_char);
	cond = cond || (b == O);
	cond_err(cond, err_msg);
}

void fill_cur_board_part_one(const int i, const int j, State s) {
	int k_start = (i == 0) ? i : i-1;
	int k_end = (i == s.m-1) ? i : i+1;
	int l_start = (j == 0) ? j : j-1;
	int l_end = (j == s.n-1) ? j : j+1;
	char a;
	for (int k = k_start; k <= k_end; k++) {
		for (int l = l_start; l <= l_end; l++) {
			a = s.board[k][l];
			switch (s.board[i][j]) {
			case D: case M:
				break;

			case W:
				s.cur_board[i][j] = O;
				break;

			case S:
				if (k == i && l == j) {
					s.cur_board[i][j] = X;
				} else {
					fill_o_err(a, s.cur_board[k][l], "s Foo");
					s.cur_board[k][l] = O;
				}
				break;

			case R:
				cond_err(j == 0, "r Foo 1");

				if (k == i && l == j) {
					s.cur_board[i][j] = X;
				} else if (k == i && l == l_start) {
					side_err(a, s.cur_board[k][l], L, "r Foo 2");					
					s.cur_board[k][l] = X;
				} else {
					fill_o_err(a, s.cur_board[k][l], "r Foo");
					s.cur_board[k][l] = O;
				}
				break;

			case T:
				cond_err(i == s.m-1, "t Foo 1");

				if (k == i && l == j) {
					s.cur_board[i][j] = X;
				} else if (k == k_end && l == j) {
					side_err(a, s.cur_board[k][l], B, "t Foo 2");				
					s.cur_board[k][l] = X;
				} else {
					fill_o_err(a, s.cur_board[k][l], "t Foo");
					s.cur_board[k][l] = O;
				}
				break;

			case L:
				cond_err(j == s.n-1, "l Foo 1");

				if (k == i && l == j) {
					s.cur_board[i][j] = X;
				} else if (k == i && l == l_end) {
					side_err(a, s.cur_board[k][l], R, "l Foo 2");				
					s.cur_board[k][l] = X;
				} else {
					fill_o_err(a, s.cur_board[k][l], "l Foo");
					s.cur_board[k][l] = O;
				}
				break;

			case B:
				cond_err(i == 0, "b Foo 1");

				if (k == i && l == j) {
					s.cur_board[i][j] = X;
				} else if (k == k_start && l == j) {
					side_err(a, s.cur_board[k][l], T, "b Foo 2");
					s.cur_board[k][l] = X;
				} else {
					fill_o_err(a, s.cur_board[k][l], "b Foo");
					s.cur_board[k][l] = O;
				}
				break;
			}
		}
	}
}

void fill_cur_board_part_two(const int i, const int j, State s) {
	if (s.board[i][j] != M) { return; }

	int k_start = (i == 0) ? i : i-1;
	int k_end = (i == s.m-1) ? i : i+1;
	int l_start = (j == 0) ? j : j-1;
	int l_end = (j == s.n-1) ? j : j+1;
	bool cond;
	char a;
	for (int k = k_start; k <= k_end; k++) {
		for (int l = l_start; l <= l_end; l++) {
			a = s.board[k][l];

			cond = (i == 0 || i == s.m-1) && (j == 0 || j == s.n-1);
			cond_err(cond, "m Foo 1");

			if (i == 0 || i == s.m-1) {
				if (k != i) {
					fill_o_err(a, s.cur_board[k][l], "m Foo 2");
					s.cur_board[k][l] = O;
				} else if (l == j-1) {
					side_err(a, s.cur_board[k][l], L, "m Foo 3");
					s.cur_board[k][l] = X;
				} else if (l == j+1) {
					side_err(a, s.cur_board[k][l], R, "m Foo 4");
					s.cur_board[k][l] = X;
				} else {
					s.cur_board[k][l] = M;
				}
				continue;
			}

			if (j == 0 || j == s.n-1) {
				if (l != j) {
					fill_o_err(a, s.cur_board[k][l], "m Foo 5");
					s.cur_board[k][l] = O;
				} else if (k == i-1) {
					side_err(a, s.cur_board[k][l], T, "m Foo 6");
					s.cur_board[k][l] = X;
				} else if (k == i+1) {
					side_err(a, s.cur_board[k][l], B, "m Foo 7");
					s.cur_board[k][l] = X;
				} else {
					s.cur_board[k][l] = M;
				}
				continue;
			}

			if (k == i && l == j) {
				s.cur_board[k][l] = M;
				continue;
			}

			if (k != i && l != j) {
				cond = s.cur_board[k][l] == X || s.cur_board[k][l] == M;
				cond_err(cond, "m Foo 8");
				s.cur_board[k][l] = O;
				continue;
			}

			if (k == i-1) {
				if (s.cur_board[k][l] == O) {
					cond = (s.cur_board[i+1][j] == X) || (s.cur_board[i+1][j] == M);
					cond_err(cond, "m Foo 9");
					s.cur_board[i+1][j] = O;

					cond = (s.cur_board[i][j-1] == O);
					cond_err(cond, "m Foo 10");
					s.cur_board[i][j-1] = X;

					cond = (s.cur_board[i][j+1] == O);
					cond_err(cond, "m Foo 11");
					s.cur_board[i][j+1] = X;

				} else if (s.cur_board[k][l] == X || s.cur_board[k][l] == M) {
					cond = (s.cur_board[i+1][j] == O);
					cond_err(cond, "m Foo 12");
					s.cur_board[i+1][j] = X;

					cond = (s.cur_board[i][j-1] == X) || (s.cur_board[i][j-1] == M);
					cond_err(cond, "m Foo 13");
					s.cur_board[i][j-1] = O;

					cond = (s.cur_board[i][j+1] == X) || s.cur_board[i][j+1] == M;
					cond_err(cond, "m Foo 14");
					s.cur_board[i][j+1] = O;

				}
			}
		}
	}
}

void fill_cur_board(State s) {
	for (int i = 0; i < s.m; i++) {
		for (int j = 0; j < s.n; j++) {
			s.cur_board[i][j] = D;
		}
	}

	for (int i = 0; i < s.m; i++) {
		for (int j = 0; j < s.n; j++) {
			fill_cur_board_part_one(i, j, s);
		}
	}

	for (int i = 0; i < s.m; i++) {
		for (int j = 0; j < s.n; j++) {
			fill_cur_board_part_two(i, j, s);
		}
	}
}

State process_parsed_input(ParsedInput in) {
	State s = (State){
		in.m,
		in.n,
		in.board,
		in.rownums,
		in.colnums,
		in.shipcounts_length,
		in.shipcounts,
		zero_int_alloc(in.m),
		zero_int_alloc(in.n),
		zero_int_mat(2, in.shipcounts_length),
		char_mat(in.m, in.n)
	};

	update_cur_rownums(s);
	update_cur_colnums(s);
	fill_cur_board(s);

	return s;
}
