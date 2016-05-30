#include "instructions_table.h"
#include <string.h>
#include "assembler_misc.h"
#include <stdint.h>
#include <stdlib.h>

int getLocation(char c[]){
	for(int i = 0; i < NUMBER_OF_INSTRUCTIONS; i++){
		if(strcmp(c, instructionSet[i].instruction) == 0) {
			return i;
		}
	}
	return -1;
}

int32_t getAddress(char label[]) {
	int i = 0;
	while(strcmp(labelsTable[i].label, label) != 0) {
		i++;
	}
	return labelsTable[i].address;
}

void decode(char line[], int address) {

	char temp[strlen(line)];
	strcpy(temp, line);
	char *saveptr;
	char *p = strtok_r(temp, " ", &saveptr);
	int location = getLocation(p);
	char *restOfInstruction = line + strlen(p) + 1;
	int code = instructionSet[location].type;

	switch (code){
		case 0:
			write(0);
			break;
		case 1:
			multiplyAsm(restOfInstruction);
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
  }
}
