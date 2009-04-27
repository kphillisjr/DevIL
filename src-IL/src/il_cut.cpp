//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 04/24/2009
//
// Filename: src-IL/src/il_cut.cpp
//
// Description: Reads a Dr. Halo .cut file
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_CUT
#include "il_manip.h"
#include "il_pal.h"
#include "il_bits.h"


// Wrap it just in case...
#ifdef _MSC_VER
#pragma pack(push, packed_struct, 1)
#endif
typedef struct CUT_HEAD
{
	ILushort	Width;
	ILushort	Height;
	ILint		Dummy;
} IL_PACKSTRUCT CUT_HEAD;
#ifdef _MSC_VER
#pragma pack(pop,  packed_struct)
#endif

ILboolean iLoadCutInternal(ILimage *Image);

//! Reads a .cut file
ILboolean ilLoadCut(ILimage *Image, ILconst_string FileName, ILstate *State)
{
	ILHANDLE	CutFile;
	ILboolean	bCut = IL_FALSE;

	CheckState();
	CutFile = iopenr(FileName);
	if (CutFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bCut;
	}

	bCut = ilLoadCutF(Image, CutFile);
	icloser(CutFile);

	return bCut;
}


//! Reads an already-opened .cut file
ILboolean ilLoadCutF(ILimage *Image, ILHANDLE File, ILstate *State)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	CheckState();
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadCutInternal(Image);
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a .cut
ILboolean ilLoadCutL(ILimage *Image, const void *Lump, ILuint Size, ILstate *State)
{
	CheckState();
	iSetInputLump(Lump, Size);
	return iLoadCutInternal(Image);
}


//@TODO: Note:  .Cut support has not been tested yet!
// A .cut can only have 1 bpp.
//	We need to add support for the .pal's PSP outputs with these...
ILboolean iLoadCutInternal(ILimage *Image, ILstate *State)
{
	CUT_HEAD	Header;
	ILuint		Size, i = 0, j;
	ILubyte		Count, Run;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Header.Width = GetLittleShort();
	Header.Height = GetLittleShort();
	Header.Dummy = GetLittleInt();

	if (Header.Width == 0 || Header.Height == 0) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	if (!ilTexImage(Image, Header.Width, Header.Height, 1, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE, NULL)) {  // always 1 bpp
		return IL_FALSE;
	}
	Image->Origin = IL_ORIGIN_LOWER_LEFT;

	Size = Header.Width * Header.Height;

	while (i < Size) {
		Count = igetc();
		if (Count == 0) { // end of row
			igetc();  // Not supposed to be here, but
			igetc();  //  PSP is putting these two bytes here...WHY?!
			continue;
		}
		if (Count & BIT_7) {  // rle-compressed
			ClearBits(Count, BIT_7);
			Run = igetc();
			for (j = 0; j < Count; j++) {
				Image->Data[i++] = Run;
			}
		}
		else {  // run of pixels
			for (j = 0; j < Count; j++) {
				Image->Data[i++] = igetc();
			}
		}
	}

	Image->Origin = IL_ORIGIN_UPPER_LEFT;  // Not sure

	/*Image->Pal.Palette = SharedPal.Palette;
	Image->Pal.PalSize = SharedPal.PalSize;
	Image->Pal.PalType = SharedPal.PalType;*/

	return ilFixImage(Image);
}

#endif//IL_NO_CUT
