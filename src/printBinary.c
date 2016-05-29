#include "assembler_misc.h"

void write(uint32_t instruction) {
	instruction = printInstruction(instruction);
	for(int i = 0; i < PRINTING_INSTRUCTION_SIZE; i++) {
		if((instruction & PRINTING_MASK) == 0){
			fprintf(fout, "0");
		}
		else {
			fprintf(fout, "1");
		}
		instruction <<= 1;
	}
}

uint32_t printInstruction(uint32_t instruction) {
	uint32_t byte4 = instruction & FOURTH_BYTE_MASK;
	uint32_t byte3 = instruction & THIRD_BYTE_MASK;
	uint32_t byte2 = instruction & SECOND_BYTE_MASK;
	uint32_t byte1 = instruction & FIRST_BYTE_MASK;
	return ((byte4 << 24) | (byte3 << 8) | (byte2 >> 8) | (byte1 >> 24));
}
