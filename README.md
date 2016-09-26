# C ARM11 Project: Binary Alarm Clock on Raspberry Pi

Team members:
* Oana Ciocioman
* Andrew Pearcy
* Marta Ungureanu
* Maurizio Zen

## Overview
A group project part of the First Year Computing Laboratory exercise at Imperial College London. It consists of 3 parts:

1. Implementing an ARM emulator in C, a program that simulates the execution of an ARM binary file on a Raspberry Pi.
2. Implementing an ARM assembler in C, a program that translates an ARM assembly source file into a binary file that can subsequently 
be executed by the emulator.
3. Writing an ARM assembly program that flashes an LED on a provided Raspberry Pi.

We also implemented an extension in C, a binary alarm clock, using the WiringPI library to manipulate the GPIO pins of the 
Raspberry Pi.

## Files
* `./doc/` - contains the project reports as well as the slides created for the final presentation
* `./programs/` - contains the assembly program for part 3, `gpio.s`, and the binary file produced using the assembler we implemented in 
part 2, `kernel.img`
* `./src/` - contains the source code for the emulator, assembler and binary alarm clock


