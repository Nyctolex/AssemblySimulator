# pragma once
# ifndef DICT_H
	# define DICT_H

	# include <stdio.h>
	# include <stdlib.h>

	// * Registers
		#define zero "zero"
		#define imm  "imm"
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

	int compare(char* exp);
# endif
// ! add comments