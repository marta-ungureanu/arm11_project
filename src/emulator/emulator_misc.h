/*
 * emulator_functions.h
 *
 *  Created on: 27 May 2016
 *      Author: marta
 */

#ifndef EMULATOR_FUNCTIONS_H_
#define EMULATOR_FUNCTIONS_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define BLOCK_SIZE 1
#define DATA_PROCESSING_MASK 12 << 24
#define MULTIPLY_MASK 264241248
#define SINGLE_DATA_TRANSFER_MASK 67 << 21
#define BRANCH_MASK 5 << 24
#define FOUR_BIT_MASK ((1 << 4) -1)
#define SIX_BIT_MASK ((1 << 6) -1)
#define EIGHT_BIT_MASK ((1 << 8) -1)
#define TWO_BIT_MASK 3
#define DP_OPERAND2_MASK ((1 << 12) -1)
#define TWENTYFOUR_BIT_MASK 0x00FFFFFF
#define MULTIPLY_ACCUMULATOR_MASK 1 << 21
#define MULTIPLY_CONDITION_MASK 1 << 20
#define MUL_SD_REG_MASK1 15 << 16
#define MUL_SD_REG_MASK2 15 << 12
#define MUL_SD_REG_MASK3 15 << 8
#define MUL_SD_REG_MASK4 15
#define FLAG_Z_MASK 1 << 30
#define FLAG_N_MASK 1 << 31
#define NEG_NO_THAT_SHIFTS_PRINTING -999999999

#define LEFT_SHIFT(uint32_t, i) (uint32_t << i)
#define RIGHT_SHIFT(uint32_t, i) (uint32_t >> i)

void pipeline(void);
void execute(int code, uint32_t instruction);
int32_t decode(uint32_t instruction);
bool checkConditionField(uint32_t instruction);
uint32_t fetchInstruction(int32_t address);
void multiply(uint32_t instruction);
void singleDataTransfer(uint32_t instruction);
void branch(uint32_t instruction);
void dataProcessing(uint32_t instruction);
void printStatus(void);
uint32_t printInstruction(int32_t address);
void load_store(uint32_t rd, uint32_t address, uint32_t flagL);
bool isImmediateOperandSet(uint32_t instruction);
bool sBitSet(uint32_t instruction);
bool isLogical(uint8_t opCode);
bool isArithmetic(uint8_t opCode);
bool checkAdditionOverflow(uint32_t a, uint32_t b);
uint32_t executeLogical(uint8_t opCode, uint8_t firstRegister, 
			uint32_t operand2Value, uint8_t destinationRegister);
uint32_t executeArithmetic(uint8_t opCode, uint8_t firstRegister, 
			   uint32_t operand2Value, uint8_t destinationRegister);
uint32_t DPRotateRight(uint32_t operand2, uint8_t value, 
		       uint8_t opCode, uint32_t instruction);
uint32_t DPShift(uint32_t operand2, uint8_t opCode, uint32_t instruction);
void setZBit(uint8_t value);
void setCBit(uint8_t value);
void setNBit(uint8_t value);
uint32_t printMessage(uint32_t address);


#endif /* EMULATOR_FUNCTIONS_H_ */
