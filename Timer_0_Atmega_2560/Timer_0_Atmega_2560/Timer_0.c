/*
 * Timer_0.c
 *
 * Created: 5/19/2020 4:39:42 PM
 *  Author: aleja
 */ 

#include <avr/interrupt.h>
#include <inttypes.h>

static volatile uint8_t SecFlag;
static volatile uint64_t Counter_milis=0;


void Timer0_Ini ( void ){
	TCNT0=0x0; /* Inicializar valor para el timer0 */
	TCCR0A|=(0<<WGM00|1<<WGM01); /* inicializa timer0 en modo 0 (normal) */
	/* Inicializar con fuente de osc. Int. */
	TCCR0B|=(0<<WGM02|0<<CS02|1<<CS01|1<<CS00); /* con Prescalador 64 */
	TIMSK0|=(1<<OCIE0A); /* habilita interrupcion del Timer0 */
	OCR0A=0XF9;
	sei(); /* habilita interrupciones (global) */
}

uint8_t Timer0_SecFlag ( void ){

	if( SecFlag ){
		SecFlag=0;
		return 1;
	}
	else{
		return 0;
	}
}

uint64_t milis()
{
	return Counter_milis;
}



ISR (TIMER0_COMPA_vect){ /* TIMER0_OVF_vect */
	static uint16_t mSecCnt;
	mSecCnt++; /* Incrementa contador de milisegundos */
	Counter_milis++;
	if( mSecCnt==1000 ){
		mSecCnt=0;
		SecFlag=1; /* Bandera de Segundos */
	}
}