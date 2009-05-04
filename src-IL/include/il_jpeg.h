//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 05/02/2009
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
	ILboolean iLoadJpegInternal(ILimage *Image, ILstate *State);
	ILboolean iSaveJpegInternal(ILimage *Image, ILstate *State);
#else
	ILboolean iLoadJpegInternal(ILimage *Image, ILconst_string FileName, ILvoid *Lump, ILuint Size, ILstate *State);
	ILboolean iSaveJpegInternal(ILimage *Image, ILconst_string FileName, ILvoid *Lump, ILuint Size, ILstate *State);
#endif

#endif//JPEG_H
