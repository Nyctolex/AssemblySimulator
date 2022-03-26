#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Line.h"

line *lineNewLine(char opcode[], char rd[], char rs[], char rt[], char imm_reg[], char imm[], int location)
{
    line *new_line = (line *)malloc(sizeof(line));
    if (new_line != NULL)
    {
        strcpy(new_line->rt, rt);
        strcpy(new_line->rs, rs);
        strcpy(new_line->rd, rd);
        strcpy(new_line->opcode, opcode);
        strcpy(new_line->imm_reg, imm_reg);
        strcpy(new_line->imm, imm);
        new_line->location = location;
        new_line->next = NULL;
    }
    return new_line;
}
line *lineGetByIndex(line *head, int index)
{
    if (index < 0)
    {
        return NULL;
    }
    line *temp = head;
    int i;
    for (i = 0; i < index; i++)
    {
        if (temp != NULL)
        {
            temp = temp->next;
        }
        else
        {
            break;
        }
    }
    return temp;
}

line *lineGetByLocation(line *head, int location)
{
    line *temp = head;
    int i;
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
line *lineLast(line *head)
{
    line *temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    return temp;
}

void lineAppendNode(line *head, line *node)
{
    line *tail = lineLast(head);
    tail->next = node;
}

line *lineAppendData(line *head, char opcode[], char rd[], char rs[], char rt[], char imm_reg[], char imm[], int location)
{
    line *new_tail = lineNewLine(opcode, rd, rs, rt, imm_reg, imm, location);
    lineAppendNode(head, new_tail);
}
void lineDeleteList(line *head)
{
    line *temp;          // A pointer to a label we are going to destroy after updating head
    while (head != NULL) // Destroy all of the list
    {
        temp = head; // temp <-- current node, head <-- head->next
        head = head->next;
        free(temp); // we destroy temp and free the memory
    }
}

int main()
{
    line *head = lineNewLine("op0", "rd", "rs", "ty", "immtr", "imm_reg", 0);
    lineAppendData(head, "op1", "rd", "rs", "ty", "immtr", "imm_reg", 5);
    lineAppendData(head, "op2", "rd", "rs", "ty", "immtr", "imm_reg", 0);
    line *temp = lineGetByLocation(head, 5);
    printf("%s", temp->opcode);
    lineDeleteList(head);
    return 0;
}