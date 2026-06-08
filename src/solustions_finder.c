#include <stdio.h>
#include <stdbool.h>

#include "utils.h"
#include "solutions_finder.h"

bool is_x_or_m_present(const int i, const int j, State s) {
	return s.cur_board[i][j] == X || s.cur_board[i][j] == M;
}

bool* next_moves_by_board(const int i, const int j, State s) {
	bool* moves = zero_bool_alloc(2);

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

bool* next_moves(const int i, const int j, State s) {
	// Next moves by board
	bool* moves = next_moves_by_board(i, j, s);

	// Next moves by row and column numbers
	moves[0] = moves[0] && s.cur_rownums[i] > s.rownums[i];
	moves[0] = moves[0] && s.cur_colnums[j] > s.colnums[i];

	moves[1] = moves[1] && s.cur_rownums[i] < s.rownums[i];
	moves[1] = moves[1] && s.cur_colnums[j] < s.colnums[i];

	// Next moves by shipcounts

	// Return next moves
	return moves;
}

void find_solutions(State s) {
	printf("\n");
	printcmat(s.cur_board, s.m, s.n);
	printf("\n");
}
