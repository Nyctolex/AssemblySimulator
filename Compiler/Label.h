#ifndef LABEL_H
#define LABEL_H


// char arr2[NUM_STRINGS][MAX_LENGTH] = { {"first string"},
//                                           {"second string"},
//                                           {"third string"},
//                                           {"fourth string"},
//                                           {"fifth string"} };
/**
 * @brief Label is a data struct of a linked list. It has the atrribute of a string name, 
 * the location in the memory of which the Label is point to, and a pointer to the next node. 
 */
typedef struct Label
{
    char name[50];      // Label's name
    //location;**
    int location; // the location of the matching line
    struct Label *next; // a pointer to the next Label
} Label;

/**
 * @brief returing the Label with the coresponding name.
 *
 * @param head - the first node in the likned list.
 * @param name
 * @return Label
 */
Label *labelGetByName(Label *head, char name[]);


/**
 * @brief Get the node in the corresponding index
 * 
 * @param head - the first node in the likned list.
 * @param index 
 * @return Label* (in the matching index)
 */
Label *labelGetByIndex(Label *head, int index);

/**
 * @brief Creating a new node with the given parameters and appending it to the end of the linked list.
 * @param head - the first node in the likned list.
 * @param name - the name of the new Label. 
 * @param location - the memory location of which the Label is pointing to.
 */
void labelAppendData(Label *head, char name[], int location);


/**
 * @brief Append the given node to the end of the linked list.
 * @param head - the first node in the likned list.
 * @param node - the node to be appended.
 */
void labelAppendNode(Label *head, Label *node);


/**
 * @brief Creating a new Label node, and assigning it the given parameters.
 * @param name - the name of the new Label
 * @param location - the memory location of which the Label is pointing to.
 * @return Label* - a pointer to the new node.
 */
Label *labelNewLabel(char name[], int location);

/**
 * @brief Getting the last node in the list.
 * @param head - the first node in the likned list.
 * @return Label* - the last label in the list.
 */
Label *labelLast(Label *head);

/**
 * @brief Return True(1) if the list contains a node with the corresponding name, else returns false(0).
 * @param head - the first node in the likned list.
 * @param name - the name to be searched for.
 * @return int 
 */
int labelListContains(Label *head, char name[]);

/**
 * @brief Free a allocted memory of every node in the list. 
 * @param head - the first node in the likned list.
 */
void labelDeleteList(Label *head);

#endif