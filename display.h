/** \file */
//
//  display.h
//  PZ80emu
//
//  Created by Peter Ezetta on 5/3/15.
//  Copyright (c) 2015 Peter Ezetta. All rights reserved.
//

#ifndef __PZ80emu__display__
#define __PZ80emu__display__

#include <stdio.h>
#include <ncurses.h>
#include <stdint.h>

#endif /* defined(__PZ80emu__display__) */

/** Determines if a bit is set or not
\param val Bitfield to check
\param bit Which bit in the field to check
\return 0 == Checked Bit False, 1 == Checked Bit True.
*/
#define IS_SET(val, bit) ((val) & (1 << (bit)))

void display_mem(WINDOW *win, uint8_t *memory);
void display_registers(WINDOW *win, z80 *cpu);
WINDOW *create_newwin(int height, int width, int starty, int startx);
