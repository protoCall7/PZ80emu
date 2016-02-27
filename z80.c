/** \file */
//
//  z80.c
//  PZ80emu
//
//  Created by Peter Ezetta on 5/3/15.
//  Copyright (c) 2015 Peter Ezetta. All rights reserved.
//

#include "z80.h"
#include <stdlib.h>
#include <stdint.h>

z80 *new_cpu() {
    z80 *cpu;
    
    if ((cpu = calloc(1, sizeof(z80))) == NULL) {
        exit(EXIT_FAILURE);
    }
    
    return cpu;
}

void reset_cpu(z80 *cpu) {
    // need to implement interrupt resetting here
    cpu->pc.W = 0x0000;
}

int run(z80 *cpu, uint8_t *memory, long runcycles) {
    int count = 0;
    // number of T cycles for each opcode
    const int cycles[15] = {4, 10, 7, 6, 4, 4, 7, 4, 4, 11, 7, 6, 4, 4, 7};
    
    do {
        uint8_t opcode = memory[cpu->pc.W++]; // fetch next opcode from memory
        cpu->counter -= cycles[opcode]; // decrease the interrupt counter by number of cycles for opcode
        count++;
        
        // opcode interpreter switch
        switch (opcode) {
            // extended instruction set 0xDDxx
            case 0xDD:
                opcode = memory[cpu->pc.W++];
                
                switch(opcode) {
                    case 0x7E:
                        // ld a,(ix+n)
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            cpu->a = memory[(index + cpu->ix.W)];
                        }
                        break;

                    case 0x46:
                        // ld b,(ix+n)
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            cpu->bc.B.h = memory[(index + cpu->ix.W)];
                        }
                        break;

                    case 0x4E:
                        // ld c,(ix+n)
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            cpu->bc.B.l = memory[(index + cpu->ix.W)];
                        }
                        break;

                    case 0x56:
                        // ld d,(ix+n)
                        {
                            uint8_t index;
                            index= memory[cpu->pc.W++];

                            cpu->de.B.h = memory[(index + cpu->ix.W)];
                        }
                        break;

                    case 0x5E:
                        // ld e,(ix+n)
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            cpu->de.B.l = memory[(index + cpu->ix.W)];
                        }
                        break;

                    case 0x66:
                        // ld h,(ix+n)
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            cpu->hl.B.h = memory[(index + cpu->ix.W)];
                        }
                        break;

                    case 0x6E:
                        // ld l,(ix+n)
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            cpu->hl.B.l = memory[(index + cpu->ix.W)];
                        }
                        break;

                    case 0x77:
                        // ld (ix+n),a
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->ix.W)] = cpu->a;
                        }
                        break;

                    case 0x70:
                        // ld (ix+n),b
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->ix.W)] = cpu->bc.B.h;
                        }
                        break;

                    case 0x71:
                        // ld (ix+n),c
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->ix.W)] = cpu->bc.B.l;
                        }
                        break;

                    case 0x72:
                        // ld (ix+n),d
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->ix.W)] = cpu->de.B.h;
                        }
                        break;

                    case 0x73:
                        // ld (ix+n),e
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->ix.W)] = cpu->de.B.l;
                        }
                        break;

                    case 0x74:
                        // ld (ix+n),h
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->ix.W)] = cpu->hl.B.h;
                        }
                        break;

                    case 0x75:
                        // ld (ix+n),l
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->ix.W)] = cpu->hl.B.l;
                        }
                        break;

                    case 0x36:
                        // ld (ix+n),n
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->ix.W)] = memory[cpu->pc.W++];
                        }
                        break;
                    case 0x21:
                        // ld ix,nn
                        cpu->ix.B.h = memory[cpu->pc.W++];
                        cpu->ix.B.l = memory[cpu->pc.W++];
                        break;
                }
                break;
        
            // extended instruction set 0xFDxx
            case 0xFD:
                opcode = memory[cpu->pc.W++];

                switch(opcode) {
                    case 0x7E:
                        // ld a,(iy+n)
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            cpu->a = memory[(index + cpu->iy.W)];
                        }
                        break;

                    case 0x46:
                        // ld b,(iy+n)
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            cpu->bc.B.h = memory[(index + cpu->iy.W)];
                        }
                        break;

                    case 0x4E:
                        // ld c,(iy+n)
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            cpu->bc.B.l = memory[(index + cpu->iy.W)];
                        }
                        break;

                    case 0x56:
                        // ld d,(iy+n)
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            cpu->de.B.h = memory[(index + cpu->iy.W)];
                        }
                        break;

                    case 0x5E:
                        // ld e,(iy+n)
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            cpu->de.B.l = memory[(index + cpu->iy.W)];
                        }
                        break;

                    case 0x66:
                        // ld h,(iy+n)
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            cpu->hl.B.h = memory[(index + cpu->iy.W)];
                        }
                        break;

                    case 0x6E:
                        // ld l,(iy+n)
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            cpu->hl.B.l = memory[(index + cpu->iy.W)];
                        }
                        break;

                    case 0x77:
                        // ld (iy+n),a
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->iy.W)] = cpu->a;
                        }
                        break;

                    case 0x70:
                        // ld (iy+n),b
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->iy.W)] = cpu->bc.B.h;
                        }
                        break;

                    case 0x71:
                        // ld (iy+n),c
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->iy.W)] = cpu->bc.B.l;
                        }
                        break;

                    case 0x72:
                        // ld (iy+n),d
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->iy.W)] = cpu->de.B.h;
                        }
                        break;

                    case 0x73:
                        // ld (iy+n),e
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->iy.W)] = cpu->de.B.l;
                        }
                        break;

                    case 0x74:
                        // ld (iy+n),h
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->iy.W)] = cpu->hl.B.h;
                        }
                        break;

                    case 0x75:
                        // ld (iy+n),l
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->iy.W)] = cpu->hl.B.l;
                        }
                        break;

                    case 0x36:
                        // ld (iy+n),n
                        {
                            uint8_t index;
                            index = memory[cpu->pc.W++];

                            memory[(index + cpu->iy.W)] = memory[cpu->pc.W++];
                        }
                        break;

                    case 0x21:
                        // ld iy,nn
                        cpu->iy.B.h = memory[cpu->pc.W++];
                        cpu->iy.B.l = memory[cpu->pc.W++];
                        break;
                }
                break;

            case 0x00:
                // nop
                break;
                
            // 8-bit transfer instructions
            case 0x0A:
                // ld a,(bc)
                cpu->a = memory[cpu->bc.W];
                break;
  
            case 0x7F:
                // ld a,a
                cpu->a = cpu->a;
                break;

            case 0x78:
                // ld a,b
                cpu->a = cpu->bc.B.h;
                break;

            case 0x79:
                // ld a,c
                cpu->a = cpu->bc.B.l;
                break;

            case 0x7A:
                // ld a,d
                cpu->a = cpu->de.B.h;
                break;

            case 0x7B:
                // ld a,e
                cpu->a = cpu->de.B.l;
                break;

            case 0x7C:
                // ld a,h
                cpu->a = cpu->hl.B.h;
                break;

            case 0x7D:
                // ld a,l
                cpu->a = cpu->hl.B.l;
                break;

            case 0x7E:
                // ld a,(hl)
                cpu->a = memory[cpu->hl.W];
                break;

            case 0x1A:
                // ld a,(de)
                cpu->a = memory[cpu->de.W];
                break;

            case 0x3A:
                // ld a,(nn)
                {
                    word nn;
                    nn.B.h = memory[cpu->pc.W++];
                    nn.B.l = memory[cpu->pc.W++];
                    cpu->a = memory[nn.W]; 
                }
                break;

            case 0x47:
                // ld b,a
                cpu->bc.B.h = cpu->a;
                break;

            case 0x40:
                // ld b,b
                cpu->bc.B.h = cpu->bc.B.h;
                break;

            case 0x41:
                // ld b,c
                cpu->bc.B.h = cpu->bc.B.l;
                break;

            case 0x42:
                // ld b,d
                cpu->bc.B.h = cpu->de.B.h;
                break;

            case 0x43:
                // ld b,e
                cpu->bc.B.h = cpu->de.B.l;
                break;

            case 0x44:
                // ld b,h
                cpu->bc.B.h = cpu->hl.B.h;
                break;

            case 0x45:
                // ld b,l
                cpu->bc.B.h = cpu->hl.B.l;
                break;

            case 0x46:
                // ld b,(hl)
                {
                    word address;
                    address.B.h = cpu->hl.B.h;
                    address.B.l = cpu->hl.B.l;
                    cpu->bc.B.h = memory[address.W];
                }
                break;

            case 0x4F:
                // ld c,a
                cpu->bc.B.l = cpu->a;
                break;

            case 0x48:
                // ld c,b
                cpu->bc.B.l = cpu->bc.B.h;

            case 0x49:
                // ld c,c
                cpu->bc.B.l = cpu->bc.B.l;
                break;

            case 0x4A:
                // ld c,d
                cpu->bc.B.l = cpu->de.B.h;
                break;

            case 0x4B:
                // ld c,e
                cpu->bc.B.l = cpu->de.B.l;
                break;

            case 0x4C:
                // ld c,h
                cpu->bc.B.l = cpu->hl.B.h;
                break;

            case 0x4D:
                // ld c,l
                cpu->bc.B.l = cpu->hl.B.l;
                break;

            case 0x4E:
                // ld c,(hl)
                {
                    word address;
                    address.B.h = cpu->hl.B.h;
                    address.B.l = cpu->hl.B.l;
                    cpu->bc.B.l = memory[address.W];
                }
                break;

            case 0x57:
                // ld d,a
                cpu->de.B.h = cpu->a;
                break;

            case 0x50:
                // ld d,b
                cpu->de.B.h = cpu->bc.B.h;
                break;

            case 0x51:
                // ld d,c
                cpu->de.B.h = cpu->bc.B.l;
                break;

            case 0x52:
                // ld d,d
                cpu->de.B.h = cpu->de.B.h;
                break;

            case 0x53:
                // ld d,e
                cpu->de.B.h = cpu->de.B.l;
                break;

            case 0x54:
                // ld d,h
                cpu->de.B.h = cpu->hl.B.h;
                break;

            case 0x55:
                // ld d,l
                cpu->de.B.h = cpu->hl.B.l;
                break;

            case 0x56:
                // ld d,(hl)
                {
                    word address;
                    address.B.h = cpu->hl.B.h;
                    address.B.l = cpu->hl.B.l;
                    cpu->de.B.h = memory[address.W];
                }
                break;

            case 0x5F:
                // ld e,a
                cpu->de.B.l = cpu->a;
                break;

            case 0x58:
                // ld e,b
                cpu->de.B.l = cpu->bc.B.h;
                break;

            case 0x59:
                // ld e,c
                cpu->de.B.l = cpu->bc.B.l;
                break;

            case 0x5A:
                // ld e,d
                cpu->de.B.l = cpu->de.B.h;
                break;

            case 0x5B:
                // ld e,e
                cpu->de.B.l = cpu->de.B.l;
                break;

            case 0x5C:
                // ld e,h
                cpu->de.B.l = cpu->hl.B.h;
                break;

            case 0x5D:
                // ld e,l
                cpu->de.B.l = cpu->hl.B.l;
                break;

            case 0x5E:
                // ld e,(hl)
                {
                    word address;
                    address.B.h = cpu->hl.B.h;
                    address.B.l = cpu->hl.B.l;
                    cpu->de.B.l = memory[address.W];
                }
                break;

            case 0x67:
                // ld h,a
                cpu->hl.B.h = cpu->a;
                break;

            case 0x60:
                // ld h,b
                cpu->hl.B.h = cpu->bc.B.h;
                break;

            case 0x61:
                // ld h,c
                cpu->hl.B.h = cpu->bc.B.l;
                break;

            case 0x62:
                // ld h,d
                cpu->hl.B.h = cpu->de.B.h;
                break;

            case 0x63:
                // ld h,e
                cpu->hl.B.h = cpu->de.B.l;
                break;

            case 0x64:
                // ld h,h
                cpu->hl.B.h = cpu->hl.B.h;
                break;

            case 0x65:
                // ld h,l
                cpu->hl.B.h = cpu->hl.B.l;
                break;

            case 0x66:
                // ld h,(hl)
                {
                    word address;
                    address.B.h = cpu->hl.B.h;
                    address.B.l = cpu->hl.B.l;
                    cpu->hl.B.h = memory[address.W];
                }
                break;

            case 0x6F:
                // ld l,a
                cpu->hl.B.l = cpu->a;
                break;

            case 0x68:
                // ld l,b
                cpu->hl.B.l = cpu->bc.B.h;
                break;

            case 0x69:
                // ld l,c
                cpu->hl.B.l = cpu->bc.B.l;
                break;

            case 0x6A:
                // ld l,d
                cpu->hl.B.l = cpu->de.B.h;
                break;

            case 0x6B:
                // ld l,e
                cpu->hl.B.l = cpu->de.B.l;
                break;

            case 0x6C:
                // ld l,h
                cpu->hl.B.l = cpu->hl.B.h;
                break;

            case 0x6D:
                // ld l,l
                cpu->hl.B.l = cpu->hl.B.l;
                break;

            case 0x6E:
                // ld l,(hl)
                {
                    word address;
                    address.B.h = cpu->hl.B.h;
                    address.B.l = cpu->hl.B.l;
                    cpu->hl.B.l = memory[address.W];
                }
                break;

            case 0x77:
                // ld (hl),a
                memory[cpu->hl.W] = cpu->a;
                break;

            case 0x70:
                // ld (hl),b
                memory[cpu->hl.W] = cpu->bc.B.h;
                break;

            case 0x71:
                // ld (hl),c
                memory[cpu->hl.W] = cpu->bc.B.l;
                break;

            case 0x72:
                // ld (hl),d
                memory[cpu->hl.W] = cpu->de.B.h;
                break;

            case 0x73:
                // ld (hl),e
                memory[cpu->hl.W] = cpu->de.B.l;
                break;

            case 0x74:
                // ld (hl),h
                memory[cpu->hl.W] = cpu->hl.B.h;
                break;

            case 0x75:
                // ld (hl),l
                memory[cpu->hl.W] = cpu->hl.B.l;
                break;

            case 0x3E:
                // ld a,n
                cpu->a = memory[cpu->pc.W++];
                break;

            case 0x06:
                // ld b,n
                cpu->bc.B.h = memory[cpu->pc.W++];
                break;

            case 0x0E:
                // ld c,n
                cpu->bc.B.l = memory[cpu->pc.W++];
                break;

            case 0x16:
                // ld d,n
                cpu->de.B.h = memory[cpu->pc.W++];
                break;

            case 0x1E:
                // ld e,n
                cpu->de.B.l = memory[cpu->pc.W++];
                break;

            case 0x26:
                // ld h,n
                cpu->hl.B.h = memory[cpu->pc.W++];
                break;

            case 0x2E:
                // ld l,n
                cpu->hl.B.l = memory[cpu->pc.W++];
                break;

            case 0x36:
                // ld (hl),n
                cpu->hl.W = memory[cpu->pc.W++];
                break;

            case 0x02:
                // ld (bc),a
                memory[cpu->bc.W] = cpu->a;
                break;

            case 0x12:
                // ld (de),a
                memory[cpu->de.W] = cpu->a;

            case 0x32:
                // ld (nn),a
                {
                    word address;
                    address.B.h = memory[cpu->pc.W++];
                    address.B.l = memory[cpu->pc.W++];

                    memory[address.W] = cpu->a;
                }
                break;

            // 16-bit transfer instructions
            case 0x01:
                // ld bc,nn
                cpu->bc.B.h = memory[cpu->pc.W++];
                cpu->bc.B.l = memory[cpu->pc.W++];
                break;

            case 0x11:
                // ld de,nn
                cpu->de.B.h = memory[cpu->pc.W++];
                cpu->de.B.l = memory[cpu->pc.W++];
                break;

            case 0x21:
                // ld hl,nn
                cpu->de.B.h = memory[cpu->pc.W++];
                cpu->de.B.l = memory[cpu->pc.W++];
                break;

            case 0x31:
                // ld sp,nn
                cpu->sp.B.h = memory[cpu->pc.W++];
                cpu->sp.B.l = memory[cpu->pc.W++];
                break;
                
            case 0x03:
                // inc bc
                cpu->bc.W++;
                break;
                
            case 0x04:
                // inc b
                cpu->bc.B.h++;
                break;
                
            case 0x05:
                // dec b
                cpu->bc.B.h--;
                break;
                
            case 0x07:
                // rlca
                if(IS_SET(cpu->a, 7) == 1) {
                    cpu->flags |= (1 << 5);
                }
                else {
                    cpu->flags &= ~(1 << 5);
                }
                cpu->a = (cpu->a << 1) | (cpu->a >> (sizeof(cpu->a) * 8 - 1));
                break;
                
            case 0x08:
                // ex af,'af
                if (cpu->a != cpu->_a) {
                    cpu->a ^= cpu->_a;
                    cpu->_a ^= cpu->a;
                    cpu->a ^= cpu->_a;
                }
                
                if (cpu->flags != cpu->_flags) {
                    cpu->flags ^= cpu->_flags;
                    cpu->_flags ^= cpu->flags;
                    cpu->flags ^= cpu->_flags;
                }
                break;
                
            case 0x09:
                // add hl,bc
                
                if ((((cpu->hl.W & 0xF000) + (cpu->bc.W & 0xF000)) & 0x1000) == 1) {
                    cpu->flags |= (1 << 5);
                }
                
                if ((cpu->hl.W += cpu->bc.W) > 0xFFFF) {
                    cpu->flags |= (1 << 0);
                    cpu->hl.W -= 65536;
                }
                break;
                
            case 0x0B:
                // dec bc
                cpu->bc.W--;
                break;
                
            case 0x0C:
                // inc c
                cpu->bc.B.h++;
                break;
                
            case 0x0D:
                // dec c
                cpu->bc.B.h--;
                break;
                
                
            default:
                return -1;
        }
        
        if (cpu->counter <= 0) {
            // interrupt tasks here
            cpu->counter += INTERRUPT_PERIOD;
        }
        
        runcycles--;
    } while (runcycles >= 0);
    
    return count;
}
