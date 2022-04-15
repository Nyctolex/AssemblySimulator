# include <stdio.h>
# include <string.h>
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
        // printf("%d, %d, %d: %s", line_index, line_loc, line_loc-line_index, line);
        return line_loc;
    }
int translate_file(char *line, int line_index, int line_loc)
    {
        char *var = "";
        int counter = 0;
        int hex;
        for (int i = 0; i < MAX_LINE_SIZE; i++) // check if line is only label
            {
                if (line[i] == '\0') break;
                if (line[i] == ':')
                    {
                        // printf("%s", line);
                        // ! search the label's val and output it
                    }
            }
        for (int i = 0; i < MAX_LINE_SIZE; i++)
            {
                if (line[i] == '\0' || line[i] == '#') break;
                else if (line[i] == ' ') continue;
                else if (line[i] == ',')
                    {
                        hex = compare(var);
                    }
                else
                    {
                        strcat(var, line[i]); // FIXME
                    }
            }
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