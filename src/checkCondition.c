#include "emulator_misc.h"
#include "armStructure.h"

/* function that returns true if the condition field of an instruction succeeds,
 * and false otherwise
 */
bool checkConditionField(uint32_t instruction) {
	uint8_t condition = instruction >> 28;
	uint32_t cprs = ARM.registers[CPSR];
	uint32_t flagV = (cprs >> 28) % 2;
	uint32_t flagZ = (cprs >> 30) % 2;
	uint32_t flagN = cprs >> 31;

	return  (condition == 0 && flagZ == 1) ||
			(condition == 1 && flagZ == 0) ||
			(condition == 10 && flagN == flagV) ||
			(condition == 11 && flagN != flagV) ||
			(condition == 12 && flagZ == 0 && flagN == flagV) ||
			(condition == 13 && (flagZ == 1 || flagN != flagV)) ||
			(condition == 14);
}
