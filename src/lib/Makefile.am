if DEBUG
  AM_CFLAGS =-I$(top_srcdir)/src/lib -Wall -g -O3
else
  AM_CFLAGS =-I$(top_srcdir)/src/lib -Wall
endif

noinst_LIBRARIES = libz80.a libmemory.a libdisplay.a
noinst_HEADERS = z80.h memory.h display.h utils.h

libz80_a_SOURCES = z80.c

libmemory_a_SOURCES = memory.c

libdisplay_a_SOURCES = display.c

@DX_RULES@

@CODE_COVERAGE_RULES@
libz80_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)
libmemory_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)
libdisplay_a_CFLAGS = $(CODE_COVERAGE_CFLAGS)
