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
    const int cycles[15] = {4, 10, 7, 6, 4, 4, 7, 4, 4, 11, 7, 6, 4, 4, 7};
    
    do {
        uint8_t opcode = memory[cpu->pc.W++]; // fetch next opcode from memory
        cpu->counter -= cycles[opcode]; // decrease the interrupt counter by number of cycles for opcode
        count++;
        
        // opcode interpreter switch
        switch (opcode) {
            case 0x00:
                // nop
                break;
                
            case 0x01:
                // ld bc,nn
                cpu->bc.B.h = memory[cpu->pc.W++];
                cpu->bc.B.l = memory[cpu->pc.W];
                break;
                
            case 0x02:
                // ld (bc),a
                memory[cpu->bc.W] = cpu->a;
                break;
                
            case 0x03:
                // inc bc
                cpu->bc.W++;
                break;
                
            case 0x04:
                // inc b
                cpu->bc.B.l++;
                break;
                
            case 0x05:
                // dec b
                cpu->bc.B.l--;
                break;
                
            case 0x06:
                // ld b,n
                cpu->bc.B.l = ((memory[cpu->pc.W]) << 8);
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
                
                
            case 0x0A:
                // ld a,(bc)
                cpu->a = memory[cpu->bc.W];
                break;
                
            case 0x0B:
                // dec bc
                cpu->bc.W--;
                break;
                
            case 0x0C:
                // inc c
                cpu->bc.B.l++;
                break;
                
            case 0x0D:
                // dec c
                cpu->bc.B.l--;
                break;
                
            case 0x0E:
                // ld c,n
                cpu->bc.B.l = memory[cpu->bc.W];
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
