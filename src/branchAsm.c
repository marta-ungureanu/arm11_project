#include "assembler_misc.h"
#include <stdlib.h>

void branchAsm(char instruction[]) {
	uint32_t binaryInstruction = BRANCH_COMMON_BITS_MASK;
	uint32_t condition = 0;
	char instructionCopy[];
	strcpy(instructionCopy, instruction);
	char *saveptr;
	char *p = strtok_r(instructionCopy, " ", &saveptr);
	
	if(strcmp(p, "bne") == 0) {
		condition = 1 << 28;
	} else if(strcmp(p, "bge") == 0) {
		condition = 10 << 28;
	} else if(strcmp(p, "blt") == 0) {
		condition = 11 << 28;
	} else if(strcmp(p, "bgt") == 0) {
		condition = 12 << 28;
	} else if(strcmp(p, "ble") == 0) {
		condition = 13 << 28;
	} else {
		condition = 14 << 28;
	}
	
	uint32_t offset = atoi(strtok_r(NULL, " ", &saveptr));
	
	//how the hell do I compute the difference between the current address and the label...
	
	binaryInstruction += condition + offset;
	write(binaryInstruction);
	
	 
}