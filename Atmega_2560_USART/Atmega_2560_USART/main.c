/*
 * Atmega_2560_USART.c
 *
 * Created: 5/3/2020 6:37:06 PM
 * Author : Aguilar Vea Alejandro
 */ 

#include <avr/io.h>
#include "usart.h"

int main(void)
{
	UART_ini(0,9600,7,0,0,1);
    /* Replace with your application code */
    while (1) 
    {
			UART_putchar('1');
    }
}

