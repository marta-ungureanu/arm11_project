#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define SIZE_OF_MEMORY 65536
#define NUMBER_OF_REGISTERS 17
#define PC 15
#define CPSR 16
#define DATA_PROCESSING_MASK 12 << 24
#define MULTIPLY_MASK 264241248
#define SINGLE_DATA_TRANSFER_MASK 67 << 21
#define BRANCH_MASK 5 << 24

#define FOUR_BIT_MASK ((1 << 4) -1)
#define SIX_BIT_MASK ((1 << 6) -1)
#define EIGHT_BIT_MASK ((1 << 8) -1)
#define TWO_BIT_MASK 3
#define DP_OPERAND2_MASK ((1 << 12) -1)

struct State {
	uint8_t memory[SIZE_OF_MEMORY];
	int registers[NUMBER_OF_REGISTERS];
};

struct State ARM;

uint32_t fetchInstruction(int address);
//void decode(uint32_t instruction);
int decode(uint32_t instruction);
void execute(int code, uint32_t instruction);
void pipeline(void);
bool checkConditionField(uint32_t instruction);
void multiply(uint32_t instruction);
void singleDataTransfer(uint32_t instruction);
void branch(uint32_t instruction);
void printStatus(void);
uint32_t printInstruction(int address);

void load_store(uint32_t rd, uint32_t address, uint32_t flagL);

uint32_t DPRotateRight(uint32_t operand2, uint8_t value, uint8_t opCode, uint32_t instruction);
uint32_t DPShift(uint32_t operand2, uint8_t opCode, uint32_t instruction);
void dataProcessing(uint32_t instruction);
bool isImmediateOperandSet(uint32_t instruction);
bool sBitSet(uint32_t instruction);
bool isLogical(uint8_t opCode);
bool isArithmetic(uint8_t opCode);

uint32_t getDPOpCode(uint32_t instruction);
uint32_t getDPRn(uint32_t instruction);
uint32_t getDPRd(uint32_t instruction);
uint32_t getDPOperand2(uint32_t instruction);
uint32_t executeLogical(uint8_t opCode, uint8_t firstRegister, uint32_t operand2Value, uint8_t destinationRegister);
int executeArithmetic(uint8_t opCode, uint8_t firstRegister, uint32_t operand2Value, uint8_t destinationRegister);
bool checkAdditionOverflow(uint32_t a, uint32_t b);

void setZBit();
void setCBit(uint8_t value);
void setNBit(uint8_t value);

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

/*	for(int i = 0; i < size; i+=4) {
		decode(fetchInstruction(i));
		printStatus();
		//printf("%d", checkConditionField(fetchInstruction(i)));
	}*/

	pipeline();
	printStatus();

	return EXIT_SUCCESS;
}

void pipeline(void) {

	uint32_t fetchedInstr;
	uint32_t instrToDecode;
	int decodedInstr;
	uint32_t instrToExecute;
	int initializedVariables = 0;

	//printf("Pc = %d\n", ARM.registers[PC]);

	while(1){
		if(decodedInstr == -1) {
			ARM.registers[PC] += 4;
			continue;
		}
		if(initializedVariables > 1) {
			instrToExecute = instrToDecode;
			instrToDecode = fetchedInstr;
			fetchedInstr = fetchInstruction(ARM.registers[PC]);
			execute(decodedInstr, instrToExecute);
			if(decodedInstr == 4 && checkConditionField(instrToExecute)) {
				return;
			}
			decodedInstr = decode(instrToDecode);
		} else if(initializedVariables > 0) {
			instrToDecode = fetchedInstr;
			fetchedInstr = fetchInstruction(ARM.registers[PC]);
			decodedInstr = decode(instrToDecode);
			initializedVariables ++;
		} else {
			fetchedInstr = fetchInstruction(ARM.registers[PC]);
			initializedVariables ++;
		}
		if(instrToExecute == 0) {
			return;
		}
		ARM.registers[PC] += 4;
		//printStatus();
	}
}

uint32_t fetchInstruction(int address) {
	uint32_t byte4 = ARM.memory[address];
	uint32_t byte3 = ARM.memory[address + 1] << 8;
	uint32_t byte2 = ARM.memory[address + 2] << 16;
	uint32_t byte1 = ARM.memory[address + 3] << 24;
	return (byte4 | byte3 | byte2 | byte1);
}

uint32_t printInstruction(int address) {
	uint32_t byte4 = ARM.memory[address + 3];
	uint32_t byte3 = ARM.memory[address + 2] << 8;
	uint32_t byte2 = ARM.memory[address + 1] << 16;
	uint32_t byte1 = ARM.memory[address] << 24;
	return (byte4 | byte3 | byte2 | byte1);
}

int decode(uint32_t instruction) {
	if(instruction == 0){
			return 0;
	}
	if (((instruction >> 4) & FOUR_BIT_MASK) == 9 && (((instruction >> 22) & SIX_BIT_MASK) == 0)) {
		return 1;
	}
	if ((DATA_PROCESSING_MASK & instruction) == 0) {
		return 2;
	}
	if ((SINGLE_DATA_TRANSFER_MASK & instruction) == 0) {
		return 3;
	}
	if ((BRANCH_MASK & instruction) == 0) {
		return 4;
	}
	return -1;
}

void execute(int code, uint32_t instruction) {
	switch(code){
	case 0:
		return;
	case 1:
		multiply(instruction);
		return;
	case 2:
		dataProcessing(instruction);
		return;
	case 3:
		singleDataTransfer(instruction);
		return;
	case 4:
		branch(instruction);
		return;
	}
}


/*void decode(uint32_t instruction) {

	if(instruction == 0){
		printf("STOP\n");
		//stop();
	} else if((MULTIPLY_MASK & instruction) == 0) {
		printf("MULTIPLY\n");
		multiply(instruction);
	} else if ((DATA_PROCESSING_MASK & instruction) == 0) {
		printf("DATA\n");
		dataProcessing(instruction);
	} else if ((SINGLE_DATA_TRANSFER_MASK & instruction) == 0) {
		printf("SINGLE\n");
		singleDataTransfer(instruction);
	} else if ((BRANCH_MASK & instruction) == 0) {
		printf("BRANCH\n");
		branch(instruction);
	} else {
		printf("???\n");
		//dealwiththat();
	}

}*/

bool checkConditionField(uint32_t instruction) {

	uint8_t condition = instruction >> 28;
	uint32_t cprs = ARM.registers[CPSR];
	uint32_t flagV = (cprs >> 28) % 2;
	uint32_t flagZ = (cprs >> 30) % 2;
	uint32_t flagN = cprs >> 31;
	//printf("V = %u \n Z = %u \n N = %u \n", flagV, flagZ, flagN);

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
			//printf("DIFERIT\n");
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
	//printf("Instruction is %u \n", instruction);

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
	//printf("%d \n", flagI);
	if(flagI) {
		
		if(!flagP && ((offset & maskRm) == rn)) {
			perror("Operation not allowed!");
			exit(EXIT_FAILURE);
		}
		offset = DPShift(offset, ((offset >> 1) & 3), instruction);//codefromdataprocessing
	}
	int sign = -1;
	if(flagU) {
		sign = 1;
	} 
	
	uint32_t address = ARM.registers[rn];
	
	if(flagP) {
		address += sign * offset;
		//printf("HEREEEEE %u \n", address);
		load_store(rd, address, flagL);
	} else {
		load_store(rd, address, flagL);
		ARM.registers[rn] += sign * offset;
	}	

}

void load_store(uint32_t rd, uint32_t address, uint32_t flagL) {
	if (flagL) {
		//printf("address value is: %u \n", address);
		if (address < (SIZE_OF_MEMORY - 3)) {
			//printf("HEREEEEE %u \n", fetchInstruction(address));
			ARM.registers[rd] = fetchInstruction(address);
		}
		else {
			printf("Error: Out of bounds memory access at address 0x%08x\n", address);
		}
	}
	else {
		//ARM.memory[address] = ARM.registers[rd];
		uint32_t registerContent = ARM.registers[rd];
		uint32_t byte4 = registerContent & EIGHT_BIT_MASK;
		uint32_t byte3 = registerContent & (EIGHT_BIT_MASK << 8);
		uint32_t byte2 = registerContent & (EIGHT_BIT_MASK << 16);
		uint32_t byte1 = registerContent & (EIGHT_BIT_MASK << 24);

		ARM.memory[address] = byte4;
		ARM.memory[address + 1] = byte3;
		ARM.memory[address + 2] = byte2;
		ARM.memory[address + 3] = byte1;
	}
}


void branch(uint32_t instruction) {

	if(!checkConditionField(instruction)) {
		//printf("is the flaw here? :D");
		return ;
	}
	//ARM.registers[PC] = 20;
	//printf("%u \n", instruction);
	int signBit = 0;
	int offset = instruction << 8;
	if (offset < 0) {
		signBit = -1 << 26;
	}
	offset >>= 6;
	offset += signBit;
	ARM.registers[PC] += offset;
	pipeline();
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
		if(i == 16) {
			if(ARM.registers[i] < 0) {
				printf("%12d (0x%08x)\n", ARM.registers[i], ARM.registers[i]);
			} else {
				printf("%11d (0x%08x)\n", ARM.registers[i], ARM.registers[i]);
			}
		} else {
			printf("%11d (0x%08x)\n", ARM.registers[i], ARM.registers[i]);
		}
		
	}

	printf("Non-zero memory:\n");

	for(int i = 0; i < SIZE_OF_MEMORY; i+=4) {
		if(fetchInstruction(i) != 0) {
			printf("0x%08x: 0x%08x\n", i, printInstruction(i));
		}
	}

}

void dataProcessing(uint32_t instruction) {

	if (!checkConditionField(instruction)) {
		return;
	}
	else {

		uint32_t opCode = getDPOpCode(instruction);
		uint8_t firstRegister = getDPRn(instruction);
		uint8_t destinationRegister = getDPRd(instruction);
		uint32_t operand2 = getDPOperand2(instruction);
		uint32_t result;
		if (isImmediateOperandSet(instruction)) {
			operand2 = DPRotateRight((operand2 & EIGHT_BIT_MASK), operand2 >> 8, opCode, instruction);
		}
		else {
			operand2 = DPShift(operand2, opCode, instruction);
		}
		if (isLogical(opCode)) {
			result = executeLogical(opCode, firstRegister, operand2, destinationRegister);
		}
		else if (isArithmetic(opCode)) {
			result = executeArithmetic(opCode, firstRegister, operand2, destinationRegister);
		}
		if (sBitSet(instruction)) {
			//printf("the result in bitset is: %d", result);
			if (result == 0) {
				setZBit();
			}
			setNBit(result >> 31);
			//printf("the n bit should be: %d\n", result >> 31);
		}
	}
}

bool isLogical(uint8_t opCode) {
	return (opCode == 0 ||
		opCode == 1 ||
		opCode == 8 ||
		opCode == 9 ||
		opCode == 12 ||
		opCode == 13);
}

bool isArithmetic(uint8_t opCode) {
	return (opCode == 2 ||
		opCode == 3 ||
		opCode == 4 ||
		opCode == 10);
}

bool isImmediateOperandSet(uint32_t instruction) {
	return (((instruction >> 25) % 2) == 1);
}

bool sBitSet(uint32_t instruction) {
	return (((instruction >> 20) % 2) == 1);
}

void setCBit(uint8_t value) {
	if (value) {
		ARM.registers[CPSR] |= (1 << 29);
	}
	else {
		ARM.registers[CPSR] &= ~(1 << 29);
	}
}

void setZBit() {
	ARM.registers[CPSR] |= 1 << 30;
}

void setNBit(uint8_t value) {
	if (value) {
		ARM.registers[CPSR] |= (1 << 31);
	}
	else {
		ARM.registers[CPSR] &= ~(1 << 31);
	}
	//ARM.registers[CPSR] ^= (-value ^ ARM.registers[CPSR]) & (1 << 31);
}

uint32_t getDPOpCode(uint32_t instruction) {
	return ((instruction >> 21) & FOUR_BIT_MASK);
}

uint32_t getDPRn(uint32_t instruction) {
	return ((instruction >> 16) & FOUR_BIT_MASK);
}

uint32_t getDPRd(uint32_t instruction) {
	return ((instruction >> 12) & FOUR_BIT_MASK);
}

uint32_t getDPOperand2(uint32_t instruction) {
	return (instruction & DP_OPERAND2_MASK);
}

uint32_t DPShift(uint32_t operand2, uint8_t opCode, uint32_t instruction ) {
	uint8_t shiftType = (operand2 >> 5) & TWO_BIT_MASK;
	uint8_t shiftValue = operand2 >> 8;
	uint32_t value = ARM.registers[(operand2 & FOUR_BIT_MASK)];
	if (shiftValue > 0) {
		switch (shiftType) {
		case 0:
			value = value << (shiftValue - 1);
			if (isLogical(opCode)) {
				setCBit(value >> 31);
			}
			return value << 1;

		case 1:
			value = value >> (shiftValue - 1);
			if (isLogical(opCode)) {
				setCBit(value % 2);
			}
			return value >> 1;

		case 2:
			while (shiftValue > 1) {
				if ((value >> 31) == 1) {
					value = (value >> 1) + (1 << 31);
				}
				else {
					value = value >> 1;
				}
				shiftValue--;
			}
			if (isLogical(opCode)) {
				setCBit(value % 2);
			}
			if ((value >> 31) == 1) {
				value = (value >> 1) + (1 << 31);
			}
			else {
				value = value >> 1;
			}
			return value;
		case 3:
			return DPRotateRight(value, shiftValue, opCode, instruction);
		}
	}
	return value;
}

uint32_t DPRotateRight(uint32_t value, uint8_t shiftValue, uint8_t opCode, uint32_t instruction) {
	while (shiftValue > 0) {
		if (value % 2 == 1) {
			value = value >> 1;
			value = value + (1 << 31);
		}
		else {
			value = value >> 1;
		}
		if (value % 2 == 1) {
			value = value >> 1;
			value = value + (1 << 31);
		}
		else {
			value = value >> 1;
		}
		shiftValue--;
	}
	if (isLogical(opCode)) {
		if (sBitSet(instruction)) {
			setCBit(value >> 31);
		}
	}
	return value;
}


uint32_t executeLogical(uint8_t opCode, uint8_t firstRegister, uint32_t operand2Value, uint8_t destinationRegister) {
	switch (opCode) {
	case 0:
		ARM.registers[destinationRegister] = (ARM.registers[firstRegister] & operand2Value);
		return ARM.registers[destinationRegister];
	case 1:
		ARM.registers[destinationRegister] = (ARM.registers[firstRegister] ^ operand2Value);
		return ARM.registers[destinationRegister];
	case 8:
		//printf("register is: %d and operand2 is: %d \n", ARM.registers[firstRegister], operand2Value);
		return ARM.registers[firstRegister] & operand2Value;
	case 9:
		return ARM.registers[firstRegister] ^ operand2Value;
	case 12:
		ARM.registers[destinationRegister] = (ARM.registers[firstRegister] | operand2Value);
		return ARM.registers[destinationRegister];
	case 13:
		ARM.registers[destinationRegister] = operand2Value;
		return ARM.registers[destinationRegister];
	}
	printf("unreachable code in executeLogical.");
	return 0;
}



int executeArithmetic(uint8_t opCode, uint8_t firstRegister, uint32_t operand2Value, uint8_t destinationRegister) {
	uint32_t result; 
	switch (opCode) {
	case 2:
		/*if (((unsigned)ARM.registers[firstRegister]) < ((unsigned)operand2Value)) {
			setCBit(0);
		}
		else {
			setCBit(1);
		}
		*/
		
		ARM.registers[destinationRegister] = (ARM.registers[firstRegister] + (~(operand2Value) +1));
		if (checkAdditionOverflow(ARM.registers[firstRegister], (~(operand2Value)+1))) {
			setCBit(0);
		}
		else {
			setCBit(1);
		}
		return ARM.registers[destinationRegister];
	case 3:
		/*if (operand2Value <= ARM.registers[firstRegister]) {
			setCBit(0);
		}
		else {
			setCBit(1);
		}
		*/
		ARM.registers[destinationRegister] = (operand2Value - ARM.registers[firstRegister]);
		/*if (ARM.registers[destinationRegister] <= operand2Value) {
			setCBit(0);
		}
		else {
			setCBit(1);
		}
		*/

		if (checkAdditionOverflow(operand2Value, ((~ARM.registers[firstRegister]) + 1))) {
			setCBit(0);
		}
		else {
			setCBit(1);
		}
		return ARM.registers[destinationRegister];
	case 4:
		ARM.registers[destinationRegister] = (ARM.registers[firstRegister] + operand2Value);
		if (checkAdditionOverflow(ARM.registers[firstRegister], operand2Value)) {
			setCBit(1);
		}
		else {
			setCBit(0);
		}
			/*
			ARM.registers[destinationRegister] < ARM.registers[firstRegister]) {
			setCBit(1);
		}
		else {
			setCBit(0);
		}
		*/
		return ARM.registers[destinationRegister];
	case 10: 
		if(operand2Value > ARM.registers[firstRegister]){
			setCBit(0);
		}
		else{
			setCBit(1);
		}
		result = ARM.registers[firstRegister] - operand2Value;
		return  result;
	}
	printf("unreachable code in executeArithmetic.");
	return 0;
}

bool checkAdditionOverflow(uint32_t a, uint32_t b) {
	return (a + b < a || a + b < b);

}
