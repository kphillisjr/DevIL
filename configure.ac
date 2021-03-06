dnl 
dnl Autoconf script for DevIL Library
dnl Originally written by Meloni Dario
dnl rewritten by Matěj Týč
dnl

dnl
dnl configure.ac initialization
dnl we use some macros that are in m4 directory
dnl
AC_INIT([DevIL], 
	1.7.5,
	[bubla@users.sourceforge.net])
dnl Minimum Autoconf version
AC_PREREQ(2.52)
dnl What header will be generated by configure script
AC_CONFIG_HEADERS([include/IL/config.h])
dnl Where are some sources?
AC_CONFIG_SRCDIR([src-IL/src/il_main.c])
dnl Where are other macro definitions?
AC_CONFIG_MACRO_DIR([m4])
dnl Where to put various auxiliary files
AC_CONFIG_AUX_DIR([build-aux])
dnl Libtool init with message that we want to build DLLs
dnl LT_INIT([win32-dll])
dnl LT_PROG_RC
dnl THE ABOVE IS LIBTOOL INIT IS THE GOOD ONE, BUT MAYBE TOO UP-TO-DATE FOR MOST PEOPLE
AC_PROG_LIBTOOL
AC_LIBTOOL_WIN32_DLL
dnl Automake init with all warnings reported
AM_INIT_AUTOMAKE([-Wall])

dnl
dnl Note that the libtool version has nothing to do
dnl with the actual DevIL version, it is a version number
dnl describing the interface version (see libtool doc)
dnl wich only changes when the interface changes  
dnl
dnl format is <current>:<revision>:<age>
dnl If nothing notable changes, increment revision
dnl current is an interface number
dnl library supports interfaces from current - age to current
dnl
DEVIL_LTVERSION=1:1:0
AC_SUBST([DEVIL_LTVERSION])


dnl
dnl Test build options
dnl
AC_ARG_ENABLE([debug],
	      [AS_HELP_STRING([--enable-debug],
			      [Compile a debug version of the library])],
              [],
	      [enable_debug="no"])

AC_ARG_ENABLE([release],
	      [AS_HELP_STRING([--disable-release],
			      [Compile a debug version of the library, default enabled if not building debug version])],
              [],
	      [enable_release="yes"
	       test "x$enable_debug" = "xyes" && enable_release="no"])

AC_ARG_ENABLE([asm],
	      [AS_HELP_STRING([--disable-asm],
			      [Use assembler routines if possible, default enable if not building debug version])],
	      [],
	      [enable_asm="yes"
	       test "x$enable_debug" = "xyes" && enable_asm="no"])

AS_IF([test "x$enable_release" = "xyes"],
      [AS_IF([test "x$CFLAGS" = "x"],
	     [CFLAGS="-O3"]) ],
      [test "x$enable_debug" = "xyes"],
      [CFLAGS="$CFLAGS -DNOINLINE -Wall -g -O0"
       AC_DEFINE([DEBUG],
		 [],
		 ["Enable debug code features"]) ])

dnl
dnl Basic conf
dnl
SUPPORTED_FORMATS=""
SUPPORTED_API=""
SUPPORTED_EXTENSION=""
AC_REVISION([$Revision: 1.48 $])
dnl AC_PREFIX_DEFAULT(/usr/local)
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
dnl AC_LANG([C++])
AC_LANG([C])
AM_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_MAKE_SET
AC_C_BIGENDIAN

dnl checks for X are to be done later...
dnl AC_PATH_XTRA

dnl Settling whether the user will want to build examples or not
AC_ARG_WITH([examples],
	    [AS_HELP_STRING([--with-examples[[=yes/no]]],
			    [Whether to build and install examples as well. Default is without examples])],
	    [],
	    [with_examples="no"])
AM_CONDITIONAL([BUILD_EXAMPLES],
	       [test "x$with_examples" = "xyes"])

dnl Do we want monolithic library (DevIL instead of IL, ILU, ILUT). Useful probably only on Windows... Discouraged.
AC_ARG_ENABLE([monolithic],
	      [AS_HELP_STRING([--enable-monolithic[[=yes/no]]],
			      [Builds only one library instead of separate ones (IL, ILU, ILUT -> DevIL). Default set to 'disabled'])],
              [],
	      [enable_monolithic="no"])
AM_CONDITIONAL([BUILD_MONOLITHIC],
	       [test "x$enable_monolithic" = "xyes"])

dnl Whether to build IL. You have to anyway :-)
dnl AC_ARG_ENABLE([IL],
dnl	      [AS_HELP_STRING([--enable-IL[[=yes/no]]],
dnl	                      [Build IL part of DevIL. You  that. Default set to yes])],
dnl              [],
dnl	      [enable_IL="yes"])
enable_IL="yes"
AM_CONDITIONAL([BUILD_IL],
	       [test "x$enable_IL" = "xyes"])

dnl Whether to build ILU...
AC_ARG_ENABLE([ILU],
	      [AS_HELP_STRING([--enable-ILU[[=yes/no]]],
			      [Build ILU part of DevIL. Default set to no])],
              [],
	      [enable_ILU="no"])

dnl Whether to build ILUT...
AC_ARG_ENABLE([ILUT],
	      [AS_HELP_STRING([--enable-ILUT[[=yes/no]]],
			      [Build ILUT part of DevIL. Default set to no. ILU will be built along whether you wan it or not since ILUT depends upon it.])],
              [],
	      [enable_ILUT="no"])
dnl We have to build ILU if we want ILUT as well...
AM_CONDITIONAL([BUILD_ILU],
	       [test "x$enable_ILU" = "xyes" -o "x$enable_ILUT" = "xyes"])
AM_CONDITIONAL([BUILD_ILUT],
	       [test "x$enable_ILUT" = "xyes"])

dnl Test for restrict c/c++ keyword
DEVIL_CHECK_RESTRICT

dnl checking if the -std=gnu99 flag is required
AS_IF([test  "x$restrict_keyword" = "xfound"],
      [DEVIL_CHECK_RESTRICT_GNU99])

dnl must add AC_CACHE_CHECK support where possible

dnl AM_MAINTAINER_MODE

dnl Now the phase of testing of extensions...
TEST_EXT([Altivec],
	 [-faltivec -maltivec],
	 [vector int vec;vec=vec_splat_u32(0);],
	 [ALTIVEC_GCC],
	 [],
	 [altivec])

TEST_EXT([SSE],
	 [-msse],
	 [__m128 v;v=_mm_sub_ss(v,v);],
	 [SSE],
	 [#include <xmmintrin.h>],
	 [sse])

TEST_EXT([SSE2],
	 [-msse2],
	 [__m128i v;v=_mm_add_epi32(v,v);],
	 [SSE2],
	 [#include <emmintrin.h>],
	 [sse2])

TEST_EXT([SSE3],
	 [-msse3],
	 [__m128d v;v=_mm_addsub_pd(v,v);],
	 [SSE3],
	 [#include <pmmintrin.h>],
	 [sse3])

dnl Do we want to use ASM routines?
AS_IF([test "x$enable_asm" = "xyes"],
      [TEST_ASM([i[[3456]]86*],
                [X86_ASM],
	        [GCC_X86_ASM],
                [x86])
       TEST_ASM([x86_64*],
                [X86_64_ASM],
                [GCC_X86_64_ASM],
                [x86_64])
       TEST_ASM([powerpc*],
                [PPC_ASM],
                [GCC_PCC_ASM],
                [ppc]) 
	AS_IF([test -n "$SUPPORTED_EXTENSION"],
	      [AC_DEFINE([IL_INLINE_ASM],
			 [1],
			 [Define if you can support at least some ASM]) ]) ])

dnl
dnl Test warning build options
dnl
AC_ARG_ENABLE([warning],
              [AC_HELP_STRING([--enable-warning],
			      [Compile breaks on warnings])],
	      [build_warning="$enable_warning"],
	      [build_warning="no"])

AS_IF([test "$build_warning" = "yes"],
      [ADD_CFLAGS([-Wall -Werror])],
      [])

dnl
dnl Memory alignment functions
dnl
AC_CHECK_FUNC([valloc], 
	      [AC_DEFINE([VALLOC],
			 [1],
			 [define if you have valloc memory allocation]) ])
AC_CHECK_FUNC([posix_memalign], 
	      [AC_DEFINE([POSIX_MEMALIGN],
			 [1],
			 [define if you have posix_memalign memory allocation]) ])

AC_CHECK_FUNC([memalign], 
	      [AC_DEFINE([MEMALIGN],
			 [1],
			 [define if you have memalign memory allocation]) ])

AC_CHECK_HEADER([mm_malloc.h],
		[AC_DEFINE([MM_MALLOC],
			   [1],
			   [define if you have mm_malloc.h header]) ])

dnl
dnl Libraries development checking
dnl
AC_ARG_WITH([zlib],
            [AC_HELP_STRING([--with-zlib=[[yes/no]]],
                            [Do wou want to use zlib?])],
	    [],
            [enable_zlib="yes"])

AS_IF([test "x$enable_zlib" = "xyes"],
      [AC_CHECK_HEADER([zlib.h], 
		       [use_z="yes"], 
		       [use_z="no"]) 
       AS_IF([test "$enable_zlib" = "yes"],
             [AC_CHECK_LIB([z],
                           [main]) ]) ])
dnl 
dnl Ask here whether the user wants to support a format, optionally do some tests whether it can be supported
dnl

TEST_FORMAT([bmp], [BMP])
TEST_FORMAT([dcx], [DCX])
TEST_FORMAT([doom],[WAD])
TEST_FORMAT([exr], [EXR], [SETTLE_OPENEXR])
TEST_FORMAT([gif], [GIF])
TEST_FORMAT([hdr], [HDR])
TEST_FORMAT([icns],[ICNS])
TEST_FORMAT([icon],[ICON])
TEST_FORMAT([jpeg],[JPG], [SETTLE_JPEG])
TEST_FORMAT([jp2], [JP2], [SETTLE_JASPER])
TEST_FORMAT([lcms],[LCMS],[SETTLE_LCMS])
TEST_FORMAT([lif], [LIF])
TEST_FORMAT([mdl], [MDL])
TEST_FORMAT([mng], [MNG], [SETTLE_MNG])
TEST_FORMAT([pcx], [PCX])
TEST_FORMAT([pcd], [PCD])
TEST_FORMAT([pic], [PIC])
TEST_FORMAT([pix], [PIX])
TEST_FORMAT([png], [PNG], [SETTLE_PNG])
TEST_FORMAT([pnm], [PNM])
TEST_FORMAT([psd], [PSD])
TEST_FORMAT([psp], [PSP])
TEST_FORMAT([pxr], [PXR])
TEST_FORMAT([raw], [RAW])
TEST_FORMAT([sgi], [SGI])
TEST_FORMAT([tga], [TGA])
TEST_FORMAT([tiff],[TIF], [SETTLE_TIFF])
TEST_FORMAT([xpm], [XPM])
TEST_FORMAT([wal], [WAL])
TEST_FORMAT([wdp], [WDP])

AC_DEFINE([IL_NO_WBMP],
	  [1],
	  [WBMP loading is not implemented  yet])

dnl
dnl ILUT APIs library checking
dnl
AS_IF([test "x$enable_ILUT" = "xyes"],
      [SETTLE_ALLEGRO
       SETTLE_DX8
       SETTLE_DX9
       SETTLE_OPENGL
       SETTLE_SDL
       SETTLE_W32
       SETTLE_X11])
 
dnl
dnl AM_COND_IF([condition],[do-if-true]) has to be kept in sync with examples/Makefile.am
dnl The AM_COND_IF seems to be too new at the moment, let's wait a little bit till things settle...
dnl

dnl AM_COND_IF([BUILD_ILU],
AS_IF([test "x$enable_ILU" = "xyes"],
	   [EXAMPLES_TO_BUILD="$EXAMPLES_TO_BUILD ILU_override ILU_read ILU_simple"])

dnl AM_COND_IF([BUILD_ILUT],
AS_IF([test "x$enable_ILUT" = "xyes"],
	   [EXAMPLES_TO_BUILD="$EXAMPLES_TO_BUILD ILUT_cpp_wrapper"])

AM_CONDITIONAL([USE_ALLEGRO], 
	       [test "x$use_allegro" = "xyes"])
dnl AM_COND_IF([USE_ALLEGRO],
AS_IF([test "x$use_allegro" = "xyes"],
	   [EXAMPLES_TO_BUILD="$EXAMPLES_TO_BUILD ILUT_allegro"])

AM_CONDITIONAL([USE_DIRECTX8],
               [test "x$use_directx8" = "xyes"])

AM_CONDITIONAL([USE_DIRECTX9],
               [test "x$use_directx9" = "xyes"])

AM_CONDITIONAL([USE_DIRECTX],
               [test "x$use_directx8" = "xyes" -o "x$use_directx9" = "xyes"])
dnl AM_COND_IF([USE_DIRECTX],
AS_IF([test "x$use_directx8" = "xyes" -o "x$use_directx9" = "xyes"],
	   [EXAMPLES_TO_BUILD="$EXAMPLES_TO_BUILD ILUT_d3d ILUT_windows"])

AM_CONDITIONAL([USE_OPENGL],
               [test "x$use_ogl" = "xyes"])

AM_CONDITIONAL([HAVE_GLUT],
	       [test "x$have_glut" = "xyes"])

AM_CONDITIONAL([USE_SDL],
               [test "x$use_sdl" = "xyes"])
dnl AM_COND_IF([USE_SDL],
AS_IF([test "x$use_sdl" = "xyes"],
	   [EXAMPLES_TO_BUILD="$EXAMPLES_TO_BUILD ILUT_sdl"])

AM_CONDITIONAL([USE_W32],
               [test "x$use_w32" = "xyes"])
dnl AM_COND_IF([USE_W32],
AS_IF([test "x$use_w32" = "xyes"],
	   [EXAMPLES_TO_BUILD="$EXAMPLES_TO_BUILD ILUT_animation"])

AM_CONDITIONAL([USE_X11],
               [test "x$use_x11" = "xyes"])
#this is instedad AM_COND_IF([USE_OPENGL && USE_X11])
AS_IF([test "x$have_glut" = "xyes" -a "x$use_x11" = "xyes"],
      [EXAMPLES_TO_BUILD="$EXAMPLES_TO_BUILD ILUT_gl ILUT_volume"])

dnl Win32
AS_CASE([$host],
        [*mingw32*],
        [ADD_CFLAGS([-mno-cygwin])
	 PLATFORM="MINGW32"],
        [*cygwin*],
        [LIBILUT_WIN32LIBPATH="-L/usr/lib/w32api"
	 PLATFORM="CYGWIN"],
        [*darwin*],
	[AC_DEFINE([MAX_OS_X],
		   [],
		   [Building on Mac OS X]) 
	 PLATFORM="OSX"])

AM_CONDITIONAL([WINDOWS_BUILD],
	       [test "x$PLATFORM" = "xMINGW32" -o "x$PLATFORM" = "xCYGWIN"])

dnl Final substitution
AC_SUBST([GENERAL_CFLAGS])
AC_SUBST([IL_CFLAGS])
AC_SUBST([IL_LIBS])
AC_SUBST([ILU_CFLAGS])
AC_SUBST([ILU_LIBS])
AC_SUBST([ILUT_CFLAGS])
AC_SUBST([ILUT_LIBS])
AC_SUBST([LIBILUT_WIN32LIBPATH])

dnl
dnl Ouput files to generate
dnl note: keep the list on one line separated by single spaces 
dnl (otherwise MSYS compilation breaks)
AC_CONFIG_FILES([Makefile 
		 lib/Makefile 
		 examples/Makefile
		 docs/Makefile 
		 data/DevIL.pc 
		 data/IL.pc data/ILU.pc 
		 data/ILUT.pc])
dnl		 projects/Makefile]) 
# src-IL/Makefile  src-IL/src/Makefile  src-IL/include/Makefile  src-ILU/Makefile  src-ILU/src/Makefile  src-ILU/include/Makefile  src-ILUT/Makefile  src-ILUT/src/Makefile  src-ILUT/include/Makefile  include/Makefile  include/IL/Makefile
AC_OUTPUT

dnl And a nice report in the end!
IL_REPORT
ILU_REPORT
ILUT_REPORT
EXTENSIONS_REPORT
EXAMPLES_REPORT
