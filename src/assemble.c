/* ARM Project 2016
 *
 * assemble.c contains the main function for assembler
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include <stdlib.h>
#include <stdint.h>
#include "assembler_misc.h"

/* Function that implements two-pass assembler, reading assembly instructions
 * from a file, storing the labels into a table and decoding every non-label
 * instruction
 */
int main(int argc, char **argv) {
	FILE *fin;
	if((fin = fopen(argv[FIN_LOCATION], "r")) == NULL) {
		perror("Error opening %s.txt!", argv[FIN_LOCATION]);
		exit(EXIT_FAILURE);
	}

	if((fout = fopen(argv[FOUT_LOCATION], "w")) == NULL) {
		perror("Error opening %s.txt!", argv[FOUT_LOCATION]);
		exit(EXIT_FAILURE);
	}

	fseek(fin, 0, SEEK_END);
	int32_t size = ftell(fin);
	rewind(fin);

	labelsTable = malloc(size);

	if(labelsTable == NULL) {
		perror("Malloc");
		exit(EXIT_FAILURE);	
	}

	finalPrint = malloc(size);

	if(finalPrint == NULL) {
		perror("Malloc");
		exit(EXIT_FAILURE);
	}

	char s[size][MAX_CHARS_PER_LINE];
	int32_t line = 0;
	int32_t noOfLabels = 0;
	int32_t addressCounter = 0;

	while(fgets(s[line], MAX_CHARS_PER_LINE, fin)) {
		if(strchr(s[line], ':')) {
			strcpy(labelsTable[noOfLabels].label, s[line]);
			labelsTable[noOfLabels].address = addressCounter;
			noOfLabels ++;
		} else if(s[line][0] != '\n'){
			line++;
			addressCounter += INSTRUCTION_SIZE;
		}
	}

	noOfInstructions = line;
	for(int32_t i = 0; i < noOfInstructions; i++) {
		if(!isLabel(s[i], noOfLabels)) {
			decode(s[i], INSTRUCTION_SIZE * i);
		}
	}

	for(int32_t i = 0; i < noOfFinalPrints; i++) {
		write(finalPrint[i]);
	}

	free(labelsTable);
	free(finalPrint);

	fclose(fin);
	fclose(fout);

	return EXIT_SUCCESS;
}
