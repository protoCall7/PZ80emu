/** \file */
//
//  memory.h
//  PZ80emu
//
//  Created by Peter Ezetta on 5/3/15.
//  Copyright (c) 2015 Peter Ezetta. All rights reserved.
//

#ifndef __PZ80emu__memory__
#define __PZ80emu__memory__

/** Size of memory for create_ram() to allocate */
#define MEMSIZE 65536

#include <stdio.h>
#include <stdint.h>

#endif /* defined(__PZ80emu__memory__) */

uint8_t *create_ram();
long load_rom(const char *filename, uint8_t *memory);
