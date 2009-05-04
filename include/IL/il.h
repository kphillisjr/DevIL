//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 05/03/2009
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
#ifndef __IL_H__

#define __il_h_
#define __IL_H__

#ifdef __cplusplus
extern "C" {
#endif

//this define controls if floats and doubles are clamped to [0..1]
//during conversion. It takes a little more time, but it is the correct
//way of doing this. If you are sure your floats are always valid,
//you can undefine this value...
#define CLAMP_HALF		1
#define CLAMP_FLOATS	1
#define CLAMP_DOUBLES	1


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

enum {
	IL_FALSE	= 0,
	IL_TRUE		= 1
};

//  Matches OpenGL's right now.
//! Data formats \link Formats Formats\endlink
enum {
	IL_COLOUR_INDEX		= 0x1900,
	IL_COLOR_INDEX		= 0x1900,
	IL_ALPHA			= 0x1906,
	IL_RGB				= 0x1907,
	IL_RGBA				= 0x1908,
	IL_BGR				= 0x80E0,
	IL_BGRA				= 0x80E1,
	IL_LUMINANCE		= 0x1909,
	IL_LUMINANCE_ALPHA	= 0x190A
};

//! Data types \link Types Types\endlink
enum {
	IL_BYTE           = 0x1400,
	IL_UNSIGNED_BYTE  = 0x1401,
	IL_SHORT          = 0x1402,
	IL_UNSIGNED_SHORT = 0x1403,
	IL_INT            = 0x1404,
	IL_UNSIGNED_INT   = 0x1405,
	IL_FLOAT          = 0x1406,
	IL_DOUBLE         = 0x140A,
	IL_HALF           = 0x140B
};

enum {
	IL_MAX_BYTE		  		= SCHAR_MAX,
	IL_MAX_UNSIGNED_BYTE  	= UCHAR_MAX,
	IL_MAX_SHORT	  		= SHRT_MAX,
	IL_MAX_UNSIGNED_SHORT 	= USHRT_MAX,
	IL_MAX_INT		  		= INT_MAX,
	IL_MAX_UNSIGNED_INT   	= UINT_MAX
};

#define IL_LIMIT(x,m,M)		(x<m?m:(x>M?M:x))
#define IL_CLAMP(x) 		IL_LIMIT(x,0,1)

enum {
	IL_VENDOR   = 0x1F00,
	IL_LOAD_EXT = 0x1F01,
	IL_SAVE_EXT = 0x1F02
};


//
// IL-specific #define's
//

#define IL_VERSION_1_7_8 1
#define IL_VERSION       178


// Attribute Bits
enum {
	IL_ORIGIN_BIT			= 0x00000001,
	IL_PAL_BIT				= 0x00000002,
	IL_FORMAT_BIT			= 0x00000004,
	IL_TYPE_BIT				= 0x00000008,
	IL_COMPRESS_BIT			= 0x00000010,
	IL_LOADFAIL_BIT			= 0x00000020,
	IL_FORMAT_SPECIFIC_BIT	= 0x00000040,
	IL_ALL_ATTRIB_BITS		= 0x000FFFFF
};

// Palette types
enum {
	IL_PAL_NONE		= 0x0400,
	IL_PAL_RGB24	= 0x0401,
	IL_PAL_RGB32	= 0x0402,
	IL_PAL_RGBA32	= 0x0403,
	IL_PAL_BGR24	= 0x0404,
	IL_PAL_BGR32	= 0x0405,
	IL_PAL_BGRA32	= 0x0406
};


// Image types
enum {
	IL_TYPE_UNKNOWN = 0x0000,
	IL_BLP			= 0x044C,  //!< Blizzard Texture Format - .blp extension
	IL_BMP			= 0x0420,  //!< Microsoft Windows Bitmap - .bmp extension
	IL_CHEAD        = 0x042F,  //!< C-Style Header - .h extension
	IL_CUT			= 0x0421,  //!< Dr. Halo - .cut extension
	IL_DCX          = 0x0438,  //!< ZSoft Multi-PCX - .dcx extension
	IL_DDS          = 0x0437,  //!< DirectDraw Surface - .dds extension
	IL_DICOM		= 0x044A,  //!< Digital Imaging and Communications in Medicine (DICOM) - .dcm and .dicom extensions
	IL_DOOM			= 0x0422,  //!< DooM walls - no specific extension
	IL_DOOM_FLAT	= 0x0423,  //!< DooM flats - no specific extension
	IL_DPX			= 0x0450,  //!< Digital Picture Exchange - .dpx extension
	IL_EXIF         = 0x043A,  //!< 
	IL_EXR			= 0x0442,  //!< OpenEXR - .exr extension
	IL_FITS			= 0x0449,  //!< Flexible Image Transport System - .fit and .fits extensions
	IL_FTX			= 0x044D,  //!< Heavy Metal: FAKK2 Texture - .ftx extension
	IL_GIF          = 0x0436,  //!< Graphics Interchange Format - .gif extension
	IL_HDR          = 0x043F,  //!< Radiance High Dynamic Range - .hdr extension
	IL_ICNS			= 0x0440,  //!< Macintosh Icon - .icns extension
	IL_ICO			= 0x0424,  //!< Microsoft Windows Icons and Cursors - .ico and .cur extensions
	IL_IFF			= 0x0447,  //!< Interchange File Format - .iff extension
	IL_ILBM         = 0x0426,  //!< Amiga IFF (FORM ILBM) - .iff, .ilbm, .lbm extensions
	IL_IWI			= 0x044B,  //!< Call of Duty Infinity Ward Image - .iwi extension
	IL_JP2			= 0x0441,  //!< Jpeg 2000 - .jp2 extension
	IL_JPG			= 0x0425,  //!< JPEG - .jpg, .jpe and .jpeg extensions
	IL_JFIF			= 0x0425,  //!<
	IL_JNG          = 0x0435,  //!< 
	IL_LIF          = 0x0434,  //!< Homeworld Texture - .lif extension
	IL_MDL          = 0x0431,  //!< Half-Life Model Texture - .mdl extension
	IL_MNG          = 0x0435,  //!< Multiple-image Network Graphics - .mng extension
	IL_MP3			= 0x0452,  //!< MPEG-1 Audio Layer 3 - .mp3 extension
	IL_PCD          = 0x0427,  //!< Kodak PhotoCD - .pcd extension
	IL_PCX          = 0x0428,  //!< ZSoft PCX - .pcx extension
	IL_PIC          = 0x0429,  //!< PIC - .pic extension
	IL_PIX          = 0x043C,  //!< PIX - .pix extension
	IL_PNG          = 0x042A,  //!< Portable Network Graphics - .png extension
	IL_PNM          = 0x042B,  //!< Portable Any Map - .pbm, .pgm, .ppm and .pnm extensions
	IL_PSD          = 0x0439,  //!< Adobe PhotoShop - .psd extension
	IL_PSP          = 0x043B,  //!< PaintShop Pro - .psp extension
	IL_PXR          = 0x043D,  //!< Pixar - .pxr extension
	IL_ROT			= 0x044E,  //!< Homeworld 2 - Relic Texture - .rot extension
	IL_SGI          = 0x042C,  //!< Silicon Graphics - .sgi, .bw, .rgb and .rgba extensions
	IL_SIN			= 0x0454,  //!< Ritualistic's SiN Textures - .swl extension
	IL_SUN			= 0x0446,  //!< Sun Raster - .sun, .ras, .rs, .im1, .im8, .im24 and .im32 extensions
	IL_TGA          = 0x042D,  //!< TrueVision Targa File - .tga, .vda, .icb and .vst extensions
	IL_TEX			= 0x0455,  //!< Serious Sam Texture - .tex extension
	IL_TEXTURE		= 0x044F,  //!< Medieval II: Total War Texture - .texture extension
	IL_TIF          = 0x042E,  //!< Tagged Image File Format - .tif and .tiff extensions
	IL_TPL			= 0x0448,  //!< Gamecube Texture - .tpl extension
	IL_UTX			= 0x0451,  //!< Unreal (and Unreal Tournament) Texture - .utx extension
	IL_VTF			= 0x0444,  //!< Valve Texture Format - .vtf extension
	IL_WAD          = 0x0453,  //!< Half-Life Textures - .wad extension
	IL_WAL          = 0x0432,  //!< Quake 2 Texture - .wal extension
	IL_WBMP			= 0x0445,  //!< Wireless Bitmap - .wbmp extension
	IL_WDP			= 0x0443,  //!< Microsoft HD Photo - .wdp and .hdp extension
	IL_XPM          = 0x043E,  //!< X Pixel Map - .xpm extension

	IL_JASC_PAL     = 0x0475   //!< PaintShop Pro Palette
};


// Error Types
enum {
	IL_NO_ERROR             = 0x0000,
	IL_INVALID_ENUM         = 0x0501,
	IL_OUT_OF_MEMORY        = 0x0502,
	IL_FORMAT_NOT_SUPPORTED = 0x0503,
	IL_INTERNAL_ERROR       = 0x0504,
	IL_INVALID_VALUE        = 0x0505,
	IL_ILLEGAL_OPERATION    = 0x0506,
	IL_ILLEGAL_FILE_VALUE   = 0x0507,
	IL_INVALID_FILE_HEADER  = 0x0508,
	IL_INVALID_PARAM        = 0x0509,
	IL_COULD_NOT_OPEN_FILE  = 0x050A,
	IL_INVALID_EXTENSION    = 0x050B,
	IL_OUT_FORMAT_SAME      = 0x050D,
	IL_STACK_OVERFLOW       = 0x050E,
	IL_STACK_UNDERFLOW      = 0x050F,
	IL_INVALID_CONVERSION   = 0x0510,
	IL_BAD_DIMENSIONS       = 0x0511,
	IL_FILE_READ_ERROR      = 0x0512,  // 05/12/2002: Addition by Sam.
	IL_FILE_WRITE_ERROR     = 0x0512,

	IL_LIB_GIF_ERROR		= 0x05E1,
	IL_LIB_JPEG_ERROR		= 0x05E2,
	IL_LIB_PNG_ERROR		= 0x05E3,
	IL_LIB_TIFF_ERROR		= 0x05E4,
	IL_LIB_MNG_ERROR		= 0x05E5,
	IL_LIB_JP2_ERROR		= 0x05E6,
	IL_LIB_EXR_ERROR		= 0x05E7,
	IL_UNKNOWN_ERROR		= 0x05FF
};


// Origin Definitions
enum {
	IL_ORIGIN_SET			= 0x0600,
	IL_ORIGIN_LOWER_LEFT	= 0x0601,
	IL_ORIGIN_UPPER_LEFT	= 0x0602,
	IL_ORIGIN_MODE			= 0x0603
};


// Format and Type Mode Definitions
enum {
	IL_FORMAT_SET	= 0x0610,
	IL_FORMAT_MODE	= 0x0611,
	IL_TYPE_SET		= 0x0612,
	IL_TYPE_MODE	= 0x0613
};


// Palette definitions
enum {
	IL_CONV_PAL = 0x0630
};


// Load fail definitions
enum {
	IL_DEFAULT_ON_FAIL = 0x0632
};


// Key colour and alpha definitions
enum {
	IL_USE_KEY_COLOUR	= 0x0635,
	IL_USE_KEY_COLOR	= 0x0635,
	IL_BLIT_BLEND		= 0x0636
};


// Interlace definitions
enum {
	IL_SAVE_INTERLACED	= 0x0639,
	IL_INTERLACE_MODE	= 0x063A
};


// Quantization definitions
enum {
	IL_QUANTIZATION_MODE	= 0x0640,
	IL_WU_QUANT				= 0x0641,
	IL_NEU_QUANT			= 0x0642,
	IL_NEU_QUANT_SAMPLE		= 0x0643,
	IL_MAX_QUANT_INDICES	= 0x0644 //XIX : ILint : Maximum number of colors to reduce to, default of 256. and has a range of 2-256
};


// Hints
enum {
	IL_FASTEST			= 0x0660,
	IL_LESS_MEM			= 0x0661,
	IL_DONT_CARE		= 0x0662,
	IL_MEM_SPEED_HINT	= 0x0665,
	IL_USE_COMPRESSION	= 0x0666,
	IL_NO_COMPRESSION	= 0x0667,
	IL_COMPRESSION_HINT	= 0x0668
};


// Compression
enum {
	IL_NVIDIA_COMPRESS	= 0x0670,
	IL_SQUISH_COMPRESS	= 0x0671
};


// Subimage types
enum {
	IL_SUB_NEXT		= 0x0680,
	IL_SUB_MIPMAP	= 0x0681,
	IL_SUB_LAYER	= 0x0682
};


// Compression definitions
enum {
	IL_COMPRESS_MODE	= 0x0700,
	IL_COMPRESS_NONE	= 0x0701,
	IL_COMPRESS_RLE		= 0x0702,
	IL_COMPRESS_LZO		= 0x0703,
	IL_COMPRESS_ZLIB	= 0x0704
};


// File format-specific values
enum {
	IL_TGA_CREATE_STAMP        = 0x0710,
	IL_JPG_QUALITY             = 0x0711,
	IL_PNG_INTERLACE           = 0x0712,
	IL_TGA_RLE                 = 0x0713,
	IL_BMP_RLE                 = 0x0714,
	IL_SGI_RLE                 = 0x0715,
	IL_TGA_ID_STRING           = 0x0717,
	IL_TGA_AUTHNAME_STRING     = 0x0718,
	IL_TGA_AUTHCOMMENT_STRING  = 0x0719,
	IL_PNG_AUTHNAME_STRING     = 0x071A,
	IL_PNG_TITLE_STRING        = 0x071B,
	IL_PNG_DESCRIPTION_STRING  = 0x071C,
	IL_TIF_DESCRIPTION_STRING  = 0x071D,
	IL_TIF_HOSTCOMPUTER_STRING = 0x071E,
	IL_TIF_DOCUMENTNAME_STRING = 0x071F,
	IL_TIF_AUTHNAME_STRING     = 0x0720,
	IL_JPG_SAVE_FORMAT         = 0x0721,
	IL_CHEAD_HEADER_STRING     = 0x0722,
	IL_PCD_PICNUM              = 0x0723,
	IL_PNG_ALPHA_INDEX         = 0x0724, // The color in the palette at this index value (0-255) is considered transparent, -1 for no trasparent color
	IL_JPG_PROGRESSIVE         = 0x0725,
	IL_VTF_COMP                = 0x0726
};


// DXTC definitions
enum {
	IL_DXTC_FORMAT      = 0x0705,
	IL_DXT1             = 0x0706,
	IL_DXT2             = 0x0707,
	IL_DXT3             = 0x0708,
	IL_DXT4             = 0x0709,
	IL_DXT5             = 0x070A,
	IL_DXT_NO_COMP      = 0x070B,
	IL_KEEP_DXTC_DATA   = 0x070C,
	IL_DXTC_DATA_FORMAT = 0x070D,
	IL_3DC              = 0x070E,
	IL_RXGB             = 0x070F,
	IL_ATI1N            = 0x0710,
	IL_DXT1A            = 0x0711  // Normally the same as IL_DXT1, except for nVidia Texture Tools.
};


// Environment map definitions
enum {
	IL_CUBEMAP_POSITIVEX = 0x00000400,
	IL_CUBEMAP_NEGATIVEX = 0x00000800,
	IL_CUBEMAP_POSITIVEY = 0x00001000,
	IL_CUBEMAP_NEGATIVEY = 0x00002000,
	IL_CUBEMAP_POSITIVEZ = 0x00004000,
	IL_CUBEMAP_NEGATIVEZ = 0x00008000,
	IL_SPHEREMAP         = 0x00010000
};


// Values
enum {
	IL_VERSION_NUM           = 0x0DE2,
	IL_IMAGE_WIDTH           = 0x0DE4,
	IL_IMAGE_HEIGHT          = 0x0DE5,
	IL_IMAGE_DEPTH           = 0x0DE6,
	IL_IMAGE_SIZE_OF_DATA    = 0x0DE7,
	IL_IMAGE_BPP             = 0x0DE8,
	IL_IMAGE_BYTES_PER_PIXEL = 0x0DE8,
	IL_IMAGE_BITS_PER_PIXEL  = 0x0DE9,
	IL_IMAGE_FORMAT          = 0x0DEA,
	IL_IMAGE_TYPE            = 0x0DEB,
	IL_PALETTE_TYPE          = 0x0DEC,
	IL_PALETTE_SIZE          = 0x0DED,
	IL_PALETTE_BPP           = 0x0DEE,
	IL_PALETTE_NUM_COLS      = 0x0DEF,
	IL_PALETTE_BASE_TYPE     = 0x0DF0,
	IL_NUM_FACES             = 0x0DE1,
	IL_NUM_IMAGES            = 0x0DF1,
	IL_NUM_MIPMAPS           = 0x0DF2,
	IL_NUM_LAYERS            = 0x0DF3,
	IL_ACTIVE_IMAGE          = 0x0DF4,
	IL_ACTIVE_MIPMAP         = 0x0DF5,
	IL_ACTIVE_LAYER          = 0x0DF6,
	IL_ACTIVE_FACE           = 0x0E00,
	IL_CUR_IMAGE             = 0x0DF7,
	IL_IMAGE_DURATION        = 0x0DF8,
	IL_IMAGE_PLANESIZE       = 0x0DF9,
	IL_IMAGE_BPC             = 0x0DFA,
	IL_IMAGE_OFFX            = 0x0DFB,
	IL_IMAGE_OFFY            = 0x0DFC,
	IL_IMAGE_CUBEFLAGS       = 0x0DFD,
	IL_IMAGE_ORIGIN          = 0x0DFE,
	IL_IMAGE_CHANNELS        = 0x0DFF
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

// This is from Win32's <wingdi.h> and <winnt.h>
#if defined(__LCC__)
	#define ILAPI __stdcall
#elif defined(_WIN32) //changed 20031221 to fix bug 840421
	#ifdef IL_STATIC_LIB
		#define ILAPI
	#else
		#ifdef IL_BUILD_LIBRARY
			#define ILAPI __declspec(dllexport)
		#else
			#define ILAPI __declspec(dllimport)
		#endif
	#endif
#elif __APPLE__
	#define ILAPI extern
#else
	#define ILAPI
#endif

// We typecast ILimage and ILstate to void if we are not building the library.  This way the internals of the
//  structure are not revealed to the user, since they are subject to change in future versions.
#if !defined(IL_BUILD_LIBRARY)
	typedef void ILimage;
	typedef void ILstate;
#else
	#include <IL/devil_internal_exports.h>
#endif


enum {
	IL_SEEK_SET	=  0,
	IL_SEEK_CUR	=  1,
	IL_SEEK_END	=  2,
	IL_EOF		= -1
};


// Callback functions for file reading
typedef void*     ILHANDLE;
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
ILAPI ILboolean ILAPIENTRY ilApplyPal(ILimage *Image, ILconst_string FileName);
ILAPI ILboolean ILAPIENTRY ilApplyProfile(ILimage *Image, ILstring InProfile, ILstring OutProfile);
ILAPI void		ILAPIENTRY ilBindImage(ILuint Image);
ILAPI ILboolean ILAPIENTRY ilBlit(ILimage *Source, ILimage *Dest, ILuint SrcX, ILuint SrcY, ILuint SrcZ, ILint DestX, ILint DestY, ILint DestZ, ILuint Width, ILuint Height, ILuint Depth, ILstate *State);
ILAPI ILboolean ILAPIENTRY ilClampNTSC(ILimage *Image);
ILAPI void		ILAPIENTRY ilClearColour(ILclampf Red, ILclampf Green, ILclampf Blue, ILclampf Alpha);
ILAPI ILboolean ILAPIENTRY ilClearImage(ILimage *Image);
ILAPI ILimage*	ILAPIENTRY ilCloneImage(ILimage *Image);
ILAPI void      ILAPIENTRY ilCloseImage(ILimage *Image);
ILAPI ILubyte*	ILAPIENTRY ilCompressDXT(ILimage *Image, ILubyte *Data, ILuint Width, ILuint Height, ILuint Depth, ILenum DXTCFormat, ILuint *DXTCSize);
ILAPI ILboolean ILAPIENTRY ilCompressFunc(ILenum Mode);
ILAPI ILboolean ILAPIENTRY ilConvertImage(ILimage *Image, ILenum DestFormat, ILenum DestType, ILstate *State);
ILAPI ILboolean ILAPIENTRY ilConvertPal(ILimage *Image, ILenum DestFormat);
ILAPI ILboolean ILAPIENTRY ilCopyImage(ILimage *Src, ILimage *Dest, ILstate *State);
ILAPI ILuint    ILAPIENTRY ilCopyPixels(ILimage *Image, ILuint XOff, ILuint YOff, ILuint ZOff, ILuint Width, ILuint Height, ILuint Depth, ILenum Format, ILenum Type, void *Data);
ILAPI ILuint    ILAPIENTRY ilCreateSubImage(ILenum Type, ILuint Num);
ILAPI ILboolean ILAPIENTRY ilDefaultImage(void);
ILAPI void		ILAPIENTRY ilDeleteImage(ILimage *Image);
ILAPI void      ILAPIENTRY ilDeleteImages(ILsizei Num, ILimage **Images);
ILAPI ILenum	ILAPIENTRY ilDetermineType(ILconst_string FileName);
ILAPI ILenum	ILAPIENTRY ilDetermineTypeF(ILHANDLE File);
ILAPI ILenum	ILAPIENTRY ilDetermineTypeL(const void *Lump, ILuint Size);
ILAPI ILboolean ILAPIENTRY ilDisable(ILenum Mode);
ILAPI ILboolean ILAPIENTRY ilDxtcDataToImage(void);
ILAPI ILboolean ILAPIENTRY ilDxtcDataToSurface(void);
ILAPI ILboolean ILAPIENTRY ilEnable(ILenum Mode);
ILAPI void		ILAPIENTRY ilFlipSurfaceDxtcData(void);
ILAPI ILboolean ILAPIENTRY ilFormatFunc(ILenum Mode, ILstate *State);
ILAPI void	    ILAPIENTRY ilGenImages(ILsizei Num, ILimage **Images);
ILAPI ILimage*	ILAPIENTRY ilGenImage(ILstate *State);
ILAPI ILubyte*  ILAPIENTRY ilGetAlpha(ILimage *Image, ILenum Type, ILstate *State);
ILAPI ILboolean ILAPIENTRY ilGetBoolean(ILenum Mode, ILstate *State);
ILAPI void      ILAPIENTRY ilGetBooleanv(ILenum Mode, ILboolean *Param, ILstate *State);
ILAPI ILubyte*  ILAPIENTRY ilGetData(ILimage *Image);
ILAPI ILuint    ILAPIENTRY ilGetDXTCData(ILimage *Image, void *Buffer, ILuint BufferSize, ILenum DXTCFormat, ILstate *State);
ILAPI ILenum    ILAPIENTRY ilGetError(void);
ILAPI ILimage*  ILAPIENTRY ilGetFace(ILimage *Image, ILuint Number);
ILAPI ILimage*  ILAPIENTRY ilGetImage(ILimage *Image, ILuint Number);
ILAPI ILint     ILAPIENTRY ilGetInteger(ILenum Mode, ILstate *State);
ILAPI void      ILAPIENTRY ilGetIntegerv(ILenum Mode, ILint *Param, ILstate *State);
ILAPI ILimage*  ILAPIENTRY ilGetLayer(ILimage *Image, ILuint Number);
ILAPI ILuint    ILAPIENTRY ilGetLumpPos(void);
ILAPI ILimage*  ILAPIENTRY ilGetMipmap(ILimage *Image, ILuint Number);
ILAPI ILubyte*  ILAPIENTRY ilGetPalette(ILimage *Image);
ILAPI ILconst_string  ILAPIENTRY ilGetString(ILenum StringName);
ILAPI ILuint	ILAPIENTRY ilGetDepth(ILimage *Image);
ILAPI ILenum	ILAPIENTRY ilGetFormat(ILimage *Image);
ILAPI ILuint	ILAPIENTRY ilGetHeight(ILimage *Image);
ILAPI ILuint	ILAPIENTRY ilGetImageSize(ILimage *Image);
ILAPI ILenum	ILAPIENTRY ilGetType(ILimage *Image);
ILAPI ILuint	ILAPIENTRY ilGetWidth(ILimage *Image);
ILAPI void      ILAPIENTRY ilHint(ILenum Target, ILenum Mode);
ILAPI ILint     ILAPIENTRY ilImageInfo(ILimage *Image, ILenum Mode);
ILAPI ILboolean	ILAPIENTRY ilInvertSurfaceDxtcDataAlpha(void);
ILAPI void      ILAPIENTRY ilInit(void);
ILAPI ILboolean ILAPIENTRY ilImageToDxtcData(ILimage *Image, ILenum Format);
ILAPI ILboolean ILAPIENTRY ilIsDisabled(ILenum Mode, ILstate *State);
ILAPI ILboolean ILAPIENTRY ilIsEnabled(ILenum Mode, ILstate *State);
ILAPI ILboolean ILAPIENTRY ilIsImage(ILuint Image);
ILAPI ILboolean ILAPIENTRY ilIsValid(ILenum Type, ILconst_string FileName);
ILAPI ILboolean ILAPIENTRY ilIsValidF(ILenum Type, ILHANDLE File);
ILAPI ILboolean ILAPIENTRY ilIsValidL(ILenum Type, void *Lump, ILuint Size);
ILAPI void      ILAPIENTRY ilKeyColour(ILclampf Red, ILclampf Green, ILclampf Blue, ILclampf Alpha);
ILAPI ILboolean ILAPIENTRY ilLoad(ILimage *Image, ILenum Type, ILconst_string FileName, ILstate *State);
ILAPI ILboolean ILAPIENTRY ilLoadF(ILimage *Image, ILenum Type, ILHANDLE File, ILstate *State);
ILAPI ILboolean ILAPIENTRY ilLoadImage(ILimage *Image, ILconst_string FileName, ILstate *State);
ILAPI ILboolean ILAPIENTRY ilLoadL(ILimage *Image, ILenum Type, const void *Lump, ILuint Size, ILstate *State);
ILAPI ILboolean ILAPIENTRY ilLoadPal(ILconst_string FileName, ILstate *State);
ILAPI void      ILAPIENTRY ilModAlpha(ILdouble AlphaValue, ILstate *State);
ILAPI ILboolean ILAPIENTRY ilOriginFunc(ILenum Mode, ILstate *State);
ILAPI ILboolean ILAPIENTRY ilOverlayImage(ILimage *Source, ILimage *Dest, ILint XCoord, ILint YCoord, ILint ZCoord, ILstate *State);
ILAPI void      ILAPIENTRY ilRegisterFormat(ILenum Format);
ILAPI ILboolean ILAPIENTRY ilRegisterLoad(ILconst_string Ext, IL_LOADPROC Load);
ILAPI ILboolean ILAPIENTRY ilRegisterMipNum(ILuint Num);
ILAPI ILboolean ILAPIENTRY ilRegisterNumFaces(ILuint Num);
ILAPI ILboolean ILAPIENTRY ilRegisterNumImages(ILuint Num);
ILAPI void      ILAPIENTRY ilRegisterOrigin(ILimage *Image, ILenum Origin);
ILAPI void      ILAPIENTRY ilRegisterPal(ILimage *Image, void *Pal, ILuint Size, ILenum Type);
ILAPI ILboolean ILAPIENTRY ilRegisterSave(ILconst_string Ext, IL_SAVEPROC Save);
ILAPI void      ILAPIENTRY ilRegisterType(ILenum Type);
ILAPI ILboolean ILAPIENTRY ilRemoveLoad(ILconst_string Ext);
ILAPI ILboolean ILAPIENTRY ilRemoveSave(ILconst_string Ext);
ILAPI void      ILAPIENTRY ilResetMemory(void); // Deprecated
ILAPI void      ILAPIENTRY ilResetRead(void);
ILAPI void      ILAPIENTRY ilResetWrite(void);
ILAPI ILboolean ILAPIENTRY ilSave(ILimage *Image, ILenum Type, ILconst_string FileName, ILstate *State);
ILAPI ILuint    ILAPIENTRY ilSaveF(ILimage *Image, ILenum Type, ILHANDLE File, ILstate *State);
ILAPI ILboolean ILAPIENTRY ilSaveImage(ILimage *Image, ILconst_string FileName, ILstate *State);
ILAPI ILuint    ILAPIENTRY ilSaveL(ILimage *Image, ILenum Type, void *Lump, ILuint Size);
ILAPI ILboolean ILAPIENTRY ilSavePal(ILimage *Image, ILconst_string FileName, ILstate *State);
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
ILAPI ILboolean ILAPIENTRY ilSurfaceToDxtcData(ILimage *Image, ILenum Format);
ILAPI ILboolean ILAPIENTRY ilTexImage(ILimage *Image, ILuint Width, ILuint Height, ILuint Depth, ILenum Format, ILenum Type, void *Data, ILstate *State);
ILAPI ILboolean ILAPIENTRY ilTexImageDxtc(ILimage *Image, ILint w, ILint h, ILint d, ILenum DxtFormat, const ILubyte* data);
ILAPI ILenum    ILAPIENTRY ilTypeFromExt(ILconst_string FileName);
ILAPI ILboolean ILAPIENTRY ilTypeFunc(ILenum Mode, ILstate *State);
ILAPI ILimage*  ILAPIENTRY ilLoadData(ILconst_string FileName, ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp, ILstate *State);
ILAPI ILimage*  ILAPIENTRY ilLoadDataF(ILHANDLE File, ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp, ILstate *State);
ILAPI ILimage*  ILAPIENTRY ilLoadDataL(void *Lump, ILuint Size, ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp, ILstate *State);
ILAPI ILboolean ILAPIENTRY ilSaveData(ILconst_string FileName);

#define imemclear(x,y) memset(x,0,y);

#ifdef __cplusplus
}
#endif

#endif // __IL_H__
#endif // __il_h__
