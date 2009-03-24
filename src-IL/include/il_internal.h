//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 03/23/2009
//
// Filename: src-IL/include/il_internal.h
//
// Description: Internal stuff for DevIL
//
//-----------------------------------------------------------------------------

#ifndef INTERNAL_H
#define INTERNAL_H
#define IL_BUILD_LIBRARY


// Local headers
/*#if (defined(_WIN32) || defined(_WIN64)) && !defined(HAVE_CONFIG_H)
	#define HAVE_CONFIG_H
#endif*/
#ifdef HAVE_CONFIG_H //if we use autotools, we have HAVE_CONFIG_H defined and we have to look for it like that
	#include <config.h>
#else // If we do not use autotools, we have to point to (possibly different) config.h than in the opposite case
	#include <IL/config.h>
#endif

// Standard headers
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/*#ifdef __cplusplus
extern "C" {
#endif*/

#include <IL/il.h>
#include <IL/devil_internal_exports.h>
#include "il_files.h"
#include "il_endian.h"

#ifndef _WIN32
	// The Microsoft HD Photo Device Porting Kit has not been ported to anything other
	//  than Windows yet, so we disable this if Windows is not the current platform.
	#define IL_NO_WDP
#endif//_WIN32

// If we do not want support for game image formats, this define removes them all.
#ifdef IL_NO_GAMES
	#define IL_NO_BLP
	#define IL_NO_DOOM
	#define IL_NO_FTX
	#define IL_NO_IWI
	#define IL_NO_LIF
	#define IL_NO_MDL
	#define IL_NO_ROT
	#define IL_NO_TPL
	#define IL_NO_UTX
	#define IL_NO_WAL
#endif//IL_NO_GAMES

// If we want to compile without support for formats supported by external libraries,
//  this define will remove them all.
#ifdef IL_NO_EXTLIBS
	#define IL_NO_EXR
	#define IL_NO_JP2
	#define IL_NO_JPG
	#define IL_NO_LCMS
	#define IL_NO_MNG
	#define IL_NO_PNG
	#define IL_NO_TIF
	#define IL_NO_WDP
	#undef IL_USE_DXTC_NVIDIA
	#undef IL_USE_DXTC_SQUISH
#endif//IL_NO_EXTLIBS

// Windows-specific
#ifdef _WIN32
	#ifdef _MSC_VER
		#if _MSC_VER > 1000
			#pragma once
			#pragma intrinsic(memcpy)
			#pragma intrinsic(memset)
			#pragma intrinsic(strcmp)
			#pragma intrinsic(strlen)
			#pragma intrinsic(strcpy)
			
			#if _MSC_VER >= 1300
				#pragma warning(disable : 4996)  // MSVC++ 8/9 deprecation warnings
			#endif
		#endif // _MSC_VER > 1000
	#endif
	#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
	#include <windows.h>
#endif//_WIN32

#ifdef _UNICODE
	#define IL_TEXT(s) L##s
	#ifndef _WIN32  // At least in Linux, fopen works fine, and wcsicmp is not defined.
		#define wcsicmp wcsncasecmp
		#define _wcsicmp wcsncasecmp
		#define _wfopen fopen
	#endif
	#define iStrCpy wcscpy
#else
	#define IL_TEXT(s) (s)
	#define iStrCpy strcpy
#endif

#ifdef IL_INLINE_ASM
	#if (defined (_MSC_VER) && defined(_WIN32))  // MSVC++ only
		#define USE_WIN32_ASM
	#endif

	#ifdef _WIN64
		#undef USE_WIN32_ASM
	//@TODO: Windows 64 compiler cannot use inline ASM, so we need to
	//  generate some MASM code at some point.
	#endif

	#ifdef _WIN32_WCE  // Cannot use our inline ASM in Windows Mobile.
		#undef USE_WIN32_ASM
	#endif
#endif
//extern ILimage *iCurImage;
#define BIT_0	0x00000001
#define BIT_1	0x00000002
#define BIT_2	0x00000004
#define BIT_3	0x00000008
#define BIT_4	0x00000010
#define BIT_5	0x00000020
#define BIT_6	0x00000040
#define BIT_7	0x00000080
#define BIT_8	0x00000100
#define BIT_9	0x00000200
#define BIT_10	0x00000400
#define BIT_11	0x00000800
#define BIT_12	0x00001000
#define BIT_13	0x00002000
#define BIT_14	0x00004000
#define BIT_15	0x00008000
#define BIT_16	0x00010000
#define BIT_17	0x00020000
#define BIT_18	0x00040000
#define BIT_19	0x00080000
#define BIT_20	0x00100000
#define BIT_21	0x00200000
#define BIT_22	0x00400000
#define BIT_23	0x00800000
#define BIT_24	0x01000000
#define BIT_25	0x02000000
#define BIT_26	0x04000000
#define BIT_27	0x08000000
#define BIT_28	0x10000000
#define BIT_29	0x20000000
#define BIT_30	0x40000000
#define BIT_31	0x80000000
#define NUL '\0'  // Easier to type and ?portable?
#if !_WIN32 || _WIN32_WCE
	int stricmp(const char *src1, const char *src2);
	int strnicmp(const char *src1, const char *src2, size_t max);
#endif//_WIN32
#ifdef _WIN32_WCE
	char *strdup(const char *src);
#endif
int iStrCmp(ILconst_string src1, ILconst_string src2);

//
// Some math functions
//
// A fast integer squareroot, completely accurate for x < 289.
// Taken from http://atoms.org.uk/sqrt/
// There is also a version that is accurate for all integers
// < 2^31, if we should need it
int iSqrt(int x);
//
// Useful miscellaneous functions
//
ILboolean	iCheckExtension(ILconst_string Arg, ILconst_string Ext);
ILbyte*		iFgets(char *buffer, ILuint maxlen);
ILboolean	iFileExists(ILconst_string FileName);
ILstring	iGetExtension(ILconst_string FileName);
ILstring	ilStrDup(ILconst_string Str);
ILuint		ilStrLen(ILconst_string Str);
ILuint		ilCharStrLen(const char *Str);
// Miscellaneous functions
void					ilDefaultStates(void);
ILenum					iGetHint(ILenum Target);
ILint					iGetInt(ILenum Mode);
void					ilRemoveRegistered(void);
ILAPI void ILAPIENTRY	ilSetCurImage(ILimage *Image);
ILuint					ilDetermineSize(ILimage *Image, ILenum Type);
//
// Rle compression
//
#define		IL_TGACOMP 0x01
#define		IL_PCXCOMP 0x02
#define		IL_SGICOMP 0x03
#define     IL_BMPCOMP 0x04
ILboolean	ilRleCompressLine(ILubyte *ScanLine, ILuint Width, ILubyte Bpp, ILubyte *Dest, ILuint *DestWidth, ILenum CompressMode);
ILuint		ilRleCompress(ILubyte *Data, ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp, ILubyte *Dest, ILenum CompressMode, ILuint *ScanTable);
void		iSetImage0(void);
// DXTC compression
ILuint			ilNVidiaCompressDXTFile(ILubyte *Data, ILuint Width, ILuint Height, ILuint Depth, ILenum DxtType);
ILAPI ILubyte*	ILAPIENTRY ilNVidiaCompressDXT(ILubyte *Data, ILuint Width, ILuint Height, ILuint Depth, ILenum DxtFormat, ILuint *DxtSize);
ILAPI ILubyte*	ILAPIENTRY ilSquishCompressDXT(ILubyte *Data, ILuint Width, ILuint Height, ILuint Depth, ILenum DxtFormat, ILuint *DxtSize);

// Conversion functions
ILboolean	ilAddAlpha(ILimage *Image);
ILboolean	ilAddAlphaKey(ILimage *Image);
ILboolean	iFastConvert(ILimage *Image, ILenum DestFormat);
ILboolean	ilFixCur(ILimage *Image);
ILboolean	ilFixImage(ILimage *Image);
ILboolean	ilRemoveAlpha(ILimage *Image);
ILboolean	ilSwapColours(ILimage *Image);
// Palette functions
ILboolean	iCopyPalette(ILpal *Dest, ILpal *Src);
// Miscellaneous functions
char*		iGetString(ILenum StringName);  // Internal version of ilGetString

//
// Image loading/saving functions
//
ILboolean ilIsValidBlp(ILconst_string FileName);
ILboolean ilIsValidBlpF(ILHANDLE File);
ILboolean ilIsValidBlpL(const void *Lump, ILuint Size);
ILboolean ilLoadBlp(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadBlpF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadBlpL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidBmp(ILconst_string CONST_RESTRICT FileName);
ILboolean ilIsValidBmpF(ILHANDLE File);
ILboolean ilIsValidBmpL(const void *Lump, ILuint Size);
ILboolean ilLoadBmp(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadBmpF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadBmpL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSaveBmp(ILimage *Image, ILconst_string FileName);
ILuint    ilSaveBmpF(ILimage *Image, ILHANDLE File);
ILuint    ilSaveBmpL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilSaveCHeader(ILimage *Image, ILconst_string FileName, char *InternalName);
ILboolean ilLoadCut(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadCutF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadCutL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidDcx(ILconst_string FileName);
ILboolean ilIsValidDcxF(ILHANDLE File);
ILboolean ilIsValidDcxL(const void *Lump, ILuint Size);
ILboolean ilLoadDcx(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadDcxF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadDcxL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidDds(ILconst_string FileName);
ILboolean ilIsValidDdsF(ILHANDLE File);
ILboolean ilIsValidDdsL(const void *Lump, ILuint Size);
ILboolean ilLoadDds(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadDdsF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadDdsL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSaveDds(ILimage *Image, ILconst_string FileName);
ILuint    ilSaveDdsF(ILimage *Image, ILHANDLE File);
ILuint    ilSaveDdsL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilIsValidDicom(ILconst_string FileName);
ILboolean ilIsValidDicomF(ILHANDLE File);
ILboolean ilIsValidDicomL(const void *Lump, ILuint Size);
ILboolean ilLoadDicom(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadDicomF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadDicomL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilLoadDoom(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadDoomF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadDoomL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilLoadDoomFlat(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadDoomFlatF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadDoomFlatL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidDpx(ILconst_string FileName);
ILboolean ilIsValidDpxF(ILHANDLE File);
ILboolean ilIsValidDpxL(const void *Lump, ILuint Size);
ILboolean ilLoadDpx(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadDpxF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadDpxL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidExr(ILconst_string FileName);
ILboolean ilIsValidExrF(ILHANDLE File);
ILboolean ilIsValidExrL(const void *Lump, ILuint Size);
ILboolean ilLoadExr(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadExrF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadExrL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSaveExr(ILimage *Image, ILconst_string FileName);
ILuint    ilSaveExrF(ILimage *Image, ILHANDLE File);
ILuint    ilSaveExrL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilIsValidFits(ILconst_string FileName);
ILboolean ilIsValidFitsF(ILHANDLE File);
ILboolean ilIsValidFitsL(const void *Lump, ILuint Size);
ILboolean ilLoadFits(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadFitsF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadFitsL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilLoadFtx(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadFtxF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadFtxL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidGif(ILconst_string FileName);
ILboolean ilIsValidGifF(ILHANDLE File);
ILboolean ilIsValidGifL(const void *Lump, ILuint Size);
ILboolean ilLoadGif(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadGifF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadGifL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidHdr(ILconst_string FileName);
ILboolean ilIsValidHdrF(ILHANDLE File);
ILboolean ilIsValidHdrL(const void *Lump, ILuint Size);
ILboolean ilLoadHdr(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadHdrF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadHdrL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSaveHdr(ILimage *Image, ILconst_string FileName);
ILuint    ilSaveHdrF(ILimage *Image, ILHANDLE File);
ILuint    ilSaveHdrL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilLoadIcon(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadIconF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadIconL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidIcns(ILconst_string FileName);
ILboolean ilIsValidIcnsF(ILHANDLE File);
ILboolean ilIsValidIcnsL(const void *Lump, ILuint Size);
ILboolean ilLoadIcns(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadIcnsF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadIcnsL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilLoadIff(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadIffF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadIffL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidIlbm(ILconst_string FileName);
ILboolean ilIsValidIlbmF(ILHANDLE File);
ILboolean ilIsValidIlbmL(const void *Lump, ILuint Size);
ILboolean ilLoadIlbm(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadIlbmF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadIlbmL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidIwi(ILconst_string FileName);
ILboolean ilIsValidIwiF(ILHANDLE File);
ILboolean ilIsValidIwiL(const void *Lump, ILuint Size);
ILboolean ilLoadIwi(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadIwiF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadIwiL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidJp2(ILconst_string FileName);
ILboolean ilIsValidJp2F(ILHANDLE File);
ILboolean ilIsValidJp2L(const void *Lump, ILuint Size);
ILboolean ilLoadJp2(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadJp2F(ILimage *Image, ILHANDLE File);
ILboolean ilLoadJp2L(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilLoadJp2LInternal(const void *Lump, ILuint Size, ILimage *Image);
ILboolean ilSaveJp2(ILimage *Image, ILconst_string FileName);
ILuint    ilSaveJp2F(ILimage *Image, ILHANDLE File);
ILuint    ilSaveJp2L(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilIsValidJpeg(ILconst_string FileName);
ILboolean ilIsValidJpegF(ILHANDLE File);
ILboolean ilIsValidJpegL(const void *Lump, ILuint Size);
ILboolean ilLoadJpeg(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadJpegF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadJpegL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSaveJpeg(ILimage *Image, ILconst_string FileName);
ILuint    ilSaveJpegF(ILimage *Image, ILHANDLE File);
ILuint    ilSaveJpegL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilIsValidLif(ILconst_string FileName);
ILboolean ilIsValidLifF(ILHANDLE File);
ILboolean ilIsValidLifL(const void *Lump, ILuint Size);
ILboolean ilLoadLif(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadLifF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadLifL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidMdl(ILconst_string FileName);
ILboolean ilIsValidMdlF(ILHANDLE File);
ILboolean ilIsValidMdlL(const void *Lump, ILuint Size);
ILboolean ilLoadMdl(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadMdlF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadMdlL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilLoadMng(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadMngF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadMngL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSaveMng(ILimage *Image, ILconst_string FileName);
ILuint    ilSaveMngF(ILimage *Image, ILHANDLE File);
ILuint    ilSaveMngL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilIsValidMp3(ILconst_string FileName);
ILboolean ilIsValidMp3F(ILHANDLE File);
ILboolean ilIsValidMp3L(const void *Lump, ILuint Size);
ILboolean ilLoadMp3(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadMp3F(ILimage *Image, ILHANDLE File);
ILboolean ilLoadMp3L(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilLoadPcd(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadPcdF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadPcdL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidPcx(ILimage *Image, ILconst_string FileName);
ILboolean ilIsValidPcxF(ILimage *Image, ILHANDLE File);
ILboolean ilIsValidPcxL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilLoadPcx(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadPcxF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadPcxL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSavePcx(ILimage *Image, ILconst_string FileName);
ILuint    ilSavePcxF(ILimage *Image, ILHANDLE File);
ILuint    ilSavePcxL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilIsValidPic(ILconst_string FileName);
ILboolean ilIsValidPicF(ILHANDLE File);
ILboolean ilIsValidPicL(const void *Lump, ILuint Size);
ILboolean ilLoadPic(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadPicF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadPicL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilLoadPix(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadPixF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadPixL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidPng(ILconst_string FileName);
ILboolean ilIsValidPngF(ILHANDLE File);
ILboolean ilIsValidPngL(const void *Lump, ILuint Size);
ILboolean ilLoadPng(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadPngF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadPngL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSavePng(ILimage *Image, ILconst_string FileName);
ILuint    ilSavePngF(ILimage *Image, ILHANDLE File);
ILuint    ilSavePngL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilIsValidPnm(ILconst_string FileName);
ILboolean ilIsValidPnmF(ILHANDLE File);
ILboolean ilIsValidPnmL(const void *Lump, ILuint Size);
ILboolean ilLoadPnm(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadPnmF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadPnmL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSavePnm(ILimage *Image, ILconst_string FileName);
ILuint    ilSavePnmF(ILimage *Image, ILHANDLE File);
ILuint    ilSavePnmL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilIsValidPsd(ILconst_string FileName);
ILboolean ilIsValidPsdF(ILHANDLE File);
ILboolean ilIsValidPsdL(const void *Lump, ILuint Size);
ILboolean ilLoadPsd(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadPsdF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadPsdL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSavePsd(ILimage *Image, ILconst_string FileName);
ILuint    ilSavePsdF(ILimage *Image, ILHANDLE File);
ILuint    ilSavePsdL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilIsValidPsp(ILconst_string FileName);
ILboolean ilIsValidPspF(ILHANDLE File);
ILboolean ilIsValidPspL(const void *Lump, ILuint Size);
ILboolean ilLoadPsp(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadPspF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadPspL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilLoadPxr(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadPxrF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadPxrL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilLoadRaw(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadRawF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadRawL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSaveRaw(ILimage *Image, ILconst_string FileName);
ILuint    ilSaveRawF(ILimage *Image, ILHANDLE File);
ILuint    ilSaveRawL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilLoadRot(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadRotF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadRotL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidRot(ILconst_string FileName);
ILboolean ilIsValidRotF(ILHANDLE File);
ILboolean ilIsValidRotL(const void *Lump, ILuint Size);
ILboolean ilIsValidSgi(ILconst_string FileName);
ILboolean ilIsValidSgiF(ILHANDLE File);
ILboolean ilIsValidSgiL(const void *Lump, ILuint Size);
ILboolean ilLoadSgi(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadSgiF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadSgiL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSaveSgi(ILimage *Image, ILconst_string FileName);
ILuint    ilSaveSgiF(ILimage *Image, ILHANDLE File);
ILuint    ilSaveSgiL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilIsValidSun(ILconst_string FileName);
ILboolean ilIsValidSunF(ILHANDLE File);
ILboolean ilIsValidSunL(const void *Lump, ILuint Size);
ILboolean ilLoadSun(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadSunF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadSunL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidTga(ILconst_string FileName);
ILboolean ilIsValidTgaF(ILHANDLE File);
ILboolean ilIsValidTgaL(const void *Lump, ILuint Size);
ILboolean ilLoadTarga(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadTargaF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadTargaL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSaveTarga(ILimage *Image, ILconst_string FileName);
ILuint    ilSaveTargaF(ILimage *Image, ILHANDLE File);
ILuint    ilSaveTargaL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilLoadTexture(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadTextureF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadTextureL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidTiff(ILconst_string FileName);
ILboolean ilIsValidTiffF(ILHANDLE File);
ILboolean ilIsValidTiffL(const void *Lump, ILuint Size);
ILboolean ilLoadTiff(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadTiffF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadTiffL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSaveTiff(ILimage *Image, ILconst_string FileName);
ILuint    ilSaveTiffF(ILimage *Image, ILHANDLE File);
ILuint    ilSaveTiffL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilIsValidTpl(ILconst_string FileName);
ILboolean ilIsValidTplF(ILHANDLE File);
ILboolean ilIsValidTplL(const void *Lump, ILuint Size);
ILboolean ilLoadTpl(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadTplF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadTplL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilLoadUtx(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadUtxF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadUtxL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidVtf(ILconst_string FileName);
ILboolean ilIsValidVtfF(ILHANDLE File);
ILboolean ilIsValidVtfL(const void *Lump, ILuint Size);
ILboolean ilLoadVtf(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadVtfF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadVtfL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSaveVtf(ILimage *Image, ILconst_string FileName);
ILuint    ilSaveVtfF(ILimage *Image, ILHANDLE File);
ILuint    ilSaveVtfL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilLoadWad(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadWadF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadWadL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilLoadWal(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadWalF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadWalL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilLoadWbmp(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadWbmpF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadWbmpL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilSaveWbmp(ILimage *Image, ILconst_string FileName);
ILuint    ilSaveWbmpF(ILimage *Image, ILHANDLE File);
ILuint    ilSaveWbmpL(ILimage *Image, void *Lump, ILuint Size);
ILboolean ilIsValidWdp(ILconst_string FileName);
ILboolean ilIsValidWdpF(ILHANDLE File);
ILboolean ilIsValidWdpL(const void *Lump, ILuint Size);
ILboolean ilLoadWdp(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadWdpF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadWdpL(ILimage *Image, const void *Lump, ILuint Size);
ILboolean ilIsValidXpm(ILconst_string FileName);
ILboolean ilIsValidXpmF(ILHANDLE File);
ILboolean ilIsValidXpmL(const void *Lump, ILuint Size);
ILboolean ilLoadXpm(ILimage *Image, ILconst_string FileName);
ILboolean ilLoadXpmF(ILimage *Image, ILHANDLE File);
ILboolean ilLoadXpmL(ILimage *Image, const void *Lump, ILuint Size);


// OpenEXR is written in C++, so we have to wrap this to avoid linker errors.
/*#ifndef IL_NO_EXR
	#ifdef __cplusplus
	extern "C" {
	#endif
		ILboolean ilLoadExr(ILconst_string FileName);
	#ifdef __cplusplus
	}
	#endif
#endif*/

//ILboolean ilLoadExr(ILconst_string FileName);


/*#ifdef __cplusplus
}
#endif*/

#endif//INTERNAL_H
