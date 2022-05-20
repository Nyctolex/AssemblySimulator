#pragma once 
#ifndef _SIMULATOR_H
#define _SIMULATOR_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instruction.h"

#define ZERO_REG 0
#define SP_REG 14

#define INST_SIZE 12

#define LINE_SIZE 5
#define NUM_REGS 16
#define NUM_IOREGS 23
#define MAX_LINES 4096
#define NUM_SECTORS 128
#define SECTOR_SIZE 512
#define LINE_MAX_SIZE 10

#define NUM_COMMANDLINE_PARAMETERS 14
// The order of the argument files
#define MEMIN 1
#define DISKIN 2
#define IRQ2IN 3
#define MEMOUT 4
#define REGOUT 5
#define trace 6
#define HWREGTRACE 7
#define CYCLES 8
#define LEDS 9
#define DISPLAY7SEG 10
#define DISKOUT 11
#define MONITOR_TXT 12
#define MONITOR_YUV 13


void write_regout(FILE* fp_regout, int* reg);
void run_instructions(Instruction* instructions, int* reg, int* ioreg, int clk_cycle, FILE* fp_trace);
void close_pf(FILE** file_pointers[], int argc);
void print_reg_state(int pc, int* reg, Instruction* inst);
void write_cycles(FILE* fp_cycles, int cycles);
void write_trace(FILE* fp_trace, int pc, Instruction* inst, int* reg);
void get_instructions(FILE* fp_memin, Instruction* head);

#endif