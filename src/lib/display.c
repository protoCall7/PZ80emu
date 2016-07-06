/** \file
   Display functions for curses management and displaying system info
 */

//
//  display.c
//  PZ80emu
//
//  Created by Peter Ezetta on 5/3/15.
//  Copyright (c) 2015 Peter Ezetta. All rights reserved.
//

#include <stdint.h>
#include <ncurses.h>
#include "z80.h"
#include "display.h"
#include "utils.h"

/** Display the contents of a block of memory to a curses window
   \param win The curses window in which to display memory.
   \param memory A pointer to the block of memory to display
 */
void display_mem(WINDOW *win, uint8_t *memory) {
	int display_counter = 0;
	int rollover_counter = 0;

	//memory display routine
	(void) mvwprintw(win, 1, 2, "Memory Display:");
	for (int i = 0; i < 128; i++) {
		if (display_counter == 0) (void) mvwprintw(win, rollover_counter+2, 2, "0x%04hhX: ", i);
		(void)wprintw(win, "%02hhX ", memory[i]);
		display_counter++;

		if(display_counter == 16)
		{
			//(void) wmove(win, i+3, 2);
			display_counter = 0;
			rollover_counter++;
			(void) wrefresh(win);
		}
		if(display_counter % 4 == 0) (void) wprintw(win, " ");
	}
}
/** Display the current register state to a curses window
   \param win The curses window in which to display register state.
   \param cpu A pointer to a z80 cpu struct for which to display registers.
 */
void display_registers(WINDOW *win, z80 *cpu) {
	// register status display
	(void) mvwprintw(win, 1, 2, "Register Status:");
	(void) mvwprintw(win, 2, 2, "pc\t  a\t  CZPSNH\t  bc\t  de\t  hl\t  ix\t  iy\t  'a\t  'CZPSNH\t  'bc\t  'de\t  'hl\t  sp");
	(void) mvwprintw(win, 3, 2, "%04x\t  %02hhX\t  %c%c%c%c%c%c\t  %04hX\t  %04x\t  %04hX\t  %04x\t  %04x\t  %02hhX\t   %c%c%c%c%c%c\t  %04x\t  %04x\t  %04X\t  %04x",
	                 cpu->pc.W,
	                 cpu->a,
	                 (bool)IS_SET(cpu->flags, 0) ? '1' : '0',
	                 (bool)IS_SET(cpu->flags, 1) ? '1' : '0',
	                 (bool)IS_SET(cpu->flags, 2) ? '1' : '0',
	                 (bool)IS_SET(cpu->flags, 3) ? '1' : '0',
	                 (bool)IS_SET(cpu->flags, 4) ? '1' : '0',
	                 (bool)IS_SET(cpu->flags, 5) ? '1' : '0',
	                 cpu->bc.W,
	                 cpu->de.W,
	                 cpu->hl.W,
	                 cpu->ix.W,
	                 cpu->iy.W,
	                 cpu->_a,
	                 (bool)IS_SET(cpu->_flags, 0) ? '1' : '0',
	                 (bool)IS_SET(cpu->_flags, 1) ? '1' : '0',
	                 (bool)IS_SET(cpu->_flags, 2) ? '1' : '0',
	                 (bool)IS_SET(cpu->_flags, 3) ? '1' : '0',
	                 (bool)IS_SET(cpu->_flags, 4) ? '1' : '0',
	                 (bool)IS_SET(cpu->_flags, 5) ? '1' : '0',
	                 cpu->_bc.W,
	                 cpu->_de.W,
	                 cpu->_hl.W,
	                 cpu->sp.W
	                 );
	(void) wrefresh(win);
}
/** Creates a new curses window
   \param height The height of the window.
   \param width The width of the window.
   \param starty The starting y coordinate for the new window.
   \param startx The starting x coordinate for the new window.
   \return curses WINDOW
 */
WINDOW *create_newwin(int height, int width, int starty, int startx) {
	WINDOW *local_win = newwin(height, width, starty, startx);
	(void) box(local_win, 0, 0);            /* 0, 0 gives default characters
	                                         * for the vertical and horizontal
	                                         * lines */
	(void) wrefresh(local_win);     // show that box

	return local_win;
}
/** Creates a new curses screen
   \param main_row
   \param main_col
 */
void create_newscreen(int main_row, int main_col) {
	initscr();
	raw();
	noecho();
	keypad(stdscr, true);
	curs_set(0);

	getmaxyx(stdscr, main_row, main_col);

	// need to put the damn color setter back in and find out why splint bitches
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLUE);
}