/* ARM Project 2016
 *
 * multiplyAsm.c contains the function that assembles a multiply instruction
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include <stdlib.h>
#include "assembler_misc.h"

/* Function that assembles a multiply instruction and then calls the write()
 * function to print it to the binary file
 *
 * PARAM: char mnemonic[], char instruction[]
 * mnemonic[]: the mnemonic of the instruction
 * instruction[]: the rest of the instruction, operands
 *
 * RETURN: void
 *
 * Depending on the mnemonic bit, the instruction has 4 or 3 operands. In the
 * first case, the accumulator bit is set. The set bit is never set.
 */
void multiplyAsm(char mnemonic[], char instruction[]) {
	uint32_t binaryInstruction = MUL_COMMON_BITS_MASK;
	uint32_t condition = SD_MUL_CONDITION_MASK;
	uint32_t accumulatorBit = 0;
	char *saveptr;
	uint32_t rd = atoi(strtok_r(instruction, "r,", &saveptr)) << MUL_RD_SHIFT;
	uint32_t rn = 0;
	uint32_t rm, rs;

	if(strcmp(mnemonic, "mla") == 0) {
		rm = atoi(strtok_r(NULL, "r, ", &saveptr));
		rs = atoi(strtok_r(NULL, "r, ", &saveptr)) << MUL_RS_SHIFT;
		rn = atoi(strtok_r(NULL, "r, ", &saveptr)) << MUL_RN_SHIFT;
		accumulatorBit = 1 << 21;
	} else {
		rm = atoi(strtok_r(NULL, "r,", &saveptr));
		rs = atoi(strtok_r(NULL, "r,", &saveptr)) << MUL_RS_SHIFT;
	}

	binaryInstruction += condition + accumulatorBit + rd + rn + rm + rs;
	write(binaryInstruction);
}

