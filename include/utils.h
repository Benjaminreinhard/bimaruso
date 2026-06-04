#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

void printi(const int i);
void printc(const char c);

void printcs(const char* arr, const int arr_length);
void printis(const int* arr, const int arr_length);

void printimat(int** mat, const int m, const int n);
void printcmat(char** mat, const int m, const int n);

void err(const char* msg);
void cond_err(const bool cond, const char* msg);
void cond_mem_err(const bool cond);

int* int_alloc(const int length);
char* char_alloc(const int length);
char* char_realloc(char* ptr, const int length);
int** int_mat(const int m, const int n);
char** char_mat(const int m, const int n);

int* cpy_int_arr(const int* arr, const int length);
char* cpy_char_arr(const char* arr, const int length);
int** cpy_int_mat(const int** mat, const int m, const int n);
char** cpy_char_mat(const char** mat, const int m, const int n);

int* zero_int_alloc(const int length);
int** zero_int_mat(const int m, const int n);

int calc_str_length(const char* str);

void cell_symbol (const char cell);

#endif
