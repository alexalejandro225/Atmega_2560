/*
 * Timer_0_Atmega_2560.c
 *
 * Created: 5/17/2020 4:59:46 PM
 * Author : aleja
 */ 

#include <avr/io.h>
#include "Timer_0.h"

int main(void)
{
	Timer0_Ini();
	
	while(1)
	{
		if( Timer0_SecFlag() ){ /* ¿ha pasado 1 Segundo? */
			/* instrucciones para encender LED */
			/* instrucciones para apagar LED */
		}
	}
	
}
