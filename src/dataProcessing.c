#include <limits.h>
#include "emulator_misc.h"
#include "armStructure.h"

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
			if (result == 0) {
				setZBit(1);
			}
			else{
				setZBit(0);
			}
			setNBit(result >> 31);
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

void setZBit(uint8_t value) {
	if (value) {
		ARM.registers[CPSR] |= (1 << 30);
	}
	else {
		ARM.registers[CPSR] &= ~(1 << 30);
	}
}

void setNBit(uint8_t value) {
	if (value) {
		ARM.registers[CPSR] |= (1 << 31);
	}
	else {
		ARM.registers[CPSR] &= ~(1 << 31);
	}
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
	uint8_t shiftValue = operand2 >> 7;
	uint32_t value = ARM.registers[(operand2 & FOUR_BIT_MASK)];
	if (shiftValue > 0) {
		switch (shiftType) {
		case 0:
			value = value << (shiftValue - 1);
			if (isLogical(opCode) && sBitSet(instruction)) {
				setCBit(value >> 31);
			}
			return value << 1;

		case 1:
			value = value >> (shiftValue - 1);
			if (isLogical(opCode) && sBitSet(instruction)) {
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
			if (isLogical(opCode) && sBitSet(instruction)) {
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
	if (isLogical(opCode) && sBitSet(instruction)) {
		setCBit(value >> 31);
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
	//	printf("is sub returning");
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
		if( ARM.registers[firstRegister] > operand2Value){
			setCBit(0);
		}
		else if (checkAdditionOverflow(operand2Value, ((~ARM.registers[firstRegister]) + 1))) {
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
	return ( a > (INT_MAX - b) || b > (INT_MAX -a));

//	return (a + b < a || a + b < b);

}
