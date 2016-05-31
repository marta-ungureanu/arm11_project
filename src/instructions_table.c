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
	//printf("temp is: %s\n", temp);

	while(temp[0] == ' ' || temp[0] == '\n' || temp[0] == '\t'){
		//p = strtok_r(NULL, " ", &saveptr);
		strcpy(temp, temp+1);
		//printf("p is: %s\n", p);

	}
	if(strlen(temp) == 0){
		return;
	}
	char *p = strtok_r(temp, " ", &saveptr);
						//exit(EXIT_SUCCESS);
	//printf("instruction is: %s\n", p);
	int location = getLocation(p);
	char *restOfInstruction = line + strlen(p) + 1;
	int code = instructionSet[location].type;
	//printf("the code is %d\n", code);
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
