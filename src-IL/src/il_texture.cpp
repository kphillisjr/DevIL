//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 04/24/2009
//
// Filename: src-IL/src/il_texture.cpp
//
// Description: Reads from a Medieval II: Total War	(by Creative Assembly)
//				Texture (.texture) file.
//
//-----------------------------------------------------------------------------

#include "il_internal.h"
#ifndef IL_NO_TEXTURE


//! Reads a TEXTURE file
ILboolean ilLoadTexture(ILimage *Image, ILconst_string FileName, ILstate *State)
{
	ILHANDLE	TextureFile;
	ILboolean	bTexture = IL_FALSE;

	CheckState();
	TextureFile = iopenr(FileName);
	if (TextureFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bTexture;
	}

	bTexture = ilLoadTextureF(Image, TextureFile, State);
	icloser(TextureFile);

	return bTexture;
}


//! Reads an already-opened TEXTURE file
ILboolean ilLoadTextureF(ILimage *Image, ILHANDLE File, ILstate *State)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	CheckState();
	iSetInputFile(File);
	FirstPos = itell();
	// From http://forums.totalwar.org/vb/showthread.php?t=70886, all that needs to be done
	//  is to strip out the first 48 bytes, and then it is DDS data.
	iseek(48, IL_SEEK_CUR);
	bRet = ilLoadDdsF(Image, File, State);
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Reads from a memory "lump" that contains a TEXTURE
ILboolean ilLoadTextureL(ILimage *Image, const void *Lump, ILuint Size, ILstate *State)
{
	CheckState();
	iSetInputLump(Lump, Size);
	// From http://forums.totalwar.org/vb/showthread.php?t=70886, all that needs to be done
	//  is to strip out the first 48 bytes, and then it is DDS data.
	iseek(48, IL_SEEK_CUR);
	return ilLoadDdsL(Image, Lump, Size, State);
}

#endif//IL_NO_TEXTURE

