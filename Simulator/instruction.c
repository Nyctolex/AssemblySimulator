#include "instruction.h"
#include "dict.c"
Instruction *instructionNewinstruction(int opcode, int rd, int rs, int rt, int imm, int location)
{
    Instruction *new_instruction = (Instruction *)malloc(sizeof(Instruction));
    if (new_instruction != NULL)
    {
        new_instruction->rt = rt;
        new_instruction->rs = rs;
        new_instruction->rd = rd;
        new_instruction->opcode = opcode;
        new_instruction->imm = imm;
        new_instruction->location = location;
        new_instruction->next = NULL;
    }
    return new_instruction;
}

Instruction *instructionGetByLocation(Instruction *head, int location)
{
    Instruction *temp = head;
    while (temp->location != location)
    {

        temp = temp->next;
        if (temp == NULL)
        {
            break;
        }
    }
    return temp;
}

Instruction *instructionLast(Instruction *head)
{
    Instruction *temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    return temp;
}
void instructionAppendNode(Instruction *head, Instruction *node)
{
    Instruction *tail = instructionLast(head);
    tail->next = node;
}

int instructionType(Instruction *inst)
{
    if ((inst->rd == IMM_REG) || (inst->rs == IMM_REG) || (inst->rt == IMM_REG))
    {
        return I_TYPE;
    }
    return R_TYPE;
}

int instructionTypeFromLine(char *line)
{
    if ((slice_atoi_hex(line, 4, 5) == IMM_REG) || (slice_atoi_hex(line, 3, 4) == IMM_REG) || (slice_atoi_hex(line, 2, 3) == IMM_REG))
    {
        return I_TYPE;
    }
    return R_TYPE;
}

// void instructionInitFromLine(Instruction *inst, char *line, char *imm, int location)
// {
//     inst->rt = slice_atoi_hex(line, 4, 5);
//     inst->rs = slice_atoi_hex(line, 3, 4);
//     inst->rd = slice_atoi_hex(line, 2, 3);
//     inst->opcode = slice_atoi_hex(line, 0, 2);
// }

void instructionAppendData(Instruction *head, int opcode, int rd, int rs, int rt, int imm, int location)
{
    Instruction *new_tail = instructionNewinstruction(opcode, rd, rs, rt, imm, location);
    instructionAppendNode(head, new_tail);
}

void instructionAppendFromLine(Instruction *head, char *line, char *imm_line, int location)
{
    int rt = slice_atoi_hex(line, 4, 5);
    int rs = slice_atoi_hex(line, 3, 4);
    int rd = slice_atoi_hex(line, 2, 3);
    int opcode = slice_atoi_hex(line, 0, 2);
    int imm = 0;
    if (instructionTypeFromLine(line) == I_TYPE)
    {
        imm = strtoul(imm_line, NULL, 16);
    }
    instructionAppendData(head, opcode, rd, rs, rt, imm, location);
}

// slice hexadecimal stirng (str[start:end]), return as int
int slice_atoi_hex(char str[], int start, int end)
{
    int len = end - start;
    char tmp[LINE_MAX_SIZE];
    strncpy(tmp, str + start, len);
    tmp[len] = '\0';
    return strtoul(tmp, NULL, 16);
}

void instructionPrintInstruction(Instruction *inst)
{
    char *opcode = int_to_opcode(inst->opcode);
    char *rd = int_to_reg(inst->rd);
    char *rs = int_to_reg(inst->rs);
    char *rt = int_to_reg(inst->rt);

    if (instructionType(inst) == R_TYPE)
    {

        printf("Location: %d    DATA: %s %s %s %s\n", inst->location, opcode, rd, rs, rt);
    }
    else
    {
        printf("Location: %d    DATA: %s %s %s %s    Imm:   %X\n", inst->location, opcode, rd, rs, rt, inst->imm);
    }
    free(opcode);
    free(rd);
    free(rs);
    free(rt);
}

void instructionPrintInstructionHex(Instruction *inst)
{
    if (instructionType(inst) == R_TYPE)
    {
        printf("Location: %d    DATA: %X %X %X %X\n", inst->location, inst->opcode, inst->rd, inst->rs, inst->rt);
    }
    else
    {
        printf("Location: %d    DATA: %X %X %X %X    Imm:   %X\n", inst->location, inst->opcode, inst->rd, inst->rs, inst->rt, inst->imm);
    }
}

void instructionPrintInstructions(Instruction *head)
{
    while (head != NULL)
    {
        instructionPrintInstruction(head);
        head = head->next;
    }
}

void instructionDeleteList(Instruction *head)
{
    Instruction *temp;   // A pointer to a line we are going to destroy after updating head
    while (head != NULL) // Destroy all of the list
    {
        temp = head; // temp <-- current node, head <-- head->next
        head = head->next;
        free(temp); // we destroy temp and free the memory
    }
}