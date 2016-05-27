#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "armStructure.h"
#include "emulator_misc.h"


int main(int argc, char **argv) {

	FILE *fin;

	if((fin = fopen(argv[1], "r")) == NULL) {
		perror("Error opening file.txt!");
		exit(EXIT_FAILURE);
	}

	fseek(fin, 0, SEEK_END);
	int size = ftell(fin);
	rewind(fin);

	fread(ARM.memory, size, 1, fin);

	pipeline();

	return EXIT_SUCCESS;
}
