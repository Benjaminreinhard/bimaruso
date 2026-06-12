#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "utils.h"
#include "input_parser.h"

#define MAX_FILE_LENGTH 1000
#define MAX_NUM 50

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
			case D: case W: case S: case R: case T: case L: case B: case M:
				if (!n_determined) { mn[1]++; }	
				n_++;
				cond_err(n_ > MAX_NUM, "At most 50 columns allowed.");
				break;

			case HASH:
				at_end = true;

			case COMMA:
				n_determined = true;
				cond_err(n_ != mn[1], "Rows are not equally long.");
				n_ = 0;
				mn[0]++;
				cond_err(mn[0] > MAX_NUM, "At most 50 rows allowed.");
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
			case D: case W: case S: case R: case T: case L: case B: case M:
				board[i][j] = a;
				j++;
				break;

			case COMMA:
				j = 0;
				i++;
				break;

			case HASH:
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
			case ZERO: case ONE: case TWO: case THREE: case FOUR:
			case FIVE: case SIX: case SEVEN: case EIGHT: case NINE: 
				at_num = true;
				num = 10 * num + (a - '0');
				cond_err(num > MAX_NUM, err_msg);
				break;

			case HASH:
				at_end = true;

			case COMMA:
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

int* det_shipcounts(const int index, const char* arr) {
	int* shipcounts = int_calloc(MAX_NUM+2);

	bool at_key = true;
	int key = 0;
	bool at_value = false;
	int value = 0;
	bool at_end = false;

	char* err_msg = "Each ship count must be of the form 'Bx: y' and x,y can at most be equal to 50.";

	for (int i = index; !at_end; i++) {
		switch (arr[i]) {
			case ZERO: case ONE: case TWO: case THREE: case FOUR:
			case FIVE: case SIX: case SEVEN: case EIGHT: case NINE:
				if (at_key) {
					key = 10 * key + (arr[i] - '0');
					cond_err(key > MAX_NUM, err_msg);
				} else {
					value = 10 * value + (arr[i] - '0');
					cond_err(key > MAX_NUM, err_msg);
				}
				break;

			case COLON:
				cond_err(at_value, err_msg);
				at_key = false; at_value = true;
				break;

			case EOF:
				at_end = true;

			case COMMA:
				cond_err(at_key, err_msg);
				at_key = true; at_value = false;
				cond_err(shipcounts[key] != 0, "A ship size appears more than once in the ship count section.");
				shipcounts[key] = value;
				key = 0;
				value = 0;
				break;

			default:
				err("Unknown characters in ship counts.");
		}
	}

	int last_key = 0;
	for (int k = 0; k < MAX_NUM+1; k++) {
		if (shipcounts[k] != 0) { last_key = k; }
	}

	shipcounts[last_key+1] = -1;
	shipcounts = int_realloc(shipcounts, last_key+2);

	return shipcounts;
}

ParsedInput parse_input(FILE* file) {
	// Make file to array
	char* file_arr = file_to_arr(file);

	// Convert file array
	const char* conv_arr = convert_arr(file_arr);
	free(file_arr);

	// Process board section
	int index = 0;

	char* title = "#board";
	check_title(index, conv_arr, title, "1st section title must be '# Board'.");
	index += calc_str_length(title);

	const int* mn = calc_mn(index, conv_arr); const int m = mn[0]; const int n = mn[1];
	const char** board = (const char**)det_board(index, m, n, conv_arr);

	// Process row numbers section
	index = det_index_of_letter(index, '#', conv_arr);

	title = "#rownumbers";
	check_title(index, conv_arr, title, "2nd section title must be '# Row numbers'.");
	index += calc_str_length(title);

	const int* rownums = det_nums(index, m, conv_arr, "Invalid row numbers section. Note that numbers can at most be equal to 50.");

	// Process column numbers section
	index = det_index_of_letter(index, '#', conv_arr);

	title = "#columnnumbers";
	check_title(index, conv_arr, title, "3rd section title must be '# Column numbers'.");
	index += calc_str_length(title);

	const int* colnums = det_nums(index, n, conv_arr, "Invalid column numbers section. Note that numbers can at most be equal to 50.");

	// Process ship counts section
	index = det_index_of_letter(index, '#', conv_arr);

	title = "#shipcounts";
	check_title(index, conv_arr, title, "4th section title must be '# Ship counts'.");
	index += calc_str_length(title);

	const int* shipcounts = (const int*)det_shipcounts(index, conv_arr);

	// Return parsed input
	return (ParsedInput){ m, n, board, rownums, colnums, shipcounts };
}

