#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([PZ80emu], [0.1])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([display.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lcurses':
AC_CHECK_LIB([curses], [main])

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h])

AC_CONFIG_MACRO_DIRS([m4])

# Enable coverage rules
AX_CODE_COVERAGE

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
