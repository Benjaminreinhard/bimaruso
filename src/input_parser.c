#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "utils.h"
#include "input_parser.h"

#define MAX_FILE_LENGTH 1000

// Helper functions
char* file_to_arr(FILE* file) {
	char* arr = char_alloc(MAX_FILE_LENGTH);

	int i = 0;
	for (;; i++) {
		cond_err(i == MAX_FILE_LENGTH, "File too long.");
		arr[i] = (char)fgetc(file);
		if (arr[i] == EOF) { break; }
	}
	rewind(file);

	arr = char_realloc(arr, i+1);

	return arr;
}

char* convert_arr(const char* arr) {
	char* conv_arr = char_alloc(MAX_FILE_LENGTH);

	int k = 0;
	for (int i = 0;; i++) {
		if (arr[i] != ' ' && arr[i] != '\n') {
			conv_arr[k] = (char)tolower(arr[i]);
			k++;
		}
		if (arr[i] == EOF) { break; }
	}

	conv_arr = char_realloc(conv_arr, k+1);

	return conv_arr;
}

int det_index_of_letter(const int index, const char letter, const char* arr) {
  int i = index;
	for (; arr[i] != letter; i++) {
		cond_err(arr[i] == EOF, "Input file not complete.");
	}

	return i;
}

void check_title(const int index, const char* arr, const char* title, const char* err_msg) {
	int title_length = calc_str_length(title);

	for (int i = 0; i < title_length; i++) {
		char a = arr[index+i];
		cond_err(a == EOF || a != title[i], err_msg);
	}
}

int* calc_mn(const int index, const char* arr) {
	int* mn = int_alloc(2);
	mn[0] = 0; mn[1] = 0;

	bool n_determined = false;
	int n_ = 0;
	bool at_end = false;

	for (int i = index; !at_end; i++) {
		switch (arr[i]) {
			case '.': case 'w': case 's': case 'r': case 't': case 'l': case 'b': case 'm':
				if (!n_determined) { mn[1]++; }	
				n_++;
				break;

			case '#':
				at_end = true;

			case ',':
				n_determined = true;
				cond_err(n_ != mn[1], "Rows are not equally long.");
				n_ = 0;
				mn[0]++;
				break;

			case EOF:
				err("Input file not complete.");

			default:
				err("Unknown characters in the board.");
		}
	}

	return mn;
}

char** det_board(const int index, const int m, const int n, const char* arr) {
	char** board = char_mat(m, n);

	bool at_end = false;

	int i = 0;
	int j = 0;

	for (int k = index; !at_end; k++) {
		char a = arr[k];
		switch (a) {
			case '.': case 'w': case 's': case 'r': case 't': case 'l': case 'b': case 'm':
				board[i][j] = a;
				j++;
				break;

			case ',':
				j = 0;
				i++;
				break;

			case '#':
				at_end = true;
		}
	}

	return board;
}

int* det_nums(const int index, const int nums_length, const char* arr, const char* err_msg) {
	int* nums = int_alloc(nums_length);

	int num = 0;
	bool at_num = false; bool at_end = false;
  int i = 0;

	for (int k = index; !at_end; k++) {
    char a = arr[k];
		switch (a) {
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': 
				at_num = true;
				num = 10 * num + (a - '0');
				break;

			case '#':
				at_end = true;

			case ',':
				cond_err(i >= nums_length || !at_num, err_msg);
				nums[i] = num;
				num = 0;
				at_num = false;
				i++;
				break;

			case EOF:
				err("Input file not complete.");

			default:
				err(err_msg);
		}
	}
	cond_err(i != nums_length, err_msg);

	return nums;
}

int calc_shipcounts_length(const int index, const char* arr) {
	int length = 0;

	bool at_key = true; bool at_value = false; bool at_num = false; bool at_end = false;

	char* err_msg = "Each ship count must be of the form 'Bx: y'.";

	for (int i = index; !at_end; i++) {
		switch (arr[i]) {
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': 
				at_num = true;
				break;

			case ':':
				cond_err(!at_num || at_value, err_msg);
				at_key = false; at_value = true; at_num = false;
				break;

			case EOF:
				at_end = true;

			case ',':
				cond_err(!at_num || at_key, err_msg);
				length++;
				at_key = true; at_value = false; at_num = false;
				break;

			default:
				err("Unknown characters in ship counts.");
		}
	}

	return length;
}

int** det_shipcounts(const int index, const int length, const char* arr) {
	int** shipcounts = int_mat(2, length);

	int num = 0; 
	bool at_end = false;
	int i = 0;

	for (int k = index; !at_end; k++) {
    char a = arr[k];
		switch (a) {
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': 
				num = 10 * num + (a - '0');
				break;

			case ':':
				shipcounts[0][i] = num;
				num = 0;
				break;

			case EOF:
				at_end = true;

			case ',':
				shipcounts[1][i] = num;
				num = 0;
				i++;
		}
	}

	return shipcounts;
}

void too_close_err(const char a, const char* err_msg) {
	cond_err(a != '.' && a != 'w', err_msg);
}

void too_close_bottom_err(const int i, const int j, const int m, const char** board, const char* err_msg) {
	if(i+1 < m) {
		too_close_err(board[i+1][j], err_msg);
	}
}

void too_close_right_err(const int i, const int j, const int n, const char** board, const char* err_msg) {
	if(j+1 < n) {
		too_close_err(board[i][j+1], err_msg);
	}
}

void too_close_bottomright_err(const int i, const int j, const int m, const int n, const char** board, const char* err_msg) {
	if(i+1 < m && j+1 < n) {
		too_close_err(board[i+1][j+1], err_msg);
	}
}

void check_board(const char** board, const int m, const int n) {
	char* err_msg;
	char a;
	bool is_in_corner;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			switch(board[i][j]) {
				case '.':
					break;

				case 'w':
					cond_err(j+1 < n && board[i][j+1] == 'm' && i+1 < m && board[i+1][j+1] == 'w', "A middle piece is surrounded by too much water.");
					cond_err(i+1 < m && board[i+1][j] == 'm' && j+1 < n && board[i+1][j+1] == 'w', "A middle piece is surrounded by too much water.");
					break;

				case 's':
					err_msg = "A single ship is too close to another ship.";
					too_close_bottom_err(i, j, m, board, err_msg);
					too_close_right_err(i, j, n, board, err_msg);
					too_close_bottomright_err(i, j, m, n, board, err_msg);
					break;

				case 'r':
					cond_err(j == 0, "A right end can not be in the first column.");
					err_msg = "A right end is too close to another ship.";
					too_close_bottom_err(i, j, m, board, err_msg);
					too_close_right_err(i, j, n, board, err_msg);
					too_close_bottomright_err(i, j, m, n, board, err_msg);
					break;

				case 't':
					cond_err(i == m-1, "A top end can not be in the last row.");
					err_msg = "A top end is too close to another ship.";
					if(i+1 < m) {
						a = board[i+1][j]; // bottom
						cond_err(a != 'm' && a != 'b' && a != '.', "A top end has a wrong entry below it.");
					}
					too_close_right_err(i, j, n, board, err_msg);
					too_close_bottomright_err(i, j, m, n, board, err_msg);
					break;

				case 'l':
					cond_err(j == n-1, "A left end can not be in the last column.");
					err_msg = "A left end is too close to another ship.";
					too_close_bottom_err(i, j, m, board, err_msg);
					if(j+1 < n) {
						a = board[i][j+1]; // right
						cond_err(a != 'm' && a != 'r' && a != '.', "A left end has a wrong entry to the right of it.");
					}
					too_close_bottomright_err(i, j, m, n, board, err_msg);
					break;

				case 'b':
					cond_err(i == 0, "A bottom end can not be in the first row.");
					err_msg = "A bottom end is too close to another ship.";
					too_close_bottom_err(i, j, m, board, err_msg);
					too_close_right_err(i, j, n, board, err_msg);
					too_close_bottomright_err(i, j, m, n, board, err_msg);
					break;

				case 'm':
					is_in_corner = (i == 0 && j == 0) || (i == 0 && j == n-1) || (i == m-1 && j == 0) || (i == m-1 && j == n-1);
					cond_err(is_in_corner, "A center piece can not be in a corner.");
					err_msg = "A bottom end is too close to another ship.";
					if(i+1 < m) {
						a = board[i+1][j]; // bottom
						cond_err(a != 'm' && a != 'b' && a != '.' && a != 'w', "A middle piece has a wrong entry below it.");
					}
					if(j+1 < n) {
						a = board[i][j+1]; // right
						cond_err(a != 'm' && a != 'r' && a != '.' && a != 'w', "A middle piece has a wrong entry to the right of it.");
					}
					too_close_bottomright_err(i, j, m, n, board, err_msg);
					if(i+1 < m && j+1 < n) {
						cond_err(board[i+1][j] == 'w' && board[i][j+1] == 'w',"A middle piece is surrounded by too much water.");
					}
					break;
			}
		}
	}
}

void check_nums(const int* nums, const int a, const int b, const char* err_msg) {
	for (int k = 0; k < a; k++) {
		cond_err(nums[k] > b, err_msg);
	}
}

void inc_ship_count(int** counts, const int length, const int ship_size) {
	for (int k = 0; k < length; k++) {
		if (counts[0][k] == ship_size) { counts[1][k]++; return; }
	}
	err("There is a ship on the board whose size does not occur in the ship counts.");
}

void check_shipcounts(const int** shipcounts, const int shipcounts_length, const char** board, const int m, const int n) {
	int length = shipcounts_length;
	int** counts = zero_int_mat(2, length);
	counts[0] = cpy_int_arr(shipcounts[0], length);

	bool at_ship = false;
	int ship_size = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			switch(board[i][j]) {
				case '.': case 'w':
					at_ship = false;
					break;

				case 't': case 'b':
					break;

				case 's':
					inc_ship_count(counts, length, 1);
					break;

				case 'r':
					if (at_ship) {
						inc_ship_count(counts, length, ship_size+1);
						at_ship = false;
						ship_size = 0;
					}
					break;

				case 'l':
					at_ship = true;
					ship_size++;
					break;

				case 'm':
					if (at_ship) { ship_size++; }
					break;
			}
		}
	}

	at_ship = false;
	ship_size = 0;
	for (int j = 0; j < n; j++) {
		for (int i = 0; i < m; i++) {
			switch(board[i][j]) {
				case '.': case 'w':
					at_ship = false;
					break;

				case 's': case 'r': case 'l':
					break;

				case 't':
					at_ship = true;
					ship_size++;
					break;

				case 'm':
					if (at_ship) { ship_size++; }
					break;

				case 'b':
					if (at_ship) {
						inc_ship_count(counts, length, ship_size+1);
						at_ship = false;
						ship_size = 0;
					}
					break;
			}
		}
	}

	for (int k = 0; k < length; k++) {
		cond_err(counts[1][k] > shipcounts[1][k],"There are too many ships of a certain size on the board, according to the ship counts.");
	}
}

ParsedInput parse_input(FILE* file) {
	// Make file to array
	char* file_arr = file_to_arr(file);

	// Convert file array
	const char* conv_arr = convert_arr(file_arr);
	free(file_arr);

	// Process board section
	int index = 0;

	char* title = "#board"; int title_length = calc_str_length(title);
	check_title(index, conv_arr, title, "1st section title must be '# Board'.");
	index += title_length;

	const int* mn = calc_mn(index, conv_arr); const int m = mn[0]; const int n = mn[1];
	const char** board = (const char**)det_board(index, m, n, conv_arr);
	// TODO: Update check_board
	check_board(board, m, n);

	// Process row numbers section
	index = det_index_of_letter(index, '#', conv_arr);

	title = "#rownumbers"; title_length = calc_str_length(title);
	check_title(index, conv_arr, title, "2nd section title must be '# Row numbers'.");
	index += title_length;

	const int* rownums = det_nums(index, m, conv_arr, "Invalid row numbers section.");
	check_nums(rownums, m, n, "Row numbers can at most be equal to the amount of columns.");

	// Process column numbers section
	index = det_index_of_letter(index, '#', conv_arr);

	title = "#columnnumbers"; title_length = calc_str_length(title);
	check_title(index, conv_arr, title, "3rd section title must be '# Column numbers'.");
	index += title_length;

	const int* colnums = det_nums(index, n, conv_arr, "Invalid column numbers section.");
	check_nums(colnums, n, m, "Column numbers can at most be equal to the amount of rows.");

	// Process ship counts section
	index = det_index_of_letter(index, '#', conv_arr);

	title = "#shipcounts"; title_length = calc_str_length(title);
	check_title(index, conv_arr, title, "4th section title must be '# Ship counts'.");
	index += title_length;

	const int shipcounts_length = calc_shipcounts_length(index, conv_arr);
	const int** shipcounts = (const int**)det_shipcounts(index, shipcounts_length, conv_arr);
	check_shipcounts(shipcounts, shipcounts_length, board, m, n);

	return (ParsedInput){ m, n, board, rownums, colnums, shipcounts_length, shipcounts };
}

