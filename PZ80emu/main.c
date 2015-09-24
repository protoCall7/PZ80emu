//
//  main.c
//  PZ80emu
//
//  Created by Peter Ezetta on 5/2/15.
//  Copyright (c) 2015 Peter Ezetta. All rights reserved.
//
//  Todo:
//  * Implement rest of instruction set
//  * Implement menu system
//  * Implement stepping
//  * Implement GetOpt
//  * Check all void casts
//  * Check implementation of add hl,bc

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ncurses.h>
#include "z80.h"
#include "memory.h"
#include "display.h"

int main(int argc, const char * argv[]) {
    WINDOW *register_display, *memory_display;
    
    // initialize the CPU
    z80 *cpu = new_cpu();
    
    // emulated memory
    uint8_t *memory = create_ram();
    long runcycles = load_rom(argv[1], memory);

    // display variables
    int main_row = 0;
    int main_col = 0;
    
    // screen init stuff (move me to a display function)
    (void) initscr();
    (void) cbreak();
    (void) noecho();
    (void) keypad(stdscr, true);
    (void) curs_set(0);
    
    getmaxyx(stdscr, main_row, main_col);
    
    // need to put the damn color setter back in and find out why splint bitches
    (void) start_color();
    (void) init_pair(1, COLOR_YELLOW, COLOR_BLUE);

    // execute!
    (void) run(cpu, memory, runcycles);

    // more display stuff
    register_display = create_newwin(5, 128, 0, 0);
    display_registers(register_display, cpu);
    
    memory_display = create_newwin(11, 63, 6, 0);
    display_mem(memory_display, memory);

    (void) wgetch(memory_display);
    (void) endwin();
    
    // memory cleanup (leaks are bad, mmkay?)
    free(cpu);
    free(memory);
    
    return 0;
}