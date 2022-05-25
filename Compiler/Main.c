# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include "Label.h"
# include "dict.h "

# define MAX_LINE_SIZE 501
# define GET_LABEL 0
# define TRANSLATE_ITER 1
# define MAX_MEMIN_SIZE 4096
# define TRUE 1
# define FALSE 0

int memin_loc = 0;

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
        int counter = 0;
        for (int i = 0; line[i] != ':'; i++)
            {
                if (isspace(line[i])) counter++;
                else label[i-counter] = line[i];
            }
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
void add_to_memin_str(char *temp_str, char *memin_str, int size, int place)
    {
        if (size == 5 || size == 2)
            {
                for (int i=0; i<size; i++)
                    {
                        memin_str[memin_loc*6+i] = temp_str[i];
                    }
                if (size == 5) memin_loc++;
            }
        else if (size == 1)
            {
                memin_str[memin_loc*6+place+1] = temp_str[0];
                if (place == 3) memin_loc++;
            }
    }
int extend_sign(int num)
{
    int mask_extend = 0xFFF00000; //sign externtion
    int mask_msb = 0x80000; // mast to determine the msb
    int sign = mask_msb & num;
    //if the msb is not 0
    if (sign > 0) {

        num = mask_extend | num;
    }
    return num;
}
void add_word(char *line, char *memin_str)
    {
        int line_loc;
        char line_val[MAX_LINE_SIZE];
        char temp_var[MAX_LINE_SIZE];
        int counter = 0;
        int j = 0;
        int hex = 0;
        for (int i = 0; i < MAX_LINE_SIZE; i++)
            {
                if (line[i] == '.')
                    {
                        i += 4;
                        continue;
                    }
                if (isalnum(line[i]))
                    {
                        if (line[i] == '0' && line[i+1] == 'x')
                            hex = 1;
                        temp_var[j] = line[i];
                        if (isspace(line[i+1]))
                            {
                                if (counter == 0)
                                    {
                                        if (hex == 1) line_loc = extend_sign(strtoul(temp_var, NULL, 16));
                                        else line_loc = atoi(temp_var);
                                        j = 0;
                                        counter++;
                                        continue;
                                    }
                                else if (counter == 1)
                                    {
                                        temp_var[j+1] = '\0';
                                        sprintf(line_val, "%05X", atoi(temp_var));
                                        break;
                                    }
                            }
                        j++;
                    }
                if ((line[i] == '#') || (line[i] == '\n')) break;
                if (isspace(line[i])) continue;
            }
        int memin_loc_temp = memin_loc;
        memin_loc = line_loc;
        add_to_memin_str(line_val, memin_str, 5, 0);
        memin_loc = memin_loc_temp;
    }
void translate_file(char *line, int line_index, int line_loc, Label *label_list, FILE *memin, char *memin_str)
    {
        char var[MAX_LINE_SIZE] = "";
        int counter = 0;
        int len = 0;
        int hex;
        Label *label;
        char temp_str[6];
        char temp_char[2];
        for (int i = 0; i < strlen(line); i++)
            {
                if (line[i] == ':') return; // skip label line
                else if (counter == 4 && isalpha(line[i]) && !(line[i] == 'x' && line[i-1] == '0')) // translate label
                    {
                        for (int j=i; !isspace(line[j]); j++)
                            {
                                var[len] = line[j];
                                var[++len] = '\0';
                            }
                        label = labelGetByName(label_list, var);
                        sprintf(temp_str, "%05X", label -> location);
                        add_to_memin_str(temp_str, memin_str, 5, 0);
                        return;
                    }
                else if (line[i] == ',' || line[i] == '$' || line[i] == '#' || line[i] == '\n') // translate reg name
                    {
                        if (var[0] == '0' && var[1] == 'x') // FIXME
                            {
                                // printf("%s %s %d %s\n", temp_str, var, strlen(var), line);
                                strcpy(temp_str, var+sizeof(char)*2);
                                // printf("%s, %s\n", temp_str, var);
                                int number = (int)strtoul(temp_str, NULL, 16);
                                // printf("number: %d, var: %s\n", number, var);
                                sprintf(temp_str, "%05X", number&0x000FFFFF);
                                add_to_memin_str(temp_str, memin_str, 5, 0);
                                return;

                            }
                        else if ((isdigit(var[0]) || (var[0] == '-')) && atoi(var) != 0) // translate imm num
                            {
                                sprintf(temp_str, "%05X", atoi(var)&0x000FFFFF);
                                add_to_memin_str(temp_str, memin_str, 5, 0);
                                return;
                            }
                        else if (is_imm(line) && atoi(var) == 0 && counter == 4) // translate 0 imm num
                            {
                                sprintf(temp_str, "%05X", atoi(var));
                                add_to_memin_str(temp_str, memin_str, 5, 0);
                                return;
                            }
                        else if (strlen(var) != 0 && !isdigit(var[0]))
                            {
                                hex = compare(var);
                                if (hex == -1)
                                    {
                                        add_word(line, memin_str);
                                        break;
                                    }
                                else
                                    {
                                        if (counter == 0) // opcode
                                            {
                                                sprintf(temp_str, "%02X", hex);
                                                add_to_memin_str(temp_str, memin_str, 2, 0);
                                            }
                                        else // reg
                                            {
                                                sprintf(temp_char, "%01X", hex);
                                                add_to_memin_str(temp_char, memin_str, 1, counter);
                                            }
                                    }
                                var[0] = '\0';
                                len = 0;
                                counter++;
                            }
                        if ((line[i] == '#') || (line[i] == '\n')) break;
                    }
                else if (isspace(line[i])) continue;
                else
                    {
                        var[len] = line[i];
                        var[++len] = '\0';
                    }
            }
        return;
    }
int iter_lines(FILE *fp, char iter_type, Label *label_list, FILE *memin, char *memin_str)
    {
        FILE *asm_file = fp;
        char line [MAX_LINE_SIZE];
        int line_index = 0;
        int line_loc = 0;
        while (fgets(line, MAX_LINE_SIZE, asm_file))
            {
                if (line[0] == '\n') return;
                if (iter_type == GET_LABEL) line_loc = search_label(line, line_index, line_loc, label_list);
                else if (iter_type == TRANSLATE_ITER) translate_file(line, line_index, line_loc, label_list, memin, memin_str);
                line_index++;
                line_loc++;
            } 
    }
void write_to_file(char *memin_str, FILE *memin)
    {
        for (int i=0; i<MAX_MEMIN_SIZE*6-1; i++) fputc(memin_str[i], memin);
    }
int main(int arg_amount, char *arg_vals[])
    {
        FILE *asm_file = fopen(arg_vals[1], "r");
        if (asm_file)
            {
                char memin_str[MAX_MEMIN_SIZE*6+1];
                for (int i=0; i<MAX_MEMIN_SIZE*6; i++)
                    {
                        if (((i+1)%6) == 0) memin_str[i] = '\n';
                        else memin_str[i] = '0';
                    }
                memin_str[0] = '0';
                memin_str[MAX_MEMIN_SIZE*6] = '\0';
                FILE *memin = fopen(arg_vals[2], "w");
                Label *label_list = labelNewLabel("", -1);
                iter_lines(asm_file, GET_LABEL, label_list, memin, memin_str);
                fseek(asm_file, 0, 0);
                iter_lines(asm_file, TRANSLATE_ITER, label_list, memin, memin_str);
                write_to_file(memin_str, memin);
                fclose(memin);
                labelDeleteList(label_list);
                fclose(asm_file);
            }
        return 0;
    }
/*
    fixes:
        remove zeros?
        add comments
        add header file
        fix hex support (letters not working)
*/