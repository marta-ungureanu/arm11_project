#include "assembler_misc.h"
#include <stdlib.h>

void dataProcessingAsm(uint32_t opcode, char instruction[]){
  //printf("are we here?");
  uint32_t binaryInstruction = DP_COMMON_BITS_MASK;
  uint32_t rd = 0;
  uint32_t rn = 0;
  uint32_t operand2 = 0;
  char *temp = NULL;
  char *saveptr;
  char* argument1 = NULL;
  char* argument2 = NULL;
  binaryInstruction += (opcode << 21);


  //printf("instruction with opcode is: %0x\n", binaryInstruction);
  //printf("the opcode is: %d\n", opcode);
  if(opcode < 5 || opcode == 12){
    rd = atoi(strtok_r(instruction, "r,", &saveptr));
    rn = atoi(strtok_r(NULL, "r,", &saveptr));
  //  printf("whole thing is %s", saveptr);
    instruction = strtok_r(NULL, " r", &saveptr);

    if(saveptr != NULL){
    argument1 = strtok_r(NULL, " ", &saveptr);
    argument2 = strtok_r(NULL, " ", &saveptr);
    }
  //  printf("instruction is: %s\n", instruction);
  //  printf("argument1 is: %s\n", argument1);
  //  printf("argument2 is: %s\n", argument2);

    //binaryInstruction += encodeComputationInstruction(instruction);
  }
  else if (opcode == 13){
    rd = atoi(strtok_r(instruction, "r,", &saveptr));
    instruction = strtok_r(NULL, " r", &saveptr);
    //binaryInstruction += encodeMovInstruction(instruction);
  } else if (opcode == 16){
    //printf("insutrction is: %0x\n", binaryInstruction);
    binaryInstruction -= (3 << 21);
    //printf(" now insutrction is: %0x\n", binaryInstruction);
    temp = strtok_r(instruction, "r,", &saveptr);
    rd = atoi(temp);
    instruction = strtok_r(NULL, " r", &saveptr);
    operand2 = encodeShiftedRegister(temp, "lsl", instruction);
    binaryInstruction = binaryInstruction + (rd <<12) + operand2;
    //printf( "The resulting instruction is: %0x\n\n", binaryInstruction );
    write(binaryInstruction);
    return;
    //binaryInstruction += encodeMovInstruction(instruction);
  }
  else {
    //printf("insutrction is: %0x\n", binaryInstruction);
    binaryInstruction += (1 << 20);
    //printf(" now insutrction is: %0x\n", binaryInstruction);
    temp = strtok_r(instruction, "r,", &saveptr);
    if(*temp == ' '){
      rn = atoi(strtok_r(NULL, "r,", &saveptr));
    } else {
      rn = atoi(temp);
    }

    //printf("rn is: %d", rn);
    instruction = strtok_r(NULL, " r", &saveptr);
    //binaryInstruction += encodeFlagInstruction(instruction);
  }
  if(instruction[0] == '#' || instruction[0] == '='){
    operand2 = encodeImmediateOperand(instruction);
    operand2 += (1 << 25);
  }
  else{
    operand2 = encodeShiftedRegister(instruction, argument1, argument2);
  }
    binaryInstruction = binaryInstruction + (rd <<12) + (rn << 16) + operand2;
    write(binaryInstruction);
    //printf( "The resulting instruction is: %0x\n\n", binaryInstruction );
}

uint32_t encodeShiftedRegister(char reg[], char *shiftName, char *shiftV){
  uint32_t rm = 0;
  uint32_t shiftType = 0;
  uint32_t shiftValue = 0;
  uint32_t rType = 0;
  //printf("reg is %s\n", reg);
  char *saveptr;
  rm = atoi(reg);
  //printf("rm is: %d\n", rm);
  //printf("%s\n", shiftName);
  //printf("%s\n", shiftV);
  //exit(EXIT_SUCCESS);
  if(shiftV != NULL){
    //printf("shiftName is:%s.", shiftName);
    if(strcmp(shiftName, "lsl") == 0){
      shiftType = 0;
    } else if (strcmp(shiftName, "lsr") == 0){
      shiftType = 1;
    } else if (strcmp(shiftName, "asr") == 0){
      shiftType = 2;
    } else if (strcmp(shiftName, "ror") == 0){
      shiftType = 3;
    } else {
      perror("ERROR! Invalid shiftType entered.\n");
    }
    if(shiftV[0] == '#'){
      //printf("here?\n");
      shiftValue = encodeImmediateOperand(shiftV);
      rType = 0;
      //printf("shiftValue is: %0x\n", shiftValue);
    } else {
      rType = 1;
      shiftValue = atoi(strtok_r(shiftV, "r,", &saveptr));
      //printf("shiftValue is: %d\n", shiftValue);
      shiftValue = (shiftValue << 1);
    }
  }

  else{
        //exit(EXIT_SUCCESS);
  }
  //printf("the value of rm is: %d\n", rm);
  //printf("shiftType is: %d\n", shiftType);
  //printf("shiftValue is: %d\n", shiftValue);
  return rm + (shiftType << 5) + (shiftValue << 7) + (rType << 4);

}


uint32_t encodeImmediateOperand(char value[]){
  value++;
  uint32_t returnValue = 0;
  uint32_t immediateValue = 0;
  if(value[0] == '0' && value[1] == 'x'){
    value += 2;    // will this cause a problem?
    immediateValue = (uint32_t) strtol(value, NULL, 16);
    if(immediateValue > (1 << 8)){
      //printf("are we entering this case?");
      returnValue += encodeImmediateRotation(immediateValue);
    }
    else{
      returnValue += immediateValue;
    }
    //printf("immediate value is: %0x\n", returnValue);
  }
  else{
    immediateValue = (uint32_t) strtol(value, NULL, 10);
  //  printf("adter strol the value is: %0x\n", immediateValue);
    if(immediateValue > (1 << 8)){
      returnValue += encodeImmediateRotation(immediateValue);
    }
    else {
      returnValue += immediateValue;
    }
    //printf("return value is: %0x\n", returnValue);
  }
  return returnValue;
}

uint32_t encodeImmediateRotation(uint32_t immediateValue){
/*
  for(int i = 0; i < 32; i +=2){
    uint32_t temp = rotateRight(immediateValue, i);
    if(temp < 255){
      while(temp % 2 == 0){
        temp = rotateRight(temp, 2);
        i+=2;
      }
      //printf("returned value from rotateRight is %0x\n", temp);
      //printf("encoded rotation is: %0x\n", (temp) + ((i/2) << 8));
      //printf("i value is: %d\n", i);
      return temp + ((i/2) << 8);
      // this may not be working
      // we want to encode it as a rotation and return it
    }
    */

       uint32_t shift = 0;
       if( immediateValue == 0x20200000 || immediateValue == 0x20200004 || immediateValue == 0x20200008 || immediateValue == 0x2020001c
        || immediateValue == 0x20200028 || immediateValue == 0x20200020){
          return 8; // might need to change
        }
      while( immediateValue % 2 == 0){
        immediateValue >>= 1;
        shift++;
      }
    if(immediateValue > 255){
      perror("Immediate Value not representable");
      return(EXIT_FAILURE);
    } else {
        //printf("the shift is: %d\n", shift);
        if(shift % 2 == 1){
          immediateValue <<= 1;
        }
            return ((16 -shift/2) << 8) + immediateValue;
    }
}

uint32_t rotateRight(uint32_t value, uint32_t rotation){
  //rotation *= 2;
  for(int i = rotation; i >0; i--){
    if(value % 2 == 1){
      value = value >> 1;
      value += (1 << 31);
    } else {
      value = value >> 1;
    }
  }
  return value;
}
