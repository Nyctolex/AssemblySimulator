#ifndef HEADER_FILE
#define HEADER_FILE
#define TRUE 1
#define FALSE 0
typedef struct Label
{
    char name[50];      // Label's name
    struct Label *next; // a pointer to the next label
} label;

/**
 * @brief Gets a pointer to the head of the label list and it's name, and return the corresponding label.
 *
 * @param head
 * @param name
 * @return Label
 */
label *labelGetByName(label *head, char name[]);
label *labelGetByIndex(label *head, int index);
void labelAppendData(label *head, char name[]);
void labelAppendNode(label *head, label *node);
label *labelNewLabel(char name[]);
label *labelLast(label *head);
int labelListContains(label *head, char name[]);

#endif