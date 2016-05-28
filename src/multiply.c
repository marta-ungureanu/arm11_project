#include "emulator_misc.h"
#include "armStructure.h"

// function that executes the multiply instructions
void multiply(uint32_t instruction) {

	// if the condition does not hold, the instruction is not executed
	if(!checkConditionField(instruction)) {
		return;
	}

	uint8_t accumulator = (instruction & MULTIPLY_ACCUMULATOR_MASK) >> 21;
	uint8_t setCondition = (instruction & MULTIPLY_CONDITION_MASK) >> 20;
	uint32_t rd = (instruction & MUL_SD_REG_MASK1) >> 16;
	uint32_t rn = (instruction & MUL_SD_REG_MASK2) >> 12;
	uint32_t rs = (instruction & MUL_SD_REG_MASK3) >> 8;
	uint32_t rm = instruction & MUL_SD_REG_MASK4;

	/* depending on the accumulate bit, the instruction performs a multiply and accumulate,
	 * or a multiply only
	 */
	if(accumulator) {
		ARM.registers[rd] = ARM.registers[rm] * ARM.registers[rs] + ARM.registers[rn];
	} else {
		ARM.registers[rd] = ARM.registers[rm] * ARM.registers[rs];
	}

	// if the set condition bit is set, the CPRS flags are updated
	if(setCondition) {
		uint32_t Nflag = ARM.registers[rd] & FLAG_N_MASK;

		// setting N flag
		if((ARM.registers[CPSR] & FLAG_N_MASK) != Nflag) {
			if(Nflag) {
				ARM.registers[CPSR] += FLAG_N_MASK;
			} else {
				ARM.registers[CPSR] -= FLAG_N_MASK;
			}
		}

		// setting Z flag
		if(ARM.registers[rd] == 0) {
			if((ARM.registers[CPSR] & FLAG_Z_MASK) == 0) {
				ARM.registers[CPSR] += FLAG_Z_MASK;
			}
		}
	}
}

