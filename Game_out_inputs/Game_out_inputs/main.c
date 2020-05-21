/*
 * Practica_8_out_inputs.c
 *
 * Created: 4/28/2020 10:22:53 AM
 * Author : Aguilar Vea Alejandro
 */ 

#define F_CPU 16000000UL // Define clock frequency
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

typedef struct button
{
	uint8_t press_button;
	uint8_t flag_press;
	uint8_t push_pull;
}button;

void winner();
void looser();
void set_led(uint8_t num_led);
void matrix_keyboard(button* input_key);

char memory_simmon[8];
int count=0;



int main(void)
{

	button input_key={0,0,0};
    while (1) 
    {
		repeat:
		
		for (int i=0; i<8; i++)
		{
			srand (PINA+PINB+PINC);
			memory_simmon[i]=((rand() % (8-1+1))+1);
			set_led(memory_simmon[i]);
		}	
		
		for (int j=0; j<8; j++)
		{
			if (8==count)
			{
				while(1){
					matrix_keyboard(&input_key);
					if (input_key.flag_press!=0)
					{
						set_led(0);
						input_key.flag_press=0;
						count=0;
						goto repeat;
					}
					winner();
				}
			}
			
			for (int k=0; k<=j; k++)
			{
				set_led(memory_simmon[k]);
				_delay_ms(1000);
				set_led(0);
				_delay_ms(1000);	
			}
			
			for(int n=0; n<=j; n++)
			{
				
				
					while(input_key.flag_press==0)
					{
						matrix_keyboard(&input_key);
						_delay_ms(200);
					}
					input_key.flag_press=0;
					set_led(0);
					
					
					
					if(memory_simmon[n]!=input_key.press_button)
					{
						while(1){
							matrix_keyboard(&input_key);
						if (input_key.flag_press!=0)
						{
							set_led(0);
							input_key.flag_press=0;
							count=0;
							goto repeat;
						}
						looser();
						}
					}
			}
			count++;
			_delay_ms(300);
		}
			
    }
}

void matrix_keyboard(button* input_key)
{
	
	DDRB=0XFF;
	DDRC=0X00;
	PORTC=0XFF;	
	PORTB=0xFB;
	for(int i=3; i>0; i--)
	{
		if(PINC==0XFE)
		{
			set_led(i);
			_delay_ms(250);
			input_key->press_button=i;
			input_key->flag_press=1;
		}
		PORTB=PORTB>>1;
	}
	
	PORTB=0xFB;
	for(int i=6; i>3; i--)
	{
		if(PINC==0XFD)
		{
			set_led(i);
			_delay_ms(250);
			input_key->press_button=i;
			input_key->flag_press=1;
		}
		PORTB=PORTB>>1;
	}
	
	PORTB=0xFB;
	for(int i=9; i>6; i--)
	{
		if(PINC==0XFB)
		{
			set_led(i);
			_delay_ms(250);
			input_key->press_button=i;
			input_key->flag_press=1;
		}
		PORTB=PORTB>>1;
	}
}

void winner()
{
	for(int z=8; z>0; z--){
		for(int j=0; j<8890; j++)
		{
			for(int i=8; i>0; i--)
			{
				if(i!=z)
			{
				set_led(i);
				}	
			}
		}
	}
}

void looser()
{
	
		for(float j=0; j<15000; j++)
		{
			for(int i=8; i>4; i--)
			{
					set_led(i);
			}
		}
		
	for(float j2=0; j2<15000; j2++)
		{
			for(int i2=4; i2>0; i2--)
			{
					set_led(i2);
			}
		}
	
}

void set_led(uint8_t num_led)
{
	static uint8_t list_ddra[]={0x00,0x03,0x03,0x06,0x06,0x05,0x05,0x0C,0x0C};
	static uint8_t list_porta[]={0x00,0x02,0x01,0x04,0x02,0x01,0x04,0x08,0x04};
	DDRA=list_ddra[num_led];
	PORTA=list_porta[num_led];
}