/** \file display.h */
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

void curses_display_mem(WINDOW *win, uint8_t *memory);
void curses_display_registers(WINDOW *win, z80 *cpu);
WINDOW *create_newwin(int height, int width, int starty, int startx);
void create_newscreen(int main_row, int main_col);
void display_mem(uint8_t *memory);
void display_registers(z80 *cpu);

#endif /* defined(__PZ80emu__display__) */
