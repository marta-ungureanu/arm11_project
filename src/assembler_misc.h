#ifndef ASSEMBLER_MISC_H_
#define ASSEMBLER_MISC_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define PRINTING_INSTRUCTION_SIZE 32
#define PRINTING_MASK 1 << 31
#define MUL_COMMON_BITS_MASK 9 << 4
#define MUL_CONDITION_MASK 0xe << 28
#define FOURTH_BYTE_MASK 0xff
#define THIRD_BYTE_MASK 0xff << 8
#define SECOND_BYTE_MASK 0xff << 16
#define FIRST_BYTE_MASK 0xff << 24

#define BRANCH_COMMON_BITS_MASK 10 << 24


struct Table {
		char label[512];
		int address;
};

struct Table *labelsTable;

FILE *fout;

void decode(char line[], int address);
void write(uint32_t instruction);
void multiplyAsm(char instruction[]);
uint32_t printInstruction(uint32_t instruction);
int32_t getAddress(char label[]);
void branchAsm(char instruction[], int address);

#endif
