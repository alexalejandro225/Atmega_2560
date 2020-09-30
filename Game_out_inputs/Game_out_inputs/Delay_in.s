
/*
 * Delay_in.s
 *
 * Created: 8/14/2020 12:56:43 AM
 *  Author: Aguilar Vea Alejandro
 */ 
 #include <avr/io.h>


.global delay_100_ms
 
 delay_100_ms:
    ldi  r18, 9
    ldi  r19, 30
    ldi  r20, 226
L2: dec  r20
    brne L2
    dec  r19
    brne L2
    dec  r18
    brne L2
	nop
	nop
	ret

.global delay_1_s

	delay_1_s:
    ldi  r18, 82
    ldi  r19, 43
    ldi  r20, 254
L3: dec  r20
    brne L3
    dec  r19
    brne L3
    dec  r18
    brne L3
	nop
	nop
	ret