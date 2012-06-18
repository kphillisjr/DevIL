include Useful_stuff.am
include IL.am
include ILU.am
include ILUT.am

modulesdir = $(MODULES_DIR)

lib_LTLIBRARIES =
il_HEADERS =  $(sys_like_include)/IL/devil_cpp_wrapper.hpp
EXTRA_DIST = $(sys_like_include)/IL/ptr_defines.h
AM_LDFLAGS = $(devil_version_flag) $(LDFLAGS)
AM_CPPFLAGS = $(CPPFLAGS)

# The CFLAG_VISIBILITY is a gnulib feature for lib symbols visibility
AM_CFLAGS = $(GENERAL_CFLAGS)

localedir = @datadir@/locale

# We have to tell that we build the library since we use some weird macros
# in the public header (sunctions' visibility stuff)
DEFS = -D'LOCALEDIR="$(localedir)"' @DEFS@

LIBS += $(LTLIBINTL)

distclean-local:
	$(RM) ../include/IL/ptr_defines.h
	$(RM) @MODULES_LST_FILENAME@

if WINDOWS_BUILD
AM_LDFLAGS += -no-undefined
endif #WINDOWS_BUILD

if BUILD_IL

lib_LTLIBRARIES += libIL.la 
il_HEADERS += $(sys_like_include)/IL/il.h

endif #BUILD_IL

if BUILD_ILU

lib_LTLIBRARIES += libILU.la 
il_HEADERS += $(sys_like_include)/IL/ilu.h $(sys_like_include)/IL/ilu_region.h

endif #BUILD_ILU

if BUILD_ILUT

lib_LTLIBRARIES += libILUT.la 
il_HEADERS += $(sys_like_include)/IL/ilut.h $(sys_like_include)/IL/ilut_config.h

endif #BUILD_ILUT

wanted_classes = 

# Do we want to build the base class?
if BUILD_BASE
wanted_classes += @BASE_CLASS@.la
endif

if BUILD_EXTRA
wanted_classes += @EXTRA_CLASS@.la
endif

if BUILD_GAME
wanted_classes += @GAME_CLASS@.la
endif

if BUILD_JPEG
wanted_classes += @JPEG_CLASS@.la
endif

if BUILD_JP2
wanted_classes += @JP2_CLASS@.la
endif

if BUILD_EXR
wanted_classes += @EXR_CLASS@.la
endif

if BUILD_PNG
wanted_classes += @PNG_CLASS@.la
endif

if BUILD_MNG
wanted_classes += @MNG_CLASS@.la
endif

if BUILD_TIFF
wanted_classes += @TIFF_CLASS@.la
endif

if BUILD_DDS
wanted_classes += @DDS_CLASS@.la
endif

if BUILD_OTHERS
wanted_classes += @OTHERS_CLASS@.la
endif

# What will the class look like?
if BUILD_MODULES

# The classes will become loadable modules...
modules_LTLIBRARIES = $(wanted_classes)
modules_DATA = @MODULES_LST_FILENAME@

else #not BUILD_MODULES

# we can't hide symbols anywhere in modular builds.
# For instance modules need those internal symbols.
AM_CFLAGS += $(CFLAG_VISIBILITY)
libIL_la_LIBADD = $(wanted_classes)
# The classes will be statically linked to the IL library...
noinst_LTLIBRARIES = $(wanted_classes)
# classes_ldflags = -static

endif #not BUILD_MODULES

# gnulib's MSVC compat stuff
if HAVE_LD_OUTPUT_DEF
defexecdir = $(bindir)
defexec_DATA =
if BUILD_IL
defexec_DATA += libIL-$(DLL_VERSION).def
endif
if BUILD_ILU
defexec_DATA += libILU-$(DLL_VERSION).def
endif
if BUILD_ILUT
defexec_DATA += libILUT-$(DLL_VERSION).def
endif
DISTCLEANFILES = $(defexec_DATA)
endif #not HAVE_LD_OUTPUT_DEF
