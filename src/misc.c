/* ARM Project 2016
 *
 * misc.c contains the decoding function as well as some helper functions
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "instructions_table.h"
#include "assembler_misc.h"

/* Function that returns the location of a instruction in instructionSet
 *
 * PARAM: char instruction[]
 * string that represents the assembly instruction
 *
 * RETURN: int32_t
 * the position where the instruction is stored in the table
 */
int32_t getLocation(char instruction[]){
	for(int i = 0; i < NUMBER_OF_INSTRUCTIONS; i++){
		if(strcmp(instruction, instructionSet[i].instruction) == 0) {
			return i;
		}
	}
	return -1;
}

/* Function that returns the address a label represents in memory
 *
 * PARAM: char label[]
 * string that represents the label
 *
 * RETURN: int32_t
 * the address in memory that the label represents
 */
int32_t getAddress(char label[]) {
	int i = 0;
	while(strcmp(labelsTable[i].label, label) != 0) {
		i++;
	}
	return labelsTable[i].address;
}

/* Function that decodes a given assembly instruction
 *
 * PARAM: char line[], int32_t address
 * line[]: string that represents the assembly instruction
 * address: 32 bit integer representing the address of the instruction in
 *          memory
 *
 * RETURN: void
 *
 * The type of instruction is determined by getting the code from
 * instructionSet depending on the mnemonic which is obtained by breaking the
 * instruction into mnemonic and operands. Depending on the type, the
 * appropriate assembler function is called
 */
void decode(char line[], int32_t address) {
	char temp[strlen(line)];
	strcpy(temp, line);
	char *saveptr;

	while(temp[FIRST_CHARACTER_LOCATION] == ' ' ||
			temp[FIRST_CHARACTER_LOCATION] == '\n' ||
			temp[FIRST_CHARACTER_LOCATION] == '\t'){
		strcpy(temp, temp + 1);
	}

	if(strlen(temp) == 0){
		return;
	}

	char *mnemonic = strtok_r(temp, " ", &saveptr);
	int location = getLocation(mnemonic);
	char *restOfInstruction = line + strlen(mnemonic) + 1;
	int code = instructionSet[location].type;

	switch (code){
	case STOP:
		write(0);
		break;
	case MULTIPLY:
		multiplyAsm(instructionSet[location].instruction, restOfInstruction);
		break;
    case DATA_PROCESSING:
    	dataProcessingAsm(instructionSet[location].opcode, restOfInstruction);
    	break;
    case SINGLE_DATA_TRANSFER:
    	singleDataTransferAsm(restOfInstruction, instructionSet[location].instruction, address);
    	break;
    case BRANCH:
		branchAsm(line, address);
    	break;
  }
}

/* Function that writes an instruction in a binary file
 *
 * PARAM: uint32_t instruction
 * unsigned 32 bit integer representing the assembled instruction
 *
 * RETURN: void
 */
void write(uint32_t instruction) {
	fwrite(&instruction, INSTRUCTION_SIZE, 1, fout);
}

/* Function that checks whether a string is a label by searching it in the
 * labelsTable
 *
 * PARAM: char string[], int32_t noOfLabels
 * string[]: a given string
 * noOfLabels: the number of labels in the table
 *
 * RETURN: bool
 * returns true if the string is a label, false otherwise
 */
bool isLabel(char string[], int32_t noOfLabels) {
	for(int i = 0; i <= noOfLabels; i++) {
		if(strcmp(labelsTable[i].label, string) == 0) {
			return true;
		}
	}
	return false;
}
