TESTS = $(check_PROGRAMS)

check_PROGRAMS = test_z80 test_memory

test_z80_SOURCES = test_z80.c
test_z80_CFLAGS = -I$(top_builddir)/src/lib
test_z80_CFLAGS += $(GLIB_CFLAGS)
test_z80_LDADD = $(top_builddir)/src/lib/libz80.a
test_z80_LDADD += $(GLIB_LIBS)

test_memory_SOURCES = test_memory.c
test_memory_CFLAGS = -I$(top_builddir)/src/lib
test_memory_CFLAGS += $(GLIB_CFLAGS)
test_memory_LDADD = $(top_builddir)/src/lib/libmemory.a
test_memory_LDADD += $(GLIB_LIBS)

@CODE_COVERAGE_RULES@
test_z80_CFLAGS += $(CODE_COVERAGE_CFLAGS)
test_memory_CFLAGS += $(CODE_COVERAGE_CFLAGS)
