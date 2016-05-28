#include "emulator_misc.h"
#include "armStructure.h"

//function that executes a branch instruction
void branch(uint32_t instruction) {
	// if the condition does not hold, the instruction is not executed	
	if(!checkConditionField(instruction)) {
		return;
	}
	int signBit = 0;
	int32_t offset = instruction & TWENTYFOUR_BIT_MASK;
	if ((offset<<8) < 0) {
		signBit = (-1) << 26;
	}
	offset *= 4;
	offset += signBit;
	ARM.registers[PC] += offset - 4;
}
