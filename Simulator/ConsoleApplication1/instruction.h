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

/**
 * @brief creates an Instruction data type from a string line.
 * @param line - the string
 * @param imm_line - (string) if the instruction is of type immidiate, this would be it's imm value.
 * @param location - the location of the line
 * @return Instruction* - the new instruction.
 */
Instruction* instructionFromLine(char* line, char* imm_line, int location);
/**
 * @brief mallocating space for a new instruction, and initiating its parameters.
 */
Instruction* instructionNewinstruction(int opcode, int rd, int rs, int rt, int imm, int location);
/**
 * @brief searching and returning the wanted instruction node from the linked list
 * 
 * @param head - the first node in the liked list.
 * @param location - the wanted location of the instruction.
 * @return Instruction* the searched instruction.
 */
Instruction* instructionGetByLocation(Instruction* head, int location);
/**
 * @brief returning the last node in the linked list.
 * @param head the first node in the liked list.
 */
Instruction* instructionLast(Instruction* head);
/**
 * @brief appending the given node to the end of the list 
 * 
 * @param head the first node in the liked list.
 * @param node the node that would be appeneded
 */
void instructionAppendNode(Instruction* head, Instruction* node);
/**
 * @brief retuning the type (R or I) of the given instruction.
 * @return int 0  for R type, 1 for I type.
 */
int instructionType(Instruction* inst);
/**
 * @brief getting the type of the instruction from it's string format.
 * @return int 0  for R type, 1 for I type.
 */
int instructionTypeFromLine(char* line);
/**
 * @brief initiation new node with the given parameters and appending it to the linked list.
 * 
 * @param head the first node in the liked list.
 */
void instructionAppendData(Instruction* head, int opcode, int rd, int rs, int rt, int imm, int location);
/**
 * @brief initiation new node with the given parameters taken from the string "line", and appending it to the linked list.
 * 
 * @param head the first node in the liked list.
 * @param line the string of the instruction in the memory
 * @param imm_line (string) if the instruction is of type immidiate, this would be it's imm value.
 * @param location the location (in pc means) of the instruction
 */
void instructionAppendFromLine(Instruction* head, char* line, char* imm_line, int location);
/**
 * @brief slicing the given string according to the given parameters, and returning the numeric value of
 * the string. 
 */
int slice_atoi_hex(char str[], int start, int end);
/**
 * @brief printing the values of the given instruction, for debug purposes.
 */
void instructionPrintInstruction(Instruction* inst);
/**
 * @brief freeing all allocated space of the given linked list.
 * @param head the first node in the liked list.
 */
void instructionDeleteList(Instruction* head);
/**
 * @brief converting a number represented in 20 bit to one represented in 32 bits.
 */
int extend_sign(int num);
/**
 * @brief convering the opcode's numeric value to the corresponding string
 */
char* int_to_opcode(int opcode);
/**
 * @brief converting the number of the register to its name
 */
char* int_to_reg(int reg);

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

#define sw_opcode  17
#define bge_opcode  14
#define beq_opcode  9
#define out_opcode  20





#endif
