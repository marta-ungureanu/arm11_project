#ifndef ASSEMBLER_MISC_H_
#define ASSEMBLER_MISC_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define PRINTING_INSTRUCTION_SIZE 32
#define PRINTING_MASK 1 << 31
#define MUL_COMMON_BITS_MASK 9 << 4
#define SD_MUL_CONDITION_MASK 0xe << 28
#define SD_COMMON_BITS_MASK 1 << 26
#define FOURTH_BYTE_MASK 0xff
#define THIRD_BYTE_MASK 0xff << 8
#define SECOND_BYTE_MASK 0xff << 16
#define FIRST_BYTE_MASK 0xff << 24

#define BRANCH_COMMON_BITS_MASK 10 << 24
#define BRANCH_BAL_MASK 14 << 28
#define BRANCH_BEQ_MASK 0
#define BRANCH_BNE_MASK 1 << 28
#define BRANCH_BGE_MASK 10 << 28
#define BRANCH_BLT_MASK 11 << 28
#define BRANCH_BGT_MASK 12 << 28
#define BRANCH_BLE_MASK 13 << 28


struct Table {
		char label[512];
		int address;
};

struct Table *labelsTable;

int32_t *finalPrint; 
int32_t noOfFinalPrints;

FILE *fout;

void decode(char line[], int address);
void write(uint32_t instruction);
void multiplyAsm(char instruction[]);
uint32_t printInstruction(uint32_t instruction);
int32_t getAddress(char label[]);
void branchAsm(char instruction[], int address);
void singleDataTransferAsm(char instruction[], char type[]);

#endif
