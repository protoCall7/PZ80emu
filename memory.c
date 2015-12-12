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

uint8_t *create_ram() {
    uint8_t *memory;
    
    if ((memory = calloc(1, sizeof(MEMSIZE))) == NULL) {
        exit(EXIT_FAILURE);
    }
    
    return memory;
}

long load_rom(const char *filename, uint8_t *memory) {
    FILE *infile;
    long file_numbytes;
    
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
    
    (void) fread(memory, sizeof(char), (size_t)file_numbytes, infile);
    if((fclose(infile)) != 0) {
        exit(EXIT_FAILURE);
    }
    
    return file_numbytes;
}