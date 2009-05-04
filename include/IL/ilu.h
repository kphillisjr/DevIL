//-----------------------------------------------------------------------------
//
// ImageLib Utility Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 03/14/2009
//
// Filename: IL/ilu.h
//
// Description: The main include file for ILU
//
//-----------------------------------------------------------------------------

// Doxygen comment
/*! \file ilu.h
    The main include file for ILU
*/

#ifndef __ilu_h_
#ifndef __ILU_H__

#define __ilu_h_
#define __ILU_H__

#include <IL/il.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifdef _WIN32
	#if (defined(IL_USE_PRAGMA_LIBS)) && (!defined(IL_BUILD_LIBRARY))
		#if defined(_MSC_VER) || defined(__BORLANDC__)
			#pragma comment(lib, "ILU.lib")
		#endif
	#endif
#endif


#define ILU_VERSION_1_7_8 1
#define ILU_VERSION       178


#define ILU_FILTER         0x2600
#define ILU_NEAREST        0x2601
#define ILU_LINEAR         0x2602
#define ILU_BILINEAR       0x2603
#define ILU_SCALE_BOX      0x2604
#define ILU_SCALE_TRIANGLE 0x2605
#define ILU_SCALE_BELL     0x2606
#define ILU_SCALE_BSPLINE  0x2607
#define ILU_SCALE_LANCZOS3 0x2608
#define ILU_SCALE_MITCHELL 0x2609


// Error types
#define ILU_INVALID_ENUM      0x0501
#define ILU_OUT_OF_MEMORY     0x0502
#define ILU_INTERNAL_ERROR    0x0504
#define ILU_INVALID_VALUE     0x0505
#define ILU_ILLEGAL_OPERATION 0x0506
#define ILU_INVALID_PARAM     0x0509


// Values
#define ILU_PLACEMENT          0x0700
#define ILU_LOWER_LEFT         0x0701
#define ILU_LOWER_RIGHT        0x0702
#define ILU_UPPER_LEFT         0x0703
#define ILU_UPPER_RIGHT        0x0704
#define ILU_CENTER             0x0705
#define ILU_CONVOLUTION_MATRIX 0x0710
  
#define ILU_VERSION_NUM IL_VERSION_NUM
#define ILU_VENDOR      IL_VENDOR


// Languages
#define ILU_ENGLISH            0x0800
#define ILU_ARABIC             0x0801
#define ILU_DUTCH              0x0802
#define ILU_JAPANESE           0x0803
#define ILU_SPANISH            0x0804
#define ILU_GERMAN             0x0805
#define ILU_FRENCH             0x0806
#define ILU_ITALIAN            0x0807


// Filters
/*
#define ILU_FILTER_BLUR         0x0803
#define ILU_FILTER_GAUSSIAN_3x3 0x0804
#define ILU_FILTER_GAUSSIAN_5X5 0x0805
#define ILU_FILTER_EMBOSS1      0x0807
#define ILU_FILTER_EMBOSS2      0x0808
#define ILU_FILTER_LAPLACIAN1   0x080A
#define ILU_FILTER_LAPLACIAN2   0x080B
#define ILU_FILTER_LAPLACIAN3   0x080C
#define ILU_FILTER_LAPLACIAN4   0x080D
#define ILU_FILTER_SHARPEN1     0x080E
#define ILU_FILTER_SHARPEN2     0x080F
#define ILU_FILTER_SHARPEN3     0x0810
*/


typedef struct ILinfo
{
	ILubyte *Data;      // the image's data
	ILuint  Width;      // the image's width
	ILuint  Height;     // the image's height
	ILuint  Depth;      // the image's depth
	ILubyte Bpp;        // bytes per pixel (not bits) of the image
	ILuint  SizeOfData; // the total size of the data (in bytes)
	ILenum  Format;     // image format (in IL enum style)
	ILenum  Type;       // image type (in IL enum style)
	ILenum  Origin;     // origin of the image
	ILubyte *Palette;   // the image's palette
	ILenum  PalType;    // palette type
	ILuint  PalSize;    // palette size
	ILenum  CubeFlags;  // flags for what cube map sides are present
	ILuint  NumNext;    // number of images following
	ILuint  NumMips;    // number of mipmaps
	ILuint  NumLayers;  // number of layers
} ILinfo;


typedef struct ILpointf {
	ILfloat x;
	ILfloat y;
} ILpointf;

typedef struct ILpointi {
	ILint x;
	ILint y;
} ILpointi;

ILAPI ILboolean      ILAPIENTRY iluAlienify(ILimage *Image);
ILAPI ILboolean      ILAPIENTRY iluBlurAvg(ILimage *Image, ILuint Iter);
ILAPI ILboolean      ILAPIENTRY iluBlurGaussian(ILimage *Image, ILuint Iter);
ILAPI ILboolean      ILAPIENTRY iluBuildMipmaps(ILimage *Image);
ILAPI ILuint         ILAPIENTRY iluColoursUsed(ILimage *Image);
ILAPI ILboolean      ILAPIENTRY iluCompareImage(ILimage *Image1, ILimage *Image2);
ILAPI ILboolean      ILAPIENTRY iluContrast(ILimage *Image, ILfloat Contrast);
ILAPI ILboolean      ILAPIENTRY iluConvolution(ILimage *Image, ILint *matrix, ILint scale, ILint bias);
ILAPI ILboolean      ILAPIENTRY iluCrop(ILimage *Image, ILuint XOff, ILuint YOff, ILuint ZOff, ILuint Width, ILuint Height, ILuint Depth);
ILAPI ILboolean      ILAPIENTRY iluEdgeDetectE(ILimage *Image);
ILAPI ILboolean      ILAPIENTRY iluEdgeDetectP(ILimage *Image, ILstate *State);
ILAPI ILboolean      ILAPIENTRY iluEdgeDetectS(ILimage *Image);
ILAPI ILboolean      ILAPIENTRY iluEmboss(ILimage *Image);
ILAPI ILboolean      ILAPIENTRY iluEnlargeCanvas(ILimage *Image, ILuint Width, ILuint Height, ILuint Depth);
ILAPI ILboolean      ILAPIENTRY iluEnlargeImage(ILimage *Image, ILfloat XDim, ILfloat YDim, ILfloat ZDim);
ILAPI ILboolean      ILAPIENTRY iluEqualize(ILimage *Image);
ILAPI ILconst_string ILAPIENTRY iluErrorString(ILenum Error);
ILAPI ILboolean      ILAPIENTRY iluFlipImage(ILimage *Image);
ILAPI ILboolean      ILAPIENTRY iluGammaCorrect(ILimage *Image, ILfloat Gamma);
ILAPI void           ILAPIENTRY iluGetImageInfo(ILimage *Image, ILinfo *Info);
ILAPI ILint          ILAPIENTRY iluGetInteger(ILenum Mode);
ILAPI void           ILAPIENTRY iluGetIntegerv(ILenum Mode, ILint *Param);
ILAPI ILstring 		 ILAPIENTRY iluGetString(ILenum StringName);
ILAPI void           ILAPIENTRY iluImageParameter(ILenum PName, ILenum Param);
ILAPI void           ILAPIENTRY iluInit(void);
ILAPI ILboolean      ILAPIENTRY iluInvertAlpha(ILimage *Image);
ILAPI ILimage*       ILAPIENTRY iluLoadImage(ILconst_string FileName);
ILAPI ILboolean      ILAPIENTRY iluMirror(ILimage *Image);
ILAPI ILboolean      ILAPIENTRY iluNegative(ILimage *Image);
ILAPI ILboolean      ILAPIENTRY iluNoisify(ILimage *Image, ILclampf Tolerance);
ILAPI ILboolean      ILAPIENTRY iluPixelize(ILimage *Image, ILuint PixSize);
ILAPI void           ILAPIENTRY iluRegionfv(ILpointf *Points, ILuint n);
ILAPI void           ILAPIENTRY iluRegioniv(ILpointi *Points, ILuint n);
ILAPI ILboolean      ILAPIENTRY iluReplaceColour(ILimage *Image, ILubyte Red, ILubyte Green, ILubyte Blue, ILfloat Tolerance);
ILAPI ILboolean      ILAPIENTRY iluRotate(ILimage *Image, ILfloat Angle);
ILAPI ILboolean      ILAPIENTRY iluRotate3D(ILimage *Image, ILfloat x, ILfloat y, ILfloat z, ILfloat Angle);
ILAPI ILboolean      ILAPIENTRY iluSaturate1f(ILimage *Image, ILfloat Saturation);
ILAPI ILboolean      ILAPIENTRY iluSaturate4f(ILimage *Image, ILfloat r, ILfloat g, ILfloat b, ILfloat Saturation);
ILAPI ILboolean      ILAPIENTRY iluScale(ILimage *Image, ILuint Width, ILuint Height, ILuint Depth);
ILAPI ILboolean      ILAPIENTRY iluScaleAlpha(ILimage *Image, ILfloat scale);
ILAPI ILboolean      ILAPIENTRY iluScaleColours(ILimage *Image, ILfloat r, ILfloat g, ILfloat b);
ILAPI ILboolean      ILAPIENTRY iluSetLanguage(ILenum Language);
ILAPI ILboolean      ILAPIENTRY iluSharpen(ILimage *Image, ILfloat Factor, ILuint Iter);
ILAPI ILboolean      ILAPIENTRY iluSwapColours(ILimage *Image);
ILAPI ILboolean      ILAPIENTRY iluWave(ILimage *Image, ILfloat Angle);

#define iluColorsUsed   iluColoursUsed
#define iluSwapColors   iluSwapColours
#define iluReplaceColor iluReplaceColour
#define iluScaleColor   iluScaleColour

#ifdef __cplusplus
}
#endif

#endif // __ILU_H__
#endif // __ilu_h_
