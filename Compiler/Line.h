#ifndef LINE_H
#define LINE_H
/**
 * @brief A data struct of liked list that of the lines in the assembly file.
 * 
 */
typedef struct line
{
    char opcode[8];
    char rd[4];
    char rs[4];
    char rt[4];
    char imm[50];
    int location; 
    struct line *next;
} Line;

/**
 * @brief Get the node in the corresponding index
 * 
 * @param head - - the first node in the likned list.
 * @param index 
 * @return Line* (in the matching index)
 */
Line *lineGetByIndex(Line *head, int index);

/**
 * @brief geting the Line with the corresponding location
 * 
 * @param head - the first node in the likned list.
 * @param location 
 * @return Line* 
 */
Line *lineGetByLocation(Line *head, int location);

/**
 * @brief Getting the last node in the list.
 * @param head - the first node in the likned list.
 * @return Line* 
 */
Line *lineLast(Line *head);

/**
 * @brief Appending the given node to the end of the given list.
 * @param head - the first node in the likned list.
 * @param node - the node to be appended.
 */
void lineAppendNode(Line *head, Line *node);

/**
 * @brief memory allocating space for a new "Line" data  and assiging it the given parameters.
 * Afterwards, appending it to the the given list.
 * @param head - the first node in the likned list.
 * @param opcode 
 * @param rd 
 * @param rs 
 * @param rt 
 * @param imm 
 * @param location 
 * @return Line* 
 */
Line *lineAppendData(Line *head, char opcode[], char rd[], char rs[], char rt[], char imm[], int location);

/**
 * @brief memory allocating space for a new "Line" data  and assiging it the given parameters.
 * @param opcode 
 * @param rd 
 * @param rs 
 * @param rt 
 * @param imm 
 * @param location 
 * @return Line* 
 */
Line *lineNewLine(char opcode[], char rd[], char rs[], char rt[], char imm[], int location);

/**
 * @brief Getting the last node in the given list.
 * 
 * @param head - - the first node in the likned list.
 * @return Line* 
 */
Line *lineLast(Line *head);

/**
 * @brief Free a allocted memory of every node in the list. 
 * @param head - the first node in the likned list.
 */
void lineDeleteList(Line *head);


/**
 * @brief returns if line is I type or R type.
 * 
 * @param line 
 * @return int - 1 for I type. 0 for R type.
 */
int lineIsImmidiate(Line *line);

/**
 * @brief Calculate the location of each line in the list.
 * @param head - the first node in the likned list.
 */
void lineCalculateLocation(Line *head);

#endif