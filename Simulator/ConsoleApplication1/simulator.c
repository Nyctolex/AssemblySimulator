#include "simulator.h"
#define _CRT_SECURE_NO_WARNINGS


//int main(int argc, char* argv[])
//{
//    return 0;
//}

int main(int argc, char* argv[])
{


    FILE* fp_memin = NULL, * fp_diskin = NULL, * fp_irq2in = NULL, * fp_memout = NULL,
        * fp_regout = NULL, * fp_trace = NULL, * fp_hwregtrace = NULL, * fp_cycles = NULL, * fp_leds = NULL, * fp_display7seg = NULL,
        * fp_diskout = NULL, * fp_monitor_txt = NULL, * fp_monitor_yuv;

    FILE** file_pointers[] = { NULL, &fp_memin, &fp_diskin, &fp_irq2in, &fp_memout,
                              &fp_regout, &fp_trace, &fp_hwregtrace, &fp_cycles, &fp_leds, &fp_display7seg, &fp_diskout, &fp_monitor_txt, &fp_monitor_yuv };
    
    //int irq2[200] = add_irq2(fp_irq2in);
    int is_in_task = 0;
    //check_interrupts(int ioreg[], int* pc, int is_task, int irq2[]);
    // when calling to reti set to zero

    const int output_file_index = 4; // All file after this inex are output files
    int regs[NUM_REGS] = { 0 }, ioreg[NUM_IOREGS] = { 0 };
    regs[SP_REG] = MAX_LINES;

    Instruction* instructions = instructionNewinstruction(-1, -1, -1, -1, -1, -1); // initiation the head of the linked list.
    char memory[MAX_LINES][LINE_MAX_SIZE];
    int disk_memory[NUM_SECTORS][SECTOR_SIZE] = { 0 };
    int pc = 0, irq = 0, busy_with_interruption = 0, cycles=0;

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
            return 1;
        }
    }
    // opening the output files
    for (i = i; i < argc2; i++)
    {

        *file_pointers[i] = fopen(argv2[i], "w");
        if (*(file_pointers[i]) == NULL)
        {
            printf("Error: The file %s couldn't open properly", argv2[i]);
            return 1;
        }
    }
    get_instructions(fp_memin, instructions, memory);
    run_instructions(instructions, regs, ioreg, fp_trace, memory);
    write_cycles(fp_cycles, cycles);
    write_regout(fp_regout, regs);
    instructionDeleteList(instructions);
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

//fetching the instructions and executing them
void run_instructions(Instruction* instructions, int regs[NUM_REGS], int* ioreg, FILE* fp_trace, char memory[][LINE_MAX_SIZE])
{
    
    int clk_cycle; //This is wrong but its good for now
    int pc = 0;
    Instruction* current_instruction;
    // int irq = (irq0enable & irq0status) | (irq1enable & irq1status) | (irq2enable & irq2status);
    while (pc != -1)
    {
        current_instruction = instructionGetByLocation(instructions, pc);
        instructionPrintInstruction(current_instruction);

        // Write trace
        write_trace(fp_trace, pc, current_instruction,regs);
        // execute the next instruction from the assembly
        pc = decode_inst(pc, regs, ioreg, current_instruction, memory);
        print_reg_state(pc, regs, current_instruction);
     
    }

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

void write_cycles(FILE* fp_cycles, int cycles)
{
    int pc_adder = 1; // adding 1 or 2 according to the type of the instruction..

    fprintf(fp_cycles, "%d", cycles);
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
// decoding the instruction and executing the correct operation.
int decode_inst(int pc, int* regs, int* ioreg, Instruction* inst, char memory[][LINE_MAX_SIZE])
{
    char int_to_char[LINE_MAX_SIZE] = { '\0' };
    int* cycle = &ioreg[CLKS_REG];
    int old_imm = inst->imm;
    int pc_adder = 1; // adding 1 or 2 according to the type of the instruction.
    int cycles_adder = 1;
    if (instructionType(inst) == I_TYPE)
    {
        cycles_adder = 2;  //! need to call guy's code
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
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] + regs[inst->rt];

        break;
    case 1: // sub
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] - regs[inst->rt];

        break;
    case 2: // mul
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] * regs[inst->rt];
        break;
    case 3: // and
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] & regs[inst->rt];

        break;
    case 4: // or
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] | regs[inst->rt];

        break;
    case 5: // xor
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] ^ regs[inst->rt];
        break;
    case 6: // sll
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] << regs[inst->rt];

        break;
    case 7: // sra
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = regs[inst->rs] >> regs[inst->rt];

        break;
    case 8: // srl
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        regs[inst->rd] = ((unsigned int)regs[inst->rs]) >> regs[inst->rt];
        break;
    case 9: // beq
        if (regs[inst->rs] == regs[inst->rt])
            pc = regs[inst->rd];
        else
            pc += pc_adder;
        break;
    case 10: // bne
        if (regs[inst->rs] != regs[inst->rt])
            pc = regs[inst->rd];
        else
            pc += pc_adder;
        break;
    case 11: // blt
        if (regs[inst->rs] < regs[inst->rt])
            pc = regs[inst->rd];
        else
            pc += pc_adder;
        break;
    case 12: // bgt
        if (regs[inst->rs] > regs[inst->rt])
            pc = regs[inst->rd];
        else
            pc += pc_adder;
        break;
    case 13: // ble
        if (regs[inst->rs] <= regs[inst->rt])
            pc = regs[inst->rd];
        else
            pc += pc_adder;
        break;
    case 14: // bge
        if (regs[inst->rs] >= regs[inst->rt])
            pc = regs[inst->rd];
        else
            pc += pc_adder;
        break;
    case 15: // jal
        pc += pc_adder;
        regs[inst->rd] = pc;
        pc = regs[inst->rs];
        break;
    case 16: // lw
        pc += pc_adder;
        printf("Loading data from line:    %d\n", regs[inst->rs] + regs[inst->rt]);
        regs[inst->rd] = extend_sign(strtoul(memory[regs[inst->rs] + regs[inst->rt]], NULL, 16));
        cycles_adder = 3; //! need to call guy's code
        break;
    case 17: // sw
        pc += pc_adder;
        printf("saving data to line:    %d\n", regs[inst->rs] + regs[inst->rt]);
        sprintf(memory[regs[inst->rs] + regs[inst->rt]], "%05X", regs[inst->rd]);
        cycles_adder = 3; //! need to call guy's code
        break;
    case 18: // reti
        break;
    case 19: // in
        break;
    case 20: // out
        break;
    case 21: // halt
        pc = -1;
        break;
    default:
        pc = -1;
    }
    regs[IMM_REG] = old_imm;
    regs[ZERO_REG] = 0;
    *cycle = *cycle + cycles_adder;
    return pc;
}
// writing the memin into the memory array, and creating the instration linked list.
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
        
        strcpy(memory[next_pc], curent_inst);
        next_pc++;
        if (instructionTypeFromLine(curent_inst) == I_TYPE)
        {
            fgets(imm_line, LINE_MAX_SIZE, fp_memin);
            imm_line[strcspn(imm_line, "\r\n")] = '\0'; // remove \n and \r
            strcpy(memory[next_pc], imm_line);
            next_pc++;
            
        }
        instructionAppendFromLine(head, curent_inst, imm_line, pc);
    }
}