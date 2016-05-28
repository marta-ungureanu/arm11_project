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
