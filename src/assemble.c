#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "assembler_misc.h"


bool isLabel(char a[], int noOfLabels);

int main(int argc, char **argv) {

	FILE *fin;
	if((fin = fopen(argv[1], "r")) == NULL) {
		perror("Error opening file.txt!");
		exit(EXIT_FAILURE);
	}
	if((fout = fopen(argv[2], "w")) == NULL) {
		perror("Error opening file.txt!");
		exit(EXIT_FAILURE);
	}

	fseek(fin, 0, SEEK_END);
	int size = ftell(fin);
	rewind(fin);


	labelsTable = malloc(size);
	finalPrint = malloc(size);
	char s[size][512];
	int line = 0;
	int noOfLabels = 0;
	int addressCounter = 0;

	while(fgets(s[line], 512, fin)) {
		if(strchr(s[line], ':')) {
			strcpy(labelsTable[noOfLabels].label, s[line]);
			labelsTable[noOfLabels].address = addressCounter;
			noOfLabels ++;
		} else {
			line++;
		}
		addressCounter += 4;
	}

	for(int i = 0; i < line; i++) {
		if(!isLabel(s[i], noOfLabels)) {
			//printf("%s\n", s[i]);
			//printf("%d\n", 4 *i);
			decode(s[i], 4 * i);

		}
	}
	for(int i = 0; i < noOfFinalPrints; i++){
		write(finalPrint[i]);
	}
	return EXIT_SUCCESS;
}

bool isLabel(char a[], int noOfLabels) {
	for(int i = 0; i <= noOfLabels; i++) {
		if(strcmp(labelsTable[i].label, a) == 0) {
			return true;
		}
	}
	return false;
}
