/** \file */
//
//  memory.c
//  PZ80emu
//
//  Created by Peter Ezetta on 5/3/15.
//  Copyright (c) 2015 Peter Ezetta. All rights reserved.
//

#include "memory.h"
#include <stdlib.h>
#include <string.h>

/** Allocates a block of RAM of size MEMSIZE 
\return Pointer to allocated block of RAM.
*/
uint8_t *create_ram(void) {
    uint8_t *memory;
    
    if ((memory = calloc(MEMSIZE, 1)) == NULL) {
        exit(EXIT_FAILURE);
    }
    
    return memory;
}

/** Loads the contents of a ROM file into memory
\param filename String containing the filename of the ROM image.
\param memory Pointer to the block of RAM to load the ROM into.
\return Number of bytes loaded into RAM.
*/
long load_rom(const char *filename, uint8_t *memory) {
    FILE *infile;
    long file_numbytes;
	size_t readbytes;
    
    infile = fopen(filename, "r");
    if (infile == NULL) {
        return 1;
    }
    
    if((fseek(infile, 0L, SEEK_END)) != 0) {
        exit(EXIT_FAILURE);
    }
    
    file_numbytes = ftell(infile);
    
    if((fseek(infile, 0L, SEEK_SET)) != 0) {
        exit(EXIT_FAILURE);
    }
    
    readbytes = fread(memory, sizeof(char), (size_t)file_numbytes, infile);
	if(readbytes != (size_t)file_numbytes) {
		exit(EXIT_FAILURE);
	}

    if((fclose(infile)) != 0) {
        exit(EXIT_FAILURE);
    }
    
    return file_numbytes;
}
