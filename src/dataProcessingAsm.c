#include "assembler_misc.h"
#include <stdlib.h>

void dataProcessingAsm(uint32_t opcode, char instruction[]){
  //printf("are we here?");
  uint32_t binaryInstruction = DP_COMMON_BITS_MASK;
  uint32_t rd = 0;
  uint32_t rn = 0;
  uint32_t operand2 = 0;
  char *saveptr;
  char* argument1 = NULL;
  char* argument2 = NULL;
  binaryInstruction += (opcode << 21);


  //printf("instruction with opcode is: %0x\n", binaryInstruction);
  //printf("the opcode is: %d\n", opcode);


  if(opcode < 5 || opcode == 9){
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
  }
  else {
    binaryInstruction += (1 << 20);
    rn = atoi(strtok_r(instruction, "r,", &saveptr));
    instruction = strtok_r(NULL, " r", &saveptr);
    //binaryInstruction += encodeFlagInstruction(instruction);
  }
  if(instruction[0] == '#'){
    operand2 = encodeImmediateOperand(instruction);
  }
  else{
    operand2 = encodeShiftedRegister(instruction, argument1, argument2);
  }
    binaryInstruction = binaryInstruction + (rd <<12) + (rn << 16) + operand2;
    write(binaryInstruction);
  printf( "The resulting instruction is: %0x\n\n", binaryInstruction );
}

uint32_t encodeShiftedRegister(char reg[], char *shiftName, char *shiftV){
  uint32_t rm = 0;
  uint32_t shiftType = 0;
  uint32_t shiftValue = 0;
  uint32_t rType = 0;
  printf("reg is %s\n", reg);
  char *saveptr;
  rm = atoi(reg);
  printf("rm is: %d\n", rm);
  //printf("%s\n", shiftName);
  //printf("%s\n", shiftV);
  //exit(EXIT_SUCCESS);
  if(shiftV != NULL){
    rType = 1;
    printf("shiftName is:%s.", shiftName);
    if(strcmp(shiftName, "lsl") == 0){
      printf("are we here?");
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
      shiftValue = encodeImmediateOperand(shiftV);
    } else {
      shiftValue = atoi(strtok_r(shiftV, "r,", &saveptr));
      //printf("shiftValue is: %d\n", shiftValue);
      //shiftValue = (shiftValue << 1);
    }
  }

  else{
        //exit(EXIT_SUCCESS);
  }
  //printf("the value of rm is: %d\n", rm);
  //printf("shiftType is: %d\n", shiftType);
  //printf("shiftValue is: %d\n", shiftValue);



/*

  printf("after the rm is removed %s\n", saveptr );
  operand2 = strtok_r(NULL, " ", &saveptr);
  printf(" once we run str again: %s", operand2);
  printf("the remaining part of operand2 is: %s\n", operand2);
  if( operand2 != NULL){
    char *shiftName = strtok_r(operand2, ", ", &saveptr);
    operand2 = strtok_r(NULL, " ", &saveptr);
    if(strcmp(shiftName, "lsl")){
      shiftType = 0;
    } else if (strcmp(shiftName, "lsr")){
      shiftType = 1;
    } else if (strcmp(shiftName, "asr")){
      shiftType = 2;
    } else if (strcmp(shiftName, "ror")){
      shiftType = 3;
    } else {
      perror("ERROR! Invalid shiftType entered.\n");
    }
    if(operand2[0] == '#'){
      shiftValue = encodeImmediateOperand(operand2);
    } else {
      shiftValue = atoi(strtok_r(operand2, "r,", &saveptr));
      printf("shiftValue is: %d\n", shiftValue);
      shiftValue = (shiftValue << 1);
    }
  }
  */
  return rm + (shiftType << 5) + (shiftValue << 8) + (rType << 4);

}


uint32_t encodeImmediateOperand(char value[]){
  value++;
  uint32_t returnValue = (1 << 25);
  uint32_t immediateValue;
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
  }

  perror("Immediate Value not representable");
  return(EXIT_FAILURE);
  //printf("code not yet written for integers large than 2^8;\n");
  return immediateValue;
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
