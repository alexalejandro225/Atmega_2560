;
; Pin_UART.asm
;
; Created: 12/9/2020 1:20:38 PM
; Author : aleja
;


; Replace with your application code
.MACRO delay_9600
	ldi  r18, 3
    ldi  r19, 40
L1: dec  r19
    brne L1
    dec  r18
    brne L1
    nop
.ENDMACRO 

#include <avr/io.h>

.global TXoPin 

	TXoPin:			
						;r24 use 
		LDI	R25,0x80	;
		STS	CLKPR,R25	;enable updating of CLKPR						
		STS	CLKPR,R24	;write the new value
		ret				;


