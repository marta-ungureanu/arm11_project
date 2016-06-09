#include "instructions_table.h"
#include <string.h>
#include "assembler_misc.h"
#include <stdint.h>
#include <stdlib.h>

int getLocation(char c[]){
	for(int32_t i = 0; i < NUMBER_OF_INSTRUCTIONS; i++) {
		if(strcmp(c, instructionSet[i].instruction) == 0) {
			return i;
		}
	}
	return -1;
}

int32_t getAddress(char label[]) {
	int32_t i = 0;
	while(strcmp(labelsTable[i].label, label) != 0) {
		i++;
	}
	return labelsTable[i].address;
}

void decode(char line[], int32_t address) {
	char temp[strlen(line)];
	strcpy(temp, line);
	char *saveptr;

	while(temp[0] == ' ' || temp[0] == '\n' || temp[0] == '\t') {
		strcpy(temp, temp + 1);
	}

	if(strlen(temp) == 0) {
		return;
	}

	char *p = strtok_r(temp, " ", &saveptr);
	int32_t location = getLocation(p);
	char *restOfInstruction = line + strlen(p) + 1;
	int32_t code = instructionSet[location].type;

	switch (code) {
		case 0:
			write(0);
			break;
		case 1:
			multiplyAsm(instructionSet[location].instruction, restOfInstruction);
			break;
    		case 2:
    			dataProcessingAsm(instructionSet[location].opcode, restOfInstruction);
    			break;
    		case 3:
    			singleDataTransferAsm(restOfInstruction, instructionSet[location].instruction, address);
    			break;
    		case 4:
			branchAsm(line, address);
    			break;
		default:
			perror("Invalid instructions");
			exit(EXIT_FAILURE);
  }
}

void write(uint32_t instruction) {
	fwrite(&instruction, 4, 1, fout);
}

bool isLabel(char a[], int noOfLabels) {
	for(int32_t i = 0; i <= noOfLabels; i++) {
		if(strcmp(labelsTable[i].label, a) == 0) {
			return true;
		}
	}
	return false;
}
