/*
 * test.c
 *
 * Created: 9/3/2020 9:28:15 PM
 * Author : alejndro aguilar vea
 * software pwm formula f_cpu/(PREESCALER*RESOLUTION*OCRA)=HZ
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t ISRcounter = 0;
volatile uint8_t PB_Out[8]={0};
void timer1_ini();

int main(void)
{
	timer1_ini();
	PB_Out[7]=0;
    while (1) 
    {
    }
}

void timer1_ini()
{
	TCCR1A|=0x00;
	TCCR1B|=(1<<CS11)|(1<<WGM12);
	TIMSK1|=(1<<OCIE1A);
	OCR1A=156;
	sei();
	DDRB=0xFF;
	PORTB=0x00;
}

ISR(TIMER1_COMPA_vect)
{

for(uint8_t i=0; i<8; i++)
{
	if(ISRcounter < PB_Out[i]) 
	{
		PORTB|=(1<<i);
		
	}
	else 
	{
		PORTB &=~(1<<i);
	}
}

ISRcounter++;
	}

