/* ARM Project 2016
 *
 * multiply.c contains the function that executes a multiply instruction
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include "emulator_misc.h"
#include "armStructure.h"

/* Function that executes a multiply instruction
 *
 * PARAM: uint32_t instruction
 * 32 bit binary representation of the instruction to be executed
 *
 * RETURN: void
 *
 * Depending on the accumulate bit, the function performs the multiplication
 * Rd := Rm x Rs or Rd := Rm x Rs + Rn. If the condition bit is set, the CPSR
 * flags N and Z are updated.
 */
void multiply(uint32_t instruction) {

	if(!checkConditionField(instruction)) {
		return;
	}

	uint8_t accumulator = (instruction & MULTIPLY_ACCUMULATOR_MASK) >> 21;
	uint8_t setCondition = (instruction & MULTIPLY_CONDITION_MASK) >> 20;
	uint32_t rd = (instruction & MUL_SD_REG_MASK1) >> 16;
	uint32_t rn = (instruction & MUL_SD_REG_MASK2) >> 12;
	uint32_t rs = (instruction & MUL_SD_REG_MASK3) >> 8;
	uint32_t rm = instruction & MUL_SD_REG_MASK4;

	if(accumulator) {
		ARM.registers[rd] = ARM.registers[rm] * ARM.registers[rs] 
				  + ARM.registers[rn];
	} else {
		ARM.registers[rd] = ARM.registers[rm] * ARM.registers[rs];
	}

	if(setCondition) {
		uint32_t Nflag = ARM.registers[rd] & FLAG_N_MASK;

		if((ARM.registers[CPSR] & FLAG_N_MASK) != Nflag) {
			if(Nflag) {
				ARM.registers[CPSR] += FLAG_N_MASK;
			} else {
				ARM.registers[CPSR] -= FLAG_N_MASK;
			}
		}

		if(ARM.registers[rd] == 0 &&
				(ARM.registers[CPSR] & FLAG_Z_MASK) == 0) {
			ARM.registers[CPSR] += FLAG_Z_MASK;
		}
	}
}
