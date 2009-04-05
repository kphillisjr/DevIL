//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 04/04/2009
//
// Filename: src-IL/src/il_sin.cpp
//
// Description: Reads from a Ritualistic SiN (.swl) file.
//
//-----------------------------------------------------------------------------

#include "il_internal.h"
#ifndef IL_NO_SIN

ILboolean iLoadSinInternal(ILimage *Image);


//! Reads a SIN file
ILboolean ilLoadSin(ILimage *Image, ILconst_string FileName)
{
	ILHANDLE	SinFile;
	ILboolean	bSin = IL_FALSE;

	SinFile = iopenr(FileName);
	if (SinFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bSin;
	}

	bSin = ilLoadSinF(Image, SinFile);
	icloser(SinFile);

	return bSin;
}


//! Reads an already-opened SIN file
ILboolean ilLoadSinF(ILimage *Image, ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadSinInternal(Image);
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Reads from a memory "lump" that contains a SIN
ILboolean ilLoadSinL(ILimage *Image, const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadSinInternal(Image);
}


// Internal function used to load the SIN.
ILboolean iLoadSinInternal(ILimage *Image)
{
	ILuint	Width, Height;
	ILimage	*CurImage;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	// The first 64 bytes are just the internal name of the image.
	iseek(64, IL_SEEK_SET);
	// Then comes the width and the height.
	Width = GetLittleUInt();
	Height = GetLittleUInt();

	if (!ilTexImage(Image, Width, Height, 1, IL_COLOR_INDEX, IL_UNSIGNED_BYTE, NULL))
		return IL_FALSE;

	// Every image is color-indexed with a RGB32 palette (alpha unimportant?).
	Image->Pal.Palette = (ILubyte*)ialloc(256 * 4);
	if (Image->Pal.Palette == NULL)
		return IL_FALSE;
	Image->Pal.PalSize = 256 * 4;
	Image->Pal.PalType = IL_PAL_RGB32;
	if (iread(Image->Pal.Palette, 256 * 4, 1) != 1)
		return IL_FALSE;

	// @TODO: Unsure what the data in between the palette and the image is.
	//  It looks like flags and values used in-game:
	//  http://quark.cvs.sourceforge.net/viewvc/*checkout*/quark/runtime/addons/Sin/DataSin.qrk?revision=1.11.
	//  You can see the values immediately in the SinView program.  They are unimportant for our
	//  purposes right now, though.  We may keep these when we implement metadata in DevIL.
	iseek(0x4D4, IL_SEEK_SET);

	// Each image has exactly 4 mipmaps, so we read each.
	CurImage = Image;
	for (int i = 0; i < 4; i++) {
		if (iread(CurImage->Data, CurImage->SizeOfData, 1) != 1)
			return IL_FALSE;
		// The origin is always in the upper left.
		CurImage->Origin = IL_ORIGIN_UPPER_LEFT;
		if (i < 3) {
			// If this is not the last image, create the next mipmap.
			CurImage->Mipmaps = ilNewImage(Width >> (i+1), Height >> (i+1), 1, IL_COLOR_INDEX, IL_UNSIGNED_BYTE, NULL);
			if (CurImage->Mipmaps == NULL)
				return IL_FALSE;
			CurImage = CurImage->Mipmaps;
			// Each mipmap uses the same palette.
			if (!iCopyPalette(&CurImage->Pal, &Image->Pal))
				return IL_FALSE;
		}
	}

	return ilFixImage(Image);
}

#endif//IL_NO_SIN

