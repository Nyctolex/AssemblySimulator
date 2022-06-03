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
int irq(int ioreg[], int* pc, int *is_task)
    {
        ioreg[irqreturn] = pc;
        pc = ioreg[irqhandler];
        is_task = 1;
    }
void IO_handler(int ioreg[], int monitor_arr[], char disk_memory[][MAX_DISK_LINE], int* pc, int* is_task, int irq2[], int *disk_cycle, char memory[LINES_MAX][LINES_MAX_SIZE], int *led, FILE *leds_file, FILE *display7seg_file)
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
        if (is_task != 1) // if in task
            if ((ioreg[irq0enable] && ioreg[irq0status]) || (ioreg[irq1enable] && ioreg[irq1status]) || (ioreg[irq2enable] && ioreg[irq2status]))
                irq(ioreg, pc, is_task);
        monitor(monitor_arr, ioreg);
        disk_command(ioreg, disk_memory, disk_cycle, memory);
        led_write(ioreg, led, leds_file, pc);
        display7seg_write(display7seg_file, ioreg, pc);
    }

void add_irq2(FILE* irq2in, int* irq2) // create array of line numbers of irq2 interrupts
    {
        char line[max_irq2_line];
        int i = 0;
        while (fgets(line, max_irq2_line, irq2in))
            {
                irq2[i++] = atoi(line);
            }
        irq2[i] = '-1';
    }
void monitor(int monitor_arr[], int ioreg[]) // print to monitor
    {
        if (ioreg[monitorcmd] == 1)
            {
                monitor_arr[ioreg[monitoraddr]] = ioreg[monitordata];
                ioreg[monitorcmd] = 0;
            }
    }
void disk_command(int ioreg[], char disk_memory[][MAX_DISK_LINE], int *disk_cycle, char memory[LINES_MAX][LINES_MAX_SIZE]) // write to or read from disk
    {
        int is_full = 0;
        int line_index = -1;
        if ((ioreg[diskcmd] != 0) && (disk_cycle == 0)) // if there is a disk cmd and the disk is available
            {
                disk_cycle = 1024;
                if (ioreg[diskstatus] == 0) // if disk is not busy
                    {
                        if (ioreg[diskcmd] == 1) // read sector
                            {
                                for (int i=1000; i<LINES_MAX; i++) // for every memory line greater than 1000
                                    {
                                        is_full = 0;
                                        for (int j=0; j<LINES_MAX_SIZE; j++) // for every element in memory line
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
                                strcpy(memory[line_index], disk_memory[disksector*SECTOR_SIZE+diskbuffer]); // read from disk
                            }
                        else if (ioreg[diskcmd] == 2) // write sector
                            {
                                strcpy(disk_memory[disksector*SECTOR_SIZE+diskbuffer], memory[line_index]); // write to disk
                            }
                    }
            }
        if ((disk_cycle != 1) && (disk_cycle != 1024)) disk_cycle--; // if the disk is not available decrease 1 from cycles until available
        if (disk_cycle == 1) // declare the disk as available next cycle
            {
                ioreg[diskcmd] = 0;
                ioreg[diskstatus] = 0;
                ioreg[irq1status] = 1;
                disk_cycle--;
            }
    }
void led_write(int ioreg[], int *led, FILE *leds_file, int *pc)
    {
        if (ioreg[leds] != ioreg[23])
            {
                ioreg[23] = ioreg[leds];
                fprintf(leds_file, "%d %08X\n", *pc, ioreg[23]); // write to leds
            }
    }
void hwregtrace_write(FILE *fp, int cycle, int read_write, int reg_num, int data) // write hwregtrace file
    {
        unsigned int data_unsigned = (unsigned) data;
        char action[6] = {0};
        char reg_name[50] = {0};
        if (read_write == 0) sprintf(action, "READ");
        else if (read_write == 1) sprintf(action, "WRITE");
        switch(reg_num)
            {
                case irq0enable:
                    sprintf(reg_name, "irq0enable");
                    break;
                case irq1enable:
                    sprintf(reg_name, "irq1enable");
                    break;
                case irq2enable:
                    sprintf(reg_name, "irq2enable");
                    break;
                case irq0status:
                    sprintf(reg_name, "irq0status");
                    break;
                case irq1status:
                    sprintf(reg_name, "irq1status");
                    break;
                case irq2status:
                    sprintf(reg_name, "irq2status");
                    break;
                case irqhandler:
                    sprintf(reg_name, "irqhandler");
                    break;
                case irqreturn:
                    sprintf(reg_name, "irqreturn");
                    break;
                case clks:
                    sprintf(reg_name, "clks");
                    break;
                case leds:
                    sprintf(reg_name, "leds");
                    break;
                case display7seg:
                    sprintf(reg_name, "display7seg");
                    break;
                case timerenable:
                    sprintf(reg_name, "timerenable");
                    break;
                case timercurrent:
                    sprintf(reg_name, "timercurrent");
                    break;
                case timermax:
                    sprintf(reg_name, "timermax");
                    break;
                case diskcmd:
                    sprintf(reg_name, "diskcmd");
                    break;
                case disksector:
                    sprintf(reg_name, "disksector");
                    break;
                case diskbuffer:
                    sprintf(reg_name, "diskbuffer");
                    break;
                case diskstatus:
                    sprintf(reg_name, "diskstatus");
                    break;
                case reserved1:
                    sprintf(reg_name, "reserved");
                    break;
                case reserved2:
                    sprintf(reg_name, "reserved");
                    break;
                case monitoraddr:
                    sprintf(reg_name, "monitoraddr");
                    break;
                case monitordata:
                    sprintf(reg_name, "monitordata");
                    break;
                case monitorcmd:
                    sprintf(reg_name, "monitorcmd");
                    break;
            }
        fprintf(fp, "%d %s %s %08X\n", cycle-1, action, reg_name, data_unsigned); // print to file
    }
void display7seg_write(FILE *display7seg_file, int ioreg[], int *pc) // write to display7seg file
    {
        if (ioreg[display7seg] != ioreg[24])
            {
                printf("%d %X\n", ioreg[display7seg], ioreg[display7seg]);
                fprintf(display7seg_file, "%d %08X\n", *pc, ioreg[display7seg]); // print to file
                ioreg[24] = ioreg[display7seg];
            }
    }
