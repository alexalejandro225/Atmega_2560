/*
 * Timer_0_Atmega_2560.c
 *
 * Created: 5/17/2020 4:59:46 PM
 * Author : aleja
 */ 

#include <avr/io.h>
#include "Timer_0.h"
#include "USART.h"
#include <time.h>


void Clock_ini(uint64_t serial_number);
void Clock_Display();

int main(void)
{
	UART_ini(0,9600,7,0,1,1);
	UART_AutoBaudRate();
	Timer0_Ini();
	Clock_ini(1590017205000);
	
	while(1)
	{
		if(Timer0_SecFlag() ){ /* ¿ha pasado 1 Segundo? */
			/* instrucciones para encender LED */
			/* instrucciones para apagar LED */
			Clock_Display();
		}
	}
	
}

void Clock_ini(uint64_t serial_number)
{
	uint32_t aux=serial_number/1000;
	set_system_time(aux - UNIX_OFFSET);
	set_zone(-7 * ONE_HOUR);
	
}

void Clock_Display(){	char buf[80];	time_t rawtime,aux_rawtime;
	struct tm timeinfo;
	time(&rawtime);
	aux_rawtime=rawtime+(milis()/1000);
	timeinfo = *localtime (&aux_rawtime );
	strftime(buf, sizeof(buf),"%H:%M:%S %Y-%m-%d",&timeinfo);
	UART_clrscr(0);
	UART_puts(0,"\r");
	UART_puts(0,buf);	}
