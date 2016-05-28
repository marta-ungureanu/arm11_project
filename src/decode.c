
#include <string.h>
#include "instructions_table.h"

void decode(char line[]) {
  char temp[strlen(line)];
  strcpy(temp, line);
  char *p = strtok(temp, " ");
  int location = getLocation(p);
  int code = instructionSet[location].type;

  switch (code){
    case 1:
      multiplyAsm(instructionSet[location].opcode, temp);
      break;
    case 2:
      dataProcessingAsm(instructionSet[location].opcode, temp);
      break;
    case 3:
      singleDataTransferAsm(instructionSet[location].opcode, temp);
      break;
    case 4:
      branchAsm(instructionSet[location].opcode, temp);
      break;
  }
}
