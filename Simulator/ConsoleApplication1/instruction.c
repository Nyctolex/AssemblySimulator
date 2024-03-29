#include "instruction.h"
Instruction* instructionNewinstruction(int opcode, int rd, int rs, int rt, int imm, int location)
{
    Instruction* new_instruction = (Instruction*)malloc(sizeof(Instruction));
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

Instruction* instructionGetByLocation(Instruction* head, int location)
{
    Instruction* temp = head; //temporary node repersenting the current node checked.
    while (temp->location != location) // while the current node is not the one we are searching for
    {
        temp = temp->next; //current node would set to the next node.
        if (temp == NULL) //if we got to the last node of the lisr -> exit and return null
        {
            break;
        }
    }
    return temp;
}

Instruction* instructionLast(Instruction* head)
{
    Instruction* temp = head; //temporary node repersenting the current node checked.
    while (temp->next != NULL) //while it is not the last node
    {
        temp = temp->next;//current node would set to the next node.
    }
    return temp;
}

void instructionAppendNode(Instruction* head, Instruction* node)
{
    Instruction* tail = instructionLast(head); //get the last node in the list
    tail->next = node; //append the node
}

int instructionType(Instruction* inst)
{
    
    if ((inst->rd == IMM_REG) && ((beq_opcode <= inst->opcode  &&  inst->opcode <= bge_opcode) || inst->opcode==sw_opcode || inst->opcode == out_opcode))
    {
        return I_TYPE;
    }
    if ((inst->rs == IMM_REG) || (inst->rt == IMM_REG)) {
        return I_TYPE;
    }
    return R_TYPE;
}

int instructionTypeFromLine(char* line)
{
    //if one of the registers in the line is the immidiate register
    int rt = slice_atoi_hex(line, 4, 5);//set rt to its numeric value
    int rs = slice_atoi_hex(line, 3, 4);//set rs to its numeric value
    int rd = slice_atoi_hex(line, 2, 3);//set rd to its numeric value
    int opcode = slice_atoi_hex(line, 0, 2);//set opcode to its numeric value
    if ((rd == IMM_REG) && ((beq_opcode <= opcode && opcode <= bge_opcode) || opcode == sw_opcode || opcode == out_opcode)) {
        return I_TYPE;
    }
    if ((rs == IMM_REG) || (rt == IMM_REG)) {
        return I_TYPE;
    }
    return R_TYPE;
}

void instructionAppendData(Instruction* head, int opcode, int rd, int rs, int rt, int imm, int location)
{
    //create a new instruction from the given parameters
    Instruction* new_tail = instructionNewinstruction(opcode, rd, rs, rt, imm, location);
    //append the new node
    instructionAppendNode(head, new_tail);
}

void instructionAppendFromLine(Instruction* head, char* line, char* imm_line, int location)
{
    int rt = slice_atoi_hex(line, 4, 5); //set rt to to its numeric value
    int rs = slice_atoi_hex(line, 3, 4); //set rs to to its numeric value
    int rd = slice_atoi_hex(line, 2, 3); //set rd to to its numeric value
    int opcode = slice_atoi_hex(line, 0, 2); //set opcode to to its numeric value
    int imm = 0;
    if (instructionTypeFromLine(line) == I_TYPE) // if the instruction is of I type
    {
        imm = extend_sign(strtoul(imm_line, NULL, 16)); //set imm to to its numeric value
    }
    instructionAppendData(head, opcode, rd, rs, rt, imm, location); //append the list with the new node
}

Instruction* instructionFromLine(char* line, char* imm_line, int location)
{
    int rt = slice_atoi_hex(line, 4, 5);//set rt to to its numeric value
    int rs = slice_atoi_hex(line, 3, 4);//set rs to to its numeric value
    int rd = slice_atoi_hex(line, 2, 3);//set rd to to its numeric value
    int opcode = slice_atoi_hex(line, 0, 2);//set opcode to to its numeric value
    int imm = 0;
    if (instructionTypeFromLine(line) == I_TYPE)// if the instruction is of I type
    {
        imm = extend_sign(strtoul(imm_line, NULL, 16));//set imm to to its numeric value
    }
    // create a new Instruction node with the given parameters taken from the line
    Instruction* new_inst = instructionNewinstruction(opcode, rd, rs, rt, imm, location);
    return new_inst;
}

// slice hexadecimal stirng (str[start:end]), return as int
int slice_atoi_hex(char str[], int start, int end)
{
    int len = end - start;
    char tmp[LINE_MAX_SIZE];
    strncpy(tmp, str + start, len); //copy the wanted part of the line to tmp
    tmp[len] = '\0'; //add null at the end of the string
    return strtoul(tmp, NULL, 16); //return its numeric value
}

int extend_sign(int num)
{
    int mask_extend = 0xFFF00000; //sign externtion
    int mask_msb = 0x80000; // mast to determine the msb
    int sign = mask_msb & num;
    //if the msb is not 0
    if (sign > 0) {

        num = mask_extend | num;
    }
    return num;
}

char* int_to_opcode(int opcode)
{
    char* opcode_srt = NULL;
    opcode_srt = (char*)malloc(sizeof(char) * 20);
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
        strcpy(opcode_srt, or );
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
        break;
    }
    return opcode_srt;
}

char* int_to_reg(int reg)
{
    char* reg_srt = (char*)malloc(sizeof(char) * 20);
    switch (reg)
    {
    case 0:
        strcpy(reg_srt, zero);
        break;
    case 1:
        strcpy(reg_srt, imm_dict);
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
        break;
    }

    return reg_srt;
}

void instructionPrintInstruction(Instruction* inst)
{


    char* opcode = int_to_opcode(inst->opcode);
    char* rd = int_to_reg(inst->rd);
    char* rs = int_to_reg(inst->rs);
    char* rt = int_to_reg(inst->rt);

    if (instructionType(inst) == R_TYPE)
    {

        printf("Location: %d    DATA: %s %s %s %s\n", inst->location, opcode, rd, rs, rt);
    }
    else
    {
        printf("Location: %d    DATA: %s %s %s %s    Imm:   %d\n", inst->location, opcode, rd, rs, rt, inst->imm);
    }
    free(opcode);
    free(rd);
    free(rs);
    free(rt);
}

void instructionPrintInstructionHex(Instruction* inst)
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

void instructionDeleteList(Instruction* head)
{
    Instruction* temp;   // A pointer to a line we are going to destroy after updating head
    while (head != NULL) // Destroy all of the list
    {
        temp = head; // temp <-- current node, head <-- head->next
        head = head->next;
        free(temp); // we destroy temp and free the memory
    }
}