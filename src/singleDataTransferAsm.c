#include "assembler_misc.h"
#include <stdlib.h>

void singleDataTransferAsm(char instruction[], char type[]) {
	char *saveptr;
	uint32_t binaryInstruction = SD_COMMON_BITS_MASK;
	uint32_t condition = SD_MUL_CONDITION_MASK;
	uint32_t flagI = 1 << 25;
	uint32_t flagP = 0;
	uint32_t flagU =  1 << 23;
	uint32_t flagL = 0;
	uint32_t rn = 0;
	uint32_t rd = atoi(strtok_r(instruction, "r,", &saveptr)) << 11;
	uint32_t offset = 0;
	uint32_t length = strlen(saveptr);
	char address[length];
	strcpy(address, saveptr);
	address[strlen(address) - 1] = '\0';
	
	if(!strcmp(type, "str")) {
		flagL = 1 << 19;
	}
	
	if(address[0] == '=') {
		strcpy(address,strtok_r(NULL, "r=", &saveptr));
		if(strstr(address, "0x")) {
			offset = (uint32_t)strtol(address, NULL, 0);
		} else {
			offset = atoi(address);
		}
		
		if(offset <= 0xFF) {
			//mov
		} else {
			flagP = 1 << 24;
			rn = 0xF << 15;
			finalPrint[noOfFinalPrints] = offset;
			noOfFinalPrints++;
		}
	} else if(strlen(address) == 4){ 
		char s[1] = {address[2]};
		rn = atoi(s) << 15;
		flagP = 1 << 24;
	} else if(address[0] == '[' && address[strlen(address) - 1] == ']') {
		flagP = 1 << 24;
		char s[1] = {address[2]};
		rn = atoi(s) << 15;
		char expression[100];
		strcpy(expression, strtok_r(address, "#", &saveptr));
		strcpy(expression, saveptr);
		expression[strlen(expression) - 1] = '\0';
		if(strchr(expression, 'x')) {
			offset = (uint32_t)strtol(address, NULL, 0);
		} else {
			offset = atoi(expression);
		}
	} else {
		char s[1] = {address[2]};
		rn = atoi(s);
		char expression[100];
		strcpy(expression, strtok_r(address, "#", &saveptr));
		strcpy(expression, saveptr);
		expression[strlen(expression) - 1] = '\0';
		if(strchr(expression, 'x')) {
			offset = (uint32_t)strtol(address, NULL, 0);
		} else {
			offset = atoi(expression);
		}
	}
	
	binaryInstruction += condition + flagI + flagP + flagU + flagL + rn + rd + offset;
	
	printf("%x \n", binaryInstruction);
	write(binaryInstruction);
}
