/** \file memory.h
 *  \brief Memory setup functions
 *
 *  Created by Peter Ezetta on 5/3/15.
 *  Copyright (c) 2015 Peter Ezetta. All rights reserved.
 *
 */


#ifndef __PZ80emu__memory__
#define __PZ80emu__memory__

#include <stdint.h>

/** Size of memory for create_ram() to allocate */
#define MEMSIZE 65536

/**
 * z80 memory class
 * \brief z80 memory class
 */
typedef struct {
	/** Pointer to block of memory for allocation */
	uint8_t *memory;

	/** Pointer to memory_load method */
	long (*memory_load)(void *self, const char *filename);

	/** Pointer to memory_free method */
	void (*memory_free)(void *self);
} memory;

memory *memory_new();
#endif /* defined(__PZ80emu__memory__) */
