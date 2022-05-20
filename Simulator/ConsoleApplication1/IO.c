# include <IO.h>
int timer(int ioreg[])
    {
        if (++ioreg[12] == ioreg[13]) // check if the timer value is the maximal
            {
                ioreg[3] = 1; // activate irqstatus0
                ioreg[12] = 0; // zero timer_current
            }
        return;
    }
int irq(int ioreg[], int *pc, int is_task)
    {
        int ioreg[7] = pc; // pc = irqreturn
        pc = ioreg[6]; // pc = irqhandler
        is_task = 1;
    }
int check_interrupts(int ioreg[], int *pc, int *is_task, int irq2[])
    {
        if (ioreg[11] == 1) // if the timer is enabled
            timer(ioreg); // update processor time
        for (int i=0; irq2[i] != '\0'; i++)
            {
                ioreg[2] = 0;
                if (irq2[i] == pc)
                    {
                        ioreg[2] = 1;
                        break;
                    }
            }
        if (is_task != 1)
            if ((ioreg[0] && ioreg[3]) || (ioreg[1] && ioreg[4]) || (ioreg[2] && ioreg[5]))
                irq(ioreg, pc, is_task);
        return;
    }
void add_irq2(FILE *irq2in, int *irq2)
    {
        char line [max_irq2_line];
        int i=0;
        while (fgets(line, max_irq2_line, irq2in))
            {
                irq2[i++] = atoi(line);
            }
        irq2[i] = '-1';
    }