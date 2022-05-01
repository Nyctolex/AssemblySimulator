#include "simulator.h"
#include "instruction.c"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{

    FILE *fp_memin = NULL, *fp_diskin = NULL, *fp_irq2in = NULL, *fp_memout = NULL,
         *fp_regout = NULL, *fp_trace = NULL, *fp_hwregtrace = NULL, *fp_cycles = NULL, *fp_leds = NULL, *fp_display7seg = NULL,
         *fp_diskout = NULL, *fp_monitor_txt = NULL, *fp_monitor_yuv;

    FILE **file_pointers[] = {NULL, &fp_memin, &fp_diskin, &fp_irq2in, &fp_memout,
                              &fp_regout, &fp_trace, &fp_hwregtrace, &fp_cycles, &fp_leds, &fp_display7seg, &fp_diskout, &fp_monitor_txt, &fp_monitor_yuv};
    const int output_file_index = 4; // All file after this inex are output files
    int reg[NUM_REGS] = {0}, ioreg[NUM_IOREGS] = {0};
    Instruction *instructions = instructionNewinstruction(-1, -1, -1, -1, -1, -1);
    int disk_memory[NUM_SECTORS][SECTOR_SIZE] = {0};
    int pc = 0, irq = 0, busy_with_interruption = 0;

    if (argc != NUM_COMMANDLINE_PARAMETERS) // check command line arguments
    {
        printf("Error: Incorrect command line arg6uments number\n");
        return 1;
    }

    int i;
    // opening the input files
    for (i = 1; i < output_file_index; i++)
    {

        *file_pointers[i] = fopen(argv[i], "r");
        if (file_pointers[i] == NULL)
        {
            printf("Error: The file %s couldn't open properly", argv[i]);
            return 1;
        }
    }

    // opening the output files
    for (i = i; i < argc; i++)
    {
        file_pointers[i] = fopen(argv[i], "w");
        if (&(file_pointers[i]) == NULL)
        {
            printf("Error: The file %s couldn't open properly", argv[i]);
            return 1;
        }
    }
    get_instructions(fp_memin, instructions);

    // run_instructions(instructions, reg, ioreg);
    instructionDeleteList(instructions);
    close_pf(file_pointers, argc);
    return 0;
}

void close_pf(FILE **file_pointers[], int argc)
{
    for (int i = 1; i < argc; i++)
    {
        fclose(*file_pointers[i]);
    }
}

void run_instructions(Instruction *instructions, int *reg, int *ioreg)
{
    int pc = 0;
    Instruction *current_instruction;
    while (pc != -1)
    {
        current_instruction = instructionGetByLocation(instructions, pc);
        instructionPrintInstruction(current_instruction);
        // execute the next instruction from the assembly
        pc = decode_inst(pc, reg, ioreg, current_instruction);
    }
}

int decode_inst(int pc, int *reg, int *ioreg, Instruction *inst)
{
    int pc_adder = 1; // adding 1 or 2 according to the type of the instruction..
    if (instructionType(inst) == I_TYPE)
    {
        reg[IMM_REG] = inst->imm;
        pc_adder = 2;
    }
    switch (inst->opcode)
    {
    case 0: // add
        reg[inst->rd] = reg[inst->rs] + reg[inst->rt];
        pc += pc_adder;
        break;
    case 1: // sub
        reg[inst->rd] = reg[inst->rs] - reg[inst->rt];
        pc += pc_adder;
        break;
    case 2: // mul
        reg[inst->rd] = reg[inst->rs] * reg[inst->rt];
        pc += pc_adder;
        break;
    case 3: // and
        reg[inst->rd] = reg[inst->rs] & reg[inst->rt];
        pc += pc_adder;
        ;
        break;
    case 4: // or
        reg[inst->rd] = reg[inst->rs] | reg[inst->rt];
        pc += pc_adder;
        break;
    case 5: // xor
        reg[inst->rd] = reg[inst->rs] ^ reg[inst->rt];
        pc += pc_adder;
        break;
    case 6: // sll
        reg[inst->rd] = reg[inst->rs] << reg[inst->rt];
        pc += pc_adder;
        break;
    case 7: // sra
        reg[inst->rd] = reg[inst->rs] >> reg[inst->rt];
        pc += pc_adder;
        break;
    case 8: // srl
        reg[inst->rd] = ((unsigned int)reg[inst->rs]) >> reg[inst->rt];
        pc += pc_adder;
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
        break;
    case 17: // sw
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

    return pc;
}

void get_instructions(FILE *fp_memin, Instruction *head)
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
    // instructionPrintInstructions(head);
}