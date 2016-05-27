/*
 * armStructure.h
 *
 *  Created on: 27 May 2016
 *      Author: marta
 */

#ifndef ARMSTRUCTURE_H_
#define ARMSTRUCTURE_H_

#define SIZE_OF_MEMORY 65536
#define NUMBER_OF_REGISTERS 17
#define PC 15
#define CPSR 16

struct State {
	uint8_t memory[SIZE_OF_MEMORY];
	int registers[NUMBER_OF_REGISTERS];
};

struct State ARM;

#endif /* ARMSTRUCTURE_H_ */
