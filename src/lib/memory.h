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

typedef struct {
	uint8_t *memory;

	long (*memory_load)(void *self, const char *filename);
	void (*memory_free)(void *self);
} memory;

memory *memory_new();
#endif /* defined(__PZ80emu__memory__) */
