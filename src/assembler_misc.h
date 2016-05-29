#ifndef ASSEMBLER_MISC_H_
#define ASSEMBLER_MISC_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define PRINTING_INSTRUCTION_SIZE 32
#define PRINTING_MASK 1 << 31
#define MUL_COMMON_BITS_MASK 9 << 4
#define MUL_CONDITION_MASK 0xe << 28


struct Table {
		char label[512];
		int address;
};

struct Table *labelsTable;

FILE *fout;

void decode(char line[]);
void write(uint32_t instruction);
void multiplyAsm(char instruction[]);

#endif
