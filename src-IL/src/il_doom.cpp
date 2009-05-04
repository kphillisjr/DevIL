//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 05/02/2009
//
// Filename: src-IL/src/il_doom.cpp
//
// Description: Reads Doom textures and flats
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_DOOM
#include "il_manip.h"
#include "il_pal.h"
#include "il_doompal.h"


ILboolean iLoadDoomInternal(ILimage *Image, ILstate *State);
ILboolean iLoadDoomFlatInternal(ILimage *Image, ILstate *State);


//
// READ A DOOM IMAGE
//

//! Reads a Doom file
ILboolean ilLoadDoom(ILimage *Image, ILconst_string FileName, ILstate *State)
{
	ILHANDLE	DoomFile;
	ILboolean	bDoom = IL_FALSE;

	// Not sure of any kind of specified extension...maybe .lmp?
	/*if (!iCheckExtension(FileName, "")) {
		ilSetError(IL_INVALID_EXTENSION);
		return NULL;
	}*/

	DoomFile = iopenr(FileName);
	if (DoomFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bDoom;
	}

	bDoom = ilLoadDoomF(Image, DoomFile, State);
	icloser(DoomFile);

	return bDoom;
}


//! Reads an already-opened Doom file
ILboolean ilLoadDoomF(ILimage *Image, ILHANDLE File, ILstate *State)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadDoomInternal(Image, State);
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a Doom texture
ILboolean ilLoadDoomL(ILimage *Image, const void *Lump, ILuint Size, ILstate *State)
{
	iSetInputLump(Lump, Size);
	return iLoadDoomInternal(Image, State);
}


// From the DTE sources (mostly by Denton Woods with corrections by Randy Heit)
ILboolean iLoadDoomInternal(ILimage *Image, ILstate *State)
{
	ILshort	width, height, graphic_header[2], column_loop, row_loop;
	ILint	column_offset, pointer_position, first_pos;
	ILubyte	post, topdelta, length;
	ILubyte	*NewData;
	ILuint	i;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	first_pos = itell();  // Needed to go back to the offset table
	width = GetLittleShort();
	height = GetLittleShort();
	graphic_header[0] = GetLittleShort();  // Not even used
	graphic_header[1] = GetLittleShort();  // Not even used

	if (!ilTexImage(Image, width, height, 1, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE, NULL, State)) {
		return IL_FALSE;
	}
	Image->Origin = IL_ORIGIN_UPPER_LEFT;

	Image->Pal.Palette = (ILubyte*)ialloc(IL_DOOMPAL_SIZE);
	if (Image->Pal.Palette == NULL) {
		return IL_FALSE;
	}
	Image->Pal.PalSize = IL_DOOMPAL_SIZE;
	Image->Pal.PalType = IL_PAL_RGBA32;
	memcpy(Image->Pal.Palette, ilDefaultDoomPal, IL_DOOMPAL_SIZE);

	// 247 is always the transparent colour (usually cyan)
	memset(Image->Data, 247, Image->SizeOfData);

	for (column_loop = 0; column_loop < width; column_loop++) {
		column_offset = GetLittleInt();
		pointer_position = itell();
		iseek(first_pos + column_offset, IL_SEEK_SET);

		while (1) {
			if (iread(&topdelta, 1, 1) != 1)
				return IL_FALSE;
			if (topdelta == 255)
				break;
			if (iread(&length, 1, 1) != 1)
				return IL_FALSE;
			if (iread(&post, 1, 1) != 1)
				return IL_FALSE; // Skip extra byte for scaling

			for (row_loop = 0; row_loop < length; row_loop++) {
				if (iread(&post, 1, 1) != 1)
					return IL_FALSE;
				if (row_loop + topdelta < height)
					Image->Data[(row_loop+topdelta) * width + column_loop] = post;
			}
			iread(&post, 1, 1); // Skip extra scaling byte
		}

		iseek(pointer_position, IL_SEEK_SET);
	}

	// Converts palette entry 247 (cyan) to transparent.
	if (ilGetBoolean(IL_CONV_PAL) == IL_TRUE) {
		NewData = (ILubyte*)ialloc(Image->SizeOfData * 4);
		if (NewData == NULL) {
			return IL_FALSE;
		}

		for (i = 0; i < Image->SizeOfData; i++) {
			NewData[i * 4] = Image->Pal.Palette[Image->Data[i]];
			NewData[i * 4] = Image->Pal.Palette[Image->Data[i]];
			NewData[i * 4] = Image->Pal.Palette[Image->Data[i]];
			NewData[i * 4 + 3] = Image->Data[i] != 247 ? 255 : 0;
		}

		if (!ilTexImage(Image, Image->Width, Image->Height, Image->Depth, IL_RGBA, Image->Type, NewData, State)) {
			ifree(NewData);
			return IL_FALSE;
		}
		Image->Origin = IL_ORIGIN_UPPER_LEFT;
		ifree(NewData);
	}

	return ilFixImage(Image);
}


//
// READ A DOOM FLAT
//

//! Reads a Doom flat file
ILboolean ilLoadDoomFlat(ILimage *Image, ILconst_string FileName, ILstate *State)
{
	ILHANDLE	FlatFile;
	ILboolean	bFlat = IL_FALSE;

	// Not sure of any kind of specified extension...maybe .lmp?
	/*if (!iCheckExtension(FileName, "")) {
		ilSetError(IL_INVALID_EXTENSION);
		return NULL;
	}*/

	FlatFile = iopenr(FileName);
	if (FlatFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bFlat;
	}

	bFlat = ilLoadDoomFlatF(Image, FlatFile, State);
	icloser(FlatFile);

	return bFlat;
}


//! Reads an already-opened Doom flat file
ILboolean ilLoadDoomFlatF(ILimage *Image, ILHANDLE File, ILstate *State)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadDoomFlatInternal(Image, State);
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a Doom flat
ILboolean ilLoadDoomFlatL(ILimage *Image, const void *Lump, ILuint Size, ILstate *State)
{
	iSetInputLump(Lump, Size);
	return iLoadDoomFlatInternal(Image, State);
}


// Basically just ireads 4096 bytes and copies the palette
ILboolean iLoadDoomFlatInternal(ILimage *Image, ILstate *State)
{
	ILubyte	*NewData;
	ILuint	i;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (!ilTexImage(Image, 64, 64, 1, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE, NULL, State)) {
		return IL_FALSE;
	}
	Image->Origin = IL_ORIGIN_UPPER_LEFT;

	Image->Pal.Palette = (ILubyte*)ialloc(IL_DOOMPAL_SIZE);
	if (Image->Pal.Palette == NULL) {
		return IL_FALSE;
	}
	Image->Pal.PalSize = IL_DOOMPAL_SIZE;
	Image->Pal.PalType = IL_PAL_RGB24;
	memcpy(Image->Pal.Palette, ilDefaultDoomPal, IL_DOOMPAL_SIZE);

	if (iread(Image->Data, 1, 4096) != 4096)
		return IL_FALSE;

	if (ilGetBoolean(IL_CONV_PAL) == IL_TRUE) {
		NewData = (ILubyte*)ialloc(Image->SizeOfData * 4);
		if (NewData == NULL) {
			return IL_FALSE;
		}

		for (i = 0; i < Image->SizeOfData; i++) {
			NewData[i * 4] = Image->Pal.Palette[Image->Data[i]];
			NewData[i * 4] = Image->Pal.Palette[Image->Data[i]];
			NewData[i * 4] = Image->Pal.Palette[Image->Data[i]];
			NewData[i * 4 + 3] = Image->Data[i] != 247 ? 255 : 0;
		}

		if (!ilTexImage(Image, Image->Width, Image->Height, Image->Depth, IL_RGBA, Image->Type, NewData, State)) {
			ifree(NewData);
			return IL_FALSE;
		}
		Image->Origin = IL_ORIGIN_UPPER_LEFT;
		ifree(NewData);
	}

	return ilFixImage(Image);
}


#endif
