//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 04/05/2009
//
// Filename: src-IL/src/il_raw.cpp
//
// Description: "Raw" file functions
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_RAW


ILboolean	iLoadRawInternal(ILimage *Image);
ILboolean	iSaveRawInternal(ILimage *Image);


//! Reads a raw file
ILboolean ilLoadRaw(ILimage *Image, ILconst_string FileName)
{
	ILHANDLE	RawFile;
	ILimage		*Image;

	// No need to check for raw
	/*if (!iCheckExtension(FileName, "raw")) {
		ilSetError(IL_INVALID_EXTENSION);
		return bRaw;
	}*/

	RawFile = iopenr(FileName);
	if (RawFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return NULL;
	}

	Image = ilLoadRawF(Image, RawFile);
	icloser(RawFile);

	return Image;
}


//! Reads an already-opened raw file
ILboolean ilLoadRawF(ILimage *Image, ILHANDLE File)
{
	ILuint	FirstPos;
	ILimage	*Image;

	iSetInputFile(File);
	FirstPos = itell();
	Image = iLoadRawInternal(Image);
	iseek(FirstPos, IL_SEEK_SET);

	return Image;
}


//! Reads from a raw memory "lump"
ILboolean ilLoadRawL(ILimage *Image, const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadRawInternal(Image);
}


// Internal function to load a raw image
ILimage *iLoadRawInternal(ILimage *Image)
{
	ILimage	*Image;
	ILuint	Width, Height, Depth, Bpp, Bpc;

	Width = GetLittleUInt();
	Height = GetLittleUInt();
	Depth = GetLittleUInt();
	Bpp = igetc();
	Bpc = igetc();

	Image = ilNewImageFull(Width, Height, Depth, Bpp, 0, ilGetTypeBpc(iCurImage->Bpc), NULL);
	if (Image == NULL)
		return NULL;
	Image->Origin = IL_ORIGIN_LOWER_LEFT;

	// Tries to read the correct amount of data
	if (iread(Image->Data, 1, Image->SizeOfData) != Image->SizeOfData)
		return IL_FALSE;

	if (ilIsEnabled(IL_ORIGIN_SET)) {
		Image->Origin = ilGetInteger(IL_ORIGIN_MODE);
	}
	else {
		Image->Origin = IL_ORIGIN_UPPER_LEFT;
	}

	if (Image->Bpp == 1)
		Image->Format = IL_LUMINANCE;
	else if (Image->Bpp == 3)
		Image->Format = IL_RGB;
	else  // 4
		Image->Format = IL_RGBA;

	if (!ilFixImage(Image)) {
		ilCloseImage(Image);
		return NULL;
	}
	return Image;
}


//! Writes a Raw file
ILboolean ilSaveRaw(ILimage *Image, const ILstring FileName)
{
	ILHANDLE	RawFile;
	ILuint		RawSize;

	if (ilGetBoolean(IL_FILE_MODE) == IL_FALSE) {
		if (iFileExists(FileName)) {
			ilSetError(IL_FILE_ALREADY_EXISTS);
			return IL_FALSE;
		}
	}

	RawFile = iopenw(FileName);
	if (RawFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return IL_FALSE;
	}

	RawSize = ilSaveRawF(Image, RawFile);
	iclosew(RawFile);

	if (RawSize == 0)
		return IL_FALSE;
	return IL_TRUE;
}


//! Writes Raw to an already-opened file
ILuint ilSaveRawF(ILimage *Image, ILHANDLE File)
{
	ILuint Pos;
	iSetOutputFile(File);
	Pos = itellw();
	if (iSaveRawInternal(Image) == IL_FALSE)
		return 0;  // Error occurred
	return itellw() - Pos;  // Return the number of bytes written.
}


//! Writes Raw to a memory "lump"
ILuint ilSaveRawL(ILimage *Image, void *Lump, ILuint Size)
{
	ILuint Pos;
	iSetOutputLump(Lump, Size);
	Pos = itellw();
	if (iSaveRawInternal(Image) == IL_FALSE)
		return 0;  // Error occurred
	return itellw() - Pos;  // Return the number of bytes written.
}


// Internal function used to load the raw data.
ILboolean iSaveRawInternal(ILimage *Image)
{
	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	SaveLittleUInt(Image->Width);
	SaveLittleUInt(Image->Height);
	SaveLittleUInt(Image->Depth);
	iputc(Image->Bpp);
	iputc(Image->Bpc);
	iwrite(Image->Data, 1, Image->SizeOfData);

	return IL_TRUE;
}


#endif//IL_NO_RAW
