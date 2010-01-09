include ../lib/Useful_stuff.am
include ../lib/IL.am

AM_CPPFLAGS = -I $(sys_like_include)

check_PROGRAMS = testil
TESTS = format_test/format_checks.sh
# I don't know what's the meaning of this... But it at least does make sense
check_SCRIPTS = format_test/generate-images.sh
#check_SCRIPTS = format_test/format_checks.sh 
EXTRA_DIST = format_test/generate-images.sh
il_library = ../lib/libIL.la

if BUILD_ILU
#check_SCRIPTS += format_test/ilu_checks.sh
TESTS += format_test/ilu_checks.sh
endif #BUILD_ILU

testil_SOURCES = format_test/testil.c
testil_LDADD =

if BUILD_ILU
testil_LDADD += ../lib/libILU.la
else
testil_LDADD += $(il_library)
endif

TESTS_ENVIRONMENT = @IL_MODULES_ENV@="$(abs_top_builddir)/lib" /bin/sh
#TESTS = $(check_SCRIPTS)
check_SCRIPTS += $(TESTS)

