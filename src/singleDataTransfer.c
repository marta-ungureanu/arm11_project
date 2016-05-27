#include <stdlib.h>
#include "emulator_misc.h"
#include "armStructure.h"

void singleDataTransfer(uint32_t instruction) {

	if(!checkConditionField(instruction)) {
		return;
	}

	uint32_t maskRn = 0xf << 16;
	uint32_t maskRd = 0xf << 12;
	uint32_t maskRm = 0xf;
	uint32_t flagL = (instruction >> 20) % 2;
	uint32_t flagU = (instruction >> 23) % 2;
	uint32_t flagP = (instruction >> 24) % 2;
	uint32_t flagI = (instruction >> 25) % 2;
	uint32_t rn = (instruction & maskRn) >> 16;
	uint32_t rd = (instruction & maskRd) >> 12;
	uint16_t offset = instruction & 0xfff;
	if(flagI) {

		if(!flagP && ((offset & maskRm) == rn)) {
			perror("Operation not allowed!");
			exit(EXIT_FAILURE);
		}
		offset = DPShift(offset, ((offset >> 1) & 3), instruction);
	}
	int sign = -1;
	if(flagU) {
		sign = 1;
	}

	uint32_t address = ARM.registers[rn];

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
			switch(address){
			case 0x20200008:
				ARM.registers[rd] = address;
				printf("One GPIO pin from 20 to 29 has been accessed\n");
				break;
			case 0x20200004:
				ARM.registers[rd] = address;
				printf("One GPIO pin from 10 to 19 has been accessed\n");
				break;
			case 0x20200000:
				ARM.registers[rd] = address;
				printf("One GPIO pin from 0 to 9 has been accessed\n");
				break;
			case 0x20200028:
				ARM.registers[rd] = address;
				printf("PIN OFF\n");
				break;
			case 0x2020001c:
				ARM.registers[rd] = address;
				printf("PIN ON\n");
				break;
			default:
				printf("Error: Out of bounds memory access at address 0x%08x\n", address);
			}

		}
		/*else if (address == 0x20200004){
			ARM.registers[rd] = address;
			printf("One GPIO pin from 10 to 19 has been accessed\n");
		}
		else {
			printf("Error: Out of bounds memory access at address 0x%08x\n", address);
		}*/
	}
	else {
		//printf("hello\n");
		uint32_t registerContent = ARM.registers[rd];
		uint32_t byte4 = registerContent & EIGHT_BIT_MASK;
		uint32_t byte3 = (registerContent & (EIGHT_BIT_MASK << 8)) >> 8;
		uint32_t byte2 = (registerContent & (EIGHT_BIT_MASK << 16)) >> 16;
		uint32_t byte1 = (registerContent & (EIGHT_BIT_MASK << 24)) >> 24;
		if(address + 3 < SIZE_OF_MEMORY){
			ARM.memory[address] = byte4;
			ARM.memory[address + 1] = byte3;
			ARM.memory[address + 2] = byte2;
			ARM.memory[address + 3] = byte1;
		} else {
			switch(address){
			case 0x20200008:
				//ARM.registers[rd] = address;
				printf("One GPIO pin from 20 to 29 has been accessed\n");
				break;
			case 0x20200004:
				//ARM.registers[rd] = address;
				printf("One GPIO pin from 10 to 19 has been accessed\n");
				break;
			case 0x20200000:
				//ARM.registers[rd] = address;
				printf("One GPIO pin from 0 to 9 has been accessed\n");
				break;
			case 0x20200028:
				//ARM.registers[rd] = address;
				printf("PIN OFF\n");
				break;
			case 0x2020001c:
				//ARM.registers[rd] = address;
				printf("PIN ON\n");
				break;
			default:
				printf("Error: Out of bounds memory access at address 0x%08x\n", address);
			}
		}

	}
}
