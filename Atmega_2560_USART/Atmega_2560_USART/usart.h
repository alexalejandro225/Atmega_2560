/*
 * usart.h
 *
 * Created: 5/3/2020 6:47:34 PM
 *  Author: Aguilar Vea Alejandro
 */ 


#ifndef USART_H_
#define USART_H_

#define BUFFER_MAX_SIZE 30
#define TX_MAX_CHECK(TX_BUFFER)
#define RX_MAX_CHECK(RX_BUFFER)
#define BAUD_RATE_CAL(FSOC,BAUD_RATE,U2X_IN) (FSOC/BAUD_RATE/U2X_IN)-1 


typedef struct USART_MEM_RING
{
	uint8_t UDR_MEM[BUFFER_MAX_SIZE];
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
	
	
	
}

void UART_autobaud_rate(uint8_t com)
{
	
}

void UART_putchar(uint8_t data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void UART_puts(uint8_t *data)
{
	
}

void UART_gets()
{
	
}

void UART_getchar()
{
	
}

void UART_available()
{
	
}

#endif /* USART_H_ */