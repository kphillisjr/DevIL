//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 03/26/2009
//
// Filename: src-IL/include/il_jpeg.h
//
// Description: Jpeg (.jpg) functions
//
//-----------------------------------------------------------------------------

#ifndef JPEG_H
#define JPEG_H

#include "il_internal.h"

ILboolean iCheckJpg(ILubyte Header[2]);
ILboolean iIsValidJpg(void);

#ifndef IL_USE_IJL
	ILboolean iLoadJpegInternal(ILimage *Image);
	ILboolean iSaveJpegInternal(ILimage *Image);
#else
	ILboolean iLoadJpegInternal(ILimage *Image, ILconst_string FileName, ILvoid *Lump, ILuint Size);
	ILboolean iSaveJpegInternal(ILimage *Image, ILconst_string FileName, ILvoid *Lump, ILuint Size);
#endif

#endif//JPEG_H
