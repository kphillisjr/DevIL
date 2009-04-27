//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 04/24/2009
//
// Filename: src-IL/src/il_tex.cpp
//
// Description: Reads from a Serious Sam (.tex) file.
//				The only specifications I was able to find were at
//				http://forum.xentax.com/viewtopic.php?f=18&p=28685 and
//				http://forums.seriouszone.com/showthread.php?t=30593.
//
//-----------------------------------------------------------------------------

#include "il_internal.h"
#ifndef IL_NO_TEX


typedef struct TEXHEAD
{
	char	Sig[4];
	ILuint	Version;
	char	Tdat[4];
	ILuint	Format;
	ILuint	Width;
	ILuint	Height;
	ILuint	Unknown1;
	ILuint	Divisor;
	ILuint	Unknown2;
	char	FrameID[4];
} TEXHEAD;

ILboolean iCheckTex(TEXHEAD &Header);
ILboolean iIsValidTex(void);
ILboolean iLoadTexInternal(ILimage *Image);


//! Checks if the file specified in FileName is a valid .tex file.
ILboolean ilIsValidTex(ILconst_string FileName)
{
	ILHANDLE	TexFile;
	ILboolean	bTex = IL_FALSE;

	if (!iCheckExtension(FileName, IL_TEXT("sgi"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bTex;
	}

	TexFile = iopenr(FileName);
	if (TexFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bTex;
	}

	bTex = ilIsValidTexF(TexFile);
	icloser(TexFile);

	return bTex;
}


//! Checks if the ILHANDLE contains a valid .tex file at the current position.
ILboolean ilIsValidTexF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidTex();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Checks if Lump is a valid .tex lump.
ILboolean ilIsValidTexL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidTex();
}


// Internal function used to get the .tex header from the current file.
ILboolean iGetTexHead(TEXHEAD &Header)
{
	iread(Header.Sig, 4, 1);
	Header.Version = GetLittleUInt();
	iread(Header.Tdat, 4, 1);
	Header.Format = GetLittleUInt();
	Header.Width = GetLittleUInt();
	Header.Height = GetLittleUInt();
	GetLittleUInt();  // Skip this...unknown.

	// The width and height are divided by 2^Divisor.
	Header.Divisor = GetLittleUInt();
	Header.Divisor = 1 << Header.Divisor;
	Header.Width = Header.Width / Header.Divisor;
	Header.Height = Header.Height / Header.Divisor;

	GetLittleUInt();  // Skip this...unknown.
	iread(Header.FrameID, 4, 1);

	return IL_TRUE;
}


//! Internal function to get the header and check it.
ILboolean iIsValidTex(void)
{
	TEXHEAD	Head;

	if (!iGetTexHead(Head))
		return IL_FALSE;
	iseek(-0x28, IL_SEEK_CUR);  // Go ahead and restore to previous state

	return iCheckTex(Head);
}


//! Internal function used to check if the HEADER is a valid .tex header.
ILboolean iCheckTex(TEXHEAD &Header)
{
	// Always starts with 'TVER'.
	if (strncmp(Header.Sig, "TVER", 4))
		return IL_FALSE;
	// We only know how to deal with version 4 right now.
	//@TODO: Last Chaos uses 0x10004:
	//  http://forum.xentax.com/viewtopic.php?f=18&p=28685
	if (Header.Version != 4)
		return IL_FALSE;
	// Signals that data will be following.
	if (strncmp(Header.Tdat, "TDAT", 4))
		return IL_FALSE;
	// Only know how to deal with these formats.
	//@TODO: May also have DXT formats.
	if (Header.Format != 0 && Header.Format != 2 && Header.Format != 3)
		return IL_FALSE;
	// Cannot have 0 width or height images.
	if (Header.Width == 0 || Header.Height == 0)
		return IL_FALSE;
	// Can be either 'FRMS' (uncompressed) or 'FRMC' (compressed)
	//@TODO: Support 'FRMC'.
	if (strncmp(Header.FrameID, "FRMS", 4))
		return IL_FALSE;

	return IL_TRUE;
}


//! Reads a TEX file
ILboolean ilLoadTex(ILimage *Image, ILconst_string FileName, ILstate *State)
{
	ILHANDLE	TexFile;
	ILboolean	bTex = IL_FALSE;

	TexFile = iopenr(FileName);
	if (TexFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bTex;
	}

	bTex = ilLoadTexF(Image, TexFile);
	icloser(TexFile);

	return bTex;
}


//! Reads an already-opened TEX file
ILboolean ilLoadTexF(ILimage *Image, ILHANDLE File, ILstate *State)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadTexInternal(Image);
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Reads from a memory "lump" that contains a TEX
ILboolean ilLoadTexL(ILimage *Image, const void *Lump, ILuint Size, ILstate *State)
{
	iSetInputLump(Lump, Size);
	return iLoadTexInternal(Image);
}


// Internal function used to load the TEX.
ILboolean iLoadTexInternal(ILimage *Image, ILstate *State)
{
	TEXHEAD	Header;
	ILenum	Format;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	// Get the header and check it.
	iGetTexHead(Header);
	if (!iCheckTex(Header)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	switch (Header.Format)
	{
		case 0x00:  // @TODO: This is really color-indexed, but I cannot find the palette!
			Format = IL_LUMINANCE;
			break;
		case 0x02:
			Format = IL_RGB;
			break;
		case 0x03:
			Format = IL_RGBA;
			break;
		default:
			ilSetError(IL_INVALID_FILE_HEADER);
			return IL_FALSE;
	}

	ilTexImage(Image, Header.Width, Header.Height, 1, Format, IL_UNSIGNED_BYTE, NULL);
	iread(Image->Data, Image->SizeOfData, 1);
	Image->Origin = IL_ORIGIN_UPPER_LEFT;

	//@TODO: Are there mipmaps present in these images?

	return ilFixImage(Image);
}

#endif//IL_NO_TEX

