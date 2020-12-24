
/*
 * TXoPin_.S
 *
 * Created: 12/9/2020 1:33:45 PM
 *  Author: aleja
 */ 
 ; Replace with your application code

#include <avr/io.h>



.global TXoPin 

	TXoPin:	

		mov r25,r24 
		out	DDRB,0x8
		out PORTB,0x8
		;************************************
		out PORTB,0x0
		call delay
		;***********************************
		ldi r20,0x8
		call left_move
		out PORTB,r24 
		mov r24,r25
		call delay
		;************************
		ldi r20,0x7
		call left_move
		out PORTB,r24 
		mov r24,r25
		call delay
		;************************
		ldi r20,0x6
		call left_move
		out PORTB,r24 
		mov r24,r25
		call delay
		;************************
		ldi r20,0x5
		call left_move
		out PORTB,r24 
		mov r24,r25
		call delay
		;************************
		ldi r20,0x4
		call left_move
		out PORTB,r24 
		mov r24,r25
		call delay
		;************************
		ldi r20,0x3
		call left_move
		out PORTB,r24 
		mov r24,r25
		call delay
		;************************
		ldi r20,0x2
		call left_move
		out PORTB,r24 
		mov r24,r25
		call delay
		;************************
		ldi r20,0x1
		call left_move
		out PORTB,r24 
		mov r24,r25
		call delay
		;************************
		andi r24,0x8
		out PORTB,r24 
		call delay
		;**************************
		ldi r24,0x8
		out PORTB,r24
		;**************************
		ret				;

delay:
	ldi  r18, 3
    ldi  r19, 40
L1: dec  r19
    brne L1
    dec  r18
    brne L1
    ret
 
left_move:
left:
	lsl r24
	dec r20
	brne left
	andi r24,0x8
	ret