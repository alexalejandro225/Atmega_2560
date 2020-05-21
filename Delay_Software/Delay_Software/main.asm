;
; Practica_7_Delay_Software.asm
;
; Created: 4/17/2020 4:05:22 PM
; Author : alejandro aguilar vea
;


;------------- definiciones e includes ------------------------------
.INCLUDE "m1280def.inc"
.equ INIT_VALUE = 0x80

;------------- init PORTS -------------------------------------------
ldi R24,INIT_VALUE
out DDRB,R24
;------------- main loop -------------------------------------------
next:
call delay_103_us;
out PINB,R24
rjmp next

delay_103_us:
    ldi  r18, 3
    ldi  r19, 32
L1: dec  r19
    brne L1
    dec  r18
    brne L1
    nop
    ret

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