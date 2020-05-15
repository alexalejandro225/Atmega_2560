/*
 * Atmega_2560_USART.c
 *
 * Created: 5/3/2020 6:37:06 PM
 * Author : Aguilar Vea Alejandro
 */ 

#include <avr/io.h>
#include "usart.h"

int main()
{
	char test[20];
	UART_ini(0,9600,7,0,1,1);
	UART_gotoxy(10,10);
	UART_setColor(Cyan);
	UART_clrscr();
	while(1)
	{
		if(UART_Available())
		{
			UART_gets(test);
		}
	}
	
	
	
}

