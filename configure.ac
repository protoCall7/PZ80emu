#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([PZ80emu], [0.1])
AC_CONFIG_SRCDIR([src/bin/PZ80emu.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([subdir-objects])

CFLAGS=''

# Checks for programs.
AC_PROG_CC_C99
AC_PROG_CPP
AC_PROG_RANLIB

AC_LANG([C])

# Checks for libraries.
AC_CHECK_LIB([curses], [initscr])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdint.h stdlib.h string.h])

PKG_CHECK_MODULES([GLIB], [glib-2.0])
AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

AC_CONFIG_MACRO_DIR([m4])

# Enable coverage rules
AX_CODE_COVERAGE

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile
				 src/bin/Makefile
				 src/lib/Makefile
				 tests/Makefile])
AC_OUTPUT
