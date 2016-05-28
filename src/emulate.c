#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "armStructure.h"
#include "emulator_misc.h"

/* reads binary instructions from a file and executes them 
 * using a pipeline
 */
int main(int argc, char **argv) {
	FILE *fin;
	
	if((fin = fopen(argv[argc - 1], "r")) == NULL) {
		perror("Error opening file.txt!");
		exit(EXIT_FAILURE);
	}

	fseek(fin, 0, SEEK_END);
	int size = ftell(fin);
	rewind(fin);

	fread(ARM.memory, size, BLOCK_SIZE, fin);
	fclose(fin);

	pipeline();
	
	return EXIT_SUCCESS;
}
