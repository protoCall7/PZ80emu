TESTS = $(check_PROGRAMS)

check_PROGRAMS = test_z80 test_memory

test_z80_SOURCES = test_z80.c
test_z80_CFLAGS = -g -I$(top_srcdir)/src/lib
test_z80_CFLAGS += $(GLIB_CFLAGS)
test_z80_LDADD = $(top_builddir)/src/lib/libz80.a
test_z80_LDADD += $(top_builddir)/src/lib/libmemory.a
test_z80_LDADD += $(top_builddir)/src/lib/libdisplay.a
test_z80_LDADD += $(GLIB_LIBS)

test_memory_SOURCES = test_memory.c
test_memory_CFLAGS = -I$(top_srcdir)/src/lib
test_memory_CFLAGS += $(GLIB_CFLAGS)
test_memory_LDADD = $(top_builddir)/src/lib/libmemory.a
test_memory_LDADD += $(GLIB_LIBS)

EXTRA_DIST = test.bin \
test_ld_a.bin \
test_ld_b.bin \
test_ld_c.bin \
test_ld_d.bin \
test_ld_e.bin \
test_ld_h.bin \
test_ld_ixn.bin \
test_ld_iyn.bin \
test_ld_l.bin

@CODE_COVERAGE_RULES@
test_z80_CFLAGS += $(CODE_COVERAGE_CFLAGS)
test_memory_CFLAGS += $(CODE_COVERAGE_CFLAGS)
