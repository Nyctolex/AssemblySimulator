# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include "Label.c"
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
int add_label(char *line, int line_loc, Label *label_list)
    {
        char label [MAX_LINE_SIZE] = "";
        for (int i = 0; line[i] != ':'; i++) label[i] = line[i];
        labelAppendData(label_list, label, line_loc);
    }
int search_label(char *line, int line_index, int line_loc, Label *label_list) // iterate a line and check if there is a label in it
    {
        for (int i = 0; i < MAX_LINE_SIZE; i++)
            {
                if (line[i] == '\0') break;
                if (line[i] == ':')
                    {
                        add_label(line, line_loc, label_list);
                        return --line_loc;
                    }
            }
        if (is_imm(line)) line_loc++;
        return line_loc;
    }
void translate_file(char *line, int line_index, int line_loc, Label *label_list)
    {
        char var[MAX_LINE_SIZE] = "";
        int counter = 0;
        int len = 0;
        int hex;
        Label *label;
        for (int i = 0; i < MAX_LINE_SIZE; i++)
            {
                if (isspace(line[i])) continue;
                else if (line[i] == ':') return;
                else if (counter == 4 && isalpha(line[i]))
                    {
                        for (int j=i; !isspace(line[j]); j++)
                            {
                                var[len] = line[j];
                                var[++len] = '\0';
                            }
                        label = labelGetByName(label_list, var);
                        printf("\n%05X", label -> location);
                        break;
                    }
                else if (line[i] == ',' || line[i] == '$' || line[i] == '#')
                    {
                        if ((isdigit(var[0]) || (var[0] == '-')) && atoi(var) != 0) printf("\n%05X", atoi(var)&0x000FFFFF);
                        else if (is_imm(line) && atoi(var) == 0 && counter == 4) printf("\n%05X", atoi(var));
                        else if (strlen(var) != 0 && !isdigit(var[0]))
                            {
                                hex = compare(var);
                                if (counter == 0) printf("%02X", hex);
                                else printf("%01X", hex);
                                var[0] = '\0';
                                len = 0;
                                counter++;
                            }
                        if (line[i] == '#') break;
                    }
                else
                    {
                        var[len] = line[i];
                        var[++len] = '\0';
                    }
            }
        printf("\n");
    }
int iter_lines(FILE *fp, char iter_type, Label *label_list)
    {
        FILE *asm_file = fp;
        char line [MAX_LINE_SIZE];
        int line_index = 0;
        int line_loc = 0;
        while (fgets(line, MAX_LINE_SIZE, asm_file))
            {
                if (iter_type == GET_LABEL) line_loc = search_label(line, line_index, line_loc, label_list);
                else if (iter_type == FINAL_ITER) translate_file(line, line_index, line_loc, label_list);
                line_index++;
                line_loc++;
            } 
    }
int main()
    {
        FILE *asm_file = fopen("Fibonacci\\fib.asm", "r"); // ! dont forget to change file name
        if (asm_file)
            {
                Label *label_list = labelNewLabel("", -1);
                iter_lines(asm_file, GET_LABEL, label_list);
                fseek(asm_file, 0, 0);
                iter_lines(asm_file, FINAL_ITER, label_list);
                labelDeleteList(label_list);
            }
        fclose(asm_file);
        return 0;
    }