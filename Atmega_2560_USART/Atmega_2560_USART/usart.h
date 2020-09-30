/*
 * usart.h
 *
 * Created: 5/3/2020 6:47:34 PM
 *  Author: Aguilar Vea Alejandro
 */ 


#ifndef USART_H_
#define USART_H_

#define Black 30
#define Red 31
#define Green 32
#define Yellow 33
#define Blue 34
#define Magenta 35
#define Cyan 36
#define White 37

int16_t myAtoi(char* str);
void myItoa( uint16_t num,uint8_t base, char *salida);
void UART_ini(uint8_t com,uint16_t baud_rate,uint8_t data_frame,uint8_t parity_bit,uint8_t double_speed,uint8_t stop_bit);
void UART_AutoBaudRate();
void UART_putchar(uint8_t com,char dato);
uint8_t UART_getchar(uint8_t com);
void UART_gets(uint8_t com,char *str);
uint8_t UART_Available(uint8_t com);
void UART_clrscr(uint8_t com);
void UART_setColor(uint8_t com, uint8_t color);
void UART_gotoxy(uint8_t com, uint8_t x, uint8_t y);
void UART_puts(uint8_t com,char *str);

#endif /* USART_H_ */