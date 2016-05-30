#include "assembler_misc.h"

void write(uint32_t instruction) {
	fwrite(&instruction, 4, 1, fout);
}

