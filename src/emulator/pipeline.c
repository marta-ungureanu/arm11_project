/* ARM Project 2016
 *
 * pipeline.c contains the function that implements the three stages pipeline
 * and functions for fetching, decoding and executing instructions
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include <stdlib.h>
#include "emulator_misc.h"

/* Function that implements the three stages pipeline
 *
 * PARAM: void
 *
 * RETURN: void
 *
 * We distinguish between 3 cases:
 * (1) no variable was initialized - the first instruction is being fetched
 * (2) a variable was initialized - we have a fetched instruction to decode
 *                                  and we fetch a new one
 * (3) two variables were initialized - we have an instruction to execute, an
 *                                      instruction to decode and we fetch a
 *                                      new one
 * If we execute a branch with a condition that holds, the pipeline "starts"
 * again, just fetching the instruction at the current PC address
 */
void pipeline(void) {
	uint32_t fetchedInstr;
	uint32_t instrToDecode;
	int32_t decodedInstr = 0;
	uint32_t instrToExecute = -1;
	int32_t initializedVariables = 0;

	while(1){
		if(decodedInstr == UNDEFINED) {
			continue;
		}
		if(initializedVariables > 1) {
			instrToExecute = instrToDecode;
			instrToDecode = fetchedInstr;
			fetchedInstr = fetchInstruction(ARM.registers[PC]);
			execute(decodedInstr, instrToExecute);
			if(decodedInstr == BRANCH
			   && checkConditionField(instrToExecute)) {
			initializedVariables = 0;
			}
			decodedInstr = decode(instrToDecode);
		} else if(initializedVariables > 0) {
			instrToDecode = fetchedInstr;
			fetchedInstr = fetchInstruction(ARM.registers[PC]);
			decodedInstr = decode(instrToDecode);
			initializedVariables ++;
		} else {
		fetchedInstr = fetchInstruction(ARM.registers[PC]);
			initializedVariables ++;
		}
		ARM.registers[PC] += 4;
	}
}

/* Function that returns a code for every instruction, specifying what kind of
 * instruction it is
 *
 * PARAM: uint32_t instruction
 * 32 bit binary representation of the instruction to be executed
 *
 * RETURN: int32_t
 * numeric value which represents the type of instruction
 */
int32_t decode(uint32_t instruction) {
	if(instruction == 0){
		return STOP;
	}
	if (((instruction >> MULTIPLY_OPCODE1_SHIFT) & FOUR_BIT_MASK)
		== MULTIPLY_OPCODE
		&& (((instruction >> MULTIPLY_OPCODE2_SHIFT) & SIX_BIT_MASK) == 0)) {
		return MULTIPLY;
	}
	if ((DATA_PROCESSING_MASK & instruction) == 0) {
		return DATA_PROCESSING;
	}
	if ((SINGLE_DATA_TRANSFER_MASK & instruction) == 0) {
		return SINGLE_DATA_TRANSFER;
	}
	if ((BRANCH_MASK & instruction) == 0) {
		return BRANCH;
	}
	return UNDEFINED;
}

/* Function that executes an instruction based on what kind of operation it
 * represents
 *
 * PARAM: int32_t code, uint32_t instruction
 * code: numeric value which represents the type of instruction
 * instruction: 32 bit binary representation of the instruction to be executed
 *
 * RETURN: void
 *
 * If the instruction has code 0, which means that the program should stop,
 * we print the status of the machine and exit the program
 */
void execute(int32_t code, uint32_t instruction) {
	switch(code){
	case STOP:
		printStatus();
		exit(EXIT_SUCCESS);
	case MULTIPLY:
		multiply(instruction);
		return;
	case DATA_PROCESSING:
		dataProcessing(instruction);
		return;
	case SINGLE_DATA_TRANSFER:
		singleDataTransfer(instruction);
		return;
	case BRANCH:
		branch(instruction);
		return;
	}
}

/* Function that returns an instruction stored in little endian at a given
 * address in the memory
 *
 * PARAM: int32_t address
 * 32 bit integer that represents an address in the memory
 *
 * RETURN: uint32
 * 32 bit binary representation of an instruction read from the memory at
 * that address
 */
uint32_t fetchInstruction(int32_t address) {
	uint32_t byte4 = ARM.memory[address];
	uint32_t byte3 = ARM.memory[address + 1] << BYTE3_SHIFT;
	uint32_t byte2 = ARM.memory[address + 2] << BYTE2_SHIFT;
	uint32_t byte1 = ARM.memory[address + 3] << BYTE1_SHIFT;
	return (byte4 | byte3 | byte2 | byte1);
}
