;
; Practica_6.asm
;
; Created: 4/2/2020 6:59:05 PM
; Author : alejandro aguilar vea
;
.INCLUDE "m1280def.inc"

; Replace with your application code

	ldi r30,1
	ldi r31,0
	
start:
	movw Y,Z
	ldi r16,7
rot_left_1:	
	lsl r28
	rol r29
	subi r16,1
	cpi r16,0
	brne rot_left_1 ;Shifting left 
;======================================================
	eor r31,r29
	eor r30,r28
;======================================================
	movw Y,Z
	ldi r16,9
rot_right_1:
	lsr r29
	ror r28
	subi r16,1
	cpi r16,0
	brne rot_right_1
;======================================================
	eor r31,r29
	eor r30,r28
;======================================================
	movw Y,Z
	ldi r16,8
rot_left_2:
	lsl r28
	rol r29
	subi r16,1
	cpi r16,0
	brne rot_left_2
;======================================================
	eor r31,r29
	eor r30,r28
;======================================================
	out PORTA,r30
	out PORTB,r31
    rjmp start
