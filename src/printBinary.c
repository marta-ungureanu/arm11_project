#include "assembler_misc.h"

void write(uint32_t instruction) {
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
