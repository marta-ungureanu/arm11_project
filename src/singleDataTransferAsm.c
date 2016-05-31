#include "assembler_misc.h"

#include <stdlib.h>
uint32_t shift = 0;
uint32_t getOffset(char address[]);
void singleDataTransferAsm(char instruction[], char type[], int pc) {
	char *saveptr;
	char restOfInstruction[strlen(instruction)];
	strcpy(restOfInstruction, instruction);

	uint32_t binaryInstruction = SD_COMMON_BITS_MASK;
	uint32_t condition = SD_MUL_CONDITION_MASK;
	uint32_t flagI = 1 << 25;
	uint32_t flagP = 0;
	flagU =  1 << 23;
	uint32_t flagL = 0;
	uint32_t rn = 0;
	uint32_t rd = atoi(strtok_r(instruction, "r,", &saveptr)) << 12;
	uint32_t offset = 0;
	uint32_t length = strlen(saveptr);

	char address[length];
	strcpy(address, saveptr);
	address[strlen(address) - 1] = '\0';

	if(strcmp(type, "str")) {
		flagL = 1 << 20;
	}
	if(address[0] == ' ') {
		strcpy(address, address + 1);
	}

	if(address[0] == '=') {
		strcpy(address,strtok_r(NULL, "r=", &saveptr));
		if(strstr(address, "0x")) {
			offset = (uint32_t)strtol(address, NULL, 16);
		} else {
			offset = atoi(address);
		}

		if(offset <= 0xFF) {
			dataProcessingAsm(13, restOfInstruction);
			return;
		} else {
			flagP = 1 << 24;
			rn = 0xF << 16;
			finalPrint[noOfFinalPrints] = offset;
			offset = (noOfInstructions + noOfFinalPrints) * 4 - pc - 8;
			//printf("the number's address is %d, pc is %d\n", (noOfInstructions + noOfFinalPrints) * 4, pc);
			noOfFinalPrints++;
			//offset = encodeImmediateOperand(immediateValue);
			//printf("offset is: %u\n", offset );
			flagI = 0;
		}
	} else if(strlen(address) == 4){
		char s[1] = {address[2]};
		rn = atoi(s) << 16;
		flagP = 1 << 24;
		flagI = 0;
	} else if(address[0] == '[' && address[strlen(address) - 1] == ']') {
		//printf("%s, %d\n", labelsTable[0].label, labelsTable[0].address);
		flagP = 1 << 24;
		char s[1] = {address[2]};
		rn = atoi(s) << 16;
		offset = getOffset(address);
		if(!shift) {
			flagI = 0;
		}
		//printf("offset is %u \n", offset);
	} else {
		char s[1] = {address[2]};
		rn = atoi(s) << 16;
		offset = getOffset(address);
		if(!shift) {
			flagI = 0;
		}
	}


	/*printf("condition is %u \n", condition >> 28);
	printf("flagI is %0x \n", flagI);
	printf("flagP is %0x \n", flagP);
	printf("flagU is %0x \n", flagU );
	printf("flagL is %0x \n", flagL);
	printf("rn is %0x \n", rn >> 15);
	printf("rd is %0x \n", rd);*/
	binaryInstruction += condition + flagI + flagP + flagU + flagL + rn + rd + offset;
	printf("%x \n", binaryInstruction);
	write(binaryInstruction);
}

uint32_t getOffset(char address[]) {
	char expression[strlen(address)];
	char *ptr;
	if(strstr(address, "lsr")) {
		printf("Here\n");
		char temp[strlen(address)];
		strcpy(temp, address);
		strtok_r(address, "r", &ptr);
		strtok_r(NULL, "r", &ptr);
		strcpy(address, ptr);
		char reg[] = {address[0]};
		int i = 0;
		while(temp[i] != '#') {
			strcpy(temp, temp + 1);
		}
		char *shiftName = "lsr";
		printf("Reg is %s shiftName is %s temp is %s \n", reg, shiftName, temp);
		shift = 1;
		return encodeShiftedRegister(reg, shiftName, temp);
	} else if(strchr(address, '#')) {
		strcpy(expression, strtok_r(address, "#", &ptr));
		strcpy(expression, ptr);

		//printf("Expression is: %s \n", expression);
		//expression[strlen(expression) - 1] = '\0';
	} else {
		shift = 1;
		strcpy(expression, strtok_r(address, ",", &ptr));
		strcpy(expression, ptr);
		if(expression[0] == '-') {
			flagU = 0;
			strcpy(expression, expression + 1);
		}
		char s[1] = {expression[1]};
		return atoi(s);
	}
	if(strchr(expression, 'x') && !strchr(expression, '-')) {
		return (uint32_t)strtol(expression, NULL, 16);
	} else if(strchr(expression, 'x')){
		strcpy(expression, expression + 1);
		flagU = 0;
		return (uint32_t)strtol(expression, NULL, 16);
	} else {
		return atoi(expression);
	}
}
