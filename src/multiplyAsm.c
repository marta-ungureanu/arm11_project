#include "assembler_misc.h"
#include <stdlib.h>

void multiplyAsm(char instruction[]) {
	int length = strlen(instruction);
	uint32_t binaryInstruction = MUL_COMMON_BITS_MASK;
	uint32_t condition = MUL_CONDITION_MASK;
	uint32_t accumulatorBit = 0;
	char *saveptr;
	uint32_t rd = atoi(strtok_r(instruction, "r,", &saveptr)) << 16;
	uint32_t rn = 0;
	uint32_t rm, rs;
	if(length > 10) {
		rm = atoi(strtok_r(NULL, "r,", &saveptr));
		rs = atoi(strtok_r(NULL, "r,", &saveptr)) << 8;
		rn = atoi(strtok_r(NULL, "r,", &saveptr)) << 12;
		accumulatorBit = 1 << 21;
	} else {
		rm = atoi(strtok_r(NULL, "r,", &saveptr));
		rs = atoi(strtok_r(NULL, "r,", &saveptr)) << 8;
	}
	binaryInstruction += condition + accumulatorBit + rd + rn + rm + rs;
	write(binaryInstruction);
}

