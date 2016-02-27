P=PZ80emu
OBJECTS=z80.o display.o memory.o
LDLIBS=-lcurses
CFLAGS=-g -Wall -O3
CC=gcc

$(P): $(OBJECTS)
