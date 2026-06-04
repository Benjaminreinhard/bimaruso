#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "utils.h"

// Print functions
void printi(const int i) {
	printf("%i ", i);
}

void printc(const char c) {
	printf("%c ", c);
}

void printcs(const char* arr, const int arr_length) {
	for (int i = 0; i < arr_length; i++) {
		printc(arr[i]);
	}
}

void printis(const int* arr, const int arr_length) {
	for (int i = 0; i < arr_length; i++) {
		printi(arr[i]);
	}
}

void printimat(int** mat, const int m, const int n) {
	for (int i = 0; i < m; i++) {
		printis(mat[i], n); printf("\n");
	}
}

void printcmat(char** mat, const int m, const int n) {
	for (int i = 0; i < m; i++) {
		printcs(mat[i], n); printf("\n");
	}
}

// Error functions
void err(const char* msg) {
	printf("Error: %s\n", msg); exit(1);
}

void cond_err(const bool cond, const char* msg) {
	if (cond) { err(msg); }
}

void cond_mem_err(const bool cond) {
	cond_err(cond, "Memory allocation failure.");
}

// Allocation functions
int* int_alloc(const int length) {
	int* ptr = malloc((size_t)length * sizeof(int));
	cond_mem_err(ptr == NULL);
	return ptr;
}

char* char_alloc(const int length) {
	char* ptr = malloc((size_t)length * sizeof(char));
	cond_mem_err(ptr == NULL);
	return ptr;
}

char* char_realloc(char* ptr, const int length) {
	ptr = realloc(ptr, (size_t)length * sizeof(char));
	cond_mem_err(ptr == NULL);
	return ptr;
}

int** int_mat(const int m, const int n) {
	int** mat = malloc((size_t)m * sizeof(int*));
	cond_mem_err(mat == NULL);

	for (int i = 0; i < m; i++) {
		mat[i] = int_alloc(n);
	}

	return mat;
}

char** char_mat(const int m, const int n) {
	char** mat = malloc((size_t)m * sizeof(char*));
	cond_mem_err(mat == NULL);

	for (int i = 0; i < m; i++) {
		mat[i] = char_alloc(n);
	}

	return mat;
}

// Copy functions
int* cpy_int_arr(const int* arr, const int length) {
	int* cpy = int_alloc(length);
	for (int i = 0; i < length; i++) {
		cpy[i] = arr[i];
	}
	return cpy;
}

char* cpy_char_arr(const char* arr, const int length) {
	char* cpy = char_alloc(length);
	for (int i = 0; i < length; i++) {
		cpy[i] = arr[i];
	}
	return cpy;
}

int** cpy_int_mat(const int** mat, const int m, const int n) {
	int** cpy = int_mat(m, n);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			cpy[i][j] = mat[i][j];
		}
	}
	return cpy;
}

char** cpy_char_mat(const char** mat, const int m, const int n) {
	char** cpy = char_mat(m, n);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			cpy[i][j] = mat[i][j];
		}
	}
	return cpy;
}

// Zero functions
int* zero_int_alloc(const int length) {
	int* arr = int_alloc(length);
	for (int i = 0; i < length; i++) {
		arr[i] = 0;
	}
	return arr;
}

int** zero_int_mat(const int m, const int n) {
	int** mat = int_mat(m, n);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			mat[i][j] = 0;
		}
	}
	return mat;
}

// Further functions
int calc_str_length(const char* str) {
	int length = 0;
	for (;;length++) {
		if (str[length] == '\0') { break; }
	}

	return length;
}

void cell_symbol(const char cell) {
	switch (cell) {
		case '.':
			printf("%s ", " "); break; // Choose \u00B7 for center dot
		case 'w':
			printf("%s ", "\u223F"); break;
		case 's':
			printf("%s ", "\u25C6"); break;
		case 'r':
			printf("%s ", "\u25B6"); break;
		case 'u':
			printf("%s ", "\u25B2"); break;
		case 'l':
			printf("%s ", "\u25C0"); break;
		case 'd':
			printf("%s ", "\u25BC"); break;
		case 'c':
			printf("%s ", "\u25FC"); break;
		}
}
