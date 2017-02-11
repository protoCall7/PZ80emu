if DEBUG
  AM_CFLAGS =-I$(top_srcdir)/src/lib -Wall -g -O3
else
  AM_CFLAGS =-I$(top_srcdir)/src/lib -Wall
endif

bin_PROGRAMS = PZ80emu
PZ80emu_SOURCES = PZ80emu.c

PZ80emu_LDADD = $(top_builddir)/src/lib/libz80.a
PZ80emu_LDADD += $(top_builddir)/src/lib/libmemory.a
PZ80emu_LDADD += $(top_builddir)/src/lib/libdisplay.a

@DX_RULES@

@CODE_COVERAGE_RULES@
PZ80emu_CFLAGS = $(AM_CFLAGS) $(CODE_COVERAGE_CFLAGS)
