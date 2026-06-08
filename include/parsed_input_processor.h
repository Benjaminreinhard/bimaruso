#ifndef PARSED_INPUT_PROCESSOR_H
#define PARSED_INPUT_PROCESSOR_H

#include "input_parser.h"

typedef struct {
	const int m;
	const int n;
	const char** board;
	const int* rownums;
	const int* colnums;
	const int* shipcounts;

	char** cur_board;
	int* cur_rownums;
	int* cur_colnums;
	int* cur_shipcounts;
} State;

State process_parsed_input(ParsedInput in);

#endif