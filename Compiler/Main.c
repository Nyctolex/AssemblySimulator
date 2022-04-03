#include<stdio.h>

int main()
    {
        FILE *memin = NULL;
        char num;
        memin = fopen("Fibonacci\\fib.asm", "r");
        if (memin)
            {
                num = fgetc(memin);
                while(num != EOF)
                    {
                        printf("%c", num);
                        num = fgetc(memin);
                    }
            }
        fclose(memin);
        return 0;
    }