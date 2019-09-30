#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([OpenWnn], [1.0.0], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([src/openwnnWrapper.cc])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
LT_INIT
AC_ENABLE_SHARED
AC_DISABLE_STATIC
AC_PROG_LIBTOOL(libtool)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
#AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
