/* ARM Project 2016
 *
 * singleDataTransfer.c contains the function that executes a single data
 * transfer instruction
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include <stdlib.h>
#include "emulator_misc.h"

/* Function that executes the single data transfer instructions
 *
 * PARAM: uint32_t instruction
 * 32 bit binary representation of the instruction to be executed
 *
 * RETURN: void
 *
 * if the flagI is set then Offset is interpreted as a shifted register taking
 * into account that a post-indexing load or store in which Rm is the same
 * register as Rn is not allowed
 *
 * if flagU is set, the offset is added to the base register, otherwise
 * is subtracted
 *
 * if flagP is set the offset is added/subtracted to the base register before
 * transferring the data, otherwise the offset is added/subtracted to the base
 * register after transferring.
 */
void singleDataTransfer(uint32_t instruction) {
	// if the condition does not hold, the instruction is not executed
	if(!checkConditionField(instruction)) {
		return;
	}

	uint32_t flagL = (instruction >> FLAG_L_SHIFT) % 2;
	uint32_t flagU = (instruction >> FLAG_U_SHIFT) % 2;
	uint32_t flagP = (instruction >> FLAG_P_SHIFT) % 2;
	uint32_t flagI = (instruction >> FLAG_I_SHIFT) % 2;
	uint32_t rn = (instruction & MUL_SD_REG_MASK1) >> RN_SHIFT;
	uint32_t rd = (instruction & MUL_SD_REG_MASK2) >> RD_SHIFT;
	uint32_t offset = instruction & OFFSET_MASK;
	uint32_t address = ARM.registers[rn];
	int32_t sign = NEGATIVE_SIGN;

	if(flagI) {
		if(!flagP && ((offset & MUL_SD_REG_MASK4) == rn)) {
			perror("Operation not allowed!");
			exit(EXIT_FAILURE);
		}
		offset = DPShift(offset, ((offset >> 1) & LAST_TWO_BITS),
				instruction);
	}

	if(flagU) {
		sign = POSITIVE_SIGN;
	}

	if(flagP) {
		address += sign * offset;
		load_store(rd, address, flagL);
	} else {
		load_store(rd, address, flagL);
		ARM.registers[rn] += sign * offset;
	}
}

/* function that load or store
 *
 * PARAM: uint32_t rd, uint32_t address, uint32_t flag
 * rd      : 32 bit binary representations of the Rd (register destination)
 * address : the address in the memory
 * flagL   : load/store
 *
 * RETURN: void
 *
 * if flagU is set, the word is loaded from memory, otherwise the word is stored
 * into memory
 *
 * if the address is out of bounds it will print a message accordingly
 */
void load_store(uint32_t rd, uint32_t address, uint32_t flagL) {
	if(flagL) {
		if(address + THREE_BYTES < SIZE_OF_MEMORY) {
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
		uint32_t byte3 = (registerContent & (EIGHT_BIT_MASK <<
				 ONE_BYTE_SHIFT)) >> ONE_BYTE_SHIFT;
		uint32_t byte2 = (registerContent & (EIGHT_BIT_MASK <<
				 TWO_BYTES_SHIFT)) >> TWO_BYTES_SHIFT;
		uint32_t byte1 = (registerContent & (EIGHT_BIT_MASK <<
				 THREE_BYTES_SHIFT)) >> THREE_BYTES_SHIFT;

		if(address + THREE_BYTES < SIZE_OF_MEMORY){
			ARM.memory[address] = byte4;
			ARM.memory[address + ONE_BYTE] = byte3;
			ARM.memory[address + TWO_BYTES] = byte2;
			ARM.memory[address + THREE_BYTES] = byte1;
		} else {
			printMessage(address);
		}
	}
}

/* Function that prints a message when the address is out of bounds taking into
 * consideration particular cases related to pins
 *
 * PARAM: uint32_t address
 * 32 bit binary representation of the out of bounds address
 *
 * RETURN: uint32_t
 *
 */
uint32_t printMessage(uint32_t address) {
	switch(address) {
		case 0x20200008:
			printf("One GPIO pin from 20 to 29 has been ");
			printf("accessed\n");
			return 1;
		case 0x20200004:
			printf("One GPIO pin from 10 to 19 has been ");
			printf("accessed\n");
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
			printf("Error: Out of bounds memory access at address");
			printf(" 0x%08x\n", address);
			return 0;
	}
}
