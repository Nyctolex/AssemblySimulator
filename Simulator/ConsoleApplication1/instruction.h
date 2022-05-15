
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdio.h>
#include <stdlib.h>
typedef struct _instruction
{
    int opcode, rd, rs, rt, imm, location;
    struct _instruction* next;
} Instruction;

#define IMM_REG 1
#define R_TYPE 0
#define I_TYPE 1
#define LINE_MAX_SIZE 10

Instruction* instructionNewinstruction(int opcode, int rd, int rs, int rt, int imm, int location);
Instruction* instructionGetByLocation(Instruction* head, int location);
Instruction* instructionLast(Instruction* head);
void instructionAppendNode(Instruction* head, Instruction* node);
int instructionType(Instruction* inst);
int instructionTypeFromLine(char* line);
void instructionAppendData(Instruction* head, int opcode, int rd, int rs, int rt, int imm, int location);
void instructionAppendFromLine(Instruction* head, char* line, char* imm_line, int location);
int slice_atoi_hex(char str[], int start, int end);
void instructionPrintInstruction(Instruction* inst);
void instructionDeleteList(Instruction* head);
#endif
