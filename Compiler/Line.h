#ifndef HEADER_FILE
#define HEADER_FILE
#define TRUE 1
#define FALSE 0
typedef struct Line
{

    char opcode[8];

    char rd[4];

    char rs[4];
    char rt[4];
    char imm_reg[50];
    char imm[50];
    int location; // Line's location (refering to index, not PC)
    int index;
    struct Line *next;
} line;

line *lineGetByIndex(line *head, int index);
line *lineGetByLocation(line *head, int location);
line *lineLast(line *head);
void lineAppendNode(line *head, line *node);
line *lineAppendData(line *head, char opcode[], char rd[], char rs[], char rt[], char imm_reg[], char imm[], int location);
line *lineNewLine(char opcode[], char rd[], char rs[], char rt[], char imm_reg[], char imm[], int location);
line *lineLast(line *head);

#endif