/*
 * USART.c
 *
 * Created: 5/19/2020 4:24:50 PM
 *  Author: Aguilar Vea Alejandro
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define MAX 128
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
//-----------------------------//
volatile USART_MEM_RING TX_1;
volatile USART_MEM_RING RX_1;
//----------------------------//
volatile USART_MEM_RING TX_2;
volatile USART_MEM_RING RX_2;


int16_t myAtoi(int8_t* str)
{
	int16_t res = 0;
	
	int16_t i = 0;
	
	if(*str=='0')
	{
		return 0;
	}

	if((*str)==' ')
	{
		return 0;
	}
	
	for (i=0; str[i] != '\0' && str[i] >= 48 && str[i] <= 57  ; ++i)
	{
		res = res * 10 + str[i] - '0';
	}
	return  res;
}

void myItoa( uint16_t num,uint8_t base, char *salida)
{
	uint8_t j=0,i=0;
	char aux_string[20];
	static char symbol[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

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

void UART_putchar(uint8_t com,char dato)
{
	switch(com)
	{
		case 0:
		TX_0.UDR_MEM[TX_0.UDR_HEAD] = dato;
		TX_0.UDR_HEAD = MOD(TX_0.UDR_HEAD+1);
		UCSR0B |= (1<<UDRIE0);
		break;
		
		case 1:
		TX_1.UDR_MEM[TX_1.UDR_HEAD] = dato;
		TX_1.UDR_HEAD = MOD(TX_1.UDR_HEAD+1);
		UCSR1B |= (1<<UDRIE1);
		break;
		
		case 2:
		TX_2.UDR_MEM[TX_2.UDR_HEAD] = dato;
		TX_2.UDR_HEAD = MOD(TX_2.UDR_HEAD+1);
		UCSR2B |= (1<<UDRIE2);
		break;
	}
}

void UART_puts(uint8_t com,char *str)
{
	while (*str)
	{
		UART_putchar(com,*str);
		str++;
	}
}

uint8_t UART_getchar(uint8_t com)
{
	uint8_t aux;
	
	switch(com)
	{
		case 0:
		while (1)
		{
			if (!IS_EMPTY(RX_0))
			{
				aux = RX_0.UDR_MEM[RX_0.UDR_TAIL];
				
				RX_0.UDR_TAIL = MOD(RX_0.UDR_TAIL+1);
				return  aux;
			}
		}
		break;
		
		case 1:
		while (1)
		{
			if (!IS_EMPTY(RX_1))
			{
				aux = RX_1.UDR_MEM[RX_1.UDR_TAIL];
				
				RX_1.UDR_TAIL = MOD(RX_1.UDR_TAIL+1);
				return  aux;
			}
		}
		break;
		
		case 2:
		while (1)
		{
			if (!IS_EMPTY(RX_2))
			{
				aux = RX_2.UDR_MEM[RX_2.UDR_TAIL];
				
				RX_2.UDR_TAIL = MOD(RX_2.UDR_TAIL+1);
				return  aux;
			}
		}
		break;
	}
	return 0;
}

void UART_gets(uint8_t com,char *str)
{
	uint8_t aux;
	uint8_t i=0;
	
	do
	{
		aux=UART_getchar(com);
		if(aux==8 && i>0 )
		{
			i--;
			str--;
			UART_putchar(com,aux);
			UART_putchar(com,' ');
			UART_putchar(com,aux);
			
		}
		if(aux!=8 && i<20)
		{
			UART_putchar(com,aux);
			*(str++)=aux;
			i++;
		}
		
	}while(aux!='\r');
	*str=0;

}

uint8_t UART_Available(uint8_t com)
{
	switch(com)
	{
		case 0:
		if (IS_EMPTY(RX_0))
		{
			return 0;
		}
		else
		{
			return 1;
		}
		break;
		
		case 1:
		if (IS_EMPTY(RX_1))
		{
			return 0;
		}
		else
		{
			return 1;
		}
		
		break;
		
		case 2:
		if (IS_EMPTY(RX_2))
		{
			return 0;
		}
		else
		{
			return 1;
		}
		break;
	}
	return 1;
}

void UART_clrscr(uint8_t com)
{
	UART_puts(0,"\e[2J");
}

void UART_setColor(uint8_t com, uint8_t color)
{
	char aux_color_screen[3]={0};
	char set_screen[10]={0};
	
	myItoa(color,10,aux_color_screen);
	strcat(set_screen,"\e[");
	strcat(set_screen,aux_color_screen);
	strcat(set_screen,"m");
	
	UART_puts(com,set_screen);
}

void UART_gotoxy(uint8_t com, uint8_t x, uint8_t y)
{
	char aux_row[4]={0};
	char aux_col[4]={0};
	char screen_x_y[15]={0};
	
	myItoa(x,10,aux_col);
	myItoa(y,10,aux_row);
	strcat(screen_x_y,"\e[");
	strcat(screen_x_y,aux_row);
	strcat(screen_x_y,";");
	strcat(screen_x_y,aux_col);
	strcat(screen_x_y,"H");
	UART_puts(com,screen_x_y);
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

ISR(USART1_RX_vect)
{
	RX_1.UDR_MEM[RX_1.UDR_HEAD] = UDR1;
	RX_1.UDR_HEAD = MOD(RX_1.UDR_HEAD+1);
}

ISR(USART1_UDRE_vect)
{
	UDR1 = TX_1.UDR_MEM[TX_1.UDR_TAIL];
	TX_1.UDR_TAIL = MOD(TX_1.UDR_TAIL+1);
	if(TX_1.UDR_TAIL == TX_1.UDR_HEAD)
	{
		UCSR1B &= ~(1<<UDRIE1);
	}
}

ISR(USART2_RX_vect)
{
	RX_2.UDR_MEM[RX_2.UDR_HEAD] = UDR2;
	RX_2.UDR_HEAD = MOD(RX_2.UDR_HEAD+1);
}

ISR(USART2_UDRE_vect)
{
	UDR2 = TX_2.UDR_MEM[TX_2.UDR_TAIL];
	TX_2.UDR_TAIL = MOD(TX_2.UDR_TAIL+1);
	if(TX_2.UDR_TAIL == TX_2.UDR_HEAD)
	{
		UCSR2B &= ~(1<<UDRIE2);
	}
}
