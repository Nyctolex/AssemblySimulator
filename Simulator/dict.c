#include <string.h>
// * Creating Strings
// * Registers
char zero[] = "zero";
char imm[] = "imm";
char v0[] = "v0";
char a0[] = "a0";
char a1[] = "a1";
char a2[] = "a2";
char a3[] = "a3";
char t0[] = "t0";
char t1[] = "t1";
char t2[] = "t2";
char s0[] = "s0";
char s1[] = "s1";
char s2[] = "s2";
char gp[] = "gp";
char sp[] = "sp";
char ra[] = "ra";
// * OpCodes
char word[] = ".word";
char add[] = "add";
char sub[] = "sub";
char mul[] = "mul";
char and[] = "and";
char or [] = "or";
char xor [] = "xor";
char sll[] = "sll";
char sra[] = "sra";
char srl[] = "srl";
char beq[] = "beq";
char bne[] = "bne";
char blt[] = "blt";
char bgt[] = "bgt";
char ble[] = "ble";
char bge[] = "bge";
char jal[] = "jal";
char lw[] = "lw";
char sw[] = "sw";
char reti[] = "reti";
char in[] = "in";
char out[] = "out";
char halt[] = "halt";
// * Compare Strings

int dict_compare(char *exp) // returns the expression's opcode
{
    // * Registers
    if (!strcmp(exp, zero))
        return 0;
    else if (!strcmp(exp, imm))
        return 1;
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
    else if (!strcmp(exp, or))
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

char *int_to_opcode(int opcode)
{
    char *opcode_srt = malloc(20);
    switch (opcode)
    {
    case 0: // add
        strcpy(opcode_srt, add);
        break;
    case 1: // sub
        strcpy(opcode_srt, sub);
        break;
    case 2: // mul
        strcpy(opcode_srt, mul);
        break;
    case 3: // and
        strcpy(opcode_srt, and);

        break;
    case 4: // or
        strcpy(opcode_srt, or);
        break;
    case 5: // xor
        strcpy(opcode_srt, xor);
        break;
    case 6: // sll
        strcpy(opcode_srt, sll);

        break;
    case 7: // sra
        strcpy(opcode_srt, sra);

        break;
    case 8: // srl
        strcpy(opcode_srt, srl);
        break;
    case 9: // beq
        strcpy(opcode_srt, beq);
        break;
    case 10: // bne
        strcpy(opcode_srt, bne);
        break;
    case 11: // blt
        strcpy(opcode_srt, blt);
        break;
    case 12: // bgt
        strcpy(opcode_srt, bgt);
        break;
    case 13: // ble
        strcpy(opcode_srt, ble);
        break;
    case 14: // bge
        strcpy(opcode_srt, bge);
        break;
    case 15: // jal
        strcpy(opcode_srt, jal);
        break;
    case 16: // lw
        strcpy(opcode_srt, lw);
        break;
    case 17: // sw
        strcpy(opcode_srt, sw);
        break;
    case 18: // reti
        strcpy(opcode_srt, reti);
        break;
    case 19: // in
        strcpy(opcode_srt, in);
        break;
    case 20: // out
        strcpy(opcode_srt, out);
        break;
    case 21: // halt
        strcpy(opcode_srt, halt);
        break;
    default:
    }
    return opcode_srt;
}

char *int_to_reg(int reg)
{
    char *reg_srt = malloc(20);
    switch (reg)
    {
    case 0:
        strcpy(reg_srt, zero);
        break;
    case 1:
        strcpy(reg_srt, imm);
        break;
    case 2:
        strcpy(reg_srt, v0);
        break;
    case 3:
        strcpy(reg_srt, a0);
        break;
    case 4:
        strcpy(reg_srt, a1);
        break;
    case 5:
        strcpy(reg_srt, a2);
        break;
    case 6:
        strcpy(reg_srt, a3);
        break;
    case 7:
        strcpy(reg_srt, t0);
        break;
    case 8:
        strcpy(reg_srt, t1);
        break;
    case 9:
        strcpy(reg_srt, t2);
        break;
    case 10:
        strcpy(reg_srt, s0);
        break;
    case 11:
        strcpy(reg_srt, s1);
        break;
    case 12:
        strcpy(reg_srt, s2);
        break;
    case 13:
        strcpy(reg_srt, gp);
        break;
    case 14:
        strcpy(reg_srt, sp);
        break;
    case 15:
        strcpy(reg_srt, ra);
        break;
    default:
    }

    return reg_srt;
}