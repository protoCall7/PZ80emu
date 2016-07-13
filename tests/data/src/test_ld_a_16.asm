; set up registers
ld bc,0x0050
ld de,0x0060
ld a,0xFF

; load ram
ld (bc),a
ld (de),a
ld (0x0070),a
