#include "assembler_misc.h"
#include <stdlib.h>

void branchAsm(char instruction[], int address) {
	uint32_t binaryInstruction = BRANCH_COMMON_BITS_MASK;
	uint32_t condition = 14;
	char instructionCopy[];
	strcpy(instructionCopy, instruction);
	char *saveptr;
	char *p = strtok_r(instructionCopy, " ", &saveptr);
	
	if(strcmp(p, "beq") == 0) {
		condition = 0;
	} else if(strcmp(p, "bne") == 0) {
		condition = 1 << 28;
	} else if(strcmp(p, "bge") == 0) {
		condition = 10 << 28;
	} else if(strcmp(p, "blt") == 0) {
		condition = 11 << 28;
	} else if(strcmp(p, "bgt") == 0) {
		condition = 12 << 28;
	} else if(strcmp(p, "ble") == 0) {
		condition = 13 << 28;
	}
	
	int32_t labelAddress = getAddress(strtok_r(NULL, " ", &saveptr));
	int32_t offset = labelAddress - address;
	offset /= 4;
	
	binaryInstruction += condition + offset;
	write(binaryInstruction);
	
	 
}