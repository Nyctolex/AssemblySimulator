#include "simulator.h"
#define _CRT_SECURE_NO_WARNINGS

int main(int argc, char* argv[])
{
    //allocating a pointer to each file we would use later
    FILE* fp_memin = NULL, * fp_diskin = NULL, * fp_irq2in = NULL, * fp_memout = NULL,
        * fp_regout = NULL, * fp_trace = NULL, * fp_hwregtrace = NULL, * fp_cycles = NULL, * fp_leds = NULL, * fp_display7seg = NULL,
        * fp_diskout = NULL, * fp_monitor_txt = NULL, * fp_monitor_yuv;
    //an array of all file pointers
    FILE** file_pointers[] = { NULL, &fp_memin, &fp_diskin, &fp_irq2in, &fp_memout,
                              &fp_regout, &fp_trace, &fp_hwregtrace, &fp_cycles, &fp_leds, &fp_display7seg, &fp_diskout, &fp_monitor_txt, &fp_monitor_yuv };

    //used for saving data for the irq2 handler
    int irq2[200];
    int disk_cycle = 0;
    //used for saving data for the disk handler
    int* disk_cycle_ptr = &disk_cycle;
    int is_in_task = 0;

    const int output_file_index = 4; // All file after this inex are output files
    // arrays which would represent the register of the proccessor and the io registers
    int regs[NUM_REGS] = { 0 }, ioreg[NUM_IOREGS+5] = { 0 };
    char memory[MAX_LINES][LINE_MAX_SIZE];
    //setting all lines to 000
    reset_memory(memory);
    char disk_memory[NUM_SECTORS * NUM_SECTOR_LINES][MAX_DISK_LINE_LEN];
    //setting all lines to 000
    reset_disk_memory(disk_memory);
    int monitor[MONITOR_SIZE * MONITOR_SIZE] = { 0 };
    int pc = 0, irq = 0, busy_with_interruption = 0;

    if (argc != NUM_COMMANDLINE_PARAMETERS) // check the number command line arguments
    {
        printf("Error: Incorrect command line arguments number\n");
        return 1;
    }

    int i;
    // opening the input files
    for (i = 1; i < output_file_index; i++)
    {

        *file_pointers[i] = fopen(argv[i], "r");

        if (*(file_pointers[i]) == NULL)
        {
            printf("Error: The file %s couldn't open properly", argv[i]);
            return -1;
        }
    }
    // opening the output files without yuv file
    for (i = i; i < argc - 1; i++)
    {

        *file_pointers[i] = fopen(argv[i], "w");
        if (*(file_pointers[i]) == NULL)
        {
            printf("Error: The file %s couldn't open properly", argv[i]);
            return -1;
        }
    }
    //open yuv in binary
    *file_pointers[i] = fopen(argv[i], "wb");
    if (*(file_pointers[i]) == NULL)
    {
        printf("Error: The file %s couldn't open properly", argv[i]);
        return -1;
    }

    //reading the fp_irq2in file to irq2 array
    add_irq2(fp_irq2in, irq2);
    //reading the memory from the input file into memory
    read_memory(fp_memin, memory);
    //reading the memory from the input file into disk memory
    read_disk_memory(fp_diskin, disk_memory);
    //run the instructions
    run_instructions(regs, ioreg, file_pointers, memory, &is_in_task, irq2, monitor, disk_memory, disk_cycle_ptr);
    //writing to the output files
    write_cycles(fp_cycles, ioreg[CLK_REG]);
    write_regout(fp_regout, regs);
    write_memout(fp_memout, memory);
    write_diskout(fp_diskout, disk_memory);
    write_monitor_txt(fp_monitor_txt, monitor);
    write_monitor_yuv(fp_monitor_yuv, monitor);
    //close all opened files
    close_pf(file_pointers, NUM_COMMANDLINE_PARAMETERS);
    return 0;
}

//closing all of the files we opened
void close_pf(FILE** file_pointers[], int argc)
{
    int i;
    for (i = 1; i < argc; i++)
    {
        fclose(*file_pointers[i]);
    }
}

void next_cycle(int* ioreg, int monitor[], char disk_memory[][MAX_DISK_LINE_LEN], int* pc_pointer, int* is_in_task, int irq2[], char memory[][LINE_MAX_SIZE], FILE** file_pointers[], int* disk_cycle_ptr) {
    ioreg[CLK_REG] = ioreg[CLK_REG] % 0xffffffff + 1; //update the value of the cycle counter
    int led = ioreg[LEDS_REG];
    FILE* leds_file = *file_pointers[LEDS];
    FILE* display7seg_file = *file_pointers[DISPLAY7SEG];
    IO_handler(ioreg, monitor, disk_memory, pc_pointer, is_in_task, irq2, disk_cycle_ptr, memory, led, leds_file, display7seg_file);

}

void reset_memory(char memory[][LINE_MAX_SIZE]) {
    int i = 0;
    for (i = 0; i < MAX_LINES; i++) {
        strcpy(memory[i], "00000");
    }
}

void reset_disk_memory(char disk_memory[][MAX_DISK_LINE_LEN]) {
    int i = 0;
    for (i = 0; i < NUM_SECTORS * NUM_SECTOR_LINES; i++) {
        strcpy(disk_memory[i], "00000");
    }
}

void read_memory(FILE* fp_memin, char memory[][LINE_MAX_SIZE])
{
    int pc = 0;
    int next_pc = 0;
    char curent_inst[LINE_MAX_SIZE];
    char imm_line[LINE_MAX_SIZE];
    while (fgets(curent_inst, LINE_MAX_SIZE, fp_memin))
    {
        pc = next_pc;
        curent_inst[strcspn(curent_inst, "\r\n")] = '\0'; // remove \n and \r
        strcpy(memory[next_pc], curent_inst);
        next_pc++;
    }
}

void read_disk_memory(FILE* fp_diskin, char disk_memory[][MAX_DISK_LINE_LEN])
{
    int i = 0;
    char line[MAX_DISK_LINE_LEN];
    while (fgets(line, MAX_DISK_LINE_LEN, fp_diskin))
    {
        line[strcspn(line, "\r\n")] = '\0'; // remove \n and \r
        strcpy(disk_memory[i], line);
        i++;
    }
}

Instruction* read_instruction(int pc, char memory[][LINE_MAX_SIZE])
{
    char* curent_inst = memory[pc];
    char* imm_line = memory[pc];
    if (instructionTypeFromLine(curent_inst) == I_TYPE)
    {
        imm_line = memory[pc + 1];
    }
    return instructionFromLine(curent_inst, imm_line, pc);
}

// help us debug the asm programs.
void print_reg_state(int pc, int* reg, Instruction* inst)
{
    const int reg_num = 16;
    char reg_name[][20] = { "zero", "imm", "v0", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "s0", "s1", "s2", "gp", "sp", "ra" };
    printf("PC:%d ", pc);
    int i;
    for (i = 0; i < reg_num; i++)
    {
        if (i == inst->rd || i == inst->rs || i == inst->rt) // if the register is used in this instruction, print it in red
            printf("\033[031m");
        printf("%s:%d  ", reg_name[i], reg[i]);
        printf("\033[0m"); //resum non color printing
    }
    printf("\n");
}

void write_cycles(FILE* fp_cycles, int* cycles)
{
    fprintf(fp_cycles, "%u", cycles);
}

void write_regout(FILE* fp_regout, int* reg)
{
    int R0 = reg[0], R1 = reg[1], R2 = reg[2], R3 = reg[3], R4 = reg[4], R5 = reg[5], R6 = reg[6], R7 = reg[7], R8 = reg[8], R9 = reg[9], R10 = reg[10], R11 = reg[11], R12 = reg[12], R13 = reg[13], R14 = reg[14], R15 = reg[15];
    fprintf(fp_regout, "%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n", R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15);
}

void write_trace(FILE* fp_trace, int pc,char memory[][LINE_MAX_SIZE], int* regs)
{
    int R0 = regs[0], R1 = regs[1], R2 = regs[2], R3 = regs[3], R4 = regs[4], R5 = regs[5], R6 = regs[6], R7 = regs[7], R8 = regs[8], R9 = regs[9], R10 = regs[10], R11 = regs[11], R12 = regs[12], R13 = regs[13], R14 = regs[14], R15 = regs[15];
    fprintf(fp_trace, "%03X %s %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n", pc, memory[pc], R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15);
}

void write_memout(FILE* fp_memout, char memory[][LINE_MAX_SIZE]) {
    int i = 0;
    char line_str[6];
    int line;
    for (i = 0; i < MAX_LINES; i++) {
        line = strtoul(memory[i], NULL, 16) & 0xFFFFF;
        fprintf(fp_memout, "%05X\n", line);
    }
}

void write_monitor_txt(FILE* fp_monitor_txt, int monitor[MONITOR_SIZE * MONITOR_SIZE]) {
    int i = 0;
    for (i = 0; i < MONITOR_SIZE * MONITOR_SIZE; i++) {
        fprintf(fp_monitor_txt, "%02X\n", monitor[i] & 0xFF);
    }
}

void write_monitor_yuv(FILE* fp_monitor_yuv, int monitor[MONITOR_SIZE * MONITOR_SIZE]) {
    int i = 0;
    int chars[2];
    for (i = 0; i < MONITOR_SIZE * MONITOR_SIZE; i++) {
        fprintf(fp_monitor_yuv, "%c", monitor[i]);
    }
}

void write_diskout(FILE* fp_diskout, char disk_memory[][MAX_DISK_LINE_LEN]) {
    int i = 0;
    int line;
    for (i = 0; i < NUM_SECTORS * NUM_SECTOR_LINES; i++) {
        line = strtoul(disk_memory[i], NULL, 16);
        fprintf(fp_diskout, "%05X\n", line & 0x000FFFFF);
    }
}

void decode_inst(int* regs, int* ioreg, Instruction* inst, char memory[][LINE_MAX_SIZE], int* pc_pointer, int* is_in_task, int irq2[], int monitor[], char disk_memory[][MAX_DISK_LINE_LEN], FILE** file_pointers[], int* disk_cycle_ptr)
{
    int io_target_reg;
   
    switch (inst->opcode)
    {
    case 0: // add
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break; //dont update target register's value
        }
        regs[inst->rd] = regs[inst->rs] + regs[inst->rt];

        break;
    case 1: // sub
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break; //dont update target register's value
        }
        regs[inst->rd] = regs[inst->rs] - regs[inst->rt];

        break;
    case 2: // mul
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break; //dont update target register's value
        }
        regs[inst->rd] = regs[inst->rs] * regs[inst->rt];
        break;
    case 3: // and
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break; //dont update target register's value
        }
        regs[inst->rd] = regs[inst->rs] & regs[inst->rt];

        break;
    case 4: // or
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break; //dont update target register's value
        }
        regs[inst->rd] = regs[inst->rs] | regs[inst->rt];

        break;
    case 5: // xor
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break; //dont update target register's value
        }
        regs[inst->rd] = regs[inst->rs] ^ regs[inst->rt];
        break;
    case 6: // sll
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break; //dont update target register's value
        }
        regs[inst->rd] = regs[inst->rs] << regs[inst->rt];

        break;
    case 7: // sra
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break; //dont update target register's value
        }
        regs[inst->rd] = regs[inst->rs] >> regs[inst->rt];

        break;
    case 8: // srl
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break; //dont update target register's value
        }
        if (regs[inst->rt] < 0) {
            regs[inst->rd] = (regs[inst->rs] >> regs[inst->rt]); // if its actualy a left shift
        }
        else
        {
            regs[inst->rd] = ((regs[inst->rs] & 0x000FFFFF) >> regs[inst->rt]); // get red of sign extentions
        }
        break;
    case 9: // beq
        if (regs[inst->rs] == regs[inst->rt])
            *pc_pointer = regs[inst->rd];
        break;
    case 10: // bne
        if (regs[inst->rs] != regs[inst->rt])
            *pc_pointer = regs[inst->rd];
        else
        break;
    case 11: // blt
        if (regs[inst->rs] < regs[inst->rt])
            *pc_pointer = regs[inst->rd];
        else
        break;
    case 12: // bgt
        if (regs[inst->rs] > regs[inst->rt])
            *pc_pointer = regs[inst->rd];
        break;
    case 13: // ble
        if (regs[inst->rs] <= regs[inst->rt])
            *pc_pointer = regs[inst->rd];
        break;
    case 14: // bge
        if (regs[inst->rs] >= regs[inst->rt])
            *pc_pointer = regs[inst->rd];
        break;
    case 15: // jal
        regs[inst->rd] = *pc_pointer;
        *pc_pointer = regs[inst->rs];
        break;
    case 16: // lw
        regs[inst->rd] = extend_sign(strtoul(memory[regs[inst->rs] + regs[inst->rt]], NULL, 16));
        //next_clk;
        break;
    case 17: // sw
        sprintf(memory[regs[inst->rs] + regs[inst->rt]], "%05X", regs[inst->rd]);
        //next_clk;
        break;
    case 18: // reti
        *is_in_task = 0;
        *pc_pointer = ioreg[IRQRETURN_REG];
        break;
    case 19: // in
        io_target_reg = regs[inst->rs] + regs[inst->rt];
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break; //dont update target register's value
        }
        regs[inst->rd] = ioreg[io_target_reg];
        hwregtrace_write(*file_pointers[HWREGTRACE], ioreg[CLK_REG], inst->opcode == 20, io_target_reg, ioreg[io_target_reg]);
        break;
    case 20: // out
        io_target_reg = regs[inst->rs] + regs[inst->rt];
        ioreg[io_target_reg] = regs[inst->rd];
        hwregtrace_write(*file_pointers[HWREGTRACE], ioreg[CLK_REG], inst->opcode == 20, io_target_reg, regs[inst->rd]);
        break;
    case 21: // halt
        *pc_pointer = -1;
        break;
    default:
        printf("Unrecognized command, exiting simulation\n");
        *pc_pointer = -1;
    }
}

void run_instructions(int regs[NUM_REGS], int* ioreg, FILE** file_pointers[], char memory[][LINE_MAX_SIZE], int* is_in_task, int irq2[], int monitor[], char disk_memory[][MAX_DISK_LINE_LEN], int* disk_cycle_ptr)
{
    FILE* fp_trace = *file_pointers[TRACE];
    int pc = 0;
    int* pc_pointer = &pc;
    int old_pc = pc;
    int temp_pc = 0;
    Instruction* current_instruction;
    while (pc != -1)
    {
        
        //fetch instruction from memory
        current_instruction = read_instruction(pc, memory);
        regs[ZERO_REG] = 0;
        regs[IMM_REG] = 0;
        if (instructionType(current_instruction) == I_TYPE) regs[IMM_REG] = current_instruction->imm;
        //Write Trace
        write_trace(*file_pointers[TRACE], *pc_pointer, memory, regs);
        pc++;
        if (instructionType(current_instruction) == I_TYPE)pc++;
        decode_inst(regs, ioreg, current_instruction, memory, pc_pointer, is_in_task, irq2, monitor, disk_memory, file_pointers, disk_cycle_ptr);
        next_clk;
        if (instructionType(current_instruction) == I_TYPE) next_clk;
         if ((current_instruction->opcode == 16) || (current_instruction->opcode == 17)) next_clk; //if it is sw or lw
       
        
        
       

    }

}

