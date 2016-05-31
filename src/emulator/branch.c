/* ARM Project 2016
 *
 * branch.c contains the function that executes a branch instruction
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include "emulator_misc.h"

/* Function that executes a branch instruction
 * 
 * PARAM: uint32_t instruction
 * 32 bit binary representation of the instruction to be executed
 *
 * RETURN: void
 *
 * Given an instruction, it determines whether or not to branch to the encoded
 * address if the condition is satisfied. 
 */
void branch(uint32_t instruction) {
	if(!checkConditionField(instruction)) {
		return;
	}
	int32_t signBit = 0;
	int32_t offset = instruction & TWENTYFOUR_BIT_MASK;
	if ((offset << BRANCH_OFFSET_SHIFT) < 0) {
		signBit = SIGNBIT_MASK;
	}
	offset *= INSTRUCTION_SIZE;
	offset += signBit;
	ARM.registers[PC] += offset - INSTRUCTION_SIZE;
}
