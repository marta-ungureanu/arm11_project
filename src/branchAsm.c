/* ARM Project 2016
 *
 * branchAsm.c contains the function that assembles a branch instruction
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */
 
#include <stdlib.h>
#include "assembler_misc.h"

/* Function that executes a branch instruction and then calls the write()
 * function to print it to the binary file
 * 
 * PARAM: char instruction[], int address
 * instruction[]: contains the instruction to be assembled
 * address: contains the position of the instruction 
 *          multiplied by INSTRUCTION_SIZE
 * 
 * RETURN: void
 * 
 * Depending on the condition, we assemble the suitable instruction
 * taking into account the offset, as well as the off-by-8 bytes effect 
 * due to the pipeline, and its sign.
 */
void branchAsm(char instruction[], int32_t address) {
	uint32_t binaryInstruction = BRANCH_COMMON_BITS_MASK;
	uint32_t condition = BRANCH_BAL_MASK;
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
	offset /= INSTRUCTION_SIZE;
	offset -= OFF_BY_8_BYTES_EFFECT;
	if(offset < 0) {
		offset -= BRANCH_OFFSET;
	}
	binaryInstruction += condition + offset;
	write(binaryInstruction);
}
