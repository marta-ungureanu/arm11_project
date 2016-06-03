/* ARM Project 2016
 *
 * dataProcessingAsm.c contains the function that encodes data processing
 * instructions
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include "assembler_misc.h"
#include <stdlib.h>

/* Function that determines which type of instruction to encode depending on
 * whether the result of an operation is saved to a register or not.
 *
 * PARAM: uint32_t opcode, char instruction[]
 * opcode: the 4 bit unique number which determines which operation to
 * encode. The opcode is determined using a table mapping strings to opcodes.
 * instruction: the remaining part of the insturction read from the assembly
 * file which details which registers, constants and possibly shifts are used.
 *
 * RETURN: void
 *
 * The fucntion determines how to split the rest of the instruction based
 * upon the opcode input. For example and add insturction takes a destination
 * register rd, a source register rn, and an operand2 which can be a constant or
 * a register, or a shifted register. It then assembles the binary function
 * using the results from the helper functions and calls the write() method to
 * write the binary insturction to the output file.
 *
 * Note: LSL is a special case and is handled first.
 */
void dataProcessingAsm(uint32_t opcode, char instruction[]){
  uint32_t binaryInstruction = DP_COMMON_BITS_MASK;
  uint32_t rd = 0;
  uint32_t rn = 0;
  uint32_t operand2 = 0;
  char *saveptr;
  char* argument1 = NULL;
  char* argument2 = NULL;

  if(opcode == LSL_OPCODE){
      char *destinationReg = strtok_r(instruction, "r,", &saveptr);
      rd = atoi(destinationReg);
      instruction = strtok_r(NULL, " r", &saveptr);
      operand2 = encodeShiftedRegister(destinationReg, "lsl", instruction);
      binaryInstruction = binaryInstruction
                        + (MOV_OPCODE << OPCODE_SHIFT)
                        +(rd << DESTINATION_REGISTER_SHIFT)
                        + operand2;
      write(binaryInstruction);
      return;
    } else if( opcode == AND_OPCODE || opcode == EOR_OPCODE
            || opcode == SUB_OPCODE || opcode == RSB_OPCODE
            || opcode == ADD_OPCODE || opcode == ORR_OPCODE){
    rd = atoi(strtok_r(instruction, "r,", &saveptr));
    rn = atoi(strtok_r(NULL, "r,", &saveptr));
    instruction = strtok_r(NULL, " r", &saveptr);

    if(saveptr != NULL){
    argument1 = strtok_r(NULL, " ", &saveptr);
    argument2 = strtok_r(NULL, " ", &saveptr);
    }
} else if(opcode == MOV_OPCODE){
    rd = atoi(strtok_r(instruction, "r,", &saveptr));
    instruction = strtok_r(NULL, " r", &saveptr);
} else {
    binaryInstruction += SET_FLAG;
    char *sourceReg = strtok_r(instruction, "r,", &saveptr);
    if(*sourceReg == ' '){
      rn = atoi(strtok_r(NULL, "r,", &saveptr));
    } else {
      rn = atoi(sourceReg);
    }
    instruction = strtok_r(NULL, " r", &saveptr);
  }
  if(instruction[0] == '#' || instruction[0] == '='){
    operand2 = encodeImmediateOperand(instruction);
    operand2 += IMMEDIATE_FLAG;
  }
  else{
    operand2 = encodeShiftedRegister(instruction, argument1, argument2);
  }
    binaryInstruction = binaryInstruction
                      + (opcode << OPCODE_SHIFT)
                      + (rd <<DESTINATION_REGISTER_SHIFT)
                      + (rn << SOURCE_REGISTER_SHIFT) + operand2;
    write(binaryInstruction);
}

/* Function that encodes a shifted register instruction of the form:
 * reg, shiftType, reg, or reg, shiftType, #immediateValue
 *
 * PARAM: char *reg, char *shiftName, char *shiftV
 * reg: the charater representation of the register to be rotated.
 *    : e.g if r3 is the register then reg = 3
 * shiftName: the 3 letter charater string which determines which shift to
 * carry out from lsl, lsr, asr, ror
 * shiftV: string representing the value to shift reg by, either a constant or
 * a register.
 *
 * RETURN: uint32_t
 * Returns the binary represntation of operand2 as the 12 least most significant
 * bits.

 * The fucntion determines how to split the rest of the instruction based
 * upon the opcode input. For example and add insturction takes a destination
 * register rd, a source register rn, and an operand2 which can be a constant or
 * a register, or a shifted register. It then assembles the binary function
 * using the results from the helper functions and calls the write() method to
 * write the binary insturction to the output file.
 *
 * Note: LSL is a special case and is handled first.
 */
uint32_t encodeShiftedRegister(char *reg, char *shiftName, char *shiftV){
  uint32_t rm = 0;
  uint32_t shiftType = 0;
  uint32_t shiftValue = 0;
  uint32_t rType = 0;
  char *saveptr;
  rm = atoi(reg);
  if(shiftV != NULL){
    if(strcmp(shiftName, "lsl") == 0){
      shiftType = LSL_SHIFT;
    } else if (strcmp(shiftName, "lsr") == 0){
      shiftType = LSR_SHIFT;
    } else if (strcmp(shiftName, "asr") == 0){
      shiftType =ASR_SHIFT;
    } else if (strcmp(shiftName, "ror") == 0){
      shiftType = ROR_SHIFT;
    } else {
      perror("ERROR! Invalid shiftType entered.\n");
    }
    if(shiftV[0] == '#'){
      shiftValue = encodeImmediateOperand(shiftV);
      rType = INTEGER_ROTATION;
    } else {
      rType = REGISTER_ROTATION;
      shiftValue = atoi(strtok_r(shiftV, "r,", &saveptr));
      shiftValue = (shiftValue << 1);
    }
  } else{
        perror("ERROR! Incorrect shift value entered");
  }
  return rm + (shiftType << SHIFT_TYPE_SHIFT)
            + (shiftValue << SHIFT_VALUE_SHIFT)
            + (rType << ROTATION_TYPE_SHIFT);
}

/* Function that encodes an immediateValue as an 8 bit value and a 4 bit shift.
 * Utilises a helped function if the value is greater than 255.
 *
 * PARAM: char value[]
 * value: The string representation of the immediate value to be encoded.
 *
 * Return uint32_t
 * Returns the binary representation of the value and the shift.
 */
uint32_t encodeImmediateOperand(char value[]){
  value++;
  uint32_t immediateValue = 0;
  immediateValue = (uint32_t) strtol(value, NULL, 0);

  if(immediateValue >= (MAX_8_BIT_REPRESENTABLE)){
     return encodeImmediateRotation(immediateValue);
  }
  return immediateValue;
}

/* Function that encodes an immediateValue as an 8 bit value and a 4 bit shift.
 * If the value has more than 8 signifiant bits it prints an error.
 *
 * PARAM: uint32_t immediateValue
 * immediateValue: The integer value to be encoded as a 8 bit value and a shift.
 *
 * Return uint32_t
 * Returns the binary representation of the value and the shift.
 */
uint32_t encodeImmediateRotation(uint32_t immediateValue){
    uint32_t shift = 0;
    while( immediateValue % 2 == 0){
        immediateValue >>= 1;
        shift++;
    }
    if(immediateValue > MAX_8_BIT_REPRESENTABLE){
        perror("Immediate Value not representable");
        return(EXIT_FAILURE);
    }
    if(shift % 2 == 1){
        immediateValue <<= 1;
    }
    return ((MAX_ROTATION -shift/2) << ROTATION_SHIFT) + immediateValue;
}
