//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 05/01/2009
//
// Filename: src-IL/src/il_ftx.cpp
//
// Description: Reads from a Heavy Metal: FAKK2 (.ftx) file.
//
//-----------------------------------------------------------------------------

#include "il_internal.h"
#ifndef IL_NO_FTX

ILboolean iLoadFtxInternal(ILimage *Image, ILstate *State);


//! Reads a FTX file
ILboolean ilLoadFtx(ILimage *Image, ILconst_string FileName, ILstate *State)
{
	ILHANDLE	FtxFile;
	ILboolean	bFtx = IL_FALSE;

	FtxFile = iopenr(FileName);
	if (FtxFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bFtx;
	}

	bFtx = ilLoadFtxF(Image, FtxFile, State);
	icloser(FtxFile);

	return bFtx;
}


//! Reads an already-opened FTX file
ILboolean ilLoadFtxF(ILimage *Image, ILHANDLE File, ILstate *State)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadFtxInternal(Image, State);
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Reads from a memory "lump" that contains a FTX
ILboolean ilLoadFtxL(ILimage *Image, const void *Lump, ILuint Size, ILstate *State)
{
	iSetInputLump(Lump, Size);
	return iLoadFtxInternal(Image, State);
}


// Internal function used to load the FTX.
ILboolean iLoadFtxInternal(ILimage *Image, ILstate *State)
{
	ILuint Width, Height, HasAlpha;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Width = GetLittleUInt();
	Height = GetLittleUInt();
	HasAlpha = GetLittleUInt();  // Kind of backwards from what I would think...

	//@TODO: Right now, it appears that all images are in RGBA format.  See if I can find specs otherwise
	//  or images that load incorrectly like this.
	//if (HasAlpha == 0) {  // RGBA format
		if (!ilTexImage(Image, Width, Height, 1, IL_RGBA, IL_UNSIGNED_BYTE, NULL, State))
			return IL_FALSE;
	//}
	//else if (HasAlpha == 1) {  // RGB format
	//	if (!ilTexImage(Width, Height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL))
	//		return IL_FALSE;
	//}
	//else {  // Unknown format
	//	ilSetError(IL_INVALID_FILE_HEADER);
	//	return IL_FALSE;
	//}

	// The origin will always be in the upper left.
	Image->Origin = IL_ORIGIN_UPPER_LEFT;

	// All we have to do for this format is read the raw, uncompressed data.
	if (iread(Image->Data, 1, Image->SizeOfData) != Image->SizeOfData)
		return IL_FALSE;

	return ilFixImage(Image);
}

#endif//IL_NO_FTX

