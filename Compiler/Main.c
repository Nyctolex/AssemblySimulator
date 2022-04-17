# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include "Label.h"
# include "dict.c"

# define MAX_LINE_SIZE 501
# define GET_LABEL 0
# define FINAL_ITER 1
# define TRUE 1
# define FALSE 0

int is_imm(char *line)
    {
        for (int i = 0; i < MAX_LINE_SIZE; i++)
            {
                if (line[i] == 'i' && line[i+1] == 'm' && line[i+2] == 'm') return TRUE;
            }
        return FALSE;
    }
int add_label(char *line, int line_loc)
    {
        char label [MAX_LINE_SIZE] = "";
        for (int i = 0; line[i] != ':'; i++) label[i] = line[i];
        printf("%s %d\n", label, line_loc);
        // labelAppendData()
        // ! append label
    }
int search_label(char *line, int line_index, int line_loc) // iterate a line and check if there is a label in it
    {
        for (int i = 0; i < MAX_LINE_SIZE; i++)
            {
                if (line[i] == '\0') break;
                if (line[i] == ':')
                    {
                        add_label(line, line_loc);
                        return line_loc;
                    }
            }
        if (is_imm(line)) line_loc++;
        return line_loc;
    }
int translate_file(char *line, int line_index, int line_loc)
    {
        char var[MAX_LINE_SIZE] = "";
        int counter = 0;
        int len = 0;
        int hex;
        for (int i = 0; i < MAX_LINE_SIZE; i++)
            {
                if (isspace(line[i])) continue;
                else if (counter == 4 && isalpha(line[i]) || line[i] == ':')
                    {
                        // printf("label");
                        break;
                        // ! take care of label
                    }
                else if (line[i] == ',' || line[i] == '$' || line[i] == '#')
                    {
                        if (strlen(var) != 0 && !isdigit(var[0]))
                            {
                                hex = compare(var);
                                // printf("%x", hex);
                                var[0] = '\0';
                                len = 0;
                                counter++;
                            }
                        // else if (isdigit(var[0])) printf("\n%s", var);
                        if (line[i] == '#') break;
                    }
                else
                    {
                        var[len] = line[i];
                        var[++len] = '\0';
                    }
            }
        // printf("\n");
    }
int iter_lines(FILE *fp, char iter_type)
    {
        FILE *asm_file = fp;
        char line [MAX_LINE_SIZE];
        int line_index = 0;
        int line_loc = 0;

        while (fgets(line, MAX_LINE_SIZE, asm_file))
            {
                if (iter_type == GET_LABEL) line_loc = search_label(line, line_index, line_loc);
                else if (iter_type == FINAL_ITER) translate_file(line, line_index, line_loc);
                line_index++;
                line_loc++;
            } 
    }
int main()
    {
        FILE *asm_file = fopen("Fibonacci\\fib.asm", "r"); // ! dont forget to change file name
        if (asm_file)
            {
                iter_lines(asm_file, GET_LABEL);
                fseek(asm_file, 0, 0);
                iter_lines(asm_file, FINAL_ITER);
            }
        fclose(asm_file);
        return 0;
    }