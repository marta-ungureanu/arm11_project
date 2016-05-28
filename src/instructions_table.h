#ifndef INSTRUCTIONS_TABLE_H_
#define INSTRUCTIONS_TABLE_H_

#define NUMBER_OF_INSTRUCTIONS 23


struct entry{
  char instruction[5];
  int type;
  int opcode;
} instructionSet[] =
 {{"add", 2, 4},
  {"sub", 2, 2},
  {"rsb", 2, 3},
  {"and", 2, 0},
  {"eor", 2, 1},
  {"orr", 2, 12},
  {"mov", 2, 13},
  {"tst", 2, 8},
  {"teq", 2, 9},
  {"cmp", 2, 10},
  {"mul", 1, 0},
  {"mla", 1, 0},
  {"ldr", 3, 0},
  {"str", 3, 0},
  {"beq", 4, 10},
  {"bne", 4, 10},
  {"bge", 4, 10},
  {"blt", 4, 10},
  {"bgt", 4, 10},
  {"ble", 4, 10},
  {"b", 4, 10},
  {"lsl", 2, 13},
  {"andeq", 2, 0}
};

int getLocation(char c[]);
void decode(char line[]);

#endif
