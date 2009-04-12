//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 03/31/2009
//
// Filename: src-IL/src/il_mdl.cpp
//
// Description: Reads a Half-Life model file (.mdl).
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_MDL
#include "il_mdl.h"


ILboolean iLoadMdlInternal(ILimage *Image);
ILboolean iIsValidMdl(void);

//! Checks if the file specified in FileName is a valid MDL file.
ILboolean ilIsValidMdl(ILconst_string FileName)
{
	ILHANDLE	MdlFile;
	ILboolean	bMdl = IL_FALSE;
	
	if (!iCheckExtension(FileName, IL_TEXT("mdl"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bMdl;
	}
	
	MdlFile = iopenr(FileName);
	if (MdlFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bMdl;
	}
	
	bMdl = ilIsValidMdlF(MdlFile);
	icloser(MdlFile);
	
	return bMdl;
}


//! Checks if the ILHANDLE contains a valid MDL file at the current position.
ILboolean ilIsValidMdlF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidMdl();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Checks if Lump is a valid MDL lump.
ILboolean ilIsValidMdlL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidMdl();
}


// Internal function to get the header and check it.
ILboolean iIsValidMdl(void)
{
	ILuint Id, Version;

	Id = GetLittleUInt();
	Version = GetLittleUInt();
	iseek(-8, IL_SEEK_CUR);  // Restore to previous position.

	// 0x54534449 == "IDST"
	if (Id != 0x54534449 || Version != 10)
		return IL_FALSE;
	return IL_TRUE;
}


//! Reads a .mdl file
ILboolean ilLoadMdl(ILimage *Image, ILconst_string FileName)
{
	ILHANDLE	MdlFile;
	ILboolean	bMdl = IL_FALSE;

	MdlFile = iopenr(FileName);
	if (MdlFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bMdl;
	}

	bMdl = ilLoadMdlF(Image, MdlFile);
	icloser(MdlFile);

	return bMdl;
}


//! Reads an already-opened .mdl file
ILboolean ilLoadMdlF(ILimage *Image, ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadMdlInternal(Image);
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a .mdl
ILboolean ilLoadMdlL(ILimage *Image, const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadMdlInternal(Image);
}


ILboolean iLoadMdlInternal(ILimage *Image)
{
	ILuint		Id, Version, NumTex, TexOff, TexDataOff, Position, ImageNum;
	ILubyte		*TempPal;
	TEX_HEAD	TexHead;
	ILimage		*BaseImage=NULL;
	ILboolean	BaseCreated = IL_FALSE;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Id = GetLittleUInt();
	Version = GetLittleUInt();

	// 0x54534449 == "IDST"
	if (Id != 0x54534449 || Version != 10) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	// Skips the actual model header.
	iseek(172, IL_SEEK_CUR);

	NumTex = GetLittleUInt();
	TexOff = GetLittleUInt();
	TexDataOff = GetLittleUInt();

	if (NumTex == 0 || TexOff == 0 || TexDataOff == 0) {
		ilSetError(IL_ILLEGAL_FILE_VALUE);
		return IL_FALSE;
	}

	iseek(TexOff, IL_SEEK_SET);

	for (ImageNum = 0; ImageNum < NumTex; ImageNum++) {
		if (iread(TexHead.Name, 1, 64) != 64)
			return IL_FALSE;
		TexHead.Flags = GetLittleUInt();
		TexHead.Width = GetLittleUInt();
		TexHead.Height = GetLittleUInt();
		TexHead.Offset = GetLittleUInt();
		Position = itell();

		if (TexHead.Offset == 0) {
			ilSetError(IL_ILLEGAL_FILE_VALUE);
			return IL_FALSE;
		}

		if (!BaseCreated) {
			ilTexImage(Image, TexHead.Width, TexHead.Height, 1, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE, NULL);
			Image->Origin = IL_ORIGIN_UPPER_LEFT;
			BaseCreated = IL_TRUE;
			BaseImage = Image;
			//Image->NumNext = NumTex - 1;  // Don't count the first image.
		}
		else {
			Image->Next = ilNewImage(TexHead.Width, TexHead.Height, 1, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE, NULL);
			Image = Image->Next;
			Image->Origin = IL_ORIGIN_UPPER_LEFT;
			Image->Format = IL_COLOUR_INDEX;
			Image->Type = IL_UNSIGNED_BYTE;
		}

		TempPal	= (ILubyte*)ialloc(768);
		if (TempPal == NULL) {
			Image = BaseImage;
			return IL_FALSE;
		}
		Image->Pal.Palette = TempPal;
		Image->Pal.PalSize = 768;
		Image->Pal.PalType = IL_PAL_RGB24;

		iseek(TexHead.Offset, IL_SEEK_SET);
		if (iread(Image->Data, TexHead.Width * TexHead.Height, 1) != 1)
			return IL_FALSE;
		if (iread(Image->Pal.Palette, 1, 768) != 768)
			return IL_FALSE;

		/*if (ilGetBoolean(IL_CONV_PAL) == IL_TRUE) {
			ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		}*/

		iseek(Position, IL_SEEK_SET);
	}

	Image = BaseImage;

	return ilFixImage(Image);
}

#endif//IL_NO_MDL