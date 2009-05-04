//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 05/02/2009
//
// Filename: src-IL/src/il_pxr.cpp
//
// Description: Reads from a Pixar (.pxr) file.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_PXR
#include "il_manip.h"
#include "il_endian.h"


#ifdef _MSC_VER
#pragma pack(push, pxr_struct, 1)
#endif
typedef struct PIXHEAD
{
	ILushort	Signature;
	ILubyte		Reserved1[413];
	ILushort	Height;
	ILushort	Width;
	ILubyte		Reserved2[4];
	ILubyte		BppInfo;
	ILubyte		Reserved3[598];
} IL_PACKSTRUCT PIXHEAD;
#ifdef _MSC_VER
#pragma pack(pop, pxr_struct)
#endif

ILboolean iLoadPxrInternal(ILimage *Image, ILstate *State);


//! Reads a Pxr file
ILboolean ilLoadPxr(ILimage *Image, ILconst_string FileName, ILstate *State)
{
	ILHANDLE	PxrFile;
	ILboolean	bPxr = IL_FALSE;

	PxrFile = iopenr(FileName);
	if (PxrFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bPxr;
	}

	bPxr = ilLoadPxrF(Image, PxrFile, State);
	icloser(PxrFile);

	return bPxr;
}


//! Reads an already-opened Pxr file
ILboolean ilLoadPxrF(ILimage *Image, ILHANDLE File, ILstate *State)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadPxrInternal(Image, State);
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a Pxr
ILboolean ilLoadPxrL(ILimage *Image, const void *Lump, ILuint Size, ILstate *State)
{
	iSetInputLump(Lump, Size);
	return iLoadPxrInternal(Image, State);
}


// Internal function used to load the Pxr.
ILboolean iLoadPxrInternal(ILimage *Image, ILstate *State)
{
	ILushort	Width, Height;
	ILubyte		Bpp;

	Width = sizeof(PIXHEAD);

	iseek(416, IL_SEEK_SET);
	Height = GetLittleUShort();
	Width = GetLittleUShort();
	iseek(424, IL_SEEK_SET);
	Bpp = (ILubyte)igetc();

	switch (Bpp)
	{
		case 0x08:
			ilTexImage(Image, Width, Height, 1, IL_LUMINANCE, IL_UNSIGNED_BYTE, NULL, State);
			break;
		case 0x0E:
			ilTexImage(Image, Width, Height, 1, IL_RGB, IL_UNSIGNED_BYTE, NULL, State);
			break;
		case 0x0F:
			ilTexImage(Image, Width, Height, 1, IL_RGBA, IL_UNSIGNED_BYTE, NULL, State);
			break;
		default:
			ilSetError(IL_INVALID_FILE_HEADER);
			return IL_FALSE;
	}

	iseek(1024, IL_SEEK_SET);
	iread(Image->Data, 1, Image->SizeOfData);
	Image->Origin = IL_ORIGIN_UPPER_LEFT;

	return IL_TRUE;
}


#endif//IL_NO_PXR
