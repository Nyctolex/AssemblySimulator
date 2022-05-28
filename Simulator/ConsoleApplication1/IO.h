# pragma once
# ifndef _IO_H
# define _IO_H


# include <stdio.h>
# include <stdlib.h>
# define max_irq2_line 20
# define MAX_DISK_LINE 10
# define SECTOR_AMOUNT 128
# define SECTOR_SIZE 128
# define LINES_MAX 4096 // ! make sure
# define LINES_MAX_SIZE 10 // ! make sure

    #define zero_reg 0
	#define imm_reg  1
	#define v0_reg  2
	#define a0_reg  3
	#define a1_reg  4
	#define a2_reg  5
	#define a3_reg  6
	#define t0_reg  7
	#define t1_reg  8
	#define t2_reg  9
	#define s0_reg  10
	#define s1_reg  11
	#define s2_reg  12
	#define gp_reg  13
	#define sp_reg  14
	#define ra_reg  15
    
    # define irq0enable 0
    # define irq1enable 1
    # define irq2enable 2
    # define irq0status 3
    # define irq1status 4
    # define irq2status 5
    # define irqhandler 6
    # define irqreturn 7
    # define clks 8
    # define leds 9
    # define display7seg 10
    # define timerenable 11
    # define timercurrent 12
    # define timermax 13
    # define diskcmd 14
    # define disksector 15
    # define diskbuffer 16
    # define diskstatus 17
    # define reserved 18
    # define reserved 19
    # define monitoraddr 20
    # define monitordata 21
    # define monitorcmd 22

int timer(int ioreg[]);
int irq(int ioreg[], int* pc, int is_task);
void IO_handler(int ioreg[], int monitor_arr[], char disk_memory[][MAX_DISK_LINE], int* pc, int* is_task, int irq2[]);
void add_irq2(FILE* irq2in, int* irq2);
void monitor(int monitor_arr[], int ioreg[]);
void disk_command(int ioreg[], char disk_memory[][MAX_DISK_LINE], int *disk_cycle, char memory[LINES_MAX][LINES_MAX_SIZE]);
void led_write(int ioreg[], int *led, FILE *leds_file, int *pc);
void hwregtrace_write(FILE *fp, int *pc, int read_write, int reg_num, int data);
void display7seg_write(FILE *display7seg_file, int ioreg[], int *pc);

# endif
// ! add comments