//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 03/23/2009
//
// Filename: src-IL/src/il_wad.cpp
//
// Description: Reads from a Half-Life texture (.wad) file.
//				Some help on this file format came from the Quake specs at
//				http://www.gamers.org/dEngine/quake/spec/quake-spec22.html.
//
//-----------------------------------------------------------------------------

#include "il_internal.h"
#ifndef IL_NO_WAD
#include <vector>
using namespace std;


// Directory entry
typedef struct
{
	ILuint		Offset;		// Position of the entry in WAD
	ILuint		Wsize;		// Size of the entry in WAD file
	ILuint		Size;		// Size of the entry in memory
	ILubyte		Ident;		// Entry identification
	ILubyte		Cmprs;		// Compression
	ILushort	Dummy;		// Not used
	char		Name[17];	// 1 to 16 characters, '\0'-padded
	bool		UseEntry;	// Whether we can read this entry
} WAD_ENTRY;


ILboolean	iLoadWadInternal(ILimage *Image);
bool		iWadGetMipmaps(ILimage *Image, ILuint Width, ILuint Height);


//! Reads a WAD file
ILboolean ilLoadWad(ILimage *Image, ILconst_string FileName)
{
	ILHANDLE	WadFile;
	ILboolean	bWad = IL_FALSE;

	WadFile = iopenr(FileName);
	if (WadFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bWad;
	}

	bWad = ilLoadWadF(Image, WadFile);
	icloser(WadFile);

	return bWad;
}


//! Reads an already-opened WAD file
ILboolean ilLoadWadF(ILimage *Image, ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadWadInternal(Image);
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a WAD
ILboolean ilLoadWadL(ILimage *Image, const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadWadInternal(Image);
}


// Internal function used to load the WAD.
ILboolean iLoadWadInternal(ILimage *Image)
{
	char		Sig[4];
	ILuint		NumEnt, DirOff;
	ILuint		Width, Height;
	ILimage		*CurImage;
	bool		BaseCreated = false;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	iread(Sig, 4, 1);
	// Always starts with 'WAD3'
	if (strncmp(Sig, "WAD3", 4)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	NumEnt = GetLittleUInt();
	DirOff = GetLittleUInt();
	// We need to have at least one entry, and the directory offset cannot be shared
	//  with the signature.
	if (NumEnt == 0 || DirOff < 4) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	vector <WAD_ENTRY> Entries(NumEnt);
	iseek(DirOff, IL_SEEK_SET);

	// Read in the entire directory.
	for (ILuint i = 0; i < NumEnt; i++) {
		Entries[i].Offset = GetLittleUInt();
		Entries[i].Wsize = GetLittleUInt();
		Entries[i].Size = GetLittleUInt();
		Entries[i].Ident = igetc();
		Entries[i].Cmprs = igetc();
		Entries[i].Dummy = GetLittleUShort();
		if (iread(Entries[i].Name, 16, 1) != 1) {
			ilSetError(IL_INVALID_FILE_HEADER);
			return IL_FALSE;
		}
		Entries[i].Name[16] = NULL;  // Just to make sure that it is NULL-terminated.

		//@TODO: Do we need to check the compression field?
		if (Entries[i].Ident == 0x44)  // We can only read type 0x44 right now.
			Entries[i].UseEntry = false;
		else
			Entries[i].UseEntry = true;
	}

	// Read all of the entries
	for (ILuint i = 0; i < NumEnt; i++) {
		iseek(Entries[i].Offset, IL_SEEK_SET);
		iseek(16, IL_SEEK_CUR);  // @TODO: We don't care about the internal name right now.

		// We read the width and height now instead of in iWadGetMipmaps, because we have to
		//  create the base image for each mipmap chain first.  If we did it in iWadGetMipmaps,
		//  we would have to reallocate memory for the base image data, since we would not
		//  know the dimensions of the image here.
		Width = GetLittleUInt();
		Height = GetLittleUInt();
		if (Width == 0 || Height == 0) {
			ilSetError(IL_ILLEGAL_FILE_VALUE);
			return IL_FALSE;
		}

		// All images are color-indexed
		if (!BaseCreated) {
			if (!ilTexImage(Image, Width, Height, 1, IL_COLOR_INDEX, IL_UNSIGNED_BYTE, NULL))
				return IL_FALSE;
			CurImage = Image;
			BaseCreated = IL_TRUE;
		}
		else {  // Create the next image in the image chain.
			CurImage->Next = ilNewImage(Width, Height, 1, IL_COLOR_INDEX, IL_UNSIGNED_BYTE, NULL);
			CurImage = CurImage->Next;
			if (CurImage == NULL)
				return IL_FALSE;
		}

		// Get all of the mipmaps (including the base one).
		if (!iWadGetMipmaps(CurImage, Width, Height))
			return IL_FALSE;
	}

	return ilFixImage(Image);
}


// Internal function to get mipmaps (including the base one)
bool iWadGetMipmaps(ILimage *Image, ILuint Width, ILuint Height)
{
	//ILuint		MipOff[4];
	ILushort	PalSize;
	ILimage		*Mipmap;
	bool		BaseCreated = false;

	//for (ILuint i = 0; i < 4; i++) {
	//	MipOff[i] = GetLittleUInt();
	//}
	// Have not seen one yet where the mipmaps did not follow exactly like this.
	iseek(16, IL_SEEK_CUR);

	for (int i = 0; i < 4; i++) {
		if (!BaseCreated) {
			if (!ilTexImage(Image, Width, Height, 1, IL_COLOR_INDEX, IL_UNSIGNED_BYTE, NULL))
				return IL_FALSE;
			Mipmap = Image;
			BaseCreated = IL_TRUE;
		}
		else {
			// Each mipmap has dimensions halved from the previous mipmap.
			Width >>= 1;  Height >>= 1;
			Mipmap->Mipmaps = ilNewImage(Width, Height, 1, IL_COLOR_INDEX, IL_UNSIGNED_BYTE, NULL);
			Mipmap = Mipmap->Mipmaps;
			if (Mipmap == NULL)
				return IL_FALSE;
		}

		if (iread(Mipmap->Data, Mipmap->SizeOfData, 1) != 1)
			// @TODO: Should I create a fake palette here or change format to IL_LUMINANCE?
			return IL_FALSE;

		// The origin will always be in the upper left.
		Mipmap->Origin = IL_ORIGIN_UPPER_LEFT;
	}

	// After the image data is a short telling the number of 3-byte palette entries.
	PalSize = GetLittleUShort();
	Image->Pal.Palette = (ILubyte*)ialloc(PalSize * 3);
	if (Image->Pal.Palette == NULL)
		return IL_FALSE;
	Image->Pal.PalSize = PalSize * 3;
	Image->Pal.PalType = IL_PAL_RGB24;
	// Immediately following is the palette.
	if (iread(Image->Pal.Palette, Image->Pal.PalSize, 1) != 1)
		return IL_FALSE;

	// Copy the palette to each of the mipmaps.
	Mipmap = Image;
	for (int i = 0; i < 3; i++) {
		if (!iCopyPalette(&Mipmap->Mipmaps->Pal, &Mipmap->Pal))
			return IL_FALSE;
		Mipmap = Mipmap->Mipmaps;
	}

	return true;
}

#endif//IL_NO_WAD

