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

bool* bool_alloc(const int length) {
	bool* ptr = malloc((size_t)length * sizeof(bool));
	cond_mem_err(ptr == NULL);
	return ptr;
}

// Callocation functions
int* int_calloc(const int length) {
	int* ptr = calloc((size_t)length, sizeof(int));
	cond_mem_err(ptr == NULL);
	return ptr;
}

char* char_calloc(const int length) {
	char* ptr = calloc((size_t)length, sizeof(char));
	cond_mem_err(ptr == NULL);
	return ptr;
}

bool* bool_calloc(const int length) {
	bool* ptr = calloc((size_t)length, sizeof(bool));
	cond_mem_err(ptr == NULL);
	return ptr;
}

// Reallocation functions
int* int_realloc(int* ptr, const int length) {
	ptr = realloc(ptr, (size_t)length * sizeof(int));
	cond_mem_err(ptr == NULL);
	return ptr;
}

char* char_realloc(char* ptr, const int length) {
	ptr = realloc(ptr, (size_t)length * sizeof(char));
	cond_mem_err(ptr == NULL);
	return ptr;
}

bool* bool_realloc(bool* ptr, const int length) {
	ptr = realloc(ptr, (size_t)length * sizeof(bool));
	cond_mem_err(ptr == NULL);
	return ptr;
}

// Matrix allocation functions
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

bool** bool_mat(const int m, const int n) {
	bool** mat = malloc((size_t)m * sizeof(bool*));
	cond_mem_err(mat == NULL);

	for (int i = 0; i < m; i++) {
		mat[i] = bool_alloc(n);
	}

	return mat;
}

// Matrix callocation functions
int** int_cmat(const int m, const int n) {
	int** mat = malloc((size_t)m * sizeof(int*));
	cond_mem_err(mat == NULL);

	for (int i = 0; i < m; i++) {
		mat[i] = int_calloc(n);
	}

	return mat;
}

char** char_cmat(const int m, const int n) {
	char** mat = malloc((size_t)m * sizeof(char*));
	cond_mem_err(mat == NULL);

	for (int i = 0; i < m; i++) {
		mat[i] = char_calloc(n);
	}

	return mat;
}

bool** bool_cmat(const int m, const int n) {
	bool** mat = malloc((size_t)m * sizeof(bool*));
	cond_mem_err(mat == NULL);

	for (int i = 0; i < m; i++) {
		mat[i] = bool_calloc(n);
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

// Further functions
int calc_str_length(const char* str) {
	int length = 0;
	for (;;length++) {
		if (str[length] == '\0') { break; }
	}

	return length;
}
