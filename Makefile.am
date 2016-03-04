bin_PROGRAMS=PZ80emu
AM_CFLAGS=-g -Wall -O3
PZ80emu_SOURCES=PZ80emu.c display.c memory.c z80.c

@CODE_COVERAGE_RULES@
PZ80emu_CFLAGS=$(AM_CFLAGS) $(CODE_COVERAGE_CFLAGS)

TESTS=$(check_PROGRAMS)
check_PROGRAMS=testPZ80emu
testPZ80emu_SOURCES=testPZ80emu.c z80.c
testPZ80emu_CFLAGS=$(AM_CFLAGS) `pkg-config --cflags glib-2.0`
testPZ80emu_LDFLAGS=`pkg-config --libs glib-2.0`
