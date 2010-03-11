//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 03/07/2009
//
// Filename: IL/il.h
//
// Description: The main include file for DevIL
//
//-----------------------------------------------------------------------------

// Doxygen comment
/*! \file il.h
    The main include file for DevIL
*/

#ifndef __il_h_
#ifndef _IL_H__

#define __il_h_
#define _IL_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined __WIN32__
#define DEVIL_EXPORT __declspec(dllexport)
#define DEVIL_IMPORT __declspec(dllimport)
#elif defined HAVE_VISIBILITY /* This set can overlap with the above, so it goes after it */
#define DEVIL_EXPORT __attribute__((__visibility__("default")))
#define DEVIL_IMPORT /* we don't need this */
#else	/* No visibility at all */
#define DEVIL_EXPORT /* we don't need this */
#define DEVIL_IMPORT /* we don't need this */
#endif

#ifdef IL_BUILD_LIBRARY
#define ILAPI DEVIL_EXPORT
#else
#define ILAPI DEVIL_IMPORT
#endif

//this define controls if floats and doubles are clampled to [0..1]
//during conversion. It takes a little more time, but it is the correct
//way of doing this. If you are sure your floats are always valid,
//you can undefine this value...
#define CLAMP_HALF		1
#define CLAMP_FLOATS	1
#define CLAMP_DOUBLES	1

/*#ifdef _WIN32_WCE
	#define IL_NO_EXR
	#define IL_NO_GIF
	#define IL_NO_JP2
	#define IL_NO_JPG
	#define IL_NO_MNG
	#define IL_NO_PNG
	#define IL_NO_TIF
	#define IL_NO_LCMS
#endif //_WIN32_WCE

#ifdef DJGPP
	#define IL_NO_EXR
	#define IL_NO_GIF
	#define IL_NO_JP2
	#define IL_NO_JPG
	#define IL_NO_MNG
	#define IL_NO_PNG
	#define IL_NO_TIF
	#define IL_NO_LCMS
#endif //DJGPP*/

#ifdef _WIN32
	#if (defined(IL_USE_PRAGMA_LIBS)) && (!defined(IL_BUILD_LIBRARY))
		#if defined(_MSC_VER) || defined(__BORLANDC__)
			#pragma comment(lib, "DevIL.lib")
		#endif
	#endif
#endif

#ifdef RESTRICT_KEYWORD
#define RESTRICT restrict
#define CONST_RESTRICT const restrict
#else
#define RESTRICT
#define CONST_RESTRICT const
#endif

#include <stdio.h>

typedef unsigned int   ILenum;
typedef unsigned char  ILboolean;
typedef unsigned int   ILbitfield;
typedef signed char    ILbyte;
typedef signed short   ILshort;
typedef int     	   ILint;
typedef size_t         ILsizei;
typedef unsigned char  ILubyte;
typedef unsigned short ILushort;
typedef unsigned int   ILuint;
typedef float          ILfloat;
typedef float          ILclampf;
typedef double         ILdouble;
typedef double         ILclampd;

#ifdef _MSC_VER
typedef __int64          ILint64;
typedef unsigned __int64 ILuint64;
#else
typedef long long int          ILint64;
typedef long long unsigned int ILuint64;
#endif


#include <limits.h>
#ifdef _UNICODE
	#ifndef _WIN32_WCE
		#include <wchar.h>
	#endif
	//if we use a define instead of a typedef,
	//ILconst_string works as intended
	#define ILchar wchar_t
	#define ILstring wchar_t*
	#define ILconst_string  wchar_t const *
#else
	//if we use a define instead of a typedef,
	//ILconst_string works as intended
	#define ILchar char
	#define ILstring char*
	#define ILconst_string char const *
#endif //_UNICODE

#define IL_FALSE			0
#define IL_TRUE				1

//  Matches OpenGL's right now.
//! Data formats \link Formats Formats\endlink
enum {
IL_COLOUR_INDEX = 0x1900,
IL_COLOR_INDEX = 0x1900,
IL_ALPHA = 0x1906,
IL_RGB,
IL_RGBA,
IL_BGR = 0x80E0,
IL_BGRA,
IL_LUMINANCE = 0x1909,
IL_LUMINANCE_ALPHA,

//! Data types \link Types Types\endlink
IL_BYTE = 0x1400,
IL_UNSIGNED_BYTE,
IL_SHORT,
IL_UNSIGNED_SHORT,
IL_INT,
IL_UNSIGNED_INT,
IL_FLOAT,
IL_DOUBLE,
IL_HALF,
};

#define IL_MAX_BYTE		  		SCHAR_MAX
#define IL_MAX_UNSIGNED_BYTE  	UCHAR_MAX
#define IL_MAX_SHORT	  		SHRT_MAX
#define IL_MAX_UNSIGNED_SHORT 	USHRT_MAX
#define IL_MAX_INT		  		INT_MAX
#define IL_MAX_UNSIGNED_INT   	UINT_MAX

#define IL_LIMIT(x,m,M)		( (x) < (m) ? (m) : ( (x) > (M) ? (M) : (x) ) )
#define IL_CLAMP(x) 		IL_LIMIT( (x), 0, 1)

#define IL_VENDOR   0x1F00
#define IL_LOAD_EXT 0x1F01
#define IL_SAVE_EXT 0x1F02


//
// IL-specific #define's
//

#define IL_VERSION_1_7_8 1
#define IL_VERSION       178


// Attribute Bits
enum {
IL_ORIGIN_BIT =	0x00000001,
IL_FILE_BIT =	0x00000002,
IL_PAL_BIT =	0x00000004,
IL_FORMAT_BIT =	0x00000008,
IL_TYPE_BIT =	0x00000010,
IL_COMPRESS_BIT =	0x00000020,
IL_LOADFAIL_BIT =	0x00000040,
IL_FORMAT_SPECIFIC_BIT =	0x00000080,
IL_ALL_ATTRIB_BITS =	0x000FFFFF,
};

// Palette types
enum {
IL_PAL_NONE = 0x400,
IL_PAL_RGB24,
IL_PAL_RGB32,
IL_PAL_RGBA32,
IL_PAL_BGR24,
IL_PAL_BGR32,
IL_PAL_BGRA32,
};


// Image types
enum {
IL_BMP,  //!< Microsoft Windows Bitmap - .bmp extension
IL_CUT,  //!< Dr. Halo - .cut extension
IL_DOOM,  //!< DooM walls - no specific extension
//IL_DOOM_FLAT,  //!< DooM flats - no specific extension
IL_ICON,  //!< Microsoft Windows Icons and Cursors - .ico and .cur extensions
IL_JPG,  //!< JPEG - .jpg, .jpe and .jpeg extensions
IL_JPEG = IL_JPG,  //!< JPEG - .jpg, .jpe and .jpeg extensions
IL_ILBM, //!< Amiga IFF (FORM ILBM) - .iff, .ilbm, .lbm extensions
IL_PCD,  //!< Kodak PhotoCD - .pcd extension
IL_PCX,  //!< ZSoft PCX - .pcx extension
IL_PIC,  //!< PIC - .pic extension
IL_PNG,  //!< Portable Network Graphics - .png extension
IL_PNM,  //!< Portable Any Map - .pbm, .pgm, .ppm and .pnm extensions
IL_SGI,  //!< Silicon Graphics - .sgi, .bw, .rgb and .rgba extensions
IL_TGA,  //!< TrueVision Targa File - .tga, .vda, .icb and .vst extensions
IL_TIFF,  //!< Tagged Image File Format - .tif and .tiff extensions
IL_CHEAD,  //!< C-Style Header - .h extension
IL_RAW,  //!< Raw Image Data - any extension
IL_MDL,  //!< Half-Life Model Texture - .mdl extension
IL_WAL,  //!< Quake 2 Texture - .wal extension
IL_LIF,  //!< Homeworld Texture - .lif extension
IL_MNG,  //!< Multiple-image Network Graphics - .mng extension
IL_GIF,  //!< Graphics Interchange Format - .gif extension
IL_DDS,  //!< DirectDraw Surface - .dds extension
IL_DCX,  //!< ZSoft Multi-PCX - .dcx extension
IL_PSD,  //!< Adobe PhotoShop - .psd extension
IL_PSP,  //!< PaintShop Pro - .psp extension
IL_PIX,  //!< PIX - .pix extension
IL_PXR,  //!< Pixar - .pxr extension
IL_XPM,  //!< X Pixel Map - .xpm extension
IL_HDR,  //!< Radiance High Dynamic Range - .hdr extension
IL_ICNS,  //!< Macintosh Icon - .icns extension
IL_JP2,  //!< Jpeg 2000 - .jp2 extension
IL_EXR,  //!< OpenEXR - .exr extension
IL_VTF,  //!< Valve Texture Format - .vtf extension
IL_WBMP,  //!< Wireless Bitmap - .wbmp extension
IL_SUN,  //!< Sun Raster - .sun, .ras, .rs, .im1, .im8, .im24 and .im32 extensions
IL_IFF,  //!< Interchange File Format - .iff extension
IL_TPL,  //!< Gamecube Texture - .tpl extension
IL_FITS,  //!< Flexible Image Transport System - .fit and .fits extensions
IL_DICOM,  //!< Digital Imaging and Communications in Medicine (DICOM) - .dcm and .dicom extensions
IL_IWI,  //!< Call of Duty Infinity Ward Image - .iwi extension
IL_BLP,  //!< Blizzard Texture Format - .blp extension
IL_FTX,  //!< Heavy Metal: FAKK2 Texture - .ftx extension
IL_ROT,  //!< Homeworld 2 - Relic Texture - .rot extension
IL_TEXTURE,  //!< Medieval II: Total War Texture - .texture extension
IL_DPX, //!< Digital Picture Exchange - .dpx extension
IL_UTX, //!< Unreal (and Unreal Tournament) Texture - .utx extension
IL_FORMATS_COUNT,	/**< How many formats do we have? */
IL_TYPE_UNKNOWN,
IL_JFIF,  //!<
IL_JNG,  //!< 
IL_EXIF,  //!< 
IL_WDP,  //!< 
};
#define IL_MP3			0x0452  //!< MPEG-1 Audio Layer 3 - .mp3 extension

#define IL_JASC_PAL     0x0475  //!< PaintShop Pro Palette

// Error Types
enum {
IL_NO_ERROR = 0,
IL_INVALID_ENUM,
IL_OUT_OF_MEMORY,
IL_FORMAT_NOT_SUPPORTED,
IL_INTERNAL_ERROR,
IL_INVALID_VALUE,
IL_ILLEGAL_OPERATION,
IL_ILLEGAL_FILE_VALUE,
IL_INVALID_FILE_HEADER,
IL_INVALID_PARAM,
IL_COULD_NOT_OPEN_FILE,
IL_INVALID_EXTENSION,
IL_FILE_ALREADY_EXISTS,
IL_STACK_OVERFLOW,
IL_STACK_UNDERFLOW,
IL_INVALID_CONVERSION,
IL_BAD_DIMENSIONS,
IL_FILE_READ_ERROR,  // 05/12/2002: Addition by Sam.
IL_FILE_WRITE_ERROR,
IL_LIB_GIF_ERROR,
IL_LIB_JPEG_ERROR,
IL_LIB_PNG_ERROR,
IL_LIB_TIFF_ERROR,
IL_LIB_MNG_ERROR,
IL_LIB_JP2_ERROR,
IL_LIB_EXR_ERROR,
IL_UNKNOWN_ERROR,
};

enum {
// Origin Definitions
IL_ORIGIN_SET = 0x0600,
IL_ORIGIN_LOWER_LEFT,
IL_ORIGIN_UPPER_LEFT,
IL_ORIGIN_MODE,

// Format and Type Mode Definitions
IL_FORMAT_SET =	0x0610,
IL_FORMAT_MODE,
IL_TYPE_SET,
IL_TYPE_MODE,

// File definitions
IL_FILE_OVERWRITE = 0x0620,
IL_FILE_MODE,
IL_CONV_PAL = 0x0630,

// Load fail definitions
IL_DEFAULT_ON_FAIL = 0x0632,

// Key colour and alpha definitions
IL_USE_KEY_COLOUR = 0x0635,
IL_USE_KEY_COLOR = 0x0635,
IL_BLIT_BLEND,

// Interlace definitions
IL_SAVE_INTERLACED = 0x0639,
IL_INTERLACE_MODE,

// Quantization definitions
IL_QUANTIZATION_MODE  = 0x0640,
IL_WU_QUANT,
IL_NEU_QUANT,
IL_NEU_QUANT_SAMPLE,
IL_MAX_QUANT_INDEXS,
IL_MAX_QUANT_INDICES,

// Hints
IL_FASTEST = 0x0660,
IL_LESS_MEM,
IL_DONT_CARE,
IL_MEM_SPEED_HINT,
IL_USE_COMPRESSION,
IL_NO_COMPRESSION,
IL_COMPRESSION_HINT,

// Compression
IL_NVIDIA_COMPRESS = 0x0670,
IL_SQUISH_COMPRESS,

// Subimage types
IL_SUB_NEXT = 0x0680,
IL_SUB_MIPMAP,
IL_SUB_LAYER,

// Compression definitions
IL_COMPRESS_MODE = 0x0700,
IL_COMPRESS_NONE,
IL_COMPRESS_RLE,
IL_COMPRESS_LZO,
IL_COMPRESS_ZLIB,
};


// not only File format-specific values
enum {
IL_VERSION_NUM,
IL_IMAGE_WIDTH,
IL_IMAGE_HEIGHT,
IL_IMAGE_DEPTH,
IL_IMAGE_SIZE_OF_DATA,
IL_IMAGE_BPP,
IL_IMAGE_BYTES_PER_PIXEL,
IL_IMAGE_BITS_PER_PIXEL,
IL_IMAGE_FORMAT,
IL_IMAGE_TYPE,
IL_PALETTE_TYPE,
IL_PALETTE_SIZE,
IL_PALETTE_BPP,
IL_PALETTE_NUM_COLS,
IL_PALETTE_BASE_TYPE,
IL_NUM_FACES,
IL_NUM_IMAGES,
IL_NUM_MIPMAPS,
IL_NUM_LAYERS,
IL_ACTIVE_IMAGE,
IL_ACTIVE_MIPMAP,
IL_ACTIVE_LAYER,
IL_ACTIVE_FACE,
IL_CUR_IMAGE,
IL_IMAGE_DURATION,
IL_IMAGE_PLANESIZE,
IL_IMAGE_BPC,
IL_IMAGE_OFFX,
IL_IMAGE_OFFY,
IL_IMAGE_CUBEFLAGS,
IL_IMAGE_ORIGIN,
IL_IMAGE_CHANNELS,
IL_TGA_CREATE_STAMP,
IL_JPG_QUALITY,
IL_PNG_INTERLACE,
IL_TGA_RLE,
IL_BMP_RLE,
IL_SGI_RLE,
IL_TGA_ID_STRING,
IL_TGA_AUTHNAME_STRING,
IL_TGA_AUTHCOMMENT_STRING,
IL_PNG_AUTHNAME_STRING,
IL_PNG_TITLE_STRING,
IL_PNG_DESCRIPTION_STRING,
IL_TIFF_DESCRIPTION_STRING,
IL_TIFF_HOSTCOMPUTER_STRING,
IL_TIFF_DOCUMENTNAME_STRING,
IL_TIFF_AUTHNAME_STRING,
IL_JPG_SAVE_FORMAT,
IL_CHEAD_HEADER_STRING,
IL_PCD_PICNUM,
IL_JPG_PROGRESSIVE,
IL_VTF_COMP,
// DXTC definitions
IL_DXTC_FORMAT,
IL_DXT1,
IL_DXT2,
IL_DXT3,
IL_DXT4,
IL_DXT5,
IL_DXT_NO_COMP,
IL_KEEP_DXTC_DATA,
IL_DXTC_DATA_FORMAT,
IL_3DC,
IL_RXGB,
IL_ATI1N,
IL_DXT1A,
IL_PNG_ALPHA_INDEX, //XIX : ILint : the color in the palette at this index value (0-255) is considered transparent, -1 for no trasparent color
};

// Environment map definitions
enum{
IL_CUBEMAP_POSITIVEX = 0x00000400,
IL_CUBEMAP_NEGATIVEX = 0x00000800,
IL_CUBEMAP_POSITIVEY = 0x00001000,
IL_CUBEMAP_NEGATIVEY = 0x00002000,
IL_CUBEMAP_POSITIVEZ = 0x00004000,
IL_CUBEMAP_NEGATIVEZ = 0x00008000,
IL_SPHEREMAP = 0x00010000,
};


# if defined __GNUC__ && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ > 0))
// __attribute__((deprecated)) is supported by GCC 3.1 and later.
#  define DEPRECATED(D) D __attribute__((deprecated))
# elif defined _MSC_VER && _MSC_VER >= 1300
// __declspec(deprecated) is supported by MSVC 7.0 and later.
#  define DEPRECATED(D) __declspec(deprecated) D
# else
#  define DEPRECATED (D) D
# endif

//
// Section shamelessly modified from the glut header.
//

// This is from Win32's <windef.h>
#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__) || defined(__LCC__)
	#define ILAPIENTRY __stdcall 
	#define IL_PACKSTRUCT
//#elif defined(linux) || defined(MACOSX) || defined(__CYGWIN__) //fix bug 840364
#elif defined( __GNUC__ )
  // this should work for any of the above commented platforms 
  // plus any platform using GCC
	#ifdef __MINGW32__
		#define ILAPIENTRY __stdcall
	#else
		#define ILAPIENTRY
	#endif
	#define IL_PACKSTRUCT __attribute__ ((packed))
#else
	#define ILAPIENTRY
	#define IL_PACKSTRUCT
#endif



#define IL_SEEK_SET	0
#define IL_SEEK_CUR	1
#define IL_SEEK_END	2
#define IL_EOF		-1


// Callback functions for file reading
typedef void* ILHANDLE;
typedef void      (ILAPIENTRY *fCloseRProc)(ILHANDLE);
typedef ILboolean (ILAPIENTRY *fEofProc)   (ILHANDLE);
typedef ILint     (ILAPIENTRY *fGetcProc)  (ILHANDLE);
typedef ILHANDLE  (ILAPIENTRY *fOpenRProc) (ILconst_string);
typedef ILint     (ILAPIENTRY *fReadProc)  (void*, ILuint, ILuint, ILHANDLE);
typedef ILint     (ILAPIENTRY *fSeekRProc) (ILHANDLE, ILint, ILint);
typedef ILint     (ILAPIENTRY *fTellRProc) (ILHANDLE);

// Callback functions for file writing
typedef void     (ILAPIENTRY *fCloseWProc)(ILHANDLE);
typedef ILHANDLE (ILAPIENTRY *fOpenWProc) (ILconst_string);
typedef ILint    (ILAPIENTRY *fPutcProc)  (ILubyte, ILHANDLE);
typedef ILint    (ILAPIENTRY *fSeekWProc) (ILHANDLE, ILint, ILint);
typedef ILint    (ILAPIENTRY *fTellWProc) (ILHANDLE);
typedef ILint    (ILAPIENTRY *fWriteProc) (const void*, ILuint, ILuint, ILHANDLE);

// Callback functions for allocation and deallocation
typedef void* (ILAPIENTRY *mAlloc)(const ILsizei);
typedef void  (ILAPIENTRY *mFree) (const void* CONST_RESTRICT);

// Registered format procedures
typedef ILenum (ILAPIENTRY *IL_LOADPROC)(ILconst_string);
typedef ILenum (ILAPIENTRY *IL_SAVEPROC)(ILconst_string);


// ImageLib Functions
ILAPI ILboolean ILAPIENTRY ilActiveFace(ILuint Number);
ILAPI ILboolean ILAPIENTRY ilActiveImage(ILuint Number);
ILAPI ILboolean ILAPIENTRY ilActiveLayer(ILuint Number);
ILAPI ILboolean ILAPIENTRY ilActiveMipmap(ILuint Number);
ILAPI ILboolean ILAPIENTRY ilApplyPal(ILconst_string FileName);
ILAPI ILboolean ILAPIENTRY ilApplyProfile(ILstring InProfile, ILstring OutProfile);
ILAPI void		ILAPIENTRY ilBindImage(ILuint Image);
ILAPI ILboolean ILAPIENTRY ilBlit(ILuint Source, ILint DestX, ILint DestY, ILint DestZ, ILuint SrcX, ILuint SrcY, ILuint SrcZ, ILuint Width, ILuint Height, ILuint Depth);
ILAPI ILboolean ILAPIENTRY ilClampNTSC(void);
ILAPI void		ILAPIENTRY ilClearColour(ILclampf Red, ILclampf Green, ILclampf Blue, ILclampf Alpha);
ILAPI ILboolean ILAPIENTRY ilClearImage(void);
ILAPI ILuint    ILAPIENTRY ilCloneCurImage(void);
ILAPI ILubyte*	ILAPIENTRY ilCompressDXT(ILubyte *Data, ILuint Width, ILuint Height, ILuint Depth, ILenum DXTCFormat, ILuint *DXTCSize);
ILAPI ILboolean ILAPIENTRY ilCompressFunc(ILenum Mode);
ILAPI ILboolean ILAPIENTRY ilConvertImage(ILenum DestFormat, ILenum DestType);
ILAPI ILboolean ILAPIENTRY ilConvertPal(ILenum DestFormat);
ILAPI ILboolean ILAPIENTRY ilCopyImage(ILuint Src);
ILAPI ILuint    ILAPIENTRY ilCopyPixels(ILuint XOff, ILuint YOff, ILuint ZOff, ILuint Width, ILuint Height, ILuint Depth, ILenum Format, ILenum Type, void *Data);
ILAPI ILuint    ILAPIENTRY ilCreateSubImage(ILenum Type, ILuint Num);
ILAPI ILboolean ILAPIENTRY ilDefaultImage(void);
ILAPI void		ILAPIENTRY ilDeleteImage(const ILuint Num);
ILAPI void      ILAPIENTRY ilDeleteImages(ILsizei Num, const ILuint *Images);
ILAPI ILenum	ILAPIENTRY ilDetermineType(ILconst_string FileName);
ILAPI ILenum	ILAPIENTRY ilDetermineTypeF(ILHANDLE File);
ILAPI ILenum	ILAPIENTRY ilDetermineTypeL(const void *Lump, ILuint Size);
ILAPI ILboolean ILAPIENTRY ilDisable(ILenum Mode);
ILAPI ILboolean ILAPIENTRY ilDxtcDataToImage(void);
ILAPI ILboolean ILAPIENTRY ilDxtcDataToSurface(void);
ILAPI ILboolean ILAPIENTRY ilEnable(ILenum Mode);
ILAPI void		ILAPIENTRY ilFlipSurfaceDxtcData(void);
ILAPI ILboolean ILAPIENTRY ilFormatFunc(ILenum Mode);
ILAPI void	    ILAPIENTRY ilGenImages(ILsizei Num, ILuint *Images);
ILAPI ILuint	ILAPIENTRY ilGenImage(void);
ILAPI ILubyte*  ILAPIENTRY ilGetAlpha(ILenum Type);
ILAPI ILboolean ILAPIENTRY ilGetBoolean(ILenum Mode);
ILAPI void      ILAPIENTRY ilGetBooleanv(ILenum Mode, ILboolean *Param);
ILAPI ILubyte*  ILAPIENTRY ilGetData(void);
ILAPI ILuint    ILAPIENTRY ilGetDXTCData(void *Buffer, ILuint BufferSize, ILenum DXTCFormat);
ILAPI ILenum    ILAPIENTRY ilGetError(void);
ILAPI ILconst_string ILAPIENTRY ilErrorString(ILenum Error);
ILAPI ILint     ILAPIENTRY ilGetInteger(ILenum Mode);
ILAPI void      ILAPIENTRY ilGetIntegerv(ILenum Mode, ILint *Param);
ILAPI ILuint    ILAPIENTRY ilGetLumpPos(void);
ILAPI ILubyte*  ILAPIENTRY ilGetPalette(void);
ILAPI ILconst_string  ILAPIENTRY ilGetString(ILenum StringName);
ILAPI void      ILAPIENTRY ilHint(ILenum Target, ILenum Mode);
ILAPI ILboolean	ILAPIENTRY ilInvertSurfaceDxtcDataAlpha(void);
ILAPI void      ILAPIENTRY ilInit(void);
ILAPI ILboolean ILAPIENTRY ilImageToDxtcData(ILenum Format);
ILAPI ILboolean ILAPIENTRY ilIsDisabled(ILenum Mode);
ILAPI ILboolean ILAPIENTRY ilIsEnabled(ILenum Mode);
ILAPI ILboolean ILAPIENTRY ilIsImage(ILuint Image);
ILAPI ILboolean ILAPIENTRY ilIsValid(ILenum Type, ILconst_string FileName);
ILAPI ILboolean ILAPIENTRY ilIsValidF(ILenum Type, ILHANDLE File);
ILAPI ILboolean ILAPIENTRY ilIsValidL(ILenum Type, void *Lump, ILuint Size);
ILAPI void      ILAPIENTRY ilKeyColour(ILclampf Red, ILclampf Green, ILclampf Blue, ILclampf Alpha);
ILAPI ILboolean ILAPIENTRY ilLoad(ILenum Type, ILconst_string FileName);
ILAPI ILboolean ILAPIENTRY ilLoadF(ILenum Type, ILHANDLE File);
ILAPI ILboolean ILAPIENTRY ilLoadImage(ILconst_string FileName);
ILAPI ILboolean ILAPIENTRY ilLoadL(ILenum Type, const void *Lump, ILuint Size);
ILAPI ILboolean ILAPIENTRY ilLoad_PAL(ILconst_string FileName);
ILAPI void      ILAPIENTRY ilModAlpha(ILdouble AlphaValue);
ILAPI ILboolean ILAPIENTRY ilOriginFunc(ILenum Mode);
ILAPI ILboolean ILAPIENTRY ilOverlayImage(ILuint Source, ILint XCoord, ILint YCoord, ILint ZCoord);
ILAPI void      ILAPIENTRY ilPopAttrib(void);
ILAPI void      ILAPIENTRY ilPushAttrib(ILuint Bits);
ILAPI void      ILAPIENTRY ilRegisterFormat(ILenum Format);
ILAPI ILboolean ILAPIENTRY ilRegisterLoad(ILconst_string Ext, IL_LOADPROC Load);
ILAPI ILboolean ILAPIENTRY ilRegisterMipNum(ILuint Num);
ILAPI ILboolean ILAPIENTRY ilRegisterNumFaces(ILuint Num);
ILAPI ILboolean ILAPIENTRY ilRegisterNumImages(ILuint Num);
ILAPI void      ILAPIENTRY ilRegisterOrigin(ILenum Origin);
ILAPI void      ILAPIENTRY ilRegisterPal(void *Pal, ILuint Size, ILenum Type);
ILAPI ILboolean ILAPIENTRY ilRegisterSave(ILconst_string Ext, IL_SAVEPROC Save);
ILAPI void      ILAPIENTRY ilRegisterType(ILenum Type);
ILAPI ILboolean ILAPIENTRY ilRemoveLoad(ILconst_string Ext);
ILAPI ILboolean ILAPIENTRY ilRemoveSave(ILconst_string Ext);
ILAPI void      ILAPIENTRY ilResetMemory(void); // Deprecated
ILAPI void      ILAPIENTRY ilResetRead(void);
ILAPI void      ILAPIENTRY ilResetWrite(void);
ILAPI ILboolean ILAPIENTRY ilSave(ILenum Type, ILconst_string FileName);
ILAPI ILuint    ILAPIENTRY ilSaveF(ILenum Type, ILHANDLE File);
ILAPI ILboolean ILAPIENTRY ilSaveImage(ILconst_string FileName);
ILAPI ILuint    ILAPIENTRY ilSaveL(ILenum Type, void *Lump, ILuint Size);
ILAPI ILboolean ILAPIENTRY ilSavePal(ILconst_string FileName);
ILAPI ILboolean ILAPIENTRY ilSetAlpha(ILdouble AlphaValue);
ILAPI ILboolean ILAPIENTRY ilSetData(void *Data);
ILAPI ILboolean ILAPIENTRY ilSetDuration(ILuint Duration);
ILAPI void      ILAPIENTRY ilSetInteger(ILenum Mode, ILint Param);
ILAPI void      ILAPIENTRY ilSetMemory(mAlloc, mFree);
ILAPI void      ILAPIENTRY ilSetPixels(ILint XOff, ILint YOff, ILint ZOff, ILuint Width, ILuint Height, ILuint Depth, ILenum Format, ILenum Type, void *Data);
ILAPI void      ILAPIENTRY ilSetRead(fOpenRProc, fCloseRProc, fEofProc, fGetcProc, fReadProc, fSeekRProc, fTellRProc);
ILAPI void      ILAPIENTRY ilSetString(ILenum Mode, const char *String);
ILAPI void      ILAPIENTRY ilSetWrite(fOpenWProc, fCloseWProc, fPutcProc, fSeekWProc, fTellWProc, fWriteProc);
ILAPI void      ILAPIENTRY ilShutDown(void);
ILAPI ILboolean ILAPIENTRY ilSurfaceToDxtcData(ILenum Format);
ILAPI ILboolean ILAPIENTRY ilTexImage(ILuint Width, ILuint Height, ILuint Depth, ILubyte NumChannels, ILenum Format, ILenum Type, void *Data);
ILAPI ILboolean ILAPIENTRY ilTexImageDxtc(ILint w, ILint h, ILint d, ILenum DxtFormat, const ILubyte* data);
ILAPI ILenum    ILAPIENTRY ilTypeFromExt(ILconst_string FileName);
ILAPI ILboolean ILAPIENTRY ilTypeFunc(ILenum Mode);
ILAPI ILboolean ILAPIENTRY ilLoad_DATA(ILconst_string FileName, ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp);
ILAPI ILboolean ILAPIENTRY ilLoadF_DATA(ILHANDLE File, ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp);
ILAPI ILboolean ILAPIENTRY ilLoadL_DATA(void *Lump, ILuint Size, ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp);
ILAPI ILboolean ILAPIENTRY ilSaveData(ILconst_string FileName);

/* How much do we want to log? Goes from virtually nothing (IL_LOG_NONE) to more than everything (IL_LOG_DEBUG) */
enum ILlog_codes {IL_LOG_NONE = 0, IL_LOG_ERROR, IL_LOG_WARNING, IL_LOG_INFO, IL_LOG_VERBOSE, IL_LOG_DEBUG, IL_LOG_CODES_COUNT};

/* Logging control functions */
ILAPI void ILAPIENTRY ilLogInit(ILconst_string logfile, ILint loglevel);
ILAPI void ILAPIENTRY ilLogExit();

// For all those weirdos that spell "colour" without the 'u'.
#define ilClearColor	ilClearColour
#define ilKeyColor      ilKeyColour

#define imemclear(x,y) memset(x,0,y);

#ifdef __cplusplus
}
#endif

#endif // _IL_H__
#endif // __il_h__
