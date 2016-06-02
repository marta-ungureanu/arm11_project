/* ARM Project 2016
 *
 * assemble.c contains the main function for assembler
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include <stdlib.h>
#include "assembler_misc.h"

/* Function that implements two-pass assembler, reading assembly instructions
 * from a file, storing the labels into a table and decoding every non-label
 * instruction
 */
int main(int argc, char **argv) {
	FILE *fin;
	if((fin = fopen(argv[FIN_LOCATION], "r")) == NULL) {
		perror("Error opening file.txt!");
		exit(EXIT_FAILURE);
	}

	if((fout = fopen(argv[FOUT_LOCATION], "w")) == NULL) {
		perror("Error opening file.txt!");
		exit(EXIT_FAILURE);
	}

	fseek(fin, 0, SEEK_END);
	int size = ftell(fin);
	rewind(fin);

	labelsTable = malloc(size);
	finalPrint = malloc(size);
	char s[size][MAX_CHARS_PER_LINE];
	int line = 0;
	int noOfLabels = 0;
	int addressCounter = 0;

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
	for(int i = 0; i < noOfInstructions; i++) {
		if(!isLabel(s[i], noOfLabels)) {
			decode(s[i], INSTRUCTION_SIZE * i);
		}
	}

	for(int i = 0; i < noOfFinalPrints; i++){
		write(finalPrint[i]);
	}

	free(labelsTable);
	free(finalPrint);

	fclose(fin);
	fclose(fout);

	return EXIT_SUCCESS;
}
