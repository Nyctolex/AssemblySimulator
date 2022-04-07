# include <stdio.h>
# include <string.h>

# define MAX_LINE_SIZE 501
# define LABEL_ITER 0
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

int get_label(char *line, int line_loc)
    {
        char label [MAX_LINE_SIZE] = "";
        for (int i = 0; line[i] != ':'; i++) label[i] = line[i];
        // printf("%s %d\n", label, line_loc);
    }

int search_label(char *line, int line_index, int line_loc) // iterate a line and check if there is a label in it
    {
        for (int i = 0; i < MAX_LINE_SIZE; i++)
            {
                if (line[i] == '\0') break;
                if (line[i] == ':') 
                    {
                        get_label(line, line_loc);
                        return line_loc;
                    }
            }
        if (is_imm(line)) line_loc++;
        printf("%d, %d, %d: %s", line_index, line_loc, line_loc-line_index, line);
        return line_loc;
    }

int translate_file(char *line, int line_index, int line_loc)
    {
        
    }

int iter_file(FILE *fp, char iter_type)
    {
        FILE *asm_file = fp;
        char line [MAX_LINE_SIZE];
        int line_index = 0;
        int line_loc = 0;

        while (fgets(line, MAX_LINE_SIZE, asm_file))
            {
                if (iter_type == LABEL_ITER) line_loc = search_label(line, line_index, line_loc);
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
                iter_file(asm_file, LABEL_ITER);
            }
        fclose(asm_file);
        return 0;
    }