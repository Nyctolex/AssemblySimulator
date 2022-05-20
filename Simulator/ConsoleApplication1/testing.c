#include <stdio.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS
int main2(int argc, char* argv[])
{
	int mask_extend = 0xFFF00000;
	int mask_msb = 0x80000;
	char* tmp = "FFFFF";
	int i = (int)strtoul(tmp, NULL, 16);
	int sign = mask_msb & i;
	//if the msb is not 0
	if (sign > 0) {
	
		i = mask_extend | i;
	}
	return 0;
}