ld iy,0x0070

ld a,0x01
ld b,0x02
ld c,0x03
ld d,0x04
ld e,0x05
ld h,0x06
ld l,0x07
	
ld (iy+0),a
ld (iy+1),b
ld (iy+2),c
ld (iy+3),d
ld (iy+4),e
ld (iy+5),h
ld (iy+6),l
