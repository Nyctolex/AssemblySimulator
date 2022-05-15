#include "simulator.h"
#include "instruction.h"
//#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS
int main(int argc, char* argv[])
{


    char* reg_srt = (char*)malloc(sizeof(char) * 20);
    strcpy(reg_srt, "alex");


    int clk_cycle = 0;

    FILE* fp_memin = NULL, * fp_diskin = NULL, * fp_irq2in = NULL, * fp_memout = NULL,
        * fp_regout = NULL, * fp_trace = NULL, * fp_hwregtrace = NULL, * fp_cycles = NULL, * fp_leds = NULL, * fp_display7seg = NULL,
        * fp_diskout = NULL, * fp_monitor_txt = NULL, * fp_monitor_yuv;

    FILE** file_pointers[] = { NULL, &fp_memin, &fp_diskin, &fp_irq2in, &fp_memout,
                              &fp_regout, &fp_trace, &fp_hwregtrace, &fp_cycles, &fp_leds, &fp_display7seg, &fp_diskout, &fp_monitor_txt, &fp_monitor_yuv };
    const int output_file_index = 4; // All file after this inex are output files
    int reg[NUM_REGS] = { 0 }, ioreg[NUM_IOREGS] = { 0 };
    Instruction* instructions = instructionNewinstruction(-1, -1, -1, -1, -1, -1);
    int disk_memory[NUM_SECTORS][SECTOR_SIZE] = { 0 };
    int pc = 0, irq = 0, busy_with_interruption = 0, cycles=0;

    int argc2 = NUM_COMMANDLINE_PARAMETERS;
    char argv2[NUM_COMMANDLINE_PARAMETERS][500 + 1] = { "", "memin.txt", "diskin.txt", "irq2in.txt", "memout.txt", "regout.txt", "trace.txt", "hwregtrace.txt", "cycles.txt", "leds.txt", "display7seg.txt", "diskout.txt", "monitor.txt", "monitor.yuv" };

    if (argc2 != NUM_COMMANDLINE_PARAMETERS) // check command line arguments
    {
        printf("Error: Incorrect command line arg6uments number\n");
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
    get_instructions(fp_memin, instructions);

    run_instructions(instructions, reg, ioreg, clk_cycle, fp_trace);
    write_cycles(fp_cycles, cycles);
    write_regout(fp_regout, reg);
    instructionDeleteList(instructions);
    close_pf(file_pointers, argc);
    return 0;
}

void debug_output(FILE* fp_debug, int pc, int* reg, Instruction* inst, int counter)
{
    const int reg_num = 16;
    char reg_name[16][20] = { "zero", "imm", "v0", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "s0", "s1", "s2", "gp", "sp", "ra" };
    int i;
    fprintf(fp_debug, "{\n"); // new line object
    fprintf(fp_debug, "\"loc\":%d,\n", inst->location);
    for (i = 0; i < reg_num; i++)

    {
        fprintf(fp_debug, "     \"%s\":%d", reg_name[i], reg[i]);
        if (i!= reg_num-1) fprintf(fp_debug, ",\n"); 
    }
    fprintf(fp_debug, "}");
    fprintf(fp_debug, "\n\n");
}

void close_pf(FILE** file_pointers[], int argc)
{
    for (int i = 1; i < argc; i++)
    {
        fclose(*file_pointers[i]);
    }
}

void run_instructions(Instruction* instructions, int* reg, int* ioreg, int clk_cycle, FILE* fp_trace)
{
    FILE* fp_debug = fopen("debug.json", "w");
    fprintf(fp_debug, "[");
    int counter = 0; // counter for the debug
    int pc = 0;
    Instruction* current_instruction;
    // int irq = (irq0enable & irq0status) | (irq1enable & irq1status) | (irq2enable & irq2status);
    while (pc != -1)
    {
        current_instruction = instructionGetByLocation(instructions, pc);
        instructionPrintInstruction(current_instruction);

        // Write trace
        write_trace(fp_trace, pc, current_instruction, reg);
        // execute the next instruction from the assembly
        pc = decode_inst(pc, reg, ioreg, current_instruction, &clk_cycle);
        print_reg_state(pc, reg, current_instruction);

         debug_output(fp_debug, pc, reg, current_instruction, counter);
        counter++; // counter for the debug

        if (pc!=-1)fprintf(fp_debug, ",");
     
    }
    fprintf(fp_debug, "]");
    fclose(fp_debug);
}

void print_reg_state(int pc, int* reg, Instruction* inst)
{
    const int reg_num = 16;
    char reg_name[][20] = { "zero", "imm", "v0", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "s0", "s1", "s2", "gp", "sp", "ra" };
    // int R0 = reg[0], R1 = reg[1], R2 = reg[2], R3 = reg[3], R4 = reg[4], R5 = reg[5], R6 = reg[6], R7 = reg[7], R8 = reg[8], R9 = reg[9], R10 = reg[10], R11 = reg[11], R12 = reg[12], R13 = reg[13], R14 = reg[14], R15 = reg[15];
    printf("PC:%03X ", pc);
    int i;
    for (i = 0; i < reg_num; i++)
    {
        if (i == inst->rd || i == inst->rs || i == inst->rt)
            printf("\033[031m");
        printf("%s:%X  ", reg_name[i], reg[i]);
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

void write_trace(FILE* fp_trace, int pc, Instruction* inst, int* reg)
{
    int R0 = reg[0], R1 = reg[1], R2 = reg[2], R3 = reg[3], R4 = reg[4], R5 = reg[5], R6 = reg[6], R7 = reg[7], R8 = reg[8], R9 = reg[9], R10 = reg[10], R11 = reg[11], R12 = reg[12], R13 = reg[13], R14 = reg[14], R15 = reg[15];
    fprintf(fp_trace, "%03X %05X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n", pc, inst->opcode, R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15);
}

int decode_inst(int pc, int* reg, int* ioreg, Instruction* inst, int* cycle)
{
    int old_imm = inst->imm;
    int pc_adder = 1; // adding 1 or 2 according to the type of the instruction.
    int cycles_adder = 1;
    if (instructionType(inst) == I_TYPE)
    {
        cycles_adder = 2;
        reg[IMM_REG] = inst->imm;
        pc_adder = 2;
    }
    else
    {
        reg[IMM_REG] = 0;
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
        reg[inst->rd] = reg[inst->rs] + reg[inst->rt];

        break;
    case 1: // sub
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        reg[inst->rd] = reg[inst->rs] - reg[inst->rt];

        break;
    case 2: // mul
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        reg[inst->rd] = reg[inst->rs] * reg[inst->rt];
        break;
    case 3: // and
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        reg[inst->rd] = reg[inst->rs] & reg[inst->rt];

        break;
    case 4: // or
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        reg[inst->rd] = reg[inst->rs] | reg[inst->rt];

        break;
    case 5: // xor
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        reg[inst->rd] = reg[inst->rs] ^ reg[inst->rt];
        break;
    case 6: // sll
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        reg[inst->rd] = reg[inst->rs] << reg[inst->rt];

        break;
    case 7: // sra
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        reg[inst->rd] = reg[inst->rs] >> reg[inst->rt];

        break;
    case 8: // srl
        pc += pc_adder;
        if (inst->rd < 2) // wrting to REG0 or REG IMM
        {
            break;
        }
        reg[inst->rd] = ((unsigned int)reg[inst->rs]) >> reg[inst->rt];
        break;
    case 9: // beq
        if (reg[inst->rs] == reg[inst->rt])
            pc = reg[inst->rd];
        else
            pc += pc_adder;
        break;
    case 10: // bne
        if (reg[inst->rs] != reg[inst->rt])
            pc = reg[inst->rd];
        else
            pc += pc_adder;
        break;
    case 11: // blt
        if (reg[inst->rs] < reg[inst->rt])
            pc = reg[inst->rd];
        else
            pc += pc_adder;
        break;
    case 12: // bgt
        if (reg[inst->rs] > reg[inst->rt])
            pc = reg[inst->rd];
        else
            pc += pc_adder;
        break;
    case 13: // ble
        if (reg[inst->rs] <= reg[inst->rt])
            pc = reg[inst->rd];
        else
            pc += pc_adder;
        break;
    case 14: // bge
        if (reg[inst->rs] >= reg[inst->rt])
            pc = reg[inst->rd];
        else
            pc += pc_adder;
        break;
    case 15: // jal
        pc += pc_adder;
        reg[inst->rd] = pc;
        pc = reg[inst->rs];
        break;
    case 16: // lw
        cycles_adder = 3;
        break;
    case 17: // sw
        cycles_adder = 3;
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
    reg[IMM_REG] = old_imm;
    reg[ZERO] = 0;
    *cycle = *cycle + cycles_adder;
    return pc;
}

void get_instructions(FILE* fp_memin, Instruction* head)
{
    int pc = 0;
    int next_pc = 0;
    char cuurent_inst[LINE_MAX_SIZE];
    char imm_line[LINE_MAX_SIZE];
    while (fgets(cuurent_inst, LINE_MAX_SIZE, fp_memin))
    {
        pc = next_pc;
        cuurent_inst[strcspn(cuurent_inst, "\r\n")] = '\0'; // remove \n and \r
        next_pc++;
        if (instructionTypeFromLine(cuurent_inst) == I_TYPE)
        {
            fgets(imm_line, LINE_MAX_SIZE, fp_memin);
            next_pc++;
        }
        instructionAppendFromLine(head, cuurent_inst, imm_line, pc);
    }
}