/* ARM Project 2016
 *
 * dataProcessing.c contains the function that executes a dataProcessing
 * instruction
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include <limits.h>
#include "emulator_misc.h"

/* Function that determines which Data Processing instruction to execute
 *
 * PARAM : uint32_t instruction
 * 32 bit binary representation of the instruction to be execute
 *
 * RETURN : void
 *
 * If the condition field is valid then dataProcessing splits the instruction
 * into its opcode, registers and operand2. It then passes operand2 to the
 * either DPRotateRight or DPShift depending on the value of the immediate flag.
 * It also sets the appropriate Z and N flag if the S bit is set.
 */
void dataProcessing(uint32_t instruction) {
	if(!checkConditionField(instruction)) {
		return;
	}

	uint32_t opCode = ((instruction >> OPCODE_SHIFT) & FOUR_BIT_MASK);
	uint8_t rn = ((instruction >> RN_SHIFT) & FOUR_BIT_MASK);
	uint8_t rd = ((instruction >> RD_SHIFT) & FOUR_BIT_MASK);
	uint32_t operand2 = (instruction & DP_OPERAND2_MASK);
	uint32_t result;

	if(isImmediateOperandSet(instruction)) {
		operand2 = DPRotateRight((operand2 & EIGHT_BIT_MASK),
			operand2 >> ROTATE_SHIFT, opCode, instruction);
	} else {
		operand2 = DPShift(operand2, opCode, instruction);
	}

	if(isLogical(opCode)) {
		result = executeLogical(opCode, rn, operand2,
			 rd);
	} else if(isArithmetic(opCode)) {
		result = executeArithmetic(opCode, rn,
                             operand2, rd);
	} else {
		printf("OPCODE UNDEFINED IN DATAPROCESSING FUNCTION.");
	}

	if(sBitSet(instruction)) {
		if(result == 0) {
			setZBit(BIT_SET);
		} else {
			setZBit(BIT_NOT_SET);
		}
		setNBit(result >> INSTRUCTION_LENGTH);
	}
}

/* Function that determines if the opcode correpsonds to a logical operation
 *
 * PARAM : uint8_t opcode
 * 8 bit binary representation of the opcode of the instruction
 *
 * RETURN : bool
 * returns true if the opcode is a logical operation (0,1,8,9,12,13)
 * returns false otherwise
 */
bool isLogical(uint8_t opCode) {
	return (opCode == OPCODE_AND ||
		opCode == OPCODE_EOR ||
		opCode == OPCODE_TST ||
		opCode == OPCODE_TEQ ||
		opCode == OPCODE_ORR ||
		opCode == OPCODE_MOV);
}

/* Function that determines if the opcode correpsonds to a arithmetic operation
 *
 * PARAM : uint8_t opcode
 * 8 bit binary representation of the opcode of the instruction
 *
 * RETURN : bool
 * returns true if the opcode is a arithmetic operation (2,3,4,10)
 * returns false otherwise
 */
bool isArithmetic(uint8_t opCode) {
	return (opCode == OPCODE_SUB ||
		opCode == OPCODE_RSB ||
		opCode == OPCODE_ADD ||
		opCode == OPCODE_CMP);
}

/* Function that checks whether the Immediate flag is set
 *
 * PARAM : uint32_t instruction
 * 32 bit binary representation of instruction
 *
 * RETURN : bool
 * returns true if the flag = 1
 */
bool isImmediateOperandSet(uint32_t instruction) {
	return ((instruction >> IMMEDIATE_FLAG_SHIFT) % 2) == BIT_SET;
}

/* Function that checks whether the Set flag is set
 *
 * PARAM : uint32_t instruction
 * 32 bit binary representation of the instruction
 *
 * RETURN : bool
 * returns true if the flag = 1
 */
bool sBitSet(uint32_t instruction) {
	return ((instruction >> SET_FLAG_SHIFT) % 2) == BIT_SET;
}

/* Function that sets the CPSR C Flag to the supplied value
 *
 * PARAM : uint8_t instruction
 * The value (1 or 0) which the C Flag should be set to
 *
 * RETURN : void
 */
void setCBit(uint8_t value) {
	if(value) {
		ARM.registers[CPSR] |= (BIT_SET << C_FLAG_SHIFT);
	} else {
		ARM.registers[CPSR] &= ~(BIT_SET << C_FLAG_SHIFT);
	}
}

/* Function that sets the CPSR Z Flag to the supplied value
 *
 * PARAM : uint8_t instruction
 * The value (1 or 0) which the Z Flag should be set to
 *
 * RETURN : void
 */
void setZBit(uint8_t value) {
	if(value) {
		ARM.registers[CPSR] |= (BIT_SET << Z_FLAG_SHIFT);
	} else {
		ARM.registers[CPSR] &= ~(BIT_SET << Z_FLAG_SHIFT);
	}
}

/* Function that sets the CPSR N Flag to the supplied value
 *
 * PARAM : uint8_t instruction
 * The value (1 or 0) which the N Flag should be set to
 *
 * RETURN : void
 */
void setNBit(uint8_t value) {
	if(value) {
		ARM.registers[CPSR] |= (BIT_SET << N_FLAG_SHIFT);
	} else {
		ARM.registers[CPSR] &= ~(BIT_SET << N_FLAG_SHIFT);
	}
}

/* Function that determines how the register values are shifted in operand2.
 *
 * PARAM : uint32_t operand2, uint8_t opCode, uint32_t instruction
 * operand2: 12 least significant bits of the instruction
 * opCode: 8 bit value representing which instruction is being executed
 * instruction: the 32 bit instruction initially passed.
 *
 * RETURN : uint32_t
 * The value of operand2, which in this case is a register shifted by either
 * an immediate value, or another register.
 *
 * The function first determines the shiftType and value stored in the register
 * which is supplied in the instruction. It then carries out one of four shifts
 * to (lsl, lsr, asr, ror). The amount it is shifted by is either an immediate
 * value or a register. These two cases are differentiated by the value of the
 * fourth bit in operand2. If the fourth bit = 1 it then a shift by a register
 * occurs, if the fourth bit = 0 then a shift by an integer occurs.
 *
 * Maximum shift value is 32.
 */
uint32_t DPShift(uint32_t operand2, uint8_t opCode, uint32_t instruction ) {
	uint8_t shiftType = (operand2 >> SHIFT_TYPE_SHIFT) & TWO_BIT_MASK;
	uint32_t value = ARM.registers[(operand2 & FOUR_BIT_MASK)];
	uint8_t shiftValue;
	if((operand2 >> SHIFT_BIT) % 2 == BIT_NOT_SET){
		shiftValue = operand2 >> OPTIONAL_REGISTER_SHIFT;
	} else {
		uint32_t registerValue = ARM.registers[(operand2 >> ROTATE_SHIFT)];
		shiftValue = (registerValue & EIGHT_BIT_MASK);
	}
	if(shiftValue > 0) {
		switch (shiftType) {
		case LSL_SHIFT:
			value = value << (shiftValue - 1);
			if(isLogical(opCode) && sBitSet(instruction)
			&& (decode(instruction) != SINGLE_DATA_TRANSFER)) {
				setCBit(value % 2);
			}
			return value << FINAL_SHIFT;

		case LSR_SHIFT:
			value = value >> (shiftValue - 1);
			if(isLogical(opCode) && sBitSet(instruction)
			&& (decode(instruction) != SINGLE_DATA_TRANSFER)) {
				setCBit(value % 2);
			}
			return value >> FINAL_SHIFT;

		case ASR_SHIFT:
			while (shiftValue > 1) {
				if((value >> INSTRUCTION_LENGTH) == BIT_SET) {
					value = (value >> 1) + (BIT_SET << INSTRUCTION_LENGTH);
				} else {
					value = value >> 1;
				}
				shiftValue--;
			}
			if(isLogical(opCode) && sBitSet(instruction)
			&& (decode(instruction) != SINGLE_DATA_TRANSFER)) {
				setCBit(value % 2);
			}
			if((value >> INSTRUCTION_LENGTH) == BIT_SET) {
				value = (value >> 1) + (BIT_SET << INSTRUCTION_LENGTH);
			} else {
				value = value >> FINAL_SHIFT;
			}
			return value;
		case ROR_SHIFT:
			return DPRotateRight(value, shiftValue,
			       opCode, instruction);
		}
	}
	return value;
}

/* Function that determines the shift of an immediate value.
 *
 * PARAM : uint32_t value, uint8_t shiftValue, uint8_t opCode,
 		   uint32_t instruction
 * value : the immediate value stored in the last 8 bits of instruction
 * shiftValue : the value stored in bits 11 - 8 of instruction.
 * opCode: 8 bit value representing which instruction is being executed
 * instruction: the 32 bit instruction initially passed.
 *
 * RETURN : uint32_t
 * The value of operand2, which in this case is an immediate value rotated
 * by a constant stored in bits 11 -8.
 *
 * The function carries out a right rotation on the value specified.
 * It rotates value by the shift value and sets the C bit if the Set Flag is
 * set in the instruction.
 */
uint32_t DPRotateRight(uint32_t value, uint8_t shiftValue, uint8_t opCode,
	uint32_t instruction) {
	shiftValue = shiftValue * 2;
	while (shiftValue > 0) {
		if(value % 2 == BIT_SET) {
			value = value >> 1;
			value = value + (BIT_SET << INSTRUCTION_LENGTH);
		} else {
			value = value >> 1;
		}
		shiftValue--;
	}
	if(isLogical(opCode) && sBitSet(instruction) &&
	 (decode(instruction) != SINGLE_DATA_TRANSFER)) {
		setCBit(value % 2);
	}
	return value;
}

/* Function that executes a logical operation
 *
 * PARAM : uint8_t opCode, uint8_t rn, uint32_t operand2Value, uint8_t rd
 * opCode: 8 bit value representing which instruction is being executed
 * rn: the supplied register to carry out the operation on.
 * operand2Value: the value calculated by shifting the bits 11 - 0
 * uint8_t rd: the register where the result will be stored.
 *
 * RETURN : uint32_t
 * The value that is calculated by the particular logical expression.
 *
 * Executes one of the following logical operations (depending on the opcode)
 * and returns the result (writing to the specified rd if necessary):
 * AND, EOR, TST, TEQ, ORR, MOV
 */
uint32_t executeLogical(uint8_t opCode, uint8_t rn,uint32_t operand2Value,
						uint8_t rd) {
	switch (opCode) {
	case OPCODE_AND:
		ARM.registers[rd] =
		(ARM.registers[rn] & operand2Value);
		return ARM.registers[rd];
	case OPCODE_EOR:
		ARM.registers[rd] =
		(ARM.registers[rn] ^ operand2Value);
		return ARM.registers[rd];
	case OPCODE_TST:
		return ARM.registers[rn] & operand2Value;
	case OPCODE_TEQ:
		return ARM.registers[rn] ^ operand2Value;
	case OPCODE_ORR:
		ARM.registers[rd] =
		(ARM.registers[rn] | operand2Value);
		return ARM.registers[rd];
	case OPCODE_MOV:
		ARM.registers[rd] = operand2Value;
		return ARM.registers[rd];
	}
	printf("unreachable code in executeLogical.");
	return 0;
}
/* Function that executes a arithmetic operation
 *
 * PARAM : uint8_t opCode, uint8_t rn, uint32_t operand2Value, uint8_t rd
 * opCode: 8 bit value representing which instruction is being executed
 * rn: the supplied register to carry out the operation on.
 * operand2Value: the value calculated by shifting the bits 11 - 0
 * uint8_t rd: the register where the result will be stored.
 *
 * RETURN : uint32_t
 * The value that is calculated by the particular arithmetic expression.
 *
 * Executes one of the following arithmetic operations (depending on the opcode)
 * and returns the result (writing to the specified rd if necessary):
 * AND, EOR, TST, TEQ, ORR, MOV
 */
uint32_t executeArithmetic(uint8_t opCode, uint8_t rn,
	uint32_t operand2Value, uint8_t rd) {
	uint32_t result;
	switch (opCode) {
	case OPCODE_SUB:
		if(checkAdditionOverflow(ARM.registers[rn],
			(~(operand2Value)+1))) {
			setCBit(BIT_NOT_SET);
		} else {
			setCBit(BIT_SET);
		}
		ARM.registers[rd] =
		(ARM.registers[rn] - operand2Value);
		return ARM.registers[rd];
	case OPCODE_RSB:
		if( ARM.registers[rn] > operand2Value){
			setCBit(BIT_NOT_SET);
		} else if(checkAdditionOverflow(operand2Value,
			  ((~ARM.registers[rn]) + 1))) {
			setCBit(BIT_NOT_SET);
		} else {
			setCBit(BIT_SET);
		}
		ARM.registers[rd] =
		(operand2Value - ARM.registers[rn]);
		return ARM.registers[rd];
	case OPCODE_ADD:
		if(checkAdditionOverflow(ARM.registers[rn],
		    operand2Value)) {
			setCBit(BIT_SET);
		} else {
			setCBit(BIT_NOT_SET);
		}
		ARM.registers[rd] =
		(ARM.registers[rn] + operand2Value);
		return ARM.registers[rd];
	case OPCODE_CMP:
		if(operand2Value > ARM.registers[rn]){
			setCBit(BIT_NOT_SET);
		} else {
			setCBit(BIT_SET);
		}
		result = ARM.registers[rn] - operand2Value;
		return  result;
	}
	printf("unreachable code in executeArithmetic.");
	return 0;
}

/* Function that checks for arithmetic overflow.
 *
 * PARAM : uint32_t a, uint32_t b
 * a: first value
 * b: second value
 *
 * RETURN : uint32_t
 * returns true if the addition of the two numbers would result in overflow.
*/
bool checkAdditionOverflow(uint32_t a, uint32_t b) {
	return ( a > (INT_MAX - b) || b > (INT_MAX - a));
}
