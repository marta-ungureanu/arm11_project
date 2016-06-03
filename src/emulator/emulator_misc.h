#ifndef EMULATOR_FUNCTIONS_H_
#define EMULATOR_FUNCTIONS_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define SIZE_OF_MEMORY 65536
#define NUMBER_OF_REGISTERS 17
#define USEABLE_REGISTERS 12
#define PC 15
#define CPSR 16

struct State {
	uint8_t memory[SIZE_OF_MEMORY];
	int32_t registers[NUMBER_OF_REGISTERS];
};

struct State ARM;

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

#define MULTIPLY_OPCODE 9
#define MULTIPLY_OPCODE1_SHIFT 4
#define MULTIPLY_OPCODE2_SHIFT 22
#define MULTIPLY_ACCUMULATOR_MASK 1 << 21
#define MULTIPLY_CONDITION_MASK 1 << 20
#define MUL_SD_REG_MASK1 15 << 16
#define MUL_SD_REG_MASK2 15 << 12
#define MUL_SD_REG_MASK3 15 << 8
#define MUL_SD_REG_MASK4 15
#define MUL_ACC_BIT_SHIFT 21
#define MUL_COND_BIT_SHIFT 20
#define MUL_REG_SHIFT1 16
#define MUL_REG_SHIFT2 12
#define MUL_REG_SHIFT3 8

#define FLAG_Z_MASK 1 << 30
#define FLAG_N_MASK 1 << 31
#define NEG_NO_THAT_SHIFTS_PRINTING -999999999
#define BRANCH_OFFSET_SHIFT 8
#define SIGNBIT_MASK (-1) << 26
#define INSTRUCTION_SIZE 4

#define STOP 0
#define MULTIPLY 1
#define DATA_PROCESSING 2
#define SINGLE_DATA_TRANSFER 3
#define BRANCH 4
#define UNDEFINED -1

#define BYTE1_SHIFT 24
#define BYTE2_SHIFT 16
#define BYTE3_SHIFT 8

#define EQUAL 0
#define NOT_EQUAL 1
#define GREATER_OR_EQUAL 10
#define LESS_THAN 11
#define GREATER_THAN 12
#define LESS_THAN_OR_EQUAL 13
#define ALWAYS 14

#define BIT_SET 1
#define BIT_NOT_SET 0

/*
 * SingleDataTransfer Constants.
*/
#define OFFSET_MASK 0xFFF
#define CONDITION_SHIFT 28
#define ONE_BYTE_SHIFT 8
#define TWO_BYTES_SHIFT 16
#define THREE_BYTES_SHIFT 24
#define ONE_BYTE 1
#define TWO_BYTES 2
#define THREE_BYTES 3
#define NEGATIVE_SIGN -1
#define POSITIVE_SIGN 1
#define FLAG_I_SHIFT 25
#define FLAG_P_SHIFT 24
#define FLAG_U_SHIFT 23
#define FLAG_L_SHIFT 20
#define RN_SHIFT 16
#define RD_SHIFT 12
#define LAST_TWO_BITS 3

/*
 * DataProcessing Constants.
*/

#define OPCODE_SHIFT 21
#define RN_SHIFT 16
#define RD_SHIFT 12

#define OPCODE_AND 0
#define OPCODE_EOR 1
#define OPCODE_SUB 2
#define OPCODE_RSB 3
#define OPCODE_ADD 4
#define OPCODE_TST 8
#define OPCODE_TEQ 9
#define OPCODE_CMP 10
#define OPCODE_ORR 12
#define OPCODE_MOV 13

#define IMMEDIATE_FLAG_SHIFT 25
#define SET_FLAG_SHIFT 20
#define V_FLAG_SHIFT 28
#define C_FLAG_SHIFT 29
#define Z_FLAG_SHIFT 30
#define N_FLAG_SHIFT 31

#define LSL_SHIFT 0
#define LSR_SHIFT 1
#define ASR_SHIFT 2
#define ROR_SHIFT 3
#define SHIFT_BIT 4
#define OPTIONAL_REGISTER_SHIFT 7
#define ROTATE_SHIFT 8
#define SHIFT_TYPE_SHIFT 5



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
