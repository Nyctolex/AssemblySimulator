#pragma once 
#ifndef _SIMULATOR_H
#define _SIMULATOR_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instruction.h"
#include "IO.h"

#define ZERO_REG 0
#define SP_REG 14
#define V0_REG 2
#define A0_REG 3
#define A1_REG 4
#define IRQRETURN_REG 7
#define CLK_REG 8
#define LEDS_REG 9

#define INST_SIZE 12
#define MONITOR_SIZE 256

#define LINE_SIZE 5
#define NUM_REGS 16
#define NUM_IOREGS 23
#define MAX_LINES 4096
#define NUM_SECTORS 128
#define NUM_SECTOR_LINES 128
#define MAX_DISK_LINE_LEN 10
#define LINE_MAX_SIZE 10

#define NUM_COMMANDLINE_PARAMETERS 14
// The order of the argument files
#define MEMIN 1
#define DISKIN 2
#define IRQ2IN 3
#define MEMOUT 4
#define REGOUT 5
#define TRACE 6
#define HWREGTRACE 7
#define CYCLES 8
#define LEDS 9
#define DISPLAY7SEG 10
#define DISKOUT 11
#define MONITOR_TXT 12
#define MONITOR_YUV 13
/**
 * @brief converting a line in the memory to an Instruction object.
 * 
 * @param pc the pc of the line in the memory (its location)
 * @param memory a pointer to the memory matrix
 * @return Instruction* the line in the memory as an instruction object
 */
Instruction* read_instruction(int pc, char memory[][LINE_MAX_SIZE]);
/**
 * @brief reseting all lines in the memory matrix to 00000
 * 
 * @param memory a pointer to the memory matrix
 */
void reset_memory(char memory[][LINE_MAX_SIZE]);
/**
 * @brief reseting all lines in the memory matrix to 00000
 * 
 * @param disk_memory a pointer to the disk memory matrix
 */
void reset_disk_memory(char disk_memory[][MAX_DISK_LINE_LEN]);
/**
 * @brief loading the image of the memory in the input file into the disk memory matrix
 * 
 * @param fp_diskin the input file
 * @param disk_memory a pointer to the disk memory matrix
 */
void read_disk_memory(FILE* fp_diskin, char disk_memory[][MAX_DISK_LINE_LEN]);
/**
 * @brief writing all data in the disk memory matrix into the output file at the end of the simulation.
 * 
 * @param fp_diskout the output file
 * @param disk_memory a pointer to the disk memory matrix
 */
void write_diskout(FILE* fp_diskout, char disk_memory[][MAX_DISK_LINE_LEN]);
/**
 * @brief writing the values of the registers to the output file at the end  of the simulation.
 * 
 * @param fp_regout the output file
 * @param reg the array of the registers
 */
void write_regout(FILE* fp_regout, int* reg);
/**
 * @brief running the instructions and simulating the assembly program after all needed files were loaded.
 * 
 * @param regs the array of the register of the proccessor
 * @param ioreg the array representing the hardware register for io devices communication
 * @param file_pointers a pointer to the array of the files pointers
 * @param memory a pointer to the memory matrix
 * @param is_in_task a pointer to the varible saving if the assembler is bussy with a task
 * @param irq2 a pointer to the array of the irq2 cycles
 * @param monitor a pointer to the monitor buffer
 * @param disk_memory a ponter to the disk memory matrix
 * @param disk_cycle_ptr a pointer to the disk cycle parameter
 */
void run_instructions(int regs[NUM_REGS], int* ioreg, FILE** file_pointers[], char memory[][LINE_MAX_SIZE], int* is_in_task, int irq2[], int monitor[], char disk_memory[][MAX_DISK_LINE_LEN], int* disk_cycle_ptr);
/**
 * @brief decodes the current instruction and executring it
 * 
 * @param regs the array of the register of the proccessor
 * @param ioreg the array representing the hardware register for io devices communication
 * @param inst the current instruction
 * @param memory a pointer to the memory matrix
 * @param pc_pointer a pointer to the varible saving the value of the current pc
 * @param is_task a pointer to the varible saving if the assembler is bussy with a task
 * @param irq2 a pointer to the array of the irq2 cycles
 */
void decode_inst(int* regs, int* ioreg, Instruction* inst, char memory[][LINE_MAX_SIZE], int* pc_pointer, int* is_task, int irq2[]);
/**
 * @brief closes all the folder we opended during the running of the program
 * 
 * @param file_pointers a pointer to the array of the files pointers
 * @param argc 
 */
void close_pf(FILE** file_pointers[], int argc);
/**
 * @brief printing the state of the registers at the moment. 
 * This function is used for logging and debugging purposes. 
 * @param pc the current pc value
 * @param reg the array of the register of the proccessor
 * @param inst the current instruction
 */
void print_reg_state(int pc, int* reg, Instruction* inst);
/**
 * @brief writing to the output file the value of the amount of cycles of the program
 *  after it was executed.
 * @param fp_cycles the output file
 * @param cycles the number of cycles.
 */
void write_cycles(FILE* fp_cycles, int* cycles);
/**
 * @brief writing the trace of the regesters to the output file after executing each assembly instruction
 * 
 * @param fp_trace a pointer to the output file
 * @param pc the value of the current pc
 * @param inst the current instruction
 * @param regs a pointer to the array of the registers
 */
void write_trace(FILE* fp_trace, int pc, Instruction* inst, int* regs);
/**
 * @brief reading the memory in the input folder and coping it to the memory matrix
 * 
 * @param fp_memin the input file
 * @param memory a pointer to the memory matrix
 */
void read_memory(FILE* fp_memin, char memory[][LINE_MAX_SIZE]);
/**
 * @brief writing an image of the memory at the end of the simulation to the output file
 * 
 * @param fp_memout a pointer to the output file 
 * @param memory  pointer to the memory matrix
 */
void write_memout(FILE* fp_memout, char memory[][LINE_MAX_SIZE]);
/**
 * @brief writing an image of the monitor at the end of the simulation to the output file
 * 
 * @param fp_monitor_txt a pointer to the output file 
 * @param monitor pointer to the monitor array
 */
void write_monitor_txt(FILE* fp_monitor_txt, int monitor[MONITOR_SIZE * MONITOR_SIZE]);
/**
 * @brief writing an image of the monitor at the end of the simulation to the output file in binary yuv format
 * 
 * @param fp_monitor_yuv a pointer to the output file 
 * @param monitor  pointer to the monitor array
 */
void write_monitor_yuv(FILE* fp_monitor_yuv, int monitor[MONITOR_SIZE * MONITOR_SIZE]);

/**
 * @brief updating the cycle and calling the IO_handler
 * 
 * @param ioreg the array representing the hardware register for io devices communication
 * @param monitor a pointer to the monitor array
 * @param disk_memory a pointer to the disk memory matrix
 * @param pc_pointer a pointer to the varible storing the current pc value
 * @param is_in_task a pointer to the varible saving if the assembler is bussy with a task
 * @param irq2 a pointer to the array of the irq2 cycles
 * @param memory a pointer to the memory matrix
 * @param file_pointers a pointer to the array of the files pointers
 * @param disk_cycle_ptr a pointer to the disk cycle parameter
 */
void next_cycle( int* ioreg, int monitor[], char disk_memory[][MAX_DISK_LINE_LEN], int* pc_pointer, int* is_in_task, int irq2[], char memory[][LINE_MAX_SIZE], FILE** file_pointers[], int* disk_cycle_ptr);
#define next_clk next_cycle(ioreg, monitor, disk_memory, pc_pointer, is_in_task, irq2, memory, file_pointers, disk_cycle_ptr)
#endif