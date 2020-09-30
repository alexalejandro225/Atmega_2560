/*
 * car_alarm_v1.c
 *
 * Created: 7/17/2020 8:48:53 PM
 * Author : Aguilar Vea Alejandro
 * Aplication:
 */ 
#define F_CPU 16000000ul
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"


int main(void)
{
	UART_ini(0,9600,7,0,0,1);
	UART_ini(2,9600,7,0,0,1);
	char entrada_uart[64];
	

	
    /* Replace with your application code */
	
    while (1) 
    {	
		UART_puts(2,"\r\n");
		_delay_ms(1000);
		UART_puts(2,"\r\n");
		_delay_ms(1000);
		UART_puts(2,"AT+CSQ\r");
		_delay_ms(1000);
		UART_gets(2,entrada_uart);
		UART_puts(0,entrada_uart);	
	}
}

