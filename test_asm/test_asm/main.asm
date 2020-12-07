;
; test_asm.asm
;
; Created: 12/1/2020 3:28:10 PM
; Author : aleja
;


; Replace with your application code
start:
    ldi r16,0x1E
	mov r0,r16
	ldi r16,1
	ldi r30,0x01
	ldi r31,0x00

	L1:
	st Z+,r16
	dec r0
	BRNE L1

	mov r31,r0
	nop


