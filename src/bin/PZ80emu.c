/** \file PZ80emu.c
 *  \brief Main application
 *
 * \mainpage
 * PZ80emu
 *
 * Created by Peter Ezetta on 5/2/15.
 * Copyright (c) 2015-2017 Peter Ezetta. All rights reserved.
 *
 * Todo:
 * - Implement rest of instruction set
 * - Implement menu system
 * - Check all void casts
 * - Fix error handling and reporting
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <ncurses.h>
#include "z80.h"
#include "memory.h"
#include "display.h"

/** PZ80 Machine Emulator */
int main(int argc, char *argv[]) {
	long runcycles = 0, filesize = 0;
    int s_flag = 0;
	int c;
    
    extern char *optarg;
    extern int optind, optopt;

    z80 *cpu = new_cpu();
	memory *mem = memory_new();

	while ((c = getopt(argc, argv, "sr:f:")) != -1) {
		switch (c) {
			case 'r':
				runcycles = strtol(optarg, NULL, 0);
				break;
                
            case 'f':
                filesize = mem->memory_load(mem, optarg);
                break;
                
            case 's':
                s_flag = 1;
                break;
                
        }
    }
    
    // make sure we got the required options, display help text if not
    if (runcycles <= 0) {
        printf("Usage: PZ80emu -f <filename> -r <runcycles>\n");
        exit(EXIT_FAILURE);
    }
    
    if (filesize <= 0) {
        printf("Usage: PZ80emu -f <filename> -r <runcycles>\n");
        exit(EXIT_FAILURE);
    }

	// execute!
	(void) run(cpu, mem->memory, runcycles, s_flag);

	// display stuff
	display_registers(cpu);
	display_mem(mem->memory);

	// memory cleanup (leaks are bad, mmkay?)
	free(cpu);
	mem->memory_free(mem);

	return 0;
}
