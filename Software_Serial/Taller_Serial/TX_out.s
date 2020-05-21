
/*
 * Assembler1.s
 *
 * Created: 4/22/2020 10:08:03 AM
 *  Author: Aguilar Vea Alejandro
 */ 

 #include <avr/io.h>
 
.macro	tx_ini_trans
	ldi r18,0
	out pb7,r18
    ldi  r18, 3
    ldi  r19, 38
L1: dec  r19
    brne L1
    dec  r18
    brne L1
    nop
	nop
.endm

.macro tx_trans_byte
	ldi r20,7
TX_bit:	
	mov r18,r24
	andi r18,0x01
	lsr r24
	out pb7,r18
	;delay of transmission
	ldi  r18, 3
    ldi  r19, 38
L2: dec  r19
    brne L2
    dec  r18
    brne L2
	;branch to transmission again
	dec r20
	brne TX_bit
	;stop bit
	ldi r18,0x01
	out pb7,r18
.endm

.global TXopin

	TXopin:	
	tx_ini_trans
	tx_trans_byte
	ret	

