#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define SIZE_OF_MEMORY 1<<16
#define NUMBER_OF_REGISTERS 17
#define PC 15
#define CPSR 16
#define DATA_PROCESSING_MASK 12 << 24
#define MULTIPLY_MASK 264241248
#define SINGLE_DATA_TRANSFER_MASK 67 << 21
#define BRANCH_MASK 5 << 24

struct State {
	uint8_t memory[SIZE_OF_MEMORY];
	uint32_t registers[NUMBER_OF_REGISTERS];
};

struct State ARM;

uint32_t fetchInstruction(int address);
void decode(uint32_t instruction);
//void pipeline(void);
bool checkConditionField(uint32_t instruction);
void multiply(uint32_t instruction);
void singleDataTransfer(uint32_t instruction);
void branch(uint32_t instruction);
void printStatus(void);

int main(int argc, char **argv) {

	FILE *fin;

	if((fin = fopen(argv[1], "r")) == NULL) {
		perror("Error opening file.txt!");
		exit(EXIT_FAILURE);
	}

	fseek(fin, 0, SEEK_END);
	int size = ftell(fin);
	rewind(fin);

	fread(ARM.memory, size, 1, fin);

	/*for(int i = 0; i < size; i+=4) {
		printf("%d", checkConditionField(fetchInstruction(i)));
	}*/

	printStatus();

	return EXIT_SUCCESS;
}

uint32_t fetchInstruction(int address) {
	uint32_t byte4 = ARM.memory[address];
	uint32_t byte3 = ARM.memory[address + 1] << 8;
	uint32_t byte2 = ARM.memory[address + 2] << 16;
	uint32_t byte1 = ARM.memory[address + 3] << 24;
	return (byte4 | byte3 | byte2 | byte1);
}

void decode(uint32_t instruction) {

	if(instruction == 0){
		printf("STOP\n");
		//stop();
	} else if((MULTIPLY_MASK & instruction) == 0) {
		printf("MULTIPLY\n");
		multiply(instruction);
	} else if ((DATA_PROCESSING_MASK & instruction) == 0) {
		printf("DATA\n");
		//dataProcessing(instruction);
	} else if ((SINGLE_DATA_TRANSFER_MASK & instruction) == 0) {
		printf("SINGLE\n");
		//singleDataTransfer(instruction);
	} else if ((BRANCH_MASK & instruction) == 0) {
		printf("BRANCH\n");
		//branch(instruction);
	} else {
		printf("???\n");
		//dealwiththat();
	}

}

bool checkConditionField(uint32_t instruction) {

	uint8_t condition = instruction >> 28;
	uint32_t cprs = ARM.registers[CPSR];
	uint8_t flagV = (cprs >> 28) % 2;
	uint8_t flagZ = (cprs >> 2) % 2;
	uint8_t flagN = cprs >> 1;

	return  (condition == 0 && flagZ == 1) ||
			(condition == 1 && flagZ == 0) ||
			(condition == 10 && flagN == flagV) ||
			(condition == 11 && flagN != flagV) ||
			(condition == 12 && flagZ == 0 && flagN == flagV) ||
			(condition == 13 && (flagZ == 1 || flagN != flagV)) ||
			(condition == 14);
}

void multiply(uint32_t instruction) {

	if(!checkConditionField(instruction)) {
		return;
	}

	uint8_t accumulator = (instruction & (1 << 21)) >> 21;
	uint8_t setCondition = (instruction & (1 << 20)) >> 20;
	uint32_t rd = (instruction & (15 << 16)) >> 16;
	uint32_t rn = (instruction & (15 << 12)) >> 12;
	uint32_t rs = (instruction & (15 << 8)) >> 8;
	uint32_t rm = instruction & 15;

	if(accumulator) {
		ARM.registers[rd] = ARM.registers[rm] * ARM.registers[rs] + ARM.registers[rn];
	} else {
		ARM.registers[rd] = ARM.registers[rm] * ARM.registers[rs];
	}

	if(setCondition) {
		uint32_t Nflag = ARM.registers[rd] & (1 << 31);

		if((ARM.registers[CPSR] & (1 << 31)) != Nflag) {
			printf("DIFERIT\n");
			if(Nflag) {
				ARM.registers[CPSR] += 1 << 31;
			} else {
				ARM.registers[CPSR] -= 1 << 31;
			}
		}
		if(ARM.registers[rd] == 0) {
			if((ARM.registers[CPSR] & (1 << 30)) == 0) {
				ARM.registers[CPSR] += 1 << 30;
			}
		}
	}
}

void singleDataTransfer(uint32_t instruction) {
        
	if(!checkConditionField(instruction)) {
		return;
	}

        uint32_t maskRn = 0xf << 16;
        uint32_t maskRd = 0xf << 12;
        uint32_t instr = instruction;
        uint8_t flagL = (instr >> 20) % 2;
        uint8_t flagU = (instr >> 3) % 2;
        uint8_t flagP = (instr >> 1) % 2;
        uint8_t flagI = (instr >> 1) % 2;
        uint8_t Rn = (instruction & maskRn) >> 16;
        uint8_t Rd = (instruction & maskRd) >> 12;
        uint16_t Offset = instruction & 0xfff;

        if(flagI == 1) {
	  //codefromdataprocessing	
	} else {
          //codefromdataprocessing  
	}

	uint8_t sign = -1;
        if(flagU == 1) {
	  sign = 1;		
	} 

	uint32_t address = ARM.registers[Rn];

	if(flagP == 1) {
	  address += sign * Offset;
	  if(flagL == 1) {
	    ARM.registers[Rd] = ARM.memory[address];	
	  } else {
            ARM.memory[address] = ARM.registers[Rd]; 
	  } 
	} else {
	    if(flagL == 1) {
              ARM.registers[Rd] = ARM.memory[address];  
	    } else {
	      ARM.memory[address] = ARM.registers[Rd];	
            }
           ARM.registers[Rn] += sign * Offset;
	}	
}


void branch(uint32_t instruction) {

	if(!checkConditionField(instruction)) {
		return ;
	}
	int signBit = 0;
	int offset = instruction << 8;
	if (offset < 0) {
		signBit = -1 << 26;
	}
	offset >>= 6;
	offset += signBit;
	ARM.registers[PC] += offset - 8;
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
		printf("\t%u (0x%08x)\n", ARM.registers[i], ARM.registers[i]);
	}

	printf("Non-zero memory:\n");

	int i = 0;
	while(fetchInstruction(i) != 0) {
		printf("0x%08x: 0x%08x\n", i, fetchInstruction(i));
		i += 4;
	}

}
