//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 03/13/2009
//
// Filename: src-IL/src/il_raw.c
//
// Description: "Raw" file functions
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_RAW


ILimage		*iLoadRawInternal(void);
ILboolean	iSaveRawInternal(void);


//! Reads a raw file
ILimage *ilLoadRaw(ILconst_string FileName)
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

	Image = ilLoadRawF(RawFile);
	icloser(RawFile);

	return Image;
}


//! Reads an already-opened raw file
ILimage *ilLoadRawF(ILHANDLE File)
{
	ILuint	FirstPos;
	ILimage	*Image;

	iSetInputFile(File);
	FirstPos = itell();
	Image = iLoadRawInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return Image;
}


//! Reads from a raw memory "lump"
ILimage *ilLoadRawL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadRawInternal();
}


// Internal function to load a raw image
ILimage *iLoadRawInternal()
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
ILboolean ilSaveRaw(const ILstring FileName)
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

	RawSize = ilSaveRawF(RawFile);
	iclosew(RawFile);

	if (RawSize == 0)
		return IL_FALSE;
	return IL_TRUE;
}


//! Writes Raw to an already-opened file
ILuint ilSaveRawF(ILHANDLE File)
{
	ILuint Pos;
	iSetOutputFile(File);
	Pos = itellw();
	if (iSaveRawInternal() == IL_FALSE)
		return 0;  // Error occurred
	return itellw() - Pos;  // Return the number of bytes written.
}


//! Writes Raw to a memory "lump"
ILuint ilSaveRawL(void *Lump, ILuint Size)
{
	ILuint Pos;
	iSetOutputLump(Lump, Size);
	Pos = itellw();
	if (iSaveRawInternal() == IL_FALSE)
		return 0;  // Error occurred
	return itellw() - Pos;  // Return the number of bytes written.
}


// Internal function used to load the raw data.
ILboolean iSaveRawInternal()
{
	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	SaveLittleUInt(iCurImage->Width);
	SaveLittleUInt(iCurImage->Height);
	SaveLittleUInt(iCurImage->Depth);
	iputc(iCurImage->Bpp);
	iputc(iCurImage->Bpc);
	iwrite(iCurImage->Data, 1, iCurImage->SizeOfData);

	return IL_TRUE;
}


#endif//IL_NO_RAW
