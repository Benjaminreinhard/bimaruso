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
	char* err_msg;
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
					fill_o_err(a, s.cur_board[k][l], "A single boat is too close to another boat.");
					s.cur_board[k][l] = O;
				}
				break;

			case R:
				cond_err(j == 0, "A right end can not in the first column.");

				if (k == i && l == j) {
					s.cur_board[i][j] = X;
				} else if (k == i && l == l_start) {
					side_err(a, s.cur_board[k][l], L, "The cell to the left of a right end needs to be filable.");					
					s.cur_board[k][l] = X;
				} else {
					fill_o_err(a, s.cur_board[k][l], "A right end ist too close to another boat.");
					s.cur_board[k][l] = O;
				}
				break;

			case T:
				cond_err(i == s.m-1, "A top end can not in the last row.");

				if (k == i && l == j) {
					s.cur_board[i][j] = X;
				} else if (k == k_end && l == j) {
					side_err(a, s.cur_board[k][l], B, "The cell on the bottom of a top end needs to be filable.");				
					s.cur_board[k][l] = X;
				} else {
					fill_o_err(a, s.cur_board[k][l], "A top end ist too close to another boat.");
					s.cur_board[k][l] = O;
				}
				break;

			case L:
				cond_err(j == s.n-1, "A left end can not in the last column.");

				if (k == i && l == j) {
					s.cur_board[i][j] = X;
				} else if (k == i && l == l_end) {
					side_err(a, s.cur_board[k][l], R, "The cell to the right of a left end needs to be filable.");				
					s.cur_board[k][l] = X;
				} else {
					fill_o_err(a, s.cur_board[k][l], "A left end ist too close to another boat.");
					s.cur_board[k][l] = O;
				}
				break;

			case B:
				cond_err(i == 0, "A bottom end can not in the first column.");

				if (k == i && l == j) {
					s.cur_board[i][j] = X;
				} else if (k == k_start && l == j) {
					side_err(a, s.cur_board[k][l], T, "The cell to the top of a bottom end needs to be filable.");
					s.cur_board[k][l] = X;
				} else {
					fill_o_err(a, s.cur_board[k][l], "A bottom end ist too close to another boat");
					s.cur_board[k][l] = O;
				}
				break;
			case M:
				cond = (i == 0 || i == s.m-1) && (j == 0 || j == s.n-1);
				cond_err(cond, "A middle piece can not be in a corner.");

				err_msg = "The cells to the left and right of a middle piece that is in the first or last row need to be fillable.";
				if (i == 0 || i == s.m-1) {
					if (k != i) {
						fill_o_err(a, s.cur_board[k][l], "A middle piece is too close to another boat.");
						s.cur_board[k][l] = O;
					} else if (l == j-1) {
						side_err(a, s.cur_board[k][l], L, err_msg);
						s.cur_board[k][l] = X;
					} else if (l == j+1) {
						side_err(a, s.cur_board[k][l], R, err_msg);
						s.cur_board[k][l] = X;
					} else {
						s.cur_board[k][l] = X;
					}
					continue;
				}

				err_msg = "The cells to the top and bottom of a middle piece that is in the first or last column need to be fillable.";
				if (j == 0 || j == s.n-1) {
					if (l != j) {
						fill_o_err(a, s.cur_board[k][l], "A middle piece is too close to another boat.");
						s.cur_board[k][l] = O;
					} else if (k == i-1) {
						side_err(a, s.cur_board[k][l], T, err_msg);
						s.cur_board[k][l] = X;
					} else if (k == i+1) {
						side_err(a, s.cur_board[k][l], B, err_msg);
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
					fill_o_err(a, s.cur_board[k][l], "A middle piece is too close to another boat.");
					s.cur_board[k][l] = O;
					continue;
				}
			}
		}
	}
}

void fill_x_cell_corners_with_o(const int i, const int j, State s) {
	if (s.cur_board[i][j] != X) { return; }

	bool cond;
	if (i-1 >= 0 && j-1 >= 0) {
		cond = s.cur_board[i-1][j-1] != O && s.cur_board[i-1][j-1] != D;
		cond_err(cond, "A ship is too close to another one.");
		s.cur_board[i-1][j-1] = O;
	}

	if (i+1 <= s.m-1 && j-1 >= 0) {
		cond = s.cur_board[i+1][j-1] != O && s.cur_board[i+1][j-1] != D;
		cond_err(cond, "A ship is too close to another one.");
		s.cur_board[i+1][j-1] = O;
	}

	if (i-1 >= 0 && j+1 <= s.n-1) {
		cond = s.cur_board[i-1][j+1] != O && s.cur_board[i-1][j+1] != D;
		cond_err(cond, "A ship is too close to another one.");
		s.cur_board[i-1][j+1] = O;
	}

	if (i+1 <= s.m-1 && j+1 <= s.n-1) {
		cond = s.cur_board[i+1][j+1] != O && s.cur_board[i+1][j+1] != D;
		cond_err(cond, "A ship is too close to another one.");
		s.cur_board[i+1][j+1] = O;
	}
}

void initialize_cur_board(State s) {
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

	for (int i = 0; i < s.m; i++) {
		for (int j = 0; j < s.n; j++) {
			fill_x_cell_corners_with_o(i, j, s);
		}
	}
}

State process_parsed_input(ParsedInput in) {
	// Determine length of shipcounts
	int length = 0;
	for (; in.shipcounts[length] != -1; length++) {}
	length++;

	// Initialize state
	State s = (State){
		in.m,
		in.n,
		in.board,
		in.rownums,
		in.colnums,
		in.shipcounts,
		int_alloc(length),
		char_mat(in.m, in.n),
		false,
		false
	};

	// Check if parsed input has obvious LOGICAL errors, unlike parsing errors
	for (int k = 0; k < s.m; k++) {
		cond_err(in.rownums[k] > s.n, "Row numbers must be smaller or equal to the number of columns.");
	}

	for (int k = 0; k < s.n; k++) {
		cond_err(in.colnums[k] > s.m, "Column numbers must be smaller or equal to the number of rows.");
	}

	int sum = 0;
	for (int k = 0;; k++) {
		if (s.shipcounts[k] == -1) { break; }
		sum+= s.shipcounts[k];
		cond_err(sum > s.m*s.n, "Ship counts are too high.");
	}

	// Determine the current board that will be used later
	initialize_cur_board(s);

	// Set last value of cur_shipcounts to -1
	s.cur_shipcounts[length-1] = -1;

	// Return the state
	return s;
}
