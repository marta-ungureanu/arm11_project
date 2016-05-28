#ifndef INSTRUCTIONS_TABLE_H_
#define INSTRUCTIONS_TABLE_H_

#define NUMBER_OF_INSTRUCTIONS 23

struct entry{
  char instruction[5];
  int type;
  int opcode;
};

struct entry instructionSet[] =
 {{"add", 1, 4},
  {"sub", 1, 2},
  {"rsb", 1, 3},
  {"and", 1, 0},
  {"eor", 1, 1},
  {"orr", 1, 12},
  {"mov", 1, 13},
  {"tst", 1, 8},
  {"teq", 1, 9},
  {"cmp", 1, 10},
  {"mul", 2, 0},
  {"mla", 2, 0},
  {"ldr", 3, 0},
  {"str", 3, 0},
  {"beq", 4, 10},
  {"bne", 4, 10},
  {"bge", 4, 10},
  {"blt", 4, 10},
  {"bgt", 4, 10},
  {"ble", 4, 10},
  {"b", 4, 10},
  {"lsl", 1, 13},
  {"andeq", 1, 0}
  };

int getLocation(char c[]);

#endif /* INSTRUCTIONS_TABLE_H_ */
