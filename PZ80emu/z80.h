//
//  z80.h
//  PZ80emu
//
//  Created by Peter Ezetta on 5/3/15.
//  Copyright (c) 2015 Peter Ezetta. All rights reserved.
//

#ifndef __PZ80emu__z80__
#define __PZ80emu__z80__

#include <stdio.h>
#include <stdint.h>
#include <ncurses.h>

#endif /* defined(__PZ80emu__z80__) */

#define INIT_PC 0x0000
#define INTERRUPT_PERIOD 10240
#define IS_SET(val, bit) ((val) & (1 << (bit)))

// define 'word' to dead with endianness and access of high/low bits
typedef union {
    unsigned short W;
    
    struct {
#ifdef LOW_ENDIAN
        uint8_t l,h;     /* ...in low-endian architecture */
#else
        uint8_t h,l;     /* ...in high-endian architecture */
#endif
    } B;
} word;

// main z80 register struct
typedef struct z80 z80;
struct z80 {
    word pc; // program counter
    int counter; // interrupt counter
    uint8_t a; // a register
    unsigned flags :6; // flags register
    word bc; // bc register pair
    word de; // de register pair
    word hl; // hl register pair
    word ix; // ix register
    word iy; // iy register
    uint8_t _a; // a' register
    unsigned _flags :6; // flags' register
    word _bc; // bc' register pair
    word _de; // de' register pair
    word _hl; // hl' register pair
    word sp; // stack pointer
};

z80 *new_cpu();
void reset_cpu(z80 *cpu); // reset function
int run(z80 *cpu, uint8_t *memory, long cycles); // run CPU function