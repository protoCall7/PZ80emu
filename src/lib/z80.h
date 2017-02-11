/** \file z80.h
 *  \brief z80 CPU data structure and functions
 *
 * Created by Peter Ezetta on 5/3/15.
 * Copyright (c) 2015 Peter Ezetta. All rights reserved.
 *
 */

#ifndef __PZ80emu__z80__
#define __PZ80emu__z80__

#include <stdint.h>

/** The initial value of the PC register */
#define INIT_PC 0x0000

/** The number of cycles to run before triggering interrupt */
#define INTERRUPT_PERIOD 10240

/** Type to deal with endianness and access of high/low bits */
typedef union {
	uint16_t W; /** 16 Bit Pair */

	/** Combination of High and Low bytes in correct order for
	 * endianness.
	 */
	struct {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		uint8_t l,h; /* ...in low-endian architecture */
#else
		uint8_t h,l; /* ...in high-endian architecture */
#endif
	} B;
} word;

/** Collection of registers comprising a Z80 CPU */
typedef struct {
	word pc; /** program counter */
	int counter; /** interrupt counter */
	uint8_t a; /** A register */
	unsigned flags : 6; /** flags register */
	word bc; /** BC register pair */
	word de; /** DE register pair */
	word hl; /** HL register pair */
	word ix; /** IX register */
	word iy; /** IY register */
	word ir; /** IR register */
	uint8_t _a; /** A' register */
	unsigned _flags : 6; /** flags' register */
	word _bc; /** BC' register pair */
	word _de; /** DE' register pair */
	word _hl; /** HL' register pair */
	word sp; /** Stack Pointer */
} z80;

z80 *new_cpu(void);
void reset_cpu(z80 *cpu); // reset function
int run(z80 *cpu, uint8_t *memory, long cycles, int s_flag); // run CPU function
void _load_reg8_mem_pair(uint8_t *reg, word *address_pair, uint8_t *memory);
void _load_reg8_mem_idx_offset(uint8_t *reg, word *index_register, uint8_t *memory, word *pc);
void _load_mem_idx_offset_reg8(uint8_t *reg, word *index_register, uint8_t *memory, word *pc);
void _load_reg16_nn(word *reg, uint8_t *memory, word *pc);
#endif /* defined(__PZ80emu__z80__) */
