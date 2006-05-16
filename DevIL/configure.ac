dnl 
dnl Autoconf script for DevIL Library
dnl Written by Meloni Dario
dnl 

dnl
dnl configure.in initialization
dnl
AC_INIT
AC_CONFIG_SRCDIR([README])

dnl
dnl Minimum Autoconf version
dnl
AC_PREREQ(2.52)

DEVIL_MAJOR=1
DEVIL_MINOR=6
DEVIL_REVISION=8
DEVIL_VERSION=$DEVIL_MAJOR$DEVIL_MINOR$DEVIL_REVISION

dnl
dnl Note that the libtool version has nothing to do
dnl with the actual DevIL version, it is a version number
dnl describing the interface version (see libtool doc)
dnl wich only changes when the interface changes  
dnl
DEVIL_LTVERSION=1:0:0
AC_SUBST(DEVIL_LTVERSION)

dnl
dnl Basic conf
dnl

SUPPORTED_FORMATS=""
SUPPORTED_API=""
SUPPORTED_EXTENSION=""
AC_REVISION($Revision: 1.5.2.6 $)
AC_CONFIG_AUX_DIR(libtools)
AC_PREFIX_DEFAULT(/usr/local)
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AC_LANG([C++])
AC_LANG([C])
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_C_BIGENDIAN
AC_LIBTOOL_WIN32_DLL
AC_PATH_XTRA

AM_INIT_AUTOMAKE("DevIL", $DEVIL_MAJOR.$DEVIL_MINOR.$DEVIL_REVISION)
AM_CONFIG_HEADER(include/IL/config.h)
AM_DISABLE_STATIC
AM_PROG_LIBTOOL
AM_MAINTAINER_MODE

dnl
dnl Check Machine Extensions
dnl
AC_DEFUN([TEST_EXT],[
AC_ARG_ENABLE($6,[  --disable-$6       Doesn't compile $1 support],
[use_$1=$enable_$1],[use_$1=yes])
AC_MSG_CHECKING([$1])
CFLAGS_TMP=$CFLAGS
CFLAGS=$CFLAGS" $2"
AC_COMPILE_IFELSE([
$5
int main() {
$3
return 0;}]
,[
if test "$use_$1" = "yes" ; then
  AC_DEFINE([$4],,[$1 extension found])
  AC_DEFINE([VECTORMEM],,[Memory is vector aligned])
  CFLAGS="$CFLAGS_TMP $2"
  AC_MSG_RESULT(yes)
  SUPPORTED_EXTENSION=$SUPPORTED_EXTENSION"$1 "
else
  AC_MSG_RESULT(disabled)
fi]
,[CFLAGS=$CFLAGS_TMP]
 [AC_MSG_RESULT(no)])]) 

TEST_EXT([Altivec],[-faltivec -maltivec],[vector int vec;vec=vec_splat_u32(0);],[ALTIVEC_GCC],[],[altivec])
TEST_EXT([SSE],[-msse],[__m128 v;v=_mm_sub_ss(v,v);],[SSE],[#include <xmmintrin.h>],[sse])
TEST_EXT([SSE2],[-msse2],[__m128i v;v=_mm_add_epi32(v,v);],[SSE2],[#include <emmintrin.h>],[sse2])
TEST_EXT([SSE3],[-msse3],[__m128d v;v=_mm_addsub_pd(v,v);],[SSE3],[#include <emmintrin.h>].[sse3])

dnl
dnl Memory alignment functions
dnl
AC_CHECK_FUNC(  valloc, [AC_DEFINE([VALLOC],,[valloc memory allocation])] )
AC_CHECK_FUNC(  posix_memalign, [AC_DEFINE([POSIX_MEMALIGN],,[posix_memalign memory allocation])] )
AC_CHECK_FUNC(  memalign, [AC_DEFINE([MEMALIGN],,[memalign memory allocation])])
AC_CHECK_HEADER(mm_malloc.h,[AC_DEFINE([MM_MALLOC],,[mm_malloc memory allocation])])

CFLAGS="-Wall $CFLAGS"
LIBIL_CFLAGS="$CFLAGS"
LIBILU_CFLAGS="$CFLAGS"
LIBILUT_CFLAGS="$CFLAGS"

dnl
dnl Libraries development checking
dnl
CPPFLAGS=$X_CFLAGS
AC_SUBST(X_CFLAGS)
AC_CHECK_HEADER(jpeglib.h,,no_jpeg=yes)
AC_CHECK_HEADER(tiffio.h,,no_tiff=yes)
AC_CHECK_HEADER(png.h,,no_png=yes)
AC_CHECK_HEADER(lcms/lcms.h,,no_lcms=yes)
AC_CHECK_HEADER(lcms.h,lcms_nodirinclude=yes)
AC_CHECK_HEADER(libmng.h,,no_mng="yes")
AC_CHECK_HEADER(d3d8.h,,use_directx="no")
case "$target" in
 	*apple-darwin*)
		AC_CHECK_HEADER(OpenGL/gl.h, [have_gl_hdr=yes], use_opengl="no")
							;;
	*)
		AC_CHECK_HEADER(GL/gl.h, [have_gl_hdr=yes], use_opengl="no")
							;;
esac
dnl AC_CHECK_HEADER(SDL.h, , use_sdl="no")
AC_CHECK_HEADER(zlib.h, LIBZ="-lz", LIBZ="")
AC_SUBST(LIBZ)

dnl Special code for LCMS
if test "$lcms_nodirinclude" = "yes" ; then
	no_lcms=no
else
	AC_DEFINE(LCMS_NODIRINCLUDE,,[LCMS include without lcms/ support])	
fi

dnl
dnl Formats
dnl
AC_DEFUN([TEST_FORMAT],[
AC_ARG_ENABLE([$1],[  --disable-$1           Doesn't compile $1 support],[use_$1="$enable_$1"],[use_$1="yes"])
if test "$no_$1" = "yes" ; then
	AC_DEFINE([IL_NO_$2],,[$2 support])
else
if test "$use_$1" = "yes" ; then
	SUPPORTED_FORMATS=$SUPPORTED_FORMATS"$2 "
else
	AC_DEFINE([IL_NO_$2],,[$2 support])
fi
fi
])

TEST_FORMAT(bmp,BMP)
TEST_FORMAT(dcx,DCX)
TEST_FORMAT(doom,WAD)
TEST_FORMAT(gif,GIF)
TEST_FORMAT(hdr,HDR)
TEST_FORMAT(icon,ICON)
TEST_FORMAT(jpeg,JPG)
TEST_FORMAT(lcms,LCMS)
TEST_FORMAT(lif,LIF)
TEST_FORMAT(mdl,MDL)
TEST_FORMAT(mng,MNG)
TEST_FORMAT(pcx,PCX)
TEST_FORMAT(pcd,PCD)
TEST_FORMAT(pic,PIC)
TEST_FORMAT(pix,PIX)
TEST_FORMAT(png,PNG)
TEST_FORMAT(pnm,PNM)
TEST_FORMAT(psd,PSD)
TEST_FORMAT(psp,PSP)
TEST_FORMAT(pxr,PXR)
TEST_FORMAT(raw,RAW)
TEST_FORMAT(sgi,SGI)
TEST_FORMAT(tga,TGA)
TEST_FORMAT(tiff,TIF)
TEST_FORMAT(xpm,XPM)
TEST_FORMAT(wal,WAL)

AC_DEFUN([TEST_API],[
AC_ARG_ENABLE($1,[  --disable-$1       Doesn't compile $1 support],
[use_$1=$enable_$1],[use_$1=yes])])

TEST_API(allegro)
TEST_API(directx)
TEST_API(opengl)
TEST_API(sdl)
TEST_API(win32)

AC_ARG_ENABLE(release,
[  --disable-release         Compile a debug version of the library],
[no_debug_symbols="$enable_release"],[no_debug_symbols="no"])

if test "$no_debug_symbols" = "yes" ; then
	      CFLAGS="-Wall -g -Werror"
    LIBIL_CFLAGS="-Wall -g -Werror"
   LIBILU_CFLAGS="-Wall -g -Werror"
  LIBILUT_CFLAGS="-Wall -g -Werror"
fi

case "$host" in
  *mingw32*)
		LIBIL_CFLAGS=$LIBIL_CFLAGS" -mno-cygwin"
		LIBILU_CFLAGS=$LIBILU_CFLAGS" -mno-cygwin"
		LIBILUT_CFLAGS=$LIBILUT_CFLAGS" -mno-cygwin"
		VERSIONINFO=""
              ;;
  *cygwin*)
		LIBILUT_WIN32LIBPATH="-L/usr/lib/w32api"
		VERSIONINFO=""
              ;;
  *)
		use_win32="no"
		use_directx="no"
		VERSIONINFO="-version-info $DEVIL_LTVERSION"
              ;;
esac
AC_SUBST(VERSIONINFO)


dnl
dnl Now we can check for bin libraries
dnl
if test "$no_jpeg" = "no" ; then
  AC_CHECK_LIB(jpeg, jpeg_start_decompress, LIBJPG="-ljpeg", LIBJPG="", )
  if test "$LIBJPG" != "-ljpeg" ; then
    no_jpeg="yes"
  else
    AC_CHECK_LIB(jpeg, jpeg_init_exif_field, use_exif="yes", use_exif="no", )
    if test "$use_exif" = "no" ; then
      AC_DEFINE(IL_USE_JPEGLIB_UNMODIFIED,,[Use jpeglib unmoified])
    fi
    AC_DEFINE(IL_USE_JPEGLIB_UNMODIFIED)
  fi
else
  LIBJPG=""
fi
AC_SUBST(LIBJPG)

case "$target" in
*-apple-darwin*)
    AC_DEFINE(IL_USE_JPEGLIB_UNMODIFIED)
    AC_DEFINE(MAX_OS_X,,[Building on Mac OS X])
esac

if test "$no_tiff" = "no" ; then
   AC_CHECK_LIB(tiff, TIFFReadRGBAImage, LIBTIFF="-ltiff", LIBTIFF="", -lm)
   if test "$LIBTIFF" != "-ltiff" ; then
     no_tiff="yes"
   fi
else
  LIBTIFF=""
fi
AC_SUBST(LIBTIFF)

if test "$no_png" = "no" ; then
   AC_CHECK_LIB(png, png_start_read_image, LIBPNG="-lpng", LIBPNG="", `libpng-config --libs`)
   if test "$LIBPNG" != "-lpng" ; then
     no_png="yes"
   fi
else
  LIBPNG=""
fi
AC_SUBST(LIBPNG)

if test "$no_lcms" = "no" ; then
   AC_CHECK_LIB(lcms,  cmsOpenProfileFromFile, LIBLCMS="-llcms", LIBLCMS="", -lm)
   if test "$LIBLCMS" != "-llcms" ; then
     no_lcms="yes"
   fi
else
  LIBLCMS=""
fi
AC_SUBST(LIBLCMS)


if test "$no_mng" = "no" ; then
   AC_CHECK_LIB(mng, mng_setcb_openstream, LIBMNG="-lmng", LIBMNG="", )
   if test "$LIBMNG" != "-lmng" ; then
     no_mng="yes"
   fi
else
  LIBMNG=""
fi
AC_SUBST(LIBMNG)

dnl
dnl ILUT APIs checking
dnl

dnl Check for SDL
AM_PATH_SDL(1.2.5,use_sdl="yes",use_sdl="no")
HSDL="$SDL_CFLAGS"
LIBSDL="$SDL_LIBS"
AC_SUBST(LIBSDL)
AC_SUBST(HSDL)

if test "$use_opengl" = "yes" ; then
case "$target" in
*-*-cygwin* | *-*-mingw32*)
	LIBGL="-lopengl32 -lglu32"
	;;
*-apple-darwin*)
	LIBGL="-Wl,-framework -Wl,OpenGL"
	;;
*-*-*)
	AC_CHECK_LIB(GL,glLoadIdentity,[LIBGL="-lGL"])
	AC_CHECK_LIB(GLU,gluPerspective,[LIBGL="-lGLU"])
	;;
esac
   if test "x$have_gl_hdr" != "xyes" ; then
     use_opengl="no"
     LIBGL=""
   fi
else
  LIBGL=""
fi

AC_SUBST(LIBGL)

if test "$use_allegro" = "yes" ; then
  AC_CHECK_PROG(allegro_config, allegro-config, yes, no, )
  if test $allegro_config = no ; then
    use_allegro="no"
    LIBALLEGRO=""
    HALLEGRO=""
  else
    LIBALLEGRO=`allegro-config --libs`
    HALLEGRO=`allegro-config --cflags`
  fi
fi
AC_SUBST(LIBALLEGRO)
AC_SUBST(HALLEGRO)

if test "$use_win32" = "no" ; then
  LIBGDI32=""
else
  LIBGDI32="-lgdi32"
fi
AC_SUBST(LIBGDI32)

dnl
dnl Prepares substitutions in config.h
dnl


if test "$use_allegro" = "yes" ; then
  AC_DEFINE(ILUT_USE_ALLEGRO,,[Support Allegro API])
  SUPPORTED_API=$SUPPORTED_API"Allegro "
fi

if test "$use_directx" = "yes" ; then
  AC_DEFINE(ILUT_USE_DIRECTX8,,[Support DirectX8 API])
  SUPPORTED_API=$SUPPORTED_API"Directx8 "
fi

if test "$use_opengl" = "yes" ; then
  AC_DEFINE(ILUT_USE_OPENGL,,[Support OpenGL API])
  SUPPORTED_API=$SUPPORTED_API"OpenGL "
  case "$target" in
    *-apple-darwin*)
	LIBILUT_CFLAGS=$LIBILUT_CFLAGS" -framework OpenGL"
  esac
fi

if test "$use_sdl" = "yes" ; then
  AC_DEFINE(ILUT_USE_SDL,,[Support SDL API])
  SUPPORTED_API=$SUPPORTED_API"SDL "
fi


AC_MSG_RESULT([ ])
AC_MSG_RESULT([+----------------------------------------+])
AC_MSG_RESULT([|     IL library  Supported formats      |])
AC_MSG_RESULT([+----------------------------------------+-------+])
AC_MSG_RESULT([$SUPPORTED_FORMATS])
AC_MSG_RESULT([ ])
AC_MSG_RESULT([+----------------------------------------+])
AC_MSG_RESULT([|      ILUT library  Supported APIs      |])
AC_MSG_RESULT([+----------------------------------------+-------+])
AC_MSG_RESULT([$SUPPORTED_API])
AC_MSG_RESULT([ ])
AC_MSG_RESULT([+----------------------------------------+])
AC_MSG_RESULT([|      Detected Machine Extensions       |])
AC_MSG_RESULT([+----------------------------------------+-------+])
AC_MSG_RESULT([$SUPPORTED_EXTENSION])

AC_MSG_RESULT([ ])

dnl
dnl Final vars
dnl
AC_SUBST(LIBIL_CFLAGS)
AC_SUBST(LIBILU_CFLAGS)
AC_SUBST(LIBILUT_CFLAGS)
AC_SUBST(LIBILUT_WIN32LIBPATH)

dnl
dnl Ouput files to generate
dnl note: keep the list on one line separated by single spaces 
dnl (otherwise MSYS compilation breaks)
AC_CONFIG_FILES([Makefile src-IL/Makefile src-IL/src/Makefile src-IL/include/Makefile src-ILU/Makefile src-ILU/src/Makefile src-ILU/include/Makefile src-ILUT/Makefile src-ILUT/src/Makefile src-ILUT/include/Makefile include/Makefile include/IL/Makefile ])
AC_OUTPUT
