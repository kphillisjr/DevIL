/* include/IL/config.h.  Generated by configure.  */
/* include/IL/config.h.in.  Generated automatically from configure.in by autoheader.  */
#ifndef __CONFIG_H__
#define __CONFIG_H__


/* Supported images formats (il) */

/* #undef IL_NO_BMP */
/* #undef IL_NO_CUT */
/* #undef IL_NO_DCX */
/* #undef IL_NO_DDS */
/* #undef IL_NO_DOOM */
/* #undef IL_NO_GIF */
/* #undef IL_NO_ICO */
/* #undef IL_NO_JPG */
#define IL_NO_LCMS 1
/* #undef IL_NO_LIF */
/* #undef IL_NO_MDL */
#define IL_NO_MNG 1
/* #undef IL_NO_PCD */
/* #undef IL_NO_PCX */
/* #undef IL_NO_PIC */
/* #undef IL_NO_PIX */
/* #undef IL_NO_PNG */
/* #undef IL_NO_PNM */
/* #undef IL_NO_PSD */
/* #undef IL_NO_PSP */
/* #undef IL_NO_PXR */
/* #undef IL_NO_RAW */
/* #undef IL_NO_SGI */
/* #undef IL_NO_TGA */
/* #undef IL_NO_TIF */
/* #undef IL_NO_WAL */
/* #undef IL_NO_XPM */
#define IL_USE_JPEGLIB_UNMODIFIED 1

/* Supported api (ilut) */

/* #undef ILUT_USE_ALLEGRO */
/* #undef ILUT_USE_DIRECTX8 */
/* #undef ILUT_USE_DIRECTX9 */
#define ILUT_USE_OPENGL 1
#define ILUT_USE_SDL 1
#define ILUT_USE_WIN32 1


/* Define if you have the <dlfcn.h> header file. */
/* #undef HAVE_DLFCN_H */

/* Name of package */
#define PACKAGE "DevIL"

/* Version number of package */
#define VERSION "1.6.6"

/* Define if your processor stores words with the most significant byte first
   (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef WORDS_BIGENDIAN */

/* Define if the X Window System is missing or not being used. */
#define X_DISPLAY_MISSING 1
#endif /* __CONFIG_H__ */
