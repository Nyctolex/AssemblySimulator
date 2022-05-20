# pragma once
# ifndef IO_H
    # define max_irq2_line 20
    # include <stdio.h>
    # include <stdlib.h>
    int timer(int ioreg[]);
    int irq(int ioreg[], int *pc, int is_task);
    int check_interrupts(int ioreg[], int *pc, int *is_task, int irq2[]);
    void add_irq2(FILE *irq2in, int *irq2)
# endif
// ! add comments