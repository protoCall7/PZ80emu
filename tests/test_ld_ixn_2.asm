ld ix,0x0070

ld a,0x01
ld b,0x02
ld c,0x03
ld d,0x04
ld e,0x05
ld h,0x06
ld l,0x07
	
ld (ix+0),a
ld (ix+1),b
ld (ix+2),c
ld (ix+3),d
ld (ix+4),e
ld (ix+5),h
ld (ix+6),l
