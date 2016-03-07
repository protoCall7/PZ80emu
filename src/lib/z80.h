/** \file */
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

/** The initial value of the PC register */
#define INIT_PC 0x0000

/** The number of cycles to run before triggering interrupt */
#define INTERRUPT_PERIOD 10240

/** Type to deal with endianness and access of high/low bits */
typedef union {
	/** 16 Bit Pair */
	unsigned short W;

	/** Combination of High and Low bytes in correct order for
	   endianness.
	 */
	struct {
#ifdef LOW_ENDIAN
		uint8_t l,h; /* ...in low-endian architecture */
#else
		uint8_t h,l; /* ...in high-endian architecture */
#endif
	} B;
} word;


/** Creates a z80 type referring to the z80 struct */
typedef struct z80 z80;

/** Collection of registers comprising a Z80 CPU */
struct z80 {
	/** program counter */
	word pc;

	/** interrupt counter */
	int counter;

	/** A register */
	uint8_t a;

	/** flags register */
	unsigned flags : 6;

	/** BC register pair */
	word bc;

	/** DE register pair */
	word de;

	/** HL register pair */
	word hl;

	/** IX register */
	word ix;

	/** IY register */
	word iy;

	/** A' register */
	uint8_t _a;

	/** flags' register */
	unsigned _flags : 6;

	/** BC' register pair */
	word _bc;

	/** DE' register pair */
	word _de;

	/** HL' register pair */
	word _hl;

	/** Stack Pointer */
	word sp;
};

z80 *new_cpu(void);
void reset_cpu(z80 *cpu); // reset function
void _load_reg8_mem_pair(uint8_t *reg, word *address_pair, uint8_t *memory);
void _load_reg8_mem_idx_offset(uint8_t *reg, word *index_register, uint8_t *memory, word *pc);
int run(z80 *cpu, uint8_t *memory, long cycles); // run CPU function
#endif /* defined(__PZ80emu__z80__) */
