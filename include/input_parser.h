#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <stdio.h>

typedef struct {
	const int m;
	const int n;
	const char** board;
	const int* rownums;
	const int* colnums;
	const int shipcounts_length;
	const int** shipcounts;
} ParsedInput;

ParsedInput parse_input(FILE* file);

#endif
