#include "instructions_table.h"
#include <string.h>
#include "assembler_misc.h"

int getLocation(char c[]){
	for(int i = 0; i < NUMBER_OF_INSTRUCTIONS; i++){
		if(strcmp(c, instructionSet[i].instruction) == 0) {
			return i;
		}
	}
	return -1;
}

void decode(char line[]) {
	char temp[strlen(line)];
	strcpy(temp, line);
	char *saveptr;
	char *p = strtok_r(temp, " ", &saveptr);
	int location = getLocation(p);
	char *restOfInstruction = line + strlen(p) + 1;
	int code = instructionSet[location].type;

	switch (code){
	case 1:
		multiplyAsm(restOfInstruction);
		break;
    case 2:
    	//dataProcessingAsm(instructionSet[location].opcode, restOfInstruction);
    	break;
    case 3:
    	//singleDataTransferAsm(instructionSet[location].opcode, restOfInstruction);
    	break;
    case 4:
		//branchAsm(restOfInstruction);
    	//branchAsm(instructionSet[location].opcode, restOfInstruction);
    	break;
  }
}
