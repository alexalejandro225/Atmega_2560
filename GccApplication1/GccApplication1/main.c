/*
 * GccApplication1.c
 *
 * Created: 12/9/2020 1:33:07 PM
 * Author : aleja
 */ 

#include <avr/io.h>

extern void TXoPin(char in);

int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		TXoPin('1');
    }
	return 0;
}

