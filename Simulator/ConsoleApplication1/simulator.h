#pragma once 
#ifndef _SIMULATOR_H
#define _SIMULATOR_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instruction.h"
#include "IO.h"

#define FP_TRACE 5
#define FP_HWREGTRACE 6
#define FP_LED 8
#define FP_DISPLAY7SEG 9

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


Instruction* read_instruction(int pc, char memory[][LINE_MAX_SIZE]);
void reset_memory(char memory[][LINE_MAX_SIZE]);
void reset_disk_memory(char disk_memory[][MAX_DISK_LINE_LEN]);
void read_disk_memory(FILE* fp_diskin, char disk_memory[][MAX_DISK_LINE_LEN]);
void write_diskout(FILE* fp_diskout, char disk_memory[][MAX_DISK_LINE_LEN]);
void write_regout(FILE* fp_regout, int* reg);
void run_instructions(int regs[NUM_REGS], int* ioreg, FILE* fp_trace, char memory[][LINE_MAX_SIZE], int* is_task, int irq2[]);
void decode_inst(int* regs, int* ioreg, Instruction* inst, char memory[][LINE_MAX_SIZE], int* pc_pointer, int* is_task, int irq2[]);
void close_pf(FILE** file_pointers[], int argc);
void print_reg_state(int pc, int* reg, Instruction* inst);
void write_cycles(FILE* fp_cycles, int* cycles);
void write_trace(FILE* fp_trace, int pc, Instruction* inst, int* regs);

void read_memory(FILE* fp_memin, char memory[][LINE_MAX_SIZE]);
//void get_instructions(FILE* fp_memin, Instruction* head, char memory[][LINE_MAX_SIZE]);
Instruction* read_instruction(int pc, char memory[][LINE_MAX_SIZE]);
void write_memout(FILE* fp_memout, char memory[][LINE_MAX_SIZE]);
void write_monitor_txt(FILE* fp_monitor_txt, int monitor[MONITOR_SIZE * MONITOR_SIZE]);
void write_monitor_yuv(FILE* fp_monitor_yuv, int monitor[MONITOR_SIZE * MONITOR_SIZE]);
void next_cycle(int* ioreg, int* pc_pointer, int* is_in_task, int irq2[]);
#define next_clk next_cycle(ioreg, monitor, disk_memory, pc_pointer, is_in_task, irq2, memory, file_pointers, disk_cycle_ptr)
#endif