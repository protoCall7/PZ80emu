ld ix,0x1020
ld (ix+0),0x00
ld (ix+1),0x01
ld (ix+2),0x02
ld (ix+3),0x03
ld (ix+4),0x04
ld (ix+5),0x05
ld (ix+6),0x06
ld (ix+7),0x07

ld a,(ix+1)
ld b,(ix+2)
ld c,(ix+3)
ld d,(ix+4)
ld e,(ix+5)
ld h,(ix+6)
ld l,(ix+7)
