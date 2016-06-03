/* ARM Project 2016
 *
 * assembler_misc.c contains all the macros used in implementing the assembler,
 * the labelsTable and the output file, fout, as well as the headers of almost
 * all the functions used
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#ifndef ASSEMBLER_MISC_H_
#define ASSEMBLER_MISC_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define PRINTING_INSTRUCTION_SIZE 32
#define PRINTING_MASK 1 << 31

#define MUL_COMMON_BITS_MASK 9 << 4
#define MUL_CONDITION_MASK 0xe << 28
#define MUL_RD_SHIFT 16
#define MUL_RS_SHIFT 8
#define MUL_RN_SHIFT 12

#define SD_MUL_CONDITION_MASK 0xe << 28
#define SD_COMMON_BITS_MASK 1 << 26
#define SD_CONDITION_MASK 0
#define FLAG_I_SHIFT 25
#define FLAG_P_SHIFT 24
#define FLAG_U_SHIFT 23
#define FLAG_L_SHIFT 20
#define RN_SHIFT 16
#define RD_SHIFT 12
#define MOV_LIMIT 0xFF
#define MOV_OPCODE 13
#define PC 15
#define HEX_BASE 16

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
#define OFF_BY_8_BYTES_EFFECT 2
#define BRANCH_OFFSET 0xff << 24

#define DP_COMMON_BITS_MASK 0xe << 28

#define FIN_LOCATION 1
#define FOUT_LOCATION 2

#define FIRST_CHARACTER_LOCATION 0

#define STOP 0
#define MULTIPLY 1
#define DATA_PROCESSING 2
#define SINGLE_DATA_TRANSFER 3
#define BRANCH 4

#define INSTRUCTION_SIZE 4
#define MAX_CHARS_PER_LINE 512

struct Table {
		char label[512];
		int address;
};

struct Table *labelsTable;

int32_t *finalPrint;
int32_t noOfFinalPrints;
int32_t noOfInstructions;
uint32_t flagU;
FILE *fout;

void decode(char line[], int32_t address);
void write(uint32_t instruction);
bool isLabel(char string[], int32_t noOfLabels);
void multiplyAsm(char mnemonic[], char instruction[]);
void dataProcessingAsm(uint32_t opcode, char instruction[]);

uint32_t encodeShiftedRegister(char instruction[], char *arg1, char *arg2);
uint32_t encodeImmediateOperand(char value[]);
uint32_t encodeImmediateRotation(uint32_t immediateValue);
uint32_t rotateRight(uint32_t immediateValue, uint32_t rotation);

uint32_t printInstruction(uint32_t instruction);
int32_t getAddress(char label[]);
void branchAsm(char instruction[], int address);
void singleDataTransferAsm(char instruction[], char type[], int address);
uint32_t getOffset(char address[]);
uint32_t convertToNumber(char expression[]);

#endif
