P=PZ80emu
OBJECTS=z80.o display.o memory.o
LDLIBS=-lcurses
CFLAGS=-g -pg -Wall -O3 -std=gnu11
CC=gcc

$(P): $(OBJECTS)
