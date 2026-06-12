#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"
#include "solver.h"

bool is_x_or_m_present(const int i, const int j, State s) {
	return s.cur_board[i][j] == X || s.cur_board[i][j] == M;
}

bool* next_moves_by_board(const int i, const int j, State s) {
	bool* moves = bool_calloc(2);

	bool m_present;
	bool can_be_o = true;
	bool can_be_x = true;

	if (i-1 >= 0) {
		if (j-1 >= 0) {
			can_be_x = can_be_x && !is_x_or_m_present(i-1, j-1, s);

			m_present = s.cur_board[i-1][j] == M || s.cur_board[i][j-1] == M;
			can_be_o = can_be_o && !(m_present && s.cur_board[i-1][j-1] == O);
		}

		if (j+1 <= s.n-1) {
			can_be_x = can_be_x && !is_x_or_m_present(i-1, j+1, s);

			m_present = s.cur_board[i][j+1] == M || s.cur_board[i-1][j] == M;
			can_be_o = can_be_o && !(m_present && s.cur_board[i-1][j+1] == O);
		}
	}

	if (i+1 <= s.m-1) {
		if (j-1 >= 0) {
			can_be_x =  can_be_x && !is_x_or_m_present(i+1, j-1, s);

			m_present = s.cur_board[i+1][j] == M || s.cur_board[i][j-1] == M;
			can_be_o = can_be_o && !(m_present && s.cur_board[i+1][j-1] == O);
		}

		if (j+1 <= s.n-1) {
			can_be_x = can_be_x && !is_x_or_m_present(i+1, j+1, s);

			m_present = s.cur_board[i][j+1] == M || s.cur_board[i+1][j] == M;
			can_be_o = can_be_o && !(m_present && s.cur_board[i+1][j+1] == O);
		}
	}

	moves[0] = can_be_o;
	moves[1] = can_be_x;

	return moves;
}

bool check_cur_rownum(const int i, State s) {
	int xm_count = 0;
	int o_count = 0;
	char a;
	for (int j = 0; j < s.n; j++) {
		a = s.cur_board[i][j];
		if (a == X || a == M) { xm_count++; }
		if (a == O) { o_count++; }
	}
	return xm_count <= s.rownums[i] && o_count <= s.n - s.rownums[i];
}

bool check_cur_colnum(const int j, State s) {
	int xm_count = 0;
	int o_count = 0;
	char a;
	for (int i = 0; i < s.m; i++) {
		a = s.cur_board[i][j];
		if (a == X || a == M) { xm_count++; }
		if (a == O) { o_count++; }
	}
	return xm_count <= s.colnums[j] && o_count <= s.m - s.colnums[j];
}

void det_cur_shipcounts(State s) {
	for (int k = 0;;k++) {
		if (s.cur_shipcounts[k] == -1) { break; }
		s.cur_shipcounts[k] = 0;
	}

	int ship_size;
	bool at_ship;
	bool cond;

	for (int i = 0; i < s.m; i++) {
		at_ship = true;
		ship_size = 0;
		for (int j = 0; j < s.n; j++) {
			switch (s.cur_board[i][j]) {
			case D:
				at_ship = false;
				ship_size = 0;
				break;
			case X:
			case M:
				if (at_ship) {
					ship_size++;
					if (j == s.n-1 || s.cur_board[i][j+1] == O) {
						cond = ship_size > 1;
						if (!cond) {
							cond = (i == 0 || s.cur_board[i-1][j] == O);
							cond = cond && (i == s.m-1 || s.cur_board[i+1][j] == O);
						}

						if (cond) {
							s.cur_shipcounts[ship_size]++;
							ship_size = 0;
							at_ship = false;
						}
					}
				}
				break;
			case O:
				at_ship = true;
				ship_size = 0;
				break;
			}
		}
	}

	for (int j = 0; j < s.n; j++) {
		at_ship = true;
		ship_size = 0;
		for (int i = 0; i < s.m; i++) {
			switch (s.cur_board[i][j]) {
			case D:
				at_ship = false;
				ship_size = 0;
				break;
			case X:
			case M:
				if (at_ship) {
					ship_size++;
					if (i == s.m-1 || s.cur_board[i+1][j] == O) {
						cond = ship_size > 1;

						if (cond) {
							s.cur_shipcounts[ship_size]++;
							ship_size = 0;
							at_ship = false;
						}
					}
				}
				break;
			case O:
				at_ship = true;
				ship_size = 0;
				break;
			}
		}
	}
}

bool* next_moves(const int i, const int j, State s) {
	// Next moves by board
	bool* moves = next_moves_by_board(i, j, s);

	// Enter next moves and see if the board is valid
	for (int k = 0; k < 2; k++) {
		if (!moves[k]) { continue; }

		s.cur_board[i][j] = (k == 0) ? O : X;

		moves[k] = moves[k] && check_cur_rownum(i, s);
		moves[k] = moves[k] && check_cur_colnum(j, s);
		
		det_cur_shipcounts(s);
		for (int l = 0;; l++) {
			if (s.cur_shipcounts[l] == -1) { break; }
			if (s.cur_shipcounts[l] > s.shipcounts[l]) { moves[k] = false; break; };
		}

		s.cur_board[i][j] = D;

	}

	// Return next moves
	return moves;
}

bool minmax_rec(int i, int j, State s) {
	if (i == s.m) {
		det_cur_shipcounts(s);
		for (int k = 0;; k++) {
			if (s.cur_shipcounts[k] == -1) { break; }
			if (s.cur_shipcounts[k] != s.shipcounts[k]) { return false; };
		}
		return true;
	}

	int i_next = (j <= s.n-2) ? i : i+1;
	int j_next = (j <= s.n-2) ? j+1 : 0;

	if (s.cur_board[i][j] != D) {
		return minmax_rec(i_next, j_next, s);
	};

	bool* moves = next_moves(i, j, s);

	for (int k = 0; k < 2; k++) {
		if (!moves[k]) { continue; }

		s.cur_board[i][j] = (k == 0) ? O : X;

		if (minmax_rec(i_next, j_next, s)) {
			free(moves);
			return true;
		}

		s.cur_board[i][j] = D;
	}

	free(moves);

	return false;
}

void solve(State* s) {
	(*s).solvable = minmax_rec(0,0, *s);
	(*s).solve_func_done = true;
}
