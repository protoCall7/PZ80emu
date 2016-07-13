/** \file PZ80emu.c
 *  \brief Main application
 *
 * \mainpage
 * PZ80emu
 *
 * Created by Peter Ezetta on 5/2/15.
 * Copyright (c) 2015 Peter Ezetta. All rights reserved.
 *
 * Todo:
 * - Implement rest of instruction set
 * - Implement menu system
 * - Implement stepping
 * - Implement GetOpt
 * - Check all void casts
 * - Fix error handling and reporting
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ncurses.h>
#include "z80.h"
#include "memory.h"
#include "display.h"

/** PZ80 Machine Emulator */
int main(int argc, const char * argv[]) {
	WINDOW *register_display, *memory_display;

	// display variables
	int main_row = 0;
	int main_col = 0;

	if (argc < 2) {
		printf("Usage: ./PZ80emu <filename.bin>\n");
		exit(EXIT_FAILURE);
	}

	z80 *cpu = new_cpu();

	// create a new memory object
	memory *mem = memory_new();

	// load memory
	long runcycles = mem->memory_load(mem, argv[1]);

	create_newscreen(main_row, main_col);

	// execute!
	(void) run(cpu, mem->memory, runcycles);

	// more display stuff
	register_display = create_newwin(5, 128, 0, 0);
	display_registers(register_display, cpu);

	memory_display = create_newwin(11, 63, 6, 0);
	display_mem(memory_display, mem->memory);

	(void) wgetch(memory_display);
	(void) endwin();

	// memory cleanup (leaks are bad, mmkay?)
	free(cpu);
	mem->memory_free(mem);

	return 0;
}
