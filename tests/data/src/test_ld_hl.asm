; set up registers with initial values
ld a,0x01
ld bc,0x0203
ld de,0x0405

; load 0x01 into memory address 0x0060
ld hl,0x0060
ld (hl),a

; load 0x02 into memory address 0x0061
ld hl,0x0061
ld (hl),b

; load 0x03 into memory address 0x0062
ld hl,0x0062
ld (hl),c

; load 0x04 into memory address 0x0063
ld hl,0x0063
ld (hl),d

; load 0x05 into memory address 0x0064
ld hl,0x0064
ld (hl),e

; load 0x00 into memory address 0x0065
ld hl,0x0065
ld (hl),h

; load 0x55 into memory address 0x0065
ld (hl),0x55

; load 0x56 into memory address 0x0066
ld hl,0x0066
ld (hl),l

; load 0x05 into register a
ld hl,0x0064
ld a,(hl)

; load 0x0400 into register bc
ld hl,0x0063
ld b,(hl)
ld hl,0x0065
ld c,(hl)

; load 0x0201 into register de
ld hl,0x0061
ld d,(hl)
ld hl,0x0060
ld e,(hl)

; load 0x5656 into register hl
ld hl,0x0066
ld l,(hl)
ld h,(hl)
