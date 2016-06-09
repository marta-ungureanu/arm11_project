/* ARM Project 2016
 *
 * singleDataTransferAsm.c contains the function that assembles a single data 
 * transfer instruction
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include "assembler_misc.h"
#include <stdlib.h>

uint32_t shift = 0;

/* Function that assembles a single data instruction and then calls the write()
 * function to print it to the binary file
 *
 * PARAM: char instruction[], char mnemonic[], int32_t pc
 * mnemonic[]: the mnemonic of the instruction
 * instruction[]: the rest of the instruction, operands
 * pc: the address of pc
 *
 * RETURN: void
 *
 * Depending on the mnemonic bit, the instruction can load/store to/from memory. 
 * If it is a load instruction, then the flagL is set.
 *
 * If the instruction contains an '=' symbol then the instruction can be 
 * interpreted as a mov or ldr. For values that can be represented on 8 bits the
 * instruction will be interpreted as a mov, otherwise as a ldr.
 *
 * If the instruction has length 4 then it is of form [Rn], resulting that the 
 * third character is the base register, with offset = 0 and it is pre-indexed.
 *
 * If the first and the last character are matching brackets then it is 
 * preindexed, otherwise it is postindexed.
 */
void singleDataTransferAsm(char instruction[], char type[], int32_t pc) {
	char *saveptr;
	char restOfInstruction[strlen(instruction)];
	strcpy(restOfInstruction, instruction);

	flagU =  1 << FLAG_U_SHIFT;
	uint32_t binaryInstruction = SD_COMMON_BITS_MASK;
	uint32_t condition = SD_MUL_CONDITION_MASK;
	uint32_t flagI = 1 << FLAG_I_SHIFT;
	uint32_t flagP = 1 << FLAG_P_SHIFT;
	uint32_t flagL = 0;
	uint32_t rn = 0;
	uint32_t rd = atoi(strtok_r(instruction, "r,", &saveptr)) << RD_SHIFT;
	uint32_t offset = 0;
	uint32_t length = strlen(saveptr);

	char address[length];
	strcpy(address, saveptr);
	address[strlen(address) - 1] = '\0';

	if(strcmp(type, "str")) {
		flagL = 1 << FLAG_L_SHIFT;
	}
	
	if(address[0] == ' ') {
		strcpy(address, address + 1);
	}

	if(address[0] == '=') {
		strcpy(address, strtok_r(NULL, "r=", &saveptr));
		offset = convertToNumber(address);
		if(offset <= MOV_LIMIT) {
			dataProcessingAsm(MOV_OPCODE, restOfInstruction);
			return;
		} else {
			rn = PC << RN_SHIFT;
			finalPrint[noOfFinalPrints] = offset;
			offset = (noOfInstructions + noOfFinalPrints) * 4 
				- pc - 8;
			noOfFinalPrints++;
			flagI = 0;
		}
	} else if(strlen(address) == 4) {
		rn = (address[2] - '0') << RN_SHIFT;
	} else if(address[0] == '[' && address[strlen(address) - 1] == ']') {
		rn = (address[2] - '0') << RN_SHIFT;
		offset = getOffset(address);
	} else {
		flagP = 0;
		rn = (address[2] - '0') << RN_SHIFT;
		offset = getOffset(address);
	}
	
	if(!shift) {
		flagI = 0;
	}
	
	binaryInstruction += condition + flagI + flagP + flagU + flagL + rn + 
			     rd + offset;
	write(binaryInstruction);
}

/* Function that computes the offset
 *
 * PARAM: char address[]
 * address[]: the rest of instruction
 *
 * RETURN: uint32_t
 * Returns the offset required by the single data transfer instruction 
 *
 * There are three cases:
 * If address contains "lsr" we need to shift to the right with a shif value the 
 * register Rm in order to obtain the offset.
 * If address contains '#' then the offset is either a hexadecimal or a decimal 
 * number.
 * If address contains only registers, the offset is the number of the third 
 * register.
 */
uint32_t getOffset(char address[]) {
	char expression[strlen(address)];
	char *ptr;
	
	if(strstr(address, "lsr")) {
		shift = 1;
		char *shiftV = address;
		strtok_r(address, "r", &ptr);
		strtok_r(NULL, "r", &ptr);
		strcpy(address, ptr);
		char reg[] = {address[0]};
		int32_t i = 0;
		while(shiftV[i] != '#') {
			strcpy(shiftV, shiftV + 1);
		}
		char *shiftName = "lsr";
		return encodeShiftedRegister(reg, shiftName, shiftV);
	} else if(strchr(address, '#')) {
		strcpy(expression, strtok_r(address, "#", &ptr));
		strcpy(expression, ptr);
	} else {
		shift = 1;
		strcpy(expression, strtok_r(address, ",", &ptr));
		strcpy(expression, ptr);
		if(expression[0] == '-') {
			flagU = 0;
			strcpy(expression, expression + 1);
		}
		return (expression[1] - '0');
	}
	return convertToNumber(expression);
}

/* Function that converts a number represented as a string into an integer 
 * taking in consideration if it is a number in hexadecimal or decimal, both 
 * positive or negative.
 *
 * PARAM: char expression[]
 * expression[]: the expression that needs to be converted to a number 
 *
 * RETURN: uint32_t
 * Returns the number obtained by converting from a string
 *
 * There are three cases:
 * If it is a positive hexadecimal number
 * If it is a negative hexadecimal number
 * If it is a number
 */
uint32_t convertToNumber(char expression[]) {
	if(strchr(expression, 'x') && !strchr(expression, '-')) {
		return (uint32_t)strtol(expression, NULL, HEX_BASE);
	} else if(strchr(expression, 'x')) {
		strcpy(expression, expression + 1);
		flagU = 0;
		return (uint32_t)strtol(expression, NULL, HEX_BASE);
	} else {
		return atoi(expression);
	}
}

