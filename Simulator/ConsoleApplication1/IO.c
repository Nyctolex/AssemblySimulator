# include "IO.h"

int timer(int ioreg[])
    {
        if (++ioreg[timercurrent] == ioreg[timermax]) // check if the timer value is the maximal
        {
            ioreg[irq0status] = 1; // activate irqstatus0
            ioreg[timercurrent] = 0; // zero timer_current
        }
        return;
    }
int irq(int ioreg[], int* pc, int is_task)
    {
        ioreg[irqreturn] = pc;
        pc = ioreg[irqhandler];
        is_task = 1;
    }
void IO_handler(int ioreg[], int monitor_arr[], char disk_memory[][MAX_DISK_LINE], int* pc, int* is_task, int irq2[], int *disk_cycle, char memory[LINES_MAX][LINES_MAX_SIZE], int *led, FILE *leds_file)
    {
        if (ioreg[timerenable] == 1) // if the timer is enabled
            timer(ioreg); // update processor time
        for (int i = 0; irq2[i] != '\0'; i++)
            {
                ioreg[irq2enable] = 0;
                if (irq2[i] == pc)
                    {
                        ioreg[irq2enable] = 1;
                        break;
                    }
            }
        if (is_task != 1)
            if ((ioreg[irq0enable] && ioreg[irq0status]) || (ioreg[irq1enable] && ioreg[irq1status]) || (ioreg[irq2enable] && ioreg[irq2status]))
                irq(ioreg, pc, is_task);
        monitor(monitor_arr, ioreg);
        disk_command(ioreg, disk_memory, disk_cycle, memory);
        led_write(ioreg, led, leds_file, pc);
    }

void add_irq2(FILE* irq2in, int* irq2)
    {
        char line[max_irq2_line];
        int i = 0;
        while (fgets(line, max_irq2_line, irq2in))
            {
                irq2[i++] = atoi(line);
            }
        irq2[i] = '-1';
    }
void monitor(int monitor_arr[], int ioreg[])
    {
        if (ioreg[monitorcmd] == 1)
            {
                monitor_arr[ioreg[monitoraddr]] = ioreg[monitordata];
                ioreg[monitorcmd] = 0;
            }
    }
void disk_command(int ioreg[], char disk_memory[][MAX_DISK_LINE], int *disk_cycle, char memory[LINES_MAX][LINES_MAX_SIZE])
    {
        int is_full = 0;
        int line_index = -1;
        if ((ioreg[diskcmd] != 0) && (disk_cycle == 0)) // if there is a disk cmd
            {
                disk_cycle = 1024;
                if (ioreg[diskstatus] == 0) // if disk is not busy
                    {
                        if (ioreg[diskcmd] == 1) // read sector
                            {
                                for (int i=1000; i<LINES_MAX; i++) // for every memory line greater than 1000
                                    {
                                        is_full = 0;
                                        for (int j=0; j<LINES_MAX_SIZE) // for every element in memory line
                                            {
                                                if (memory[i][j] != 0) // check if an element is occupied
                                                    {
                                                        is_full = 1;
                                                        break;
                                                    }
                                            }
                                        if (is_full == 1) continue;
                                        else if (is_full == 1) line_index = i;
                                    }
                                strcopy(memory[line_index], disk_memory[disksector*SECTOR_SIZE+diskbuffer]);
                            }
                        else if (ioreg[diskcmd] == 2) // write sector
                            {
                                strcopy(disk_memory[disksector*SECTOR_SIZE+diskbuffer], memory[line_index]);
                            }
                    }
            }
        if ((disk_cycle != 1) && (disk_cycle != 1024)) disk_cycle--;
        if (disk_cycle == 1)
            {
                ioreg[diskcmd] = 0;
                ioreg[diskstatus] = 0;
                ioreg[irq1status] = 1;
                disk_cycle--;
            }
    }
void led_write(int ioreg[], int *led, FILE *leds_file, int *pc)
    {
        if (ioreg[leds] != led)
            {
                led = ioreg[leds];
                fptus("%d %08X\n", pc, atoi(led));
            }
    }