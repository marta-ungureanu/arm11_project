/* ARM Project 2016
 *
 * emulate.c contains the main function for emulator
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include <stdlib.h>
#include <limits.h>
#include "emulator_misc.h"

/* Function that reads binary instructions from a file and executes them using
 * a pipeline
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
