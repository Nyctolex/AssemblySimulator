#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INST_SIZE 12

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