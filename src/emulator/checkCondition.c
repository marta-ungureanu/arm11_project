/* ARM Project 2016
 *
 * checkCondition.c contains a function that checks if the condition holds
 * when an instruction is executed
 *
 * Group 3
 * Members: abp14, oc1115, mu515, mz4715
 */

#include "emulator_misc.h"

/* Function that returns true if the condition field of an instruction succeeds,
 * and false otherwise
 *
 * PARAM: uint32_t instruction
 * 32 bit binary representation of the instruction to be checked
 *
 * RETURN: bool
 */
bool checkConditionField(uint32_t instruction) {
	uint8_t condition = instruction >> 28;
	uint32_t cprs = ARM.registers[CPSR];
	uint32_t flagV = (cprs >> 28) % 2;
	uint32_t flagZ = (cprs >> 30) % 2;
	uint32_t flagN = cprs >> 31;

	return  (condition == EQUAL && flagZ == BIT_SET) ||
			(condition == NOT_EQUAL && flagZ == BIT_NOT_SET) ||
			(condition == GREATER_OR_EQUAL && flagN == flagV) ||
			(condition == LESS_THAN && flagN != flagV) ||
			(condition == GREATER_THAN && flagZ == BIT_NOT_SET &&
					flagN == flagV) ||
			(condition == LESS_THAN_OR_EQUAL &&
					(flagZ == BIT_SET || flagN != flagV)) ||
			(condition == ALWAYS);
}
