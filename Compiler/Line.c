
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Line.h"
#include "RegisterConst.h"

Line *lineNewLine(char opcode[], char rd[], char rs[], char rt[], char imm[], int location)
{
    Line *new_line = (Line *)malloc(sizeof(Line));
    if (new_line != NULL)
    {
        strcpy(new_line->rt, rt);
        strcpy(new_line->rs, rs);
        strcpy(new_line->rd, rd);
        strcpy(new_line->opcode, opcode);
        strcpy(new_line->imm, imm);
        new_line->location = location;
        new_line->next = NULL;
    }
    return new_line;
}
Line *lineGetByIndex(Line *head, int index)
{
    if (index < 0)
    {
        return NULL;
    }
    Line *temp = head;
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

Line *lineGetByLocation(Line *head, int location)
{
    Line *temp = head;
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
Line *lineLast(Line *head)
{
    Line *temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    return temp;
}

void lineAppendNode(Line *head, Line *node)
{
    Line *tail = lineLast(head);
    tail->next = node;
}

Line *lineAppendData(Line *head, char opcode[], char rd[], char rs[], char rt[], char imm[], int location)
{
    Line *new_tail = lineNewLine(opcode, rd, rs, rt,  imm, location);
    lineAppendNode(head, new_tail);
}
void lineDeleteList(Line *head)
{
    Line *temp;          // A pointer to a line we are going to destroy after updating head
    while (head != NULL) // Destroy all of the list
    {
        temp = head; // temp <-- current node, head <-- head->next
        head = head->next;
        free(temp); // we destroy temp and free the memory
    }
}

int lineIsImmidiate(Line *line){
  // if one of the registers is the imm register.
  if ((strcmp(IMM_REG, line->rs) == 0) || (strcmp(IMM_REG, line->rd) == 0) || (strcmp(IMM_REG, line->rt) == 0)){
    return TRUE;
  }
  return FALSE;
}

void lineCalculateLocation(Line *head)
{
  int counter = 0;
    Line *temp = head; //line pointer to the current line struct.  
    while (temp != NULL) // Destroy all of the list
    {
        temp->location = counter;
        if (lineIsImmidiate(temp)){
          counter += 2;
        }
        else{
          counter++;
        }
        temp = temp->next;
    }
}