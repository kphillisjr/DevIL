//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 05/02/2009
//
// Filename: src-IL/src/il_pix.cpp
//
// Description: Reads from an Alias | Wavefront .pix file.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_PIX
#include "il_manip.h"
#include "il_endian.h"


#ifdef _MSC_VER
#pragma pack(push, pix_struct, 1)
#endif
typedef struct PIXHEAD
{
	ILushort	Width;
	ILushort	Height;
	ILushort	OffX;
	ILushort	OffY;
	ILushort	Bpp;
} IL_PACKSTRUCT PIXHEAD;
#ifdef _MSC_VER
#pragma pack(pop, pix_struct)
#endif

ILboolean iCheckPix(PIXHEAD *Header);
ILboolean iLoadPixInternal(ILimage *Image, ILstate *State);


// Internal function used to get the Pix header from the current file.
ILboolean iGetPixHead(PIXHEAD *Header)
{
	Header->Width = GetBigUShort();
	Header->Height = GetBigUShort();
	Header->OffX = GetBigUShort();
	Header->OffY = GetBigUShort();
	Header->Bpp = GetBigUShort();

	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidPix()
{
	PIXHEAD	Head;

	if (!iGetPixHead(&Head))
		return IL_FALSE;
	iseek(-(ILint)sizeof(PIXHEAD), IL_SEEK_CUR);

	return iCheckPix(&Head);
}


// Internal function used to check if the HEADER is a valid Pix header.
ILboolean iCheckPix(PIXHEAD *Header)
{
	if (Header->Width == 0 || Header->Height == 0)
		return IL_FALSE;
	if (Header->Bpp != 24)
		return IL_FALSE;
	//if (Header->OffY != Header->Height)
	//	return IL_FALSE;

	return IL_TRUE;
}


//! Reads a Pix file
ILboolean ilLoadPix(ILimage *Image, ILconst_string FileName, ILstate *State)
{
	ILHANDLE	PixFile;
	ILboolean	bPix = IL_FALSE;

	PixFile = iopenr(FileName);
	if (PixFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bPix;
	}

	bPix = ilLoadPixF(Image, PixFile, State);
	icloser(PixFile);

	return bPix;
}


//! Reads an already-opened Pix file
ILboolean ilLoadPixF(ILimage *Image, ILHANDLE File, ILstate *State)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadPixInternal(Image, State);
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a Pix
ILboolean ilLoadPixL(ILimage *Image, const void *Lump, ILuint Size, ILstate *State)
{
	iSetInputLump(Lump, Size);
	return iLoadPixInternal(Image, State);
}


// Internal function used to load the Pix.
ILboolean iLoadPixInternal(ILimage *Image, ILstate *State)
{
	PIXHEAD	Header;
	ILuint	i, j;
	ILubyte	ByteHead, Colour[3];

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (!iGetPixHead(&Header))
		return IL_FALSE;
	if (!iCheckPix(&Header)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	if (!ilTexImage(Image, Header.Width, Header.Height, 1, IL_BGR, IL_UNSIGNED_BYTE, NULL, State))
		return IL_FALSE;

	for (i = 0; i < Image->SizeOfData; ) {
		ByteHead = igetc();
		if (iread(Colour, 1, 3) != 3)
			return IL_FALSE;
		for (j = 0; j < ByteHead; j++) {
			Image->Data[i++] = Colour[0];
			Image->Data[i++] = Colour[1];
			Image->Data[i++] = Colour[2];
		}
	}

	Image->Origin = IL_ORIGIN_UPPER_LEFT;

	return ilFixImage(Image);
}

#endif//IL_NO_PIX
