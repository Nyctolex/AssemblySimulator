#include <string.h>
#include "dict.h"
#define _CRT_SECURE_NO_WARNINGS

// * Creating Strings
// * Registers

// * Compare Strings
int dict_compare(char* exp) // returns the expression's opcode
{
    // * Registers
    if (!strcmp(exp, zero))
        return 0; 

    else if (!strcmp(exp, imm_dict)) return 1;
    else if (!strcmp(exp, v0))
        return 2;
    else if (!strcmp(exp, a0))
        return 3;
    else if (!strcmp(exp, a1))
        return 4;
    else if (!strcmp(exp, a2))
        return 5;
    else if (!strcmp(exp, a3))
        return 6;
    else if (!strcmp(exp, t0))
        return 7;
    else if (!strcmp(exp, t1))
        return 8;
    else if (!strcmp(exp, t2))
        return 9;
    else if (!strcmp(exp, s0))
        return 10;
    else if (!strcmp(exp, s1))
        return 11;
    else if (!strcmp(exp, s2))
        return 12;
    else if (!strcmp(exp, gp))
        return 13;
    else if (!strcmp(exp, sp))
        return 14;
    else if (!strcmp(exp, ra))
        return 15;
    // * OpCodes
    else if (!strcmp(exp, word))
        return -1;
    else if (!strcmp(exp, add))
        return 0;
    else if (!strcmp(exp, sub))
        return 1;
    else if (!strcmp(exp, mul))
        return 2;
    else if (!strcmp(exp, and))
        return 3;
    else if (!strcmp(exp, or ))
        return 4;
    else if (!strcmp(exp, xor))
        return 5;
    else if (!strcmp(exp, sll))
        return 6;
    else if (!strcmp(exp, sra))
        return 7;
    else if (!strcmp(exp, srl))
        return 8;
    else if (!strcmp(exp, beq))
        return 9;
    else if (!strcmp(exp, bne))
        return 10;
    else if (!strcmp(exp, blt))
        return 11;
    else if (!strcmp(exp, bgt))
        return 12;
    else if (!strcmp(exp, ble))
        return 13;
    else if (!strcmp(exp, bge))
        return 14;
    else if (!strcmp(exp, jal))
        return 15;
    else if (!strcmp(exp, lw))
        return 16;
    else if (!strcmp(exp, sw))
        return 17;
    else if (!strcmp(exp, reti))
        return 18;
    else if (!strcmp(exp, in))
        return 19;
    else if (!strcmp(exp, out))
        return 20;
    else if (!strcmp(exp, halt))
        return 21;
}

