#include <stdlib.h>
#include "emulator_misc.h"
#include "armStructure.h"

void singleDataTransfer(uint32_t instruction) {
	if(!checkConditionField(instruction)) {
		return;
	}

	uint32_t flagL = (instruction >> 20) % 2;
	uint32_t flagU = (instruction >> 23) % 2;
	uint32_t flagP = (instruction >> 24) % 2;
	uint32_t flagI = (instruction >> 25) % 2;
	uint32_t rn = (instruction & MUL_SD_REG_MASK1) >> 16;
	uint32_t rd = (instruction & MUL_SD_REG_MASK2) >> 12;
	uint16_t offset = instruction & 0xfff;
	uint32_t address = ARM.registers[rn];
	int32_t sign = -1;

	if(flagI) {

		if(!flagP && ((offset & MUL_SD_REG_MASK4) == rn)) {
			perror("Operation not allowed!");
			exit(EXIT_FAILURE);
		}
		offset = DPShift(offset, ((offset >> 1) & 3), instruction);
	}
	if(flagU) {
		sign = 1;
	}
	if(flagP) {
		address += sign * offset;
		load_store(rd, address, flagL);
	} else {
		load_store(rd, address, flagL);
		ARM.registers[rn] += sign * offset;
	}

}

void load_store(uint32_t rd, uint32_t address, uint32_t flagL) {
	if (flagL) {
		if (address < (SIZE_OF_MEMORY - 3)) {
			ARM.registers[rd] = fetchInstruction(address);
		} else {
			uint32_t status = printMessage(address);
			if(status) {
				ARM.registers[rd] = address;
			}

		}
	} else {
		uint32_t registerContent = ARM.registers[rd];
		uint32_t byte4 = registerContent & EIGHT_BIT_MASK;
		uint32_t byte3 = (registerContent & (EIGHT_BIT_MASK << 8)) 
				 >> 8;
		uint32_t byte2 = (registerContent & (EIGHT_BIT_MASK << 16)) 
				 >> 16;
		uint32_t byte1 = (registerContent & (EIGHT_BIT_MASK << 24)) 
				 >> 24;

		if(address + 3 < SIZE_OF_MEMORY){
			ARM.memory[address] = byte4;
			ARM.memory[address + 1] = byte3;
			ARM.memory[address + 2] = byte2;
			ARM.memory[address + 3] = byte1;
		} else {
			printMessage(address);
		}
	}
}

uint32_t printMessage(uint32_t address) {
	switch(address) {
		case 0x20200008:
			printf("One GPIO pin from 20 to 29 has been accessed\n");
			return 1;
		case 0x20200004:
			printf("One GPIO pin from 10 to 19 has been accessed\n");
			return 1;
		case 0x20200000:
			printf("One GPIO pin from 0 to 9 has been accessed\n");
			return 1;
		case 0x20200028:
			printf("PIN OFF\n");
			return 1;
		case 0x2020001c:
			printf("PIN ON\n");
			return 1;
		default:
			printf("Error: Out of bounds memory access at address 0x%08x\n", address);
			return 0;
	}
}
