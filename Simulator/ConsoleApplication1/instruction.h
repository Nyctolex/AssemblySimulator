#pragma once 
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"
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
Instruction* instructionFromLine(char* line, char* imm_line, int location);
void instructionDeleteList(Instruction* head);
int extend_sign(int num);



#define zero "zero"
#define imm_dict  "imm"
#define v0  "v0"
#define a0  "a0"
#define a1  "a1"
#define a2  "a2"
#define a3  "a3"
#define t0  "t0"
#define t1  "t1"
#define t2  "t2"
#define s0  "s0"
#define s1  "s1"
#define s2  "s2"
#define gp  "gp"
#define sp  "sp"
#define ra  "ra"
// * OpCodes
#define word  ".word"
#define add  "add"
#define sub  "sub"
#define mul  "mul"
#define and  "and"
#define or   "or"
#define xor  "xor"
#define sll  "sll"
#define sra  "sra"
#define srl  "srl"
#define beq  "beq"
#define bne  "bne"
#define blt  "blt"
#define bgt  "bgt"
#define ble  "ble"
#define bge  "bge"
#define jal  "jal"
#define lw  "lw"
#define sw  "sw"
#define reti  "reti"
#define in  "in"
#define out  "out"
#define halt  "halt"

int dict_compare(char* exp);
char* int_to_opcode(int opcode);
char* int_to_reg(int reg);



#endif
