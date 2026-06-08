#include <stdlib.h>

#include "utils.h"
#include "input_parser.h"
#include "parsed_input_processor.h"
#include "solver.h"
#include "renderer.h"

int main(void) {
	// Parse input file
	FILE* file = fopen("input.md", "r");
	cond_err(file == NULL, "File not found.");

	ParsedInput in = parse_input(file);
	fclose(file);

	// Process parsed input to state struct
	State s = process_parsed_input(in);

	// Find solutions
	solve(&s);

	// Render state
	render(s);

	return 0;
}
