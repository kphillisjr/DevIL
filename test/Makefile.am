include ../lib/Useful_stuff.am
include ../lib/IL.am

AM_CPPFLAGS = -I $(all_include)

check_PROGRAMS = testil
check_SCRIPTS = format_test/format_checks.sh 

if BUILD_MODULES
il_library = libIL_test.la
check_LTLIBRARIES = libIL_test.la
else # !BUILD_MODULES
il_library = ../lib/libIL.la
endif # !BUILD_MODULES

libIL_test_la_SOURCES = $(libIL_la_SOURCES)
#this is THE reason why we build the test library - we just need to do the trick with the modules location
libIL_test_la_CPPFLAGS = -I $(il_include) -I $(all_include) -D'MODULES_LST="@MODULES_LST_FILENAME@"' -D'MODULES_PATH="../lib"'
libIL_test_la_CFLAGS = $(libIL_la_CFLAGS)
libIL_test_la_CXXFLAGS = $(libIL_la_CXXFLAGS)
libIL_test_la_LDFLAGS = $(libIL_la_LDFLAGS) -rpath $(libdir)

if BUILD_ILU
check_SCRIPTS += format_test/ilu_checks.sh
endif #BUILD_ILU

testil_SOURCES = format_test/testil.c
testil_LDADD =
if BUILD_ILU
testil_LDADD += ../lib/libILU.la
else
testil_LDADD += $(il_library)
endif

TESTS_ENVIRONMENT = /bin/bash
TESTS = $(check_SCRIPTS)
