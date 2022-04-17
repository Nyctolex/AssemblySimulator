#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Label.h"
#include "RegisterConst.h"
Label *labelNewLabel(char name[], int location)
{
    Label *new_label = (Label *)malloc(sizeof(Label));
    if (new_label != NULL) // making sure the memory allocation succeed
    {
        strcpy(new_label->name, name);
        new_label->next = NULL;
        new_label->location = location;
    }
    return new_label;
}

Label *labelLast(Label *head)
{
    Label *temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    return temp;
}

void labelAppendNode(Label *head, Label *node)
{
    Label *tail = labelLast(head); // get the last node
    tail->next = node;
}

void labelAppendData(Label *head, char name[], int location)
{
    Label *new_tail = labelNewLabel(name, location); // creatin a new node
    labelAppendNode(head, new_tail);                 // appending it to the end of the list
}

Label *labelGetByIndex(Label *head, int index)
{
    if (index < 0) // making sure the index is valid
    {
        return NULL;
    }
    Label *temp = head;
    int i;
    for (i = 0; i < index; i++)
    {
        if (temp != NULL) // if we are not out of range
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

Label *labelGetByName(Label *head, char name[])
{
    Label *temp = head;
    int i;
    while (strcmp(temp->name, name))
    {

        temp = temp->next;
        if (temp == NULL) // while we are no out of range
        {
            break;
        }
    }
    return temp;
}
int labelListContains(Label *head, char name[])
{
    while (head != NULL)
    {
        if (strcmp(head->name, name) == 0)
        {
            return TRUE;
        }
        head = head->next;
    }
    return FALSE;
}

void labelDeleteList(Label *head)
{
    Label *temp;         // A pointer to a Label we are going to destroy after updating head
    while (head != NULL) // Destroy all of the list
    {
        temp = head; // temp <-- current node, head <-- head->next
        head = head->next;
        free(temp); // we destroy temp and free the memory
    }
}
void labelPrintList(Label *head)
{
    while (head != NULL)
    {
        printf("Name: %s\n  Location:%d\n", head->name, head->location);
        head = head->next;
    }
}