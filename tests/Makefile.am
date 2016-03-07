TESTS = $(check_PROGRAMS)

check_PROGRAMS = test_z80

test_z80_SOURCES = test_z80.c
test_z80_CFLAGS = -I$(top_builddir)/src/lib
test_z80_CFLAGS += $(GLIB_CFLAGS)
test_z80_LDADD = $(top_builddir)/src/lib/libz80.a
test_z80_LDADD += $(GLIB_LIBS)

@CODE_COVERAGE_RULES@
test_z80_CFLAGS += $(CODE_COVERAGE_CFLAGS)
