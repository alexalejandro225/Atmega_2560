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
#include <string.h>

#define MAX 64
#define MOD(idx) ((idx+1)&(MAX-1))
#define IS_EMPTY(buffer) (buffer.UDR_HEAD == buffer.UDR_TAIL)
#define IS_FULL(buffer) (MOD(buffer.UDR_HEAD)==MOD(buffer.UDR_TAIL-1))
#define BAUD_RATE_CAL(FSOC,BAUD_RATE,U2X_IN) (FSOC/BAUD_RATE/U2X_IN)-1

#define Black 30
#define Red 31
#define Green 32
#define Yellow 33
#define Blue 34
#define Magenta 35
#define Cyan 36
#define White 37

typedef struct USART_MEM_RING
{
	uint8_t UDR_MEM[MAX];
	uint8_t UDR_HEAD;
	uint8_t UDR_TAIL;
}USART_MEM_RING;

volatile USART_MEM_RING TX_0;
volatile USART_MEM_RING RX_0;

char symbol[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

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
	
	*UART_begin|=(1<<RXCIE0|1<<RXEN0|1<<TXEN0|0<<UCSZ02);
	
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

void UART_puts( char *str)
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
		aux=UART_getchar();
		UART_putchar(aux);
		
		if(aux==8 && i>0 )
		{
			i--;
			str--;	
			UART_putchar(' ');
			UART_putchar(aux);
			
		}
		if(aux!=8 && i<10)
		{
			*(str++)=aux;
			i++; 
		}
		
	}while(aux!='\r');
	*str=0;

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

void UART_clrscr()
{
	UART_puts("\e[2J");
}

void UART_setColor(char color)
{
	char aux_color_screen[3];
	char set_screen[10];
	
	myItoa(color,10,aux_color_screen);
	strcat(set_screen,"\e[");
	strcat(set_screen,aux_color_screen);
	strcat(set_screen,"m");
	
	UART_puts(set_screen);
}

void UART_gotoxy(char x, char y)
{
	char aux_row[4];
	char aux_col[4];
	char screen_x_y[15];
	
	myItoa(x,10,aux_row);
	myItoa(y,10,aux_col);
	strcat(screen_x_y,"\e[");
	strcat(screen_x_y,aux_row);
	strcat(screen_x_y,";");
	strcat(screen_x_y,aux_col);
	strcat(screen_x_y,"H");
	UART_puts(screen_x_y);
}

void myItoa( int num,int base, char *salida)
{
	int j,i;
	char aux_string[16];
	i=0,j=0;

	if(num!=0){
		while(num){
			*(aux_string+i)=symbol[num%base];
			num=num/base;
			i++;
		}
		*(aux_string+i)=0;
		

		while(i)
		{
			i--;
			(*(salida+j))=aux_string[i];
			j++;

		}
		(*(salida+j))=0;
	}
	else
	{
		
		salida[0]='0';
		salida[1]=0;
		
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