#include "assembler_misc.h"

#include <stdlib.h>
uint32_t getOffset(char address[]);
void singleDataTransferAsm(char instruction[], char type[]) {
	char *saveptr;
	char *restOfInstruction = malloc(strlen(instruction));
	strcpy(restOfInstruction, instruction);




	uint32_t binaryInstruction = SD_COMMON_BITS_MASK;
	uint32_t condition = SD_MUL_CONDITION_MASK;
	uint32_t flagI = 1 << 25;
	uint32_t flagP = 0;
	uint32_t flagU =  1 << 23;
	uint32_t flagL = 0;
	uint32_t rn = 0;
	uint32_t rd = atoi(strtok_r(instruction, "r,", &saveptr)) << 12;
	uint32_t offset = 0;
	uint32_t length = strlen(saveptr);

	char address[length];
	strcpy(address, saveptr);
	char immediateValue[length];
	strcpy(immediateValue, address);
	address[strlen(address) - 1] = '\0';
	printf("address is:%s\n", address);
	if(strcmp(type, "str")) {
		flagL = 1 << 20;
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
			noOfFinalPrints++;
			offset = encodeImmediateOperand(immediateValue);
			printf("offset is: %u\n", offset );
			flagI = 0;
		}
	} else if(strlen(address) == 4){
		char s[1] = {address[2]};
		rn = atoi(s) << 16;
		flagP = 1 << 24;
		flagI = 0;
	} else if(address[0] == '[' && address[strlen(address) - 1] == ']') {
		flagP = 1 << 24;
		char s[1] = {address[2]};
		rn = atoi(s) << 16;
		flagI = 0;
		offset = getOffset(address);
		printf("offset is %u \n", offset);
	} else {
		char s[1] = {address[2]};
		rn = atoi(s) << 16;
		offset = getOffset(address);
	}


	printf("condition is %u \n", condition >> 28);
	printf("flagI is %0x \n", flagI);
	printf("flagP is %0x \n", flagP);
	printf("flagU is %0x \n", flagU );
	printf("flagL is %0x \n", flagL);
	printf("rn is %0x \n", rn >> 15);
	printf("rd is %0x \n", rd);
	binaryInstruction += condition + flagI + flagP + flagU + flagL + rn + rd + offset;
	printf("%x \n", binaryInstruction);
	write(binaryInstruction);
}

uint32_t getOffset(char address[]) {
	char *expression = malloc(strlen(address));
	char *ptr;
	strcpy(expression, strtok_r(address, "#", &ptr));
	strcpy(expression, ptr);
	expression[strlen(expression) - 1] = '\0';
	if(strchr(expression, 'x')) {
		return (uint32_t)strtol(address, NULL, 16);
	} else {
		return atoi(expression);
	}
}
