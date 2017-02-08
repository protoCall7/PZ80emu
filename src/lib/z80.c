/** \file z80.c */
//
//  z80.c
//  PZ80emu
//
//  Created by Peter Ezetta on 5/3/15.
//  Copyright (c) 2015 Peter Ezetta. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "z80.h"
#include "utils.h"

/**
 * Fills out a new z80 CPU struct
 * \return A z80 struct
 */
z80 *new_cpu(void) {
	z80 *cpu;
	if ((cpu = calloc(1, sizeof(z80))) == NULL) {
		exit(EXIT_FAILURE);
	}

	return cpu;
}

/**
 * Triggers the reset state on the z80 CPU
 * \param cpu A z80 struct to reset.
 */
void reset_cpu(z80 *cpu) {
	// need to implement interrupt resetting here
	cpu->pc.W = 0x0000;
}

/**
 * Loads a value into an 8-bit register from the memory location stored in a 16-bit register pair
 * \param reg register to load
 * \param address_pair 16-bit register pair containing the memory address to load
 * from
 * \param memory block of memory containing the value to load
 */
void _load_reg8_mem_pair(uint8_t *reg, word *address_pair, uint8_t *memory) {
	word address;
	address.B.h = address_pair->B.h;
	address.B.l = address_pair->B.l;
	*reg = memory[address.W];
}

/**
 * Loads a value into an 8-bit register from the memory location stored in an index register + an offset from memory.
 * \param reg register to load
 * \param index_register pointer to ix or iy index register
 * \param memory block of memory containing the value to load
 * \param pc pointer to program counter
 */
void _load_reg8_mem_idx_offset(uint8_t *reg, word *index_register, uint8_t *memory, word *pc) {
	uint8_t index = memory[pc->W++];

	*reg = memory[(index + index_register->W)];
}

/**
 * Loads a value into a memory location stored in an index register + an offset from a register.
 * \param reg register to load from
 * \param index_register pointer to ix or iy index register
 * \param memory block of memory containing the value to load
 * \param pc pointer to program counter
 */
void _load_mem_idx_offset_reg8(uint8_t *reg, word *index_register, uint8_t *memory, word *pc) {
	uint8_t index = memory[pc->W++];

	memory[(index + index_register->W)] = *reg;
}

/**
 * Loads a user supplied value into a 16 bit register pair
 * \param reg register to load
 * \param memory block of memory to retrieve nn from
 * \param pc pointer to program counter
 */
void _load_reg16_nn(word *reg, uint8_t *memory, word *pc) {
	word nn;
	nn.B.l = memory[pc->W++];
	nn.B.h = memory[pc->W++];

	reg->W = nn.W;
}

void _add_a_reg8(z80 *cpu, uint8_t *reg) {
	// carry flag
	if ((cpu->a + *reg) > 255) {
		cpu->flags |= (1 << 0);
	} else {
		cpu->flags &= ~1;
	}

	// zero flag
	if ((cpu->a + *reg) == 0) {
		cpu->flags |= (1 << 1);
	} else {
		cpu->flags &= ~(1 << 1);
	}

	// overflow flag
	if (cpu->a > 0) {
		if ((cpu->a + *reg) < 0) {
			cpu->flags |= (1 << 2);
		} else {
			cpu->flags &= ~(1 << 2);
		}
	} else if (cpu->a < 0) {
		if ((cpu->a + *reg) > 0) {
			cpu->flags |= (1 << 2);
		} else {
			cpu->flags &= ~(1 << 2);
		}
	} else {
		cpu->flags &= ~(1 << 2);
	}

	// sign flag
	if ((127 < (cpu->a + *reg)) && ((cpu->a + *reg) < 256)) {
		cpu->flags |= (1 << 3);
	} else {
		cpu->flags &= ~(1 << 3);
	}

	// reset N flag
	cpu->flags &= ~(1 << 4);

	// half carry flag
	if(((cpu->a & 0x0F) + (*reg & 0x0F) & 0x10) == 0x10) {
		cpu->flags |= (1 << 5);
	} else {
		cpu->flags &= ~(1 << 5);
	}

	cpu->a += *reg;
}

/**
 * Runs the cpu
 * \param cpu A z80 cpu struct to run.
 * \param memory An allocated block of memory to pass to the cpu.
 * \param runcycles The number of clock cycles to run the cpu.
 * \return Count of cycles executed.
 */
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
				_load_reg8_mem_idx_offset(&cpu->a, &cpu->ix, memory, &cpu->pc);
				break;

			case 0x46:
				// ld b,(ix+n)
				_load_reg8_mem_idx_offset(&cpu->bc.B.h, &cpu->ix, memory, &cpu->pc);
				break;

			case 0x4E:
				// ld c,(ix+n)
				_load_reg8_mem_idx_offset(&cpu->bc.B.l, &cpu->ix, memory, &cpu->pc);
				break;

			case 0x56:
				// ld d,(ix+n)
				_load_reg8_mem_idx_offset(&cpu->de.B.h, &cpu->ix, memory, &cpu->pc);
				break;

			case 0x5E:
				// ld e,(ix+n)
				_load_reg8_mem_idx_offset(&cpu->de.B.l, &cpu->ix, memory, &cpu->pc);
				break;

			case 0x66:
				// ld h,(ix+n)
				_load_reg8_mem_idx_offset(&cpu->hl.B.h, &cpu->ix, memory, &cpu->pc);
				break;

			case 0x6E:
				// ld l,(ix+n)
				_load_reg8_mem_idx_offset(&cpu->hl.B.l, &cpu->ix, memory, &cpu->pc);
				break;

			case 0x77:
				// ld (ix+n),a
				_load_mem_idx_offset_reg8(&cpu->a, &cpu->ix, memory, &cpu->pc);
				break;

			case 0x70:
				// ld (ix+n),b
				_load_mem_idx_offset_reg8(&cpu->bc.B.h, &cpu->ix, memory, &cpu->pc);
				break;

			case 0x71:
				// ld (ix+n),c
				_load_mem_idx_offset_reg8(&cpu->bc.B.l, &cpu->ix, memory, &cpu->pc);
				break;

			case 0x72:
				// ld (ix+n),d
				_load_mem_idx_offset_reg8(&cpu->de.B.h, &cpu->ix, memory, &cpu->pc);
				break;

			case 0x73:
				// ld (ix+n),e
				_load_mem_idx_offset_reg8(&cpu->de.B.l, &cpu->ix, memory, &cpu->pc);
				break;

			case 0x74:
				// ld (ix+n),h
				_load_mem_idx_offset_reg8(&cpu->hl.B.h, &cpu->ix, memory, &cpu->pc);
				break;

			case 0x75:
				// ld (ix+n),l
				_load_mem_idx_offset_reg8(&cpu->hl.B.l, &cpu->ix, memory, &cpu->pc);
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
				_load_reg16_nn(&cpu->ix, memory, &cpu->pc);
				break;

			case 0xE3:
				// ex (sp),ix
				if (memory[cpu->sp.W + 1] != cpu->ix.B.h) {
					cpu->ix.B.h ^= memory[cpu->sp.W + 1];
					memory[cpu->sp.W + 1] ^= cpu->ix.B.h;
					cpu->ix.B.h ^= memory[cpu->sp.W + 1];
				}

				if (memory[cpu->sp.W] != cpu->ix.B.l) {
					cpu->ix.B.l ^= memory[cpu->sp.W];
					memory[cpu->sp.W] ^= cpu->ix.B.l;
					cpu->ix.B.l ^= memory[cpu->sp.W];
				}
				break;

			}
			break;


		// extended instruction set 0xFDxx
		case 0xFD:
			opcode = memory[cpu->pc.W++];

			switch(opcode) {
			case 0x7E:
				// ld a,(iy+n)
				_load_reg8_mem_idx_offset(&cpu->a, &cpu->iy, memory, &cpu->pc);
				break;

			case 0x46:
				// ld b,(iy+n)
				_load_reg8_mem_idx_offset(&cpu->bc.B.h, &cpu->iy, memory, &cpu->pc);
				break;

			case 0x4E:
				// ld c,(iy+n)
				_load_reg8_mem_idx_offset(&cpu->bc.B.l, &cpu->iy, memory, &cpu->pc);
				break;

			case 0x56:
				// ld d,(iy+n)
				_load_reg8_mem_idx_offset(&cpu->de.B.h, &cpu->iy, memory, &cpu->pc);
				break;

			case 0x5E:
				// ld e,(iy+n)
				_load_reg8_mem_idx_offset(&cpu->de.B.l, &cpu->iy, memory, &cpu->pc);
				break;

			case 0x66:
				// ld h,(iy+n)
				_load_reg8_mem_idx_offset(&cpu->hl.B.h, &cpu->iy, memory, &cpu->pc);
				break;

			case 0x6E:
				// ld l,(iy+n)
				_load_reg8_mem_idx_offset(&cpu->hl.B.l, &cpu->iy, memory, &cpu->pc);
				break;

			case 0x77:
				// ld (iy+n),a
				_load_mem_idx_offset_reg8(&cpu->a, &cpu->iy, memory, &cpu->pc);
				break;

			case 0x70:
				// ld (iy+n),b
				_load_mem_idx_offset_reg8(&cpu->bc.B.h, &cpu->iy, memory, &cpu->pc);
				break;

			case 0x71:
				// ld (iy+n),c
				_load_mem_idx_offset_reg8(&cpu->bc.B.l, &cpu->iy, memory, &cpu->pc);
				break;

			case 0x72:
				// ld (iy+n),d
				_load_mem_idx_offset_reg8(&cpu->de.B.h, &cpu->iy, memory, &cpu->pc);
				break;

			case 0x73:
				// ld (iy+n),e
				_load_mem_idx_offset_reg8(&cpu->de.B.l, &cpu->iy, memory, &cpu->pc);
				break;

			case 0x74:
				// ld (iy+n),h
				_load_mem_idx_offset_reg8(&cpu->hl.B.h, &cpu->iy, memory, &cpu->pc);
				break;

			case 0x75:
				// ld (iy+n),l
				_load_mem_idx_offset_reg8(&cpu->hl.B.l, &cpu->iy, memory, &cpu->pc);
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
				_load_reg16_nn(&cpu->iy, memory, &cpu->pc);
				break;


			case 0xE3:
				// ex (sp),iy

				if (memory[cpu->sp.W + 1] != cpu->iy.B.h) {
					cpu->iy.B.h ^= memory[cpu->sp.W + 1];
					memory[cpu->sp.W + 1] ^= cpu->iy.B.h;
					cpu->iy.B.h ^= memory[cpu->sp.W + 1];
				}

				if (memory[cpu->sp.W] != cpu->iy.B.l) {
					cpu->iy.B.l ^= memory[cpu->sp.W];
					memory[cpu->sp.W] ^= cpu->iy.B.l;
					cpu->iy.B.l ^= memory[cpu->sp.W];
				}
				break;

			}
			break;

		case 0x00:
			// nop
			break;

		// 8-bit transfer instructions
		case 0x0A:
			// ld a,(bc)
			_load_reg8_mem_pair(&cpu->a, &cpu->bc, memory);
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
			_load_reg8_mem_pair(&cpu->a, &cpu->hl, memory);
			break;

		case 0x1A:
			// ld a,(de)
			cpu->a = memory[cpu->de.W];
			break;

		case 0x3A:
			// ld a,(nn)
		{
			word nn;
			nn.B.l = memory[cpu->pc.W++];
			nn.B.h = memory[cpu->pc.W++];
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
			_load_reg8_mem_pair(&cpu->bc.B.h, &cpu->hl, memory);
			break;

		case 0x4F:
			// ld c,a
			cpu->bc.B.l = cpu->a;
			break;

		case 0x48:
			// ld c,b
			cpu->bc.B.l = cpu->bc.B.h;
			break;

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
			_load_reg8_mem_pair(&cpu->bc.B.l, &cpu->hl, memory);
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
			_load_reg8_mem_pair(&cpu->de.B.h, &cpu->hl, memory);
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
			_load_reg8_mem_pair(&cpu->de.B.l, &cpu->hl, memory);
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
			_load_reg8_mem_pair(&cpu->hl.B.h, &cpu->hl, memory);
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
			_load_reg8_mem_pair(&cpu->hl.B.l, &cpu->hl, memory);
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
			memory[cpu->hl.W] = memory[cpu->pc.W++];
			break;

		case 0x02:
			// ld (bc),a
			memory[cpu->bc.W] = cpu->a;
			break;

		case 0x12:
			// ld (de),a
			memory[cpu->de.W] = cpu->a;
			break;

		case 0x32:
			// ld (nn),a
		{
			word address;
			address.B.l = memory[cpu->pc.W++];
			address.B.h = memory[cpu->pc.W++];

			memory[address.W] = cpu->a;
		}
		break;

		// 16-bit transfer instructions
		case 0x01:
			// ld bc,nn
			_load_reg16_nn(&cpu->bc, memory, &cpu->pc);
			break;

		case 0x11:
			// ld de,nn
			_load_reg16_nn(&cpu->de, memory, &cpu->pc);
			break;

		case 0x21:
			// ld hl,nn
			_load_reg16_nn(&cpu->hl, memory, &cpu->pc);
			break;

		case 0x31:
			// ld sp,nn
			_load_reg16_nn(&cpu->sp, memory, &cpu->pc);
			break;

		case 0x2A:
			// ld hl,(nn)
			cpu->hl.W = (memory[cpu->pc.W++] << 8);
			cpu->hl.W |= cpu->pc.W++;
			break;

		case 0x03:
			/// @bug inc bc isn't operating on flags
			cpu->bc.W++;
			break;

		case 0x04:
			/// @bug inc b isn't operating on flags
			cpu->bc.B.h++;
			break;

		case 0x05:
			/// @bug dec b isn't operating on flags
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

		// Register Exchange Instructions
		case 0xEB:
			// ex hl,de
			if(cpu->hl.W != cpu->de.W) {
				cpu->hl.W ^= cpu->de.W;
				cpu->de.W ^= cpu->hl.W;
				cpu->hl.W ^= cpu->de.W;
			}
			break;

		case 0xE3:
			// ex (sp),hl

			if (memory[cpu->sp.W + 1] != cpu->hl.B.h) {
				cpu->hl.B.h ^= memory[cpu->sp.W + 1];
				memory[cpu->sp.W + 1] ^= cpu->hl.B.h;
				cpu->hl.B.h ^= memory[cpu->sp.W + 1];
			}

			if (memory[cpu->sp.W] != cpu->hl.B.l) {
				cpu->hl.B.l ^= memory[cpu->sp.W];
				memory[cpu->sp.W] ^= cpu->hl.B.l;
				cpu->hl.B.l ^= memory[cpu->sp.W];
			}
			break;

		case 0x08:
			// ex af,af'
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

		case 0xD9:
			// exx
			if (cpu->bc.W != cpu->_bc.W) {
				cpu->bc.W ^= cpu->_bc.W;
				cpu->_bc.W ^= cpu->bc.W;
				cpu->bc.W ^= cpu->_bc.W;
			}

			if (cpu->de.W != cpu->_de.W) {
				cpu->de.W ^= cpu->_de.W;
				cpu->_de.W ^= cpu->de.W;
				cpu->de.W ^= cpu->_de.W;
			}

			if (cpu->hl.W != cpu->_hl.W) {
				cpu->hl.W ^= cpu->_hl.W;
				cpu->_hl.W ^= cpu->hl.W;
				cpu->hl.W ^= cpu->_hl.W;
			}
			break;

		// add byte instructions
		case 0x87:
			// add a,a
			_add_a_reg8(cpu, &cpu->a);
			break;

		case 0x80:
			// add a,b
			_add_a_reg8(cpu, &cpu->bc.B.h);
			break;

		case 0x81:
			// add a,c
			_add_a_reg8(cpu, &cpu->bc.B.l);
			break;

		case 0x82:
			// add a,d
			_add_a_reg8(cpu, &cpu->de.B.h);
			break;

		case 0x83:
			// add a,e
			_add_a_reg8(cpu, &cpu->de.B.l);
			break;

		case 0x84:
			// add a,h
			_add_a_reg8(cpu, &cpu->hl.B.h);
			break;

		case 0x85:
			// add a,l
			_add_a_reg8(cpu, &cpu->hl.B.l);
			break;

		case 0x86:
			// add a,(hl)
			_add_a_reg8(cpu, &memory[cpu->hl.W]);
			break;

		case 0x09:
			// add hl,bc

			// reset N flag
			cpu->flags &= (0 << 4);

			// check for half carry
			if ((((cpu->hl.W & 0x0FFF) + (cpu->bc.W & 0x0FFF)) & 0x1000) == 0x1000) {
				cpu->flags |= (1 << 5);
			} else {
				cpu->flags &= (0 << 5);
			}

			// check for carry and add
			if ((cpu->hl.W + cpu->bc.W) > 0xFFFF) {
				cpu->hl.W += cpu->bc.W;
				cpu->flags |= (1 << 0);
				cpu->hl.W -= 65536;
			} else {
				cpu->hl.W += cpu->bc.W;
				cpu->flags &= ~1;
			}

			break;

		case 0x0B:
			/// @bug dec bc isn't operating on flags
			cpu->bc.W--;
			break;

		case 0x0C:
			/// @bug inc c isn't operating on flags
			cpu->bc.B.h++;
			break;

		case 0x0D:
			/// @bug dec c isn't operating on flags
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
	} while (runcycles > 0);

	return count;
}
