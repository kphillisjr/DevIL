ACLOCAL_AMFLAGS = -I m4

SUBDIRS = gnulib lib bin docs test data po #tests

#SUBDIRS = src-IL src-ILU src-ILUT include
#DIST_SUBDIRS = src-IL src-ILU src-ILUT include
EXTRA_DIST = build-aux/config.rpath\
	README.unix\
	README.win Libraries.txt \
	CREDITS MSVC++.txt \
	configure.bat \
	README.macosx \
	m4/gnulib-cache.m4

# (uncoment this if you want to include examples to the distributed tarball)
# Actually it has to be commented now... It is currently very troublesome to include the projects in the distribution archive...
#SUBDIRS += projects

DISTCHECK_CONFIGURE_FLAGS = --enable-ILU --with-examples # --enable-ILUT #ILUT makes trouble
