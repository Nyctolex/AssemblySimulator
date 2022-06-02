# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include "Label.h"
# include "dict.h"

# define MAX_LINE_SIZE 501
# define GET_LABEL 0
# define TRANSLATE_ITER 1
# define MAX_MEMIN_SIZE 4096
# define TRUE 1
# define FALSE 0

int memin_loc = 0;

int is_imm(char *line) // checks if the line has an imm (not in the 1st reg)
    {
        int counter = 0;
        int jump = 0;
        for (int i = 0; i < MAX_LINE_SIZE; i++) // for every char in the line
            {
                if (i == 0)
                    {
                        if (isalpha(line[i])) counter++;
                    }
                else if (line[i] == '$' && isspace(line[i-1])) counter++;
                if (line[i] == 'b' && line[i+1] == 'e' && line[i+2] == 'q') jump=1; // if the op is beq
                if (line[i] == 'b' && line[i+1] == 'n' && line[i+2] == 'e') jump=1; // if the op is bne
                if (line[i] == 'b' && line[i+1] == 'l' && line[i+2] == 't') jump=1; // if the op is blt
                if (line[i] == 'b' && line[i+1] == 'g' && line[i+2] == 't') jump=1; // if the op is bgt
                if (line[i] == 'b' && line[i+1] == 'l' && line[i+2] == 'e') jump=1; // if the op is ble
                if (line[i] == 'b' && line[i+1] == 'g' && line[i+2] == 'e') jump=1; // if the op is bge
                if (line[i] == 's' && line[i+1] == 'w') jump=1; // if the op is sw
                if (line[i] == 'o' && line[i+1] == 'u' && line[i+2] == 't') jump=1; // if the op is out
                if (line[i] == 'i' && line[i+1] == 'm' && line[i+2] == 'm' && counter != 1) return TRUE;
                if (line[i] == 'i' && line[i+1] == 'm' && line[i+2] == 'm' && counter == 1 && jump == 1) return TRUE;
                if (line[i] == 'i' && line[i+1] == 'm' && line[i+2] == 'm' && counter == 1) return FALSE;
            }
        return FALSE;
    }
void add_label(char *line, int line_loc, Label *label_list) // add new label to the label list
    {
        char label [MAX_LINE_SIZE] = "";
        int counter = 0;
        for (int i = 0; line[i] != ':'; i++) // iterate until ":"
            {
                if (isspace(line[i])) counter++;
                else label[i-counter] = line[i];
            }
        labelAppendData(label_list, label, line_loc); // add label to list
    }
int search_label(char *line, int line_index, int line_loc, Label *label_list) // iterate a line and check if there is a label in it,
    {
        for (int i = 0; i < MAX_LINE_SIZE; i++)
            {
                if (line[i] == '\0' || line[i] == '\n') break;
                if (line[i] == ':')
                    {
                        add_label(line, line_loc, label_list); // if the line has ":" add the label to the list with it's location
                        return --line_loc;
                    }
            }
        if (is_imm(line)) line_loc++;
        return line_loc;
    }
void add_to_memin_str(char *temp_str, char *memin_str, int size, int place) // add string to the memin string
    {
        if (size == 5 || size == 2)
            {
                for (int i=0; i<size; i++)
                    {
                        memin_str[memin_loc*6+i] = temp_str[i]; // add the string to it's place
                    }
                if (size == 5) memin_loc++;
            }
        else if (size == 1)
            {
                memin_str[memin_loc*6+place+1] = temp_str[0]; // add the string to it's place
                if (place == 3) memin_loc++;
            }
    }
int extend_sign(int num) // extend sign of the number
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
void add_word(char *line, char *memin_str) // add .word data to memin
    {
        int line_loc;
        char line_val[MAX_LINE_SIZE];
        char temp_var[MAX_LINE_SIZE];
        int counter = 0;
        int j = 0;
        int hex = 0;
        for (int i = 0; i < MAX_LINE_SIZE; i++)
            {
                if (line[i] == '.') // skip the "word" and move to the rest of the line
                    {
                        i += 4;
                        continue;
                    }
                if (isalnum(line[i]))
                    {
                        if (line[i] == '0' && (line[i+1] == 'x' || line[i+1] == 'X')) // if the number is hex
                            hex = 1;
                        temp_var[j] = line[i]; // save the var's chars in string
                        if (isspace(line[i+1])) // when the number ends
                            {
                                if (counter == 0) // line place
                                    {
                                        if (hex == 1) line_loc = extend_sign(strtoul(temp_var, NULL, 16));
                                        else line_loc = atoi(temp_var);
                                        j = 0;
                                        counter++;
                                        hex = 0;
                                        continue;
                                    }
                                else if (counter == 1) // line data
                                    {
                                        temp_var[j+1] = '\0';
                                        if (hex == 1)
                                            {
                                                for (int k = 2; k < 7; k++) line_val[k - 2] = temp_var[k]; // remove "0x" from data
                                                int len = strlen(line_val);
                                                if (len < 5) // zero padding
                                                    {
                                                        for (int l=0; l<5-len; l++)
                                                            {
                                                                for (int k=0; k<5; k++)
                                                                    {
                                                                        line_val[5-k] = line_val[5-k-1];
                                                                    }
                                                            }
                                                        for (int k=0; k<5-len; k++)
                                                            {
                                                                line_val[k] = '0';
                                                            }
                                                    }
                                            }
                                        else sprintf(line_val, "%05X", atoi(temp_var));
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
void translate_file(char *line, int line_index, int line_loc, Label *label_list, FILE *memin, char *memin_str) // switch asm with it's opcode
    {
        char var[MAX_LINE_SIZE] = "";
        int counter = 0;
        int len = 0;
        int hex;
        int hex_num = 0;
        Label *label;
        char temp_str[7];
        char temp_char[2];
        int not_imm = 0;
        for (int i = 0; i < strlen(line); i++) // check if the imm is hex
            {
                if ((line[i] == '0') && (line[i+1] == 'x' || line[i+1] == 'X'))
                    {
                        hex_num = 1;
                        break;
                    }
            }
        for (int i = 0; i < strlen(line); i++) // check if line is empty
            {
                if (!(line[i] == ' '))
                    {
                        if (line[i] != '\n') break;
                        else return;
                    }
            }
        for (int i = 0; i < strlen(line); i++)
            {
                if (line[i] == ':') return; // skip label line
                else if (counter == 4 && isalpha(line[i]) && hex_num != 1) // translate label
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
                else if (line[i] == ',' || line[i] == '$' || line[i] == '#' || line[i] == '\n' || i == strlen(line)-1) // translate name
                    {
                        if (i == strlen(line)-1) // if the line is finished close the var
                            {
                                var[len] = line[i];
                                var[++len] = '\0';
                            }
                        if (var[0] == '0' && (var[1] == 'x' || var[1] == 'X') && not_imm == 0) // translate hex imm num
                            {
                                strcpy(temp_str, var+sizeof(char)*2);
                                int number = (int)strtoul(temp_str, NULL, 16);
                                sprintf(temp_str, "%05X", number&0x000FFFFF);
                                add_to_memin_str(temp_str, memin_str, 5, 0);
                                return;
                            }
                        else if ((isdigit(var[0]) || (var[0] == '-')) && atoi(var) != 0 && not_imm == 0) // translate dec imm num
                            {
                                sprintf(temp_str, "%05X", atoi(var)&0x000FFFFF);
                                add_to_memin_str(temp_str, memin_str, 5, 0);
                                return;
                            }
                        else if (is_imm(line) && atoi(var) == 0 && counter == 4 && not_imm == 0) // translate 0 imm num
                            {
                                sprintf(temp_str, "%05X", atoi(var));
                                add_to_memin_str(temp_str, memin_str, 5, 0);
                                return;
                            }
                        else if (strlen(var) != 0 && !isdigit(var[0])) // translate opcodes and regs
                            {
                                hex = compare(var);
                                if (hex == -1) // if the opcode is ".word"
                                    {
                                        add_word(line, memin_str);
                                        break;
                                    }
                                else
                                    {
                                        if (counter == 0) // translate opcode
                                            {
                                                sprintf(temp_str, "%02X", hex);
                                                add_to_memin_str(temp_str, memin_str, 2, 0);
                                            }
                                        else // translate reg
                                            {
                                                if ((hex == 1) && (counter == 1)) not_imm = 1;
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
void iter_lines(FILE *fp, char iter_type, Label *label_list, FILE *memin, char *memin_str) // iterate over all the lines in the file
    {
        FILE *asm_file = fp;
        char line [MAX_LINE_SIZE];
        int line_index = 0;
        int line_loc = 0;
        while (fgets(line, MAX_LINE_SIZE, asm_file))
            {
                if (line[0] == '\n') return;
                if (iter_type == GET_LABEL) line_loc = search_label(line, line_index, line_loc, label_list); // iterate and find all labels
                else if (iter_type == TRANSLATE_ITER) translate_file(line, line_index, line_loc, label_list, memin, memin_str); // iterate and translate to hex
                line_index++;
                line_loc++;
            } 
    }
void write_to_file(char *memin_str, FILE *memin) // write output to memin file
    {
        for (int i=0; i<MAX_MEMIN_SIZE*6-1; i++) fputc(memin_str[i], memin);
    }
int main(int arg_amount, char *arg_vals[])
    {
        FILE *asm_file = fopen(arg_vals[1], "r");
        if (asm_file)
            {
                char memin_str[MAX_MEMIN_SIZE*6+1];
                for (int i=0; i<MAX_MEMIN_SIZE*6; i++) // zero the memin_str, and put {Enter} after 5 chars
                    {
                        if (((i+1)%6) == 0) memin_str[i] = '\n';
                        else memin_str[i] = '0';
                    }
                memin_str[0] = '0';
                memin_str[MAX_MEMIN_SIZE*6] = '\0';
                FILE *memin = fopen(arg_vals[2], "w");
                Label *label_list = labelNewLabel("", -1);
                iter_lines(asm_file, GET_LABEL, label_list, memin, memin_str); // iterate and find all labels
                fseek(asm_file, 0, 0); // go to the beginning of the file
                iter_lines(asm_file, TRANSLATE_ITER, label_list, memin, memin_str); // iterate and translate to hex
                write_to_file(memin_str, memin); // write output to memin file
                fclose(memin);
                labelDeleteList(label_list); // delete all of the labels
                fclose(asm_file);
            }
        return 0;
    }
/*
    fixes:
        remove zeros?
        add comments
        add header file
        ! this:
            add $t0, $imm, $imm, 3
            add $imm, $zero, $imm, 3
            .word 5 0x2
            ! is printing 2E at line 6
        ? should .word overwrite the n or n+1 line ?
        ? is a commented line counted for labels ?
*/