/** \file memory.c */
//
//  Created by Peter Ezetta on 5/3/15.
//  Copyright (c) 2015 Peter Ezetta. All rights reserved.
//

#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Loads the contents of a ROM file into memory
 * \param filename String containing the filename of the ROM image.
 * \param self Pointer to the block of RAM to load the ROM into.
 * \return Number of bytes loaded into RAM.
 */
static long memory_load(void *self, const char *filename) {
	memory *mem = self;

	FILE *infile = fopen(filename, "r");
	if (infile == NULL) {
		return -1;
	}

	if((fseek(infile, 0L, SEEK_END)) != 0) {
		return -1;
	}

	long file_numbytes = ftell(infile);

	if((fseek(infile, 0L, SEEK_SET)) != 0) {
		return -1;
	}

	size_t readbytes = fread(mem->memory, sizeof(char), (size_t)file_numbytes, infile);
	if(readbytes != (size_t)file_numbytes) {
		return -1;
	}

	if((fclose(infile)) != 0) {
		return -1;
	}

	return file_numbytes;
}

/**
 * Frees an allocated memory object
 * \param self memory object to free
 */
static void memory_free(void *self) {
	memory *mem = self;

	free(mem->memory);
	free(mem);
}

/**
 * Allocates a block of RAM of size MEMSIZE
 * \return Pointer to allocated block of RAM.
 */
memory *memory_new(void) {
	// allocate memory
	memory *mem;
	mem = malloc(sizeof(memory));

	if ((mem->memory = calloc(MEMSIZE, 1)) == NULL) {
		exit(EXIT_FAILURE);
	}

	// tie methods to the object
	mem->memory_load = &memory_load;
	mem->memory_free = &memory_free;

	return mem;
}
