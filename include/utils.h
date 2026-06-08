#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#define D '.'
#define W 'w'
#define S 's'
#define R 'r'
#define T 't'
#define L 'l'
#define B 'b'
#define M 'm'

#define HASH '#'
#define COMMA ','
#define COLON ':'

#define ZERO '0'
#define ONE '1'
#define TWO '2'
#define THREE '3'
#define FOUR '4'
#define FIVE '5'
#define SIX '6'
#define SEVEN '7'
#define EIGHT '8'
#define NINE '9'

#define O 'o'
#define X 'x'

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
bool* bool_alloc(const int length);
char* char_realloc(char* ptr, const int length);
int* int_realloc(int* ptr, const int length);
int** int_mat(const int m, const int n);
char** char_mat(const int m, const int n);

int* cpy_int_arr(const int* arr, const int length);
char* cpy_char_arr(const char* arr, const int length);
int** cpy_int_mat(const int** mat, const int m, const int n);
char** cpy_char_mat(const char** mat, const int m, const int n);

int* zero_int_alloc(const int length);
char* zero_char_alloc(const int length);
bool* zero_bool_alloc(const int length);
int** zero_int_mat(const int m, const int n);

int calc_str_length(const char* str);

#endif
