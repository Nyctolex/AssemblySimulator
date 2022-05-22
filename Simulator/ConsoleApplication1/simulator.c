#include "simulator.h"
#define _CRT_SECURE_NO_WARNINGS

int main(int argc, char* argv[])
{


    FILE* fp_memin = NULL, * fp_diskin = NULL, * fp_irq2in = NULL, * fp_memout = NULL,
        * fp_regout = NULL, * fp_trace = NULL, * fp_hwregtrace = NULL, * fp_cycles = NULL, * fp_leds = NULL, * fp_display7seg = NULL,
        * fp_diskout = NULL, * fp_monitor_txt = NULL, * fp_monitor_yuv;

    FILE** file_pointers[] = { NULL, &fp_memin, &fp_diskin, &fp_irq2in, &fp_memout,
                              &fp_regout, &fp_trace, &fp_hwregtrace, &fp_cycles, &fp_leds, &fp_display7seg, &fp_diskout, &fp_monitor_txt, &fp_monitor_yuv };
    
    int irq2[200];

    int is_in_task = 0;

    const int output_file_index = 4; // All file after this inex are output files
    int regs[NUM_REGS] = { 0 }, ioreg[NUM_IOREGS] = { 0 };
    regs[SP_REG] = MAX_LINES;

    //Instruction* instructions = instructionNewinstruction(-1, -1, -1, -1, -1, -1); // initiation the head of the linked list.
    char memory[MAX_LINES][LINE_MAX_SIZE];
    reset_memory(memory);
    char disk_memory[NUM_SECTORS* NUM_SECTOR_LINES][MAX_DISK_LINE_LEN];
    reset_disk_memory(disk_memory);
    int pc = 0, irq = 0, busy_with_interruption = 0;

    int argc2 = NUM_COMMANDLINE_PARAMETERS; //temporary only
    char argv2[NUM_COMMANDLINE_PARAMETERS][500 + 1] = { "", "memin.txt", "diskin.txt", "irq2in.txt", "memout.txt", "regout.txt", "trace.txt", "hwregtrace.txt", "cycles.txt", "leds.txt", "display7seg.txt", "diskout.txt", "monitor.txt", "monitor.yuv" };

    if (argc2 != NUM_COMMANDLINE_PARAMETERS) // check command line arguments
    {
        printf("Error: Incorrect command line arguments number\n");
        return 1;
    }

    int i;
    // opening the input files
    for (i = 1; i < output_file_index; i++)
    {

        *file_pointers[i] = fopen(argv2[i], "r");

        if (*(file_pointers[i]) == NULL)
        {
            printf("Error: The file %s couldn't open properly", argv2[i]);
            return -1;
        }
    }
    // opening the output files
    for (i = i; i < argc2; i++)
    {

        *file_pointers[i] = fopen(argv2[i], "w");
        if (*(file_pointers[i]) == NULL)
        {
            printf("Error: The file %s couldn't open properly", argv2[i]);
            return -1;
        }
    }


    add_irq2(fp_irq2in, irq2);
    //get_instructions(fp_memin, instructions, memory);
    read_memory(fp_memin, memory);
    read_disk_memory(fp_diskin, disk_memory);
    run_instructions(regs, ioreg, fp_trace, memory, &is_in_task, irq2);
    write_cycles(fp_cycles, ioreg[CLK_REG]);
    write_regout(fp_regout, regs);
    write_memout(fp_memout, memory);
    write_diskout(fp_diskout, disk_memory);
    //instructionDeleteList(instructions); //----------------------------------
    close_pf(file_pointers, argc);
    return 0;
}

//closing all of the files we opened
void close_pf(FILE** file_pointers[], int argc)
{
    for (int i = 1; i < argc; i++)
    {
        fclose(*file_pointers[i]);
    }
}

void next_cycle( int* ioreg, int* pc_pointer,int* is_in_task, int irq2[]) {
    ioreg[CLK_REG] ++;
    check_interrupts(ioreg, pc_pointer, is_in_task, irq2);

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
        if (instructionTypeFromLine(curent_inst) == I_TYPE)
        {
            fgets(imm_line, LINE_MAX_SIZE, fp_memin);
            imm_line[strcspn(imm_line, "\r\n")] = '\0'; // remove \n and \r
            strcpy(memory[next_pc], imm_line);
            next_pc++;

        }
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
    // int R0 = reg[0], R1 = reg[1], R2 = reg[2], R3 = reg[3], R4 = reg[4], R5 = reg[5], R6 = reg[6], R7 = reg[7], R8 = reg[8], R9 = reg[9], R10 = reg[10], R11 = reg[11], R12 = reg[12], R13 = reg[13], R14 = reg[14], R15 = reg[15];
    printf("PC:%d ", pc);
    int i;
    for (i = 0; i < reg_num; i++)
    {
        if (i == inst->rd || i == inst->rs || i == inst->rt)
            printf("\033[031m");
        printf("%s:%d  ", reg_name[i], reg[i]);
        printf("\033[0m");
    }
    printf("\n\n");
    // printf("PC:%03X Zero:%08X imm:%08X v0:%08X a0:%08X a1:%08X a2:%08X a3:%08X t0:%08X t1:%08X t2:%08X s0:%08X s1:%08X s2:%08X gp:%08X sp:%08X ra:%08X\n", pc, R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15);
}

void write_cycles(FILE* fp_cycles, int* cycles)
{
    int pc_adder = 1; // adding 1 or 2 according to the type of the instruction..
    fprintf(fp_cycles, "%d", &cycles);
}

void write_regout(FILE* fp_regout, int* reg)
{
    int R0 = reg[0], R1 = reg[1], R2 = reg[2], R3 = reg[3], R4 = reg[4], R5 = reg[5], R6 = reg[6], R7 = reg[7], R8 = reg[8], R9 = reg[9], R10 = reg[10], R11 = reg[11], R12 = reg[12], R13 = reg[13], R14 = reg[14], R15 = reg[15];
    fprintf(fp_regout, "%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n%08X\n", R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15);
}

void write_trace(FILE* fp_trace, int pc, Instruction* inst, int* regs)
{
    int R0 = regs[0], R1 = regs[1], R2 = regs[2], R3 = regs[3], R4 = regs[4], R5 = regs[5], R6 = regs[6], R7 = regs[7], R8 = regs[8], R9 = regs[9], R10 = regs[10], R11 = regs[11], R12 = regs[12], R13 = regs[13], R14 = regs[14], R15 = regs[15];
    fprintf(fp_trace, "%03X %05X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n", pc, inst->opcode, R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15);
}

void write_memout(FILE* fp_memout, char memory[][LINE_MAX_SIZE]) {
    int i = 0;
    int line;
    for (i = 0; i < MAX_LINES; i++) {
        line = strtoul(memory[i], NULL, 16);
        fprintf(fp_memout, "%05X\n", line);
    }
}

void write_diskout(FILE* fp_diskout, char disk_memory[][MAX_DISK_LINE_LEN]) {
    int i = 0;
    int line;
    for (i = 0; i < NUM_SECTORS * NUM_SECTOR_LINES; i++) {
        line = strtoul(disk_memory[i], NULL, 16);
        fprintf(fp_diskout, "%05X\n", line);
    }
}
// decoding the instruction and executing the correct operation.


//-------------------------------------------
void get_instructions(FILE* fp_memin, Instruction* head, char memory[][LINE_MAX_SIZE])
{
    int pc = 0;
    int next_pc = 0;
    char curent_inst[LINE_MAX_SIZE];
    char imm_line[LINE_MAX_SIZE];
    while (fgets(curent_inst, LINE_MAX_SIZE, fp_memin))
    {
        pc = next_pc;
        curent_inst[strcspn(curent_inst, "\r\n")] = '\0'; // remove \n and \r
        

        printf("%s\n", curent_inst);
        next_pc++;
        if (instructionTypeFromLine(curent_inst) == I_TYPE)
        {
            fgets(imm_line, LINE_MAX_SIZE, fp_memin);
            imm_line[strcspn(imm_line, "\r\n")] = '\0'; // remove \n and \r
            next_pc++;
            
        }
        instructionAppendFromLine(head, curent_inst, imm_line, pc);
        
    }
}

void decode_inst(int* regs, int* ioreg, Instruction* inst, char memory[][LINE_MAX_SIZE], int* pc_pointer, int* is_in_task, int irq2[])
{
    //char int_to_char[LINE_MAX_SIZE] = { '\0' }; ------------------
    int old_imm = inst->imm;
    int pc_adder = 1; // adding 1 or 2 according to the type of the instruction.
    //int cycles_adder = 1; ----------------------------
    if (instructionType(inst) == I_TYPE)
    {
        next_clk;
        regs[IMM_REG] = inst->imm;
        pc_adder = 2;
    }
    else
    {
        regs[IMM_REG] = 0;
        old_imm = 0;
    }
    switch (inst->opcode)
    {
    case 0: // add
        *pc_pointer += pc_adder;
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] + regs[inst->rt];

        break;
    case 1: // sub
        *pc_pointer += pc_adder;
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] - regs[inst->rt];

        break;
    case 2: // mul
        *pc_pointer += pc_adder;
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] * regs[inst->rt];
        break;
    case 3: // and
        *pc_pointer += pc_adder;
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] & regs[inst->rt];

        break;
    case 4: // or
        *pc_pointer += pc_adder;
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] | regs[inst->rt];

        break;
    case 5: // xor
        *pc_pointer += pc_adder;
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] ^ regs[inst->rt];
        break;
    case 6: // sll
        *pc_pointer += pc_adder;
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] << regs[inst->rt];

        break;
    case 7: // sra
        *pc_pointer += pc_adder;
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] >> regs[inst->rt];

        break;
    case 8: // srl
        *pc_pointer += pc_adder;
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = (int)(((unsigned int)regs[inst->rs]) >> regs[inst->rt]);
        break;
    case 9: // beq
        if (regs[inst->rs] == regs[inst->rt])
            *pc_pointer = regs[inst->rd];
        else
            *pc_pointer += pc_adder;
        break;
    case 10: // bne
        if (regs[inst->rs] != regs[inst->rt])
            *pc_pointer = regs[inst->rd];
        else
            *pc_pointer += pc_adder;
        break;
    case 11: // blt
        if (regs[inst->rs] < regs[inst->rt])
            *pc_pointer = regs[inst->rd];
        else
            *pc_pointer += pc_adder;
        break;
    case 12: // bgt
        if (regs[inst->rs] > regs[inst->rt])
            *pc_pointer = regs[inst->rd];
        else
            *pc_pointer += pc_adder;
        break;
    case 13: // ble
        if (regs[inst->rs] <= regs[inst->rt])
            *pc_pointer = regs[inst->rd];
        else
            *pc_pointer += pc_adder;
        break;
    case 14: // bge
        if (regs[inst->rs] >= regs[inst->rt])
            *pc_pointer = regs[inst->rd];
        else
            *pc_pointer += pc_adder;
        break;
    case 15: // jal
        *pc_pointer += pc_adder;
        regs[inst->rd] = *pc_pointer;
        *pc_pointer = regs[inst->rs];
        break;
    case 16: // lw
        next_clk;
        *pc_pointer += pc_adder;
        regs[inst->rd] = extend_sign(strtoul(memory[regs[inst->rs] + regs[inst->rt]], NULL, 16));
        break;
    case 17: // sw
        next_clk;
        *pc_pointer += pc_adder;
        sprintf(memory[regs[inst->rs] + regs[inst->rt]], "%05X", regs[inst->rd]);
        break;
    case 18: // reti
        *pc_pointer += pc_adder;
        *is_in_task = 0;
        *pc_pointer = ioreg[IRQRETURN_REG];
        break;
    case 19: // in
        *pc_pointer += pc_adder;
        if (inst->rd <= IMM_REG) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = ioreg[regs[inst->rs] + regs[inst->rt]];
        break;
    case 20: // out
        *pc_pointer += pc_adder;
        ioreg[regs[inst->rs] + regs[inst->rt]] = regs[inst->rd];
        break;
    case 21: // halt
        *pc_pointer = -1;
        break;
    default:
        printf("Unrecognized command\n");
        *pc_pointer = -1;
    }
    regs[IMM_REG] = old_imm;
    regs[ZERO_REG] = 0;
}

void run_instructions(int regs[NUM_REGS], int* ioreg, FILE* fp_trace, char memory[][LINE_MAX_SIZE], int* is_in_task, int irq2[])
{

    int clk_cycle; //This is wrong but its good for now
    int pc = 0;
    int* pc_pointer = &pc;
    Instruction* current_instruction;
    while (pc != -1)
    {

        next_clk;
        current_instruction = read_instruction(pc, memory);
        //current_instruction = NULL;
        instructionPrintInstruction(current_instruction);

        // Write trace
        write_trace(fp_trace, pc, current_instruction, regs);

        // execute the next instruction from the assembly
        decode_inst(regs, ioreg, current_instruction, memory, pc_pointer, is_in_task, irq2);
        print_reg_state(pc, regs, current_instruction);

    }

}

