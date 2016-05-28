#include "instructions_table.h"
#include <string.h>

int getLocation(char c[]){
  for(int i = 0; i < NUMBER_OF_INSTRUCTIONS; i++){
    if(strcmp(c, instructionSet[i].instruction) == 0) {
      return i;
    }
  }
  return -1;
}

void decode(char line[]) {
  char temp[strlen(line)];
  strcpy(temp, line);
  char *p = strtok(temp, " ");
  int location = getLocation(p);
  int code = instructionSet[location].type;

  switch (code){
    case 1:
      //multiplyAsm(instructionSet[location].opcode, temp);
      break;
    case 2:
      //dataProcessingAsm(instructionSet[location].opcode, temp);
      break;
    case 3:
      //singleDataTransferAsm(instructionSet[location].opcode, temp);
      break;
    case 4:
      //branchAsm(instructionSet[location].opcode, temp);
      break;
  }
}
