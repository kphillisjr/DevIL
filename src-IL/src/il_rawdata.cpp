//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 05/03/2009
//
// Filename: src-IL/src/rawdata.cpp
//
// Description: "Raw" file functions
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
//#ifndef IL_NO_DATA
#include "il_manip.h"


ILimage *iLoadDataInternal(ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp, ILstate *State);


//! Reads a raw data file
ILimage* ILAPIENTRY ilLoadData(ILconst_string FileName, ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp, ILstate *State)
{
	ILHANDLE	RawFile;
	ILimage		*Image;

	// No need to check for raw data
	/*if (!iCheckExtension(FileName, "raw")) {
		ilSetError(IL_INVALID_EXTENSION);
		return bRaw;
	}*/

	RawFile = iopenr(FileName);
	if (RawFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return NULL;
	}

	Image = ilLoadDataF(RawFile, Width, Height, Depth, Bpp, State);
	icloser(RawFile);

	return Image;
}


//! Reads an already-opened raw data file
ILimage* ILAPIENTRY ilLoadDataF(ILHANDLE File, ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp, ILstate *State)
{
	ILuint	FirstPos;
	ILimage	*Image;

	iSetInputFile(File);
	FirstPos = itell();
	Image = iLoadDataInternal(Width, Height, Depth, Bpp, State);
	iseek(FirstPos, IL_SEEK_SET);

	return Image;
}


//! Reads from a raw data memory "lump"
ILimage* ILAPIENTRY ilLoadDataL(void *Lump, ILuint Size, ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp, ILstate *State)
{
	iSetInputLump(Lump, Size);
	return iLoadDataInternal(Width, Height, Depth, Bpp, State);
}


// Internal function to load a raw data image
ILimage *iLoadDataInternal(ILuint Width, ILuint Height, ILuint Depth, ILubyte Bpp, ILstate *State)
{
	ILimage *Image;

	if (((Bpp != 1) && (Bpp != 3) && (Bpp != 4)) || Width == 0 || Height == 0 || Depth == 0) {
		ilSetError(IL_INVALID_PARAM);
		return NULL;
	}

	Image = ilNewImage(Width, Height, Depth, ilGetFormatBpp(Bpp), IL_UNSIGNED_BYTE, NULL, State);
	if (Image == NULL)
		return NULL;
	Image->Origin = IL_ORIGIN_UPPER_LEFT;

	// Tries to read the correct amount of data
	if (iread(Image->Data, Width * Height * Depth * Bpp, 1) != 1)
		return NULL;

	if (Image->Bpp == 1)
		Image->Format = IL_LUMINANCE;
	else if (Image->Bpp == 3)
		Image->Format = IL_RGB;
	else  // 4
		Image->Format = IL_RGBA;

	if (!ilFixImage(Image, State)) {
		ilCloseImage(Image);
		return NULL;
	}
	return Image;
}


//! Save the current image to FileName as raw data
ILboolean ILAPIENTRY ilSaveData(ILimage *Image, ILconst_string FileName)
{
	ILHANDLE DataFile;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	DataFile = iopenr(FileName);
	if (DataFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return IL_FALSE;
	}

	iwrite(Image->Data, 1, Image->SizeOfData);
	icloser(DataFile);

	return IL_TRUE;
}


//#endif//IL_NO_DATA
