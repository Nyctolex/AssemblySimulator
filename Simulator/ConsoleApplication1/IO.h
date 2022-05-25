# pragma once
# ifndef _IO_H
# define _IO_H


# include <stdio.h>
# include <stdlib.h>
# define max_irq2_line 20
# define MAX_DISK_LINE 10
# define SECTOR_AMOUNT 128
# define SECTOR_SIZE 128

int timer(int ioreg[]);
int irq(int ioreg[], int* pc, int is_task);
void IO_handler(int ioreg[], int monitor_arr[], char disk_memory[][MAX_DISK_LINE], int* pc, int* is_task, int irq2[]);
void add_irq2(FILE* irq2in, int* irq2);
void monitor(int monitor_arr[], int ioreg[]);
# endif
// ! add comments