# pragma once
# ifndef _IO_H
#define _IO_H


# include <stdio.h>
# include <stdlib.h>
# define max_irq2_line 20

int timer(int ioreg[]);
int irq(int ioreg[], int* pc, int is_task);
void check_interrupts(int ioreg[], int* pc, int* is_task, int irq2[]);
void add_irq2(FILE* irq2in, int* irq2);
# endif
// ! add comments