lib_LTLIBRARIES = libILU.la

libILU_la_SOURCES = ilu_alloc.cpp        ilu_internal.cpp   ilu_noise.cpp  \
                    ilu_scale.cpp        ilu_error.cpp      ilu_main.cpp  \
                    ilu_rotate.cpp       ilu_scaling.cpp    ilu_filter.cpp  \
                    ilu_manip.cpp        ilu_scale2d.cpp    ilu_states.cpp  \
                    ilu_filter_rcg.cpp   ilu_mipmap.cpp     ilu_scale3d.cpp  \
                    ilu_utilities.cpp 	  ilu_region.cpp 

AM_CFLAGS = @LIBILU_CFLAGS@

libILU_la_LDFLAGS = @WIN32_EXTRA_LDFLAGS@ @VERSIONINFO@ \
                    $(top_srcdir)/src-IL/src/libIL.la

INCLUDES = -I$(top_builddir)/include -I$(top_builddir)/src-ILU/include
