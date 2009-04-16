lib_LTLIBRARIES = libILUT.la

libILUT_la_SOURCES = ilut_allegro.cpp      ilut_main.cpp  \
                     ilut_sdlsurface.cpp   ilut_states.cpp  \
                     ilut_internal.cpp     ilut_opengl.cpp  \
                     ilut_win32.cpp        ilut_directx.cpp  \
					 ilut_directx9.cpp     ilut_x11.cpp 

AM_CFLAGS = @LIBILUT_CFLAGS@

libILUT_la_LDFLAGS = @WIN32_EXTRA_LDFLAGS@ @VERSIONINFO@ \
                     $(top_builddir)/src-IL/src/libIL.la \
                     $(top_builddir)/src-ILU/src/libILU.la \
                     @LIBSDL@ @LIBALLEGRO@ @LIBILUT_WIN32LIBPATH@

libILUT_la_LIBADD = @LIBGL@ @LIBGDI32@ @X_LIBS@ @LIBX11@

INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/src-ILUT/include \
           @HSDL@ @HALLEGRO@
