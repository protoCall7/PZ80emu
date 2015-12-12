z80: z80.c z80.h
	gcc z80.c -c

display: display.c display.h
	gcc display.c -c

memory: memory.c memory.h
	gcc memory.c -c

PZ80emu: z80.o display.o memory.o main.c
	gcc z80.o display.o memory.o main.c -o PZ80emu -lcurses

clean:
	rm -rf *.o PZ80emu

all: PZ80emu
