AM_CFLAGS = -I$(top_builddir)/src/lib -Wall

bin_PROGRAMS = PZ80emu
PZ80emu_SOURCES = PZ80emu.c

PZ80emu_LDADD = $(top_builddir)/src/lib/libz80.a
PZ80emu_LDADD += $(top_builddir)/src/lib/libmemory.a
PZ80emu_LDADD += $(top_builddir)/src/lib/libdisplay.a

@CODE_COVERAGE_RULES@
PZ80emu_CFLAGS = $(AM_CFLAGS) $(CODE_COVERAGE_CFLAGS)
