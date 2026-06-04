#include <stdlib.h>

#include "utils.h"
#include "input_parser.h"
#include "solutions_finder.h"

int main(void) {
	// Process input file
	FILE* file = fopen("input.md", "r");
	cond_err(file == NULL, "File not found.");

	ParsedInput in = parse_input(file);
	fclose(file);

	// Find solutions
	find_solutions(in);

	return 0;
}
