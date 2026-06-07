#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "utils.h"
#include "parsed_input_processor.h"

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

void fill_cur_board_per_cell(const int i, const int j, State s) {
	int k_start = (i == 0) ? i : i-1;
	int k_end = (i == s.m-1) ? i : i+1;
	int l_start = (j == 0) ? j : j-1;
	int l_end = (j == s.n-1) ? j : j+1;
	char a;
	bool cond;
	for (int k = k_start; k <= k_end; k++) {
		for (int l = l_start; l <= l_end; l++) {
			a = s.board[k][l];
			switch (s.board[i][j]) {
			case D:
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
			case M:
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
						s.cur_board[k][l] = X;
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
						s.cur_board[k][l] = X;
					}
					continue;
				}

				if (k == i && l == j) {
					s.cur_board[k][l] = M;
					continue;
				}

				if (k != i && l != j) {
					fill_o_err(a, s.cur_board[k][l], "m Foo 8");
					s.cur_board[k][l] = O;
					continue;
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
			fill_cur_board_per_cell(i, j, s);
		}
	}
}

void fill_cur_rownums(State s) {
	int count = 0;
	for (int i = 0; i < s.m; i++) {
		for (int j = 0; j < s.n; j++) {
			char a = s.cur_board[i][j];
			if (a == X || a == M) { count++; }
		}
		cond_err(count > s.rownums[i], "Row numbers are too low.");
		s.cur_rownums[i] = count;
		count = 0;
	}
}

void fill_cur_colnums(State s) {
	int count = 0;
	for (int j = 0; j < s.n; j++) {
		for (int i = 0; i < s.m; i++) {
			char a = s.cur_board[i][j];
			if (a == X || a == M) { count++; }
		}
		cond_err(count > s.colnums[j], "Column numbers are too low.");
		s.cur_colnums[j] = count;
		count = 0;
	}
}

// void fill_cur_shipcounts(State s) {
// 	int ship_size = 0;
// 	bool at_ship = false;
// 	for (int i = 0; i < s.m; i++) {
// 		for (int j = 0; j < s.n; j++) {
// 			switch (s.cur_board[i][j]) {
// 			case D:
// 				at_ship = false;
// 				ship_size = 0;
// 				break;
// 			case O:
// 				if (at_ship) {
// 					if (s.cur_shipcounts[ship_size] < s.shipcounts[ship_size]) {
// 						s.cur_shipcounts[ship_size]++;
// 					}
// 				};
// 				break;
// 			case X: case M:
// 				at_ship = true;
// 				ship_size++;
// 				break;
// 			}
// 		}
// 	}
// }

State process_parsed_input(ParsedInput in) {
	State s = (State){
		in.m,
		in.n,
		in.board,
		in.rownums,
		in.colnums,
		in.shipcounts_length,
		in.shipcounts,
		char_mat(in.m, in.n),
		zero_int_alloc(in.m),
		zero_int_alloc(in.n),
		zero_int_alloc(in.shipcounts_length)
	};

	fill_cur_board(s);
	fill_cur_rownums(s);
	fill_cur_colnums(s);
	// fill_cur_shipcounts(s);

	// printis(s.shipcounts, s.shipcounts_length);
	// printf("\n");
	// printis(s.cur_shipcounts, s.shipcounts_length);

	return s;
}
