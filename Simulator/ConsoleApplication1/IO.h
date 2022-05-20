# pragma once
# ifndef IO_H
    # define max_irq2_line 20
    # include <stdio.h>
    int timer(int ioreg[]);
    int irq(int ioreg[], int *pc, int is_task);
    int check_interrupts(int ioreg[], int *pc, int *is_task, int irq2[]);
    int add_irq2(int *irq2in);
# endif
// ! add comments