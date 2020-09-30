/*
 * Atmega_2560_USART.c
 *
 * Created: 5/3/2020 6:37:06 PM
 * Author : Aguilar Vea Alejandro
 */ 

#include <avr/io.h>
#include "USART.h"

int main()
{
	char cad[40];
	uint16_t num;
	UART_ini(0,9600,7,0,0,1);
	
	
	while(1) {
		UART_getchar(0);
		UART_clrscr(0);
		UART_gotoxy(0,2,2);
		UART_setColor(0,Yellow); //Definirlo en UART.h
		UART_puts(0,"INTRODUCE UN NUMERO");
		UART_gotoxy(0,22,2);
		UART_setColor(0,Green); //Definirlo en UART.h
		UART_gets(0,cad);
		num = myAtoi(cad);
		myItoa(num,16,cad);
		UART_gotoxy(0,5,3);
		UART_setColor(0,Blue); //Definirlo en UART.h
		UART_puts(0,"HEX: ");
		UART_puts(0,cad);
		myItoa(num,2,cad);
		UART_gotoxy(0,5,4);
		UART_puts(0,"BIN: ");
		UART_puts(0,cad);

	}
}

