#include <stdlib.h>
#include "emulator_misc.h"
#include "armStructure.h"

/* function that implements the three stages pipeline
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * we distinguish between 3 cases:
 * no variable was initialized - the first instruction is being fetched
 * a variable was initialized - we have a fetched instruction to decode and we
 *                              fetch a new one
 * two variables were initialized - we have an instruction to execute, an
 *                                  instruction to decode and we fetch a new one
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * if we execute a branch with a condition that holds, the pipeline
 * "starts" again, just fetching the instruction at the current PC
 * address
 */
void pipeline(void) {
	uint32_t fetchedInstr;
	uint32_t instrToDecode;
	int32_t decodedInstr = 0;
	uint32_t instrToExecute = -1;
	int32_t initializedVariables = 0;

	while(1){
		if(decodedInstr == -1) {
			continue;
		}
		if(initializedVariables > 1) {
			instrToExecute = instrToDecode;
			instrToDecode = fetchedInstr;
			fetchedInstr = fetchInstruction(ARM.registers[PC]);
			execute(decodedInstr, instrToExecute);
			if(decodedInstr == 4 
			   && checkConditionField(instrToExecute)) {
			initializedVariables =0;
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

/* function that returns a code for every instruction, specifying what kind of
 * instruction it is
 */
int32_t decode(uint32_t instruction) {
	if(instruction == 0){
		return 0;
	}
	if (((instruction >> 4) & FOUR_BIT_MASK) == 9 
	    && (((instruction >> 22) & SIX_BIT_MASK) == 0)) {
		return 1;
	}
	if ((DATA_PROCESSING_MASK & instruction) == 0) {
		return 2;
	}
	if ((SINGLE_DATA_TRANSFER_MASK & instruction) == 0) {
		return 3;
	}
	if ((BRANCH_MASK & instruction) == 0) {
		return 4;
	}
	return -1;
}

// function that execute an instruction based on what kind of operation it represents
void execute(int32_t code, uint32_t instruction) {
	switch(code){
	case 0:
		printStatus();
		exit(EXIT_SUCCESS);
		return;
	case 1:
		multiply(instruction);
		return;
	case 2:
		dataProcessing(instruction);
		return;
	case 3:
		singleDataTransfer(instruction);
		return;
	case 4:
		branch(instruction);
		return;
	}
}

/* function that returns an instruction stored in little endian at a given address
 * in the memory
 */
uint32_t fetchInstruction(int32_t address) {
	uint32_t byte4 = ARM.memory[address];
	uint32_t byte3 = ARM.memory[address + 1] << 8;
	uint32_t byte2 = ARM.memory[address + 2] << 16;
	uint32_t byte1 = ARM.memory[address + 3] << 24;
	return (byte4 | byte3 | byte2 | byte1);
}
