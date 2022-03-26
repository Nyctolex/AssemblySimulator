#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Label.h"

label *labelNewLabel(char name[])
{
    label *new_label = (label *)malloc(sizeof(label));
    if (new_label != NULL)
    {
        strcpy(new_label->name, name);
        new_label->next = NULL;
    }
    return new_label;
}

label *labelLast(label *head)
{
    label *temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    return temp;
}

void labelAppendNode(label *head, label *node)
{
    label *tail = labelLast(head);
    tail->next = node;
}

void labelAppendData(label *head, char name[])
{
    label *new_tail = labelNewLabel(name);
    labelAppendNode(head, new_tail);
}

label *labelGetByIndex(label *head, int index)
{
    if (index < 0)
    {
        return NULL;
    }
    label *temp = head;
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

label *labelGetByName(label *head, char name[])
{
    label *temp = head;
    int i;
    while (strcmp(temp->name, name))
    {

        temp = temp->next;
        if (temp == NULL)
        {
            break;
        }
    }
    return temp;
}
int labelListContains(label *head, char name[])
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

void labelDeleteList(label *head)
{
    label *temp;         // A pointer to a label we are going to destroy after updating head
    while (head != NULL) // Destroy all of the list
    {
        temp = head; // temp <-- current node, head <-- head->next
        head = head->next;
        free(temp); // we destroy temp and free the memory
    }
}
int main()
{
    label *head = labelNewLabel("Alex");
    labelAppendData(head, "sivan");
    labelAppendData(head, "oz");
    labelAppendData(head, "noam");
    // label *temp = labelGetByName(head, "Alex");
    printf("%d", labelListContains(head, "noam"));
    printf("%s", head->name);
    labelDeleteList(head);
    return 0;
}