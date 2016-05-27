#include "emulator_misc.h"
#include "armStructure.h"

uint32_t printInstruction(int address) {
	uint32_t byte4 = ARM.memory[address + 3];
	uint32_t byte3 = ARM.memory[address + 2] << 8;
	uint32_t byte2 = ARM.memory[address + 1] << 16;
	uint32_t byte1 = ARM.memory[address] << 24;
	return (byte4 | byte3 | byte2 | byte1);
}

void printStatus(void) {

	printf("Registers:\n");

	for(int i = 0; i < NUMBER_OF_REGISTERS; i++) {
		if(i < 10) {
			printf("$%d  :", i);
		} else if(i < 13) {
			printf("$%d :", i);
		} else if(i == 15) {
			printf("PC  :");
		} else if(i == 16) {
			printf("CPSR:");
		} else {
			continue;
		}
		if(ARM.registers[i] < -999999999) {
			printf("%12d (0x%08x)\n", ARM.registers[i], ARM.registers[i]);
		} else {
			printf("%11d (0x%08x)\n", ARM.registers[i], ARM.registers[i]);
		}

	}

	printf("Non-zero memory:\n");

	for(int i = 0; i < SIZE_OF_MEMORY; i+=4) {
		if(fetchInstruction(i) != 0) {
			printf("0x%08x: 0x%08x\n", i, printInstruction(i));
		}
	}
}

