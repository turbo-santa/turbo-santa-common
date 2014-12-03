#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([third_party/gtest/src/gtest-internal-inl.h])
# AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])
AC_LANG([C++])

# CXX='g++-4.8'
CPPFLAGS='-I${top_srcdir} -I${top_srcdir}/third_party/gtest/include'
CXXFLAGS='-std=c++11 -O2' # -pg for profiling.
CFLAGS=''
LDFLAGS='-L/usr/local/lib'
LIBS='-lglog -lncurses'

# Checks for programs.
AC_PROG_CXX([g++-4.8 g++])
AC_PROG_CC
# AC_PROG_RANLIB
# AM_PROG_AR
LT_INIT([disable-shared])

# Checks for libraries.
# Cannot check for glog since it is all C++ and does not provide a non-mangled function.
# AC_SEARCH_LIBS([], [glog], [], [AC_MSG_ERROR([Could not find glog/logging.h. Please install from: code.google.com/p/google-glog/])])

# Checks for header files.
AC_CHECK_HEADERS([glog/logging.h], [], [AC_MSG_ERROR([Could not find glog/logging.h. Please install from: code.google.com/p/google-glog/])])
AC_CHECK_HEADERS([float.h glog/logging.h limits.h stddef.h stdlib.h string.h strings.h sys/time.h sys/timeb.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_STRERROR_R
AC_CHECK_FUNCS([dup2 fchdir getcwd getpagesize gettimeofday memset mkdir munmap regcomp rmdir socket strcasecmp strchr strdup strerror strrchr strstr strtol strtoull])

AC_CONFIG_FILES([Makefile
                 back_end/Makefile
                 back_end/graphics/Makefile
                 back_end/memory/Makefile
                 back_end/opcode_executor/Makefile
                 back_end/clocktroller/Makefile
                 test_harness/Makefile])

AC_CONFIG_SUBDIRS([third_party/gtest])

AC_OUTPUT
