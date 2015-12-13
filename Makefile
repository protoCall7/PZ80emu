z80: z80.c z80.h
	gcc z80.c  -ggdb -c

display: display.c display.h
	gcc display.c -c -ggdb

memory: memory.c memory.h
	gcc memory.c -c -ggdb

PZ80emu: z80.o display.o memory.o main.c
	gcc z80.o display.o memory.o main.c -o PZ80emu -lcurses -ggdb

clean:
	rm -rf *.o PZ80emu

all: PZ80emu
