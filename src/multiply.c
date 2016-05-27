#include "emulator_misc.h"
#include "armStructure.h"

void multiply(uint32_t instruction) {

	if(!checkConditionField(instruction)) {
		return;
	}

	uint8_t accumulator = (instruction & (1 << 21)) >> 21;
	uint8_t setCondition = (instruction & (1 << 20)) >> 20;
	uint32_t rd = (instruction & (15 << 16)) >> 16;
	uint32_t rn = (instruction & (15 << 12)) >> 12;
	uint32_t rs = (instruction & (15 << 8)) >> 8;
	uint32_t rm = instruction & 15;

	if(accumulator) {
		ARM.registers[rd] = ARM.registers[rm] * ARM.registers[rs] + ARM.registers[rn];
	} else {
		ARM.registers[rd] = ARM.registers[rm] * ARM.registers[rs];
	}

	if(setCondition) {
		uint32_t Nflag = ARM.registers[rd] & (1 << 31);

		if((ARM.registers[CPSR] & (1 << 31)) != Nflag) {
			if(Nflag) {
				ARM.registers[CPSR] += 1 << 31;
			} else {
				ARM.registers[CPSR] -= 1 << 31;
			}
		}
		if(ARM.registers[rd] == 0) {
			if((ARM.registers[CPSR] & (1 << 30)) == 0) {
				ARM.registers[CPSR] += 1 << 30;
			}
		}
	}
}

