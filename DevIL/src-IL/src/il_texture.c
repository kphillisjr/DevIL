//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 02/16/2009
//
// Filename: src-IL/src/il_texture.c
//
// Description: Reads from a Medieval II: Total War	(by Creative Assembly)
//				Texture (.texture) file.
//
//-----------------------------------------------------------------------------

#include "il_internal.h"
#ifndef IL_NO_TEXTURE


//! Reads a TEXTURE file
ILboolean ilLoad_TEXTURE(ILconst_string FileName)
{
	ILHANDLE	TextureFile;
	ILboolean	bTexture = IL_FALSE;
	
	TextureFile = iopenr(FileName);
	if (TextureFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bTexture;
	}

	bTexture = ilLoadF_TEXTURE(TextureFile);
	icloser(TextureFile);

	return bTexture;
}


//! Reads an already-opened TEXTURE file
ILboolean ilLoadF_TEXTURE(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	// From http://forums.totalwar.org/vb/showthread.php?t=70886, all that needs to be done
	//  is to strip out the first 48 bytes, and then it is DDS data.
	iseek(48, IL_SEEK_CUR);
	bRet = ilLoadF_DDS(File);
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Reads from a memory "lump" that contains a TEXTURE
ILboolean ilLoadL_TEXTURE(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	// From http://forums.totalwar.org/vb/showthread.php?t=70886, all that needs to be done
	//  is to strip out the first 48 bytes, and then it is DDS data.
	iseek(48, IL_SEEK_CUR);
	return ilLoadL_DDS(Lump, Size);
}

#endif//IL_NO_TEXTURE

