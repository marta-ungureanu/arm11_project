/* ARM Project 2016
 *
 * print.c contains functions that print the status of the ARM machine
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include "emulator_misc.h"

/* Function that prints a 4 byte instruction found at a given address in memory
 *
 * PARAM: int32_t address
 * 32 bit integer that represents an address in the memory
 *
 * RETURN: uint32_t
 * 32 bit binary representation of an instruction as it is stored in memory, in
 * little endian
 */
uint32_t printInstruction(int32_t address) {
	uint32_t byte4 = ARM.memory[address + 3];
	uint32_t byte3 = ARM.memory[address + 2] << BYTE3_SHIFT;
	uint32_t byte2 = ARM.memory[address + 1] << BYTE2_SHIFT;
	uint32_t byte1 = ARM.memory[address] << BYTE1_SHIFT;
	return (byte4 | byte3 | byte2 | byte1);
}

/* Function that prints the status of the ARM machine
 *
 * PARAM: void
 *
 * RETURN: void
 */
void printStatus(void) {
	printf("Registers:\n");
	for(int32_t i = 0; i < NUMBER_OF_REGISTERS; i++) {
		if(i < 10) {
			printf("$%d  :", i);
		} else if(i <= USEABLE_REGISTERS) {
			printf("$%d :", i);
		} else if(i == PC) {
			printf("PC  :");
		} else if(i == CPSR) {
			printf("CPSR:");
		} else {
			continue;
		}
		if(ARM.registers[i] < NEG_NO_THAT_SHIFTS_PRINTING) {
			printf("%12d (0x%08x)\n",
			       ARM.registers[i], ARM.registers[i]);
		} else {
			printf("%11d (0x%08x)\n",
			       ARM.registers[i], ARM.registers[i]);
		}
	}
	printf("Non-zero memory:\n");
	for(int32_t i = 0; i < SIZE_OF_MEMORY; i += INSTRUCTION_SIZE) {
		if(fetchInstruction(i) != STOP) {
			printf("0x%08x: 0x%08x\n", i, printInstruction(i));
		}
	}
}
