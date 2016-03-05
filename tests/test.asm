;
; test.asm
;

org 0x0040

; Variables
x: equ 5

y: equ 7

; code
ld a,x        ; copy 5 into the A register
ld b,y        ; copy 7 into the B register

add a,b       ; add the A and B registers (result stored to A)
ld (0x0010),a ; save the result to RAM at 0x0010

jp 0x0000     ; loop forever
