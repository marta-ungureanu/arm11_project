#include "assembler_misc.h"
#include <stdlib.h>

void branchAsm(char instruction[], int address) {
	uint32_t binaryInstruction = BRANCH_COMMON_BITS_MASK;
	uint32_t condition = BRANCH_BAL_MASK;
	strcpy(instruction, instruction);
	char *saveptr;
	char *p = strtok_r(instruction, " ", &saveptr);
	if(strcmp(p, "beq") == 0) {
		condition = BRANCH_BEQ_MASK;
	} else if(strcmp(p, "bne") == 0) {
		condition = BRANCH_BNE_MASK;
	} else if(strcmp(p, "bge") == 0) {
		condition = BRANCH_BGE_MASK;
	} else if(strcmp(p, "blt") == 0) {
		condition = BRANCH_BLT_MASK;
	} else if(strcmp(p, "bgt") == 0) {
		condition = BRANCH_BGT_MASK;
	} else if(strcmp(p, "ble") == 0) {
		condition = BRANCH_BLE_MASK;
	}
	char *q = strtok_r(NULL, " ", &saveptr);
	q[strlen(q) - 1] = ':';
	int32_t labelAddress = getAddress(strcat(q, "\n"));
	int32_t offset = labelAddress - address;
	offset /= 4;
	offset -= 2;
	if(offset < 0) {
		offset -= 0xff << 24;
	}
	binaryInstruction += condition + offset;
	write(binaryInstruction);
}