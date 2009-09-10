include ../lib/Useful_stuff.am
include ../lib/IL.am

AM_CPPFLAGS = -I $(all_include)

check_PROGRAMS = testil
check_SCRIPTS = format_test/format_checks.sh 

il_library = ../lib/libIL.la

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

TESTS_ENVIRONMENT = @IL_MODULES_ENV@="../lib" /bin/bash
TESTS = $(check_SCRIPTS)
