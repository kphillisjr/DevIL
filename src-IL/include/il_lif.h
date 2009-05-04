//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 05/01/2009
//
// Filename: src-IL/include/il_lif.h
//
// Description: Reads a Homeworld image.
//
//-----------------------------------------------------------------------------


#ifndef LIF_H
#define LIF_H

#include "il_internal.h"

typedef struct LIF_HEAD
{
    char	Id[8];			//"Willy 7"
    ILuint	Version;		// Version Number (260)
    ILuint	Flags;			// Usually 50
    ILuint	Width;
	ILuint	Height;
    ILuint	PaletteCRC;		// CRC of palettes for fast comparison.
    ILuint	ImageCRC;		// CRC of the image.
	ILuint	PalOffset;		// Offset to the palette (not used).
	ILuint	TeamEffect0;	// Team effect offset 0
	ILuint	TeamEffect1;	// Team effect offset 1
} LIF_HEAD;

ILboolean iIsValidLif(void);
ILboolean iCheckLif(LIF_HEAD *Header);
ILboolean iLoadLifInternal(ILimage *Image, ILstate *State);

#endif//LIF_H
