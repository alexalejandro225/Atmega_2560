/*
 * usart.h
 *
 * Created: 5/3/2020 6:47:34 PM
 *  Author: Aguilar Vea Alejandro
 */ 


#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define MAX 64
#define MOD(idx) ((idx+1)&(MAX-1))
#define IS_EMPTY(buffer) (buffer.UDR_HEAD == buffer.UDR_TAIL)
#define IS_FULL(buffer) (MOD(buffer.UDR_HEAD)==MOD(buffer.UDR_TAIL-1))
#define BAUD_RATE_CAL(FSOC,BAUD_RATE,U2X_IN) (FSOC/BAUD_RATE/U2X_IN)-1
 


typedef struct USART_MEM_RING
{
	uint8_t UDR_MEM[MAX];
	uint8_t UDR_HEAD;
	uint8_t UDR_TAIL;
}USART_MEM_RING;

USART_MEM_RING TX_0={{0},0,0},RX_0={{0},0,0};
	


void UART_ini(uint8_t com,uint16_t baud_rate,uint8_t data_frame,uint8_t parity_bit,uint8_t double_speed,uint8_t stop_bit)
{
	volatile uint8_t *UART_begin;	
	
	UART_begin=(volatile uint8_t *)(0xC0+(com*8));
	
	if(0==double_speed)
	{
		*UART_begin|=(0<<U2X0);
	}
	else
	{
		*UART_begin|=(1<<U2X0);
	}
	
	//-----------------------------------------------//
	(UART_begin++);
	
	*UART_begin|=(1<<RXEN0|1<<TXEN0|0<<UCSZ02);
	
	(UART_begin++);
	
	if (5==data_frame)
	{
		*UART_begin|=(0<<UCSZ01|0<<UCSZ00);
	}
	else if (6==data_frame)
	{
		*UART_begin|=(0<<UCSZ01|1<<UCSZ00);
	}
	else if (7==data_frame)
	{
		*UART_begin|=(1<<UCSZ01|0<<UCSZ00);
	}
	//------------------------------------------------//
	if(1==stop_bit)
	{
		*UART_begin|=(0<<USBS0);
	}
	else if (2==stop_bit)
	{
		*UART_begin|=(1<<USBS0);
	}
	//----------------------------------------------//
	if (0==parity_bit)
	{
		*UART_begin|=(0<<UPM01|0<<UPM00);
	}
	else if (1==parity_bit)
	{
		*UART_begin|=(1<<UPM01|1<<UPM00);
	}
	else if(2==parity_bit)
	{
		*UART_begin|=(1<<UPM01|0<<UPM00);
	}
	//---------------------------------------------//
	*UART_begin|=(0<<UMSEL00|0<<UMSEL01);
	
	(UART_begin+=2);
	
	if(0==double_speed)
	{
		*UART_begin=BAUD_RATE_CAL(16000000UL,baud_rate,16);
	}
	else
	{
		*UART_begin=BAUD_RATE_CAL(16000000UL,baud_rate,8);
	}
	
	sei();
	
}

void UART_AutoBaudRate(){
	
	TCCR0B = 0x02;
	while(PINE&((1<<PE0)));
	
	TCNT0=0;
	
	while(!(PINE&(1<<PE0)));
	TCCR0B = 0x00;
	UBRR0 = TCNT0-1;
}

void UART_putchar(char dato)
{
	TX_0.UDR_MEM[TX_0.UDR_HEAD] = dato;
	TX_0.UDR_HEAD = MOD(TX_0.UDR_HEAD+1);
	UCSR0B |= (1<<UDRIE0);
}

void UART_puts(uint8_t *str)
{
	while (*str)
	{
		UART_putchar(*str);
		str++;
	}
}

uint8_t UART_getchar()
{
	uint8_t aux;
	
	while (1)
	{
		if (!IS_EMPTY(RX_0))
		{
			aux = RX_0.UDR_MEM[RX_0.UDR_TAIL];
			RX_0.UDR_TAIL = MOD(RX_0.UDR_TAIL+1);
			return  aux;
		}
	}
}

void UART_gets(char *str)
{
	uint8_t aux;
	uint8_t i=0;
	do
	{
		aux = UART_getchar();
		
		if (aux == 8)
		{
			if (i>0)
			{	
				UART_putchar(aux);
				UART_putchar(' ');
				UART_putchar(aux);
				i--;	
			}
		}
		else
		{
			*str = aux;
			str++;	
		}
	} while (aux!=13);
	
}

uint8_t UART_Available()
{
	if (IS_EMPTY(RX_0))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

ISR(USART0_RX_vect)
{
	RX_0.UDR_MEM[RX_0.UDR_HEAD] = UDR0;
	RX_0.UDR_HEAD = MOD(RX_0.UDR_HEAD+1);
}

ISR(USART0_UDRE_vect)
{
	UDR0 = TX_0.UDR_MEM[TX_0.UDR_TAIL];
	TX_0.UDR_TAIL = MOD(TX_0.UDR_TAIL+1);
	if(TX_0.UDR_TAIL == TX_0.UDR_HEAD)
	{
		UCSR0B &= ~(1<<UDRIE0);
	}
}

#endif /* USART_H_ */