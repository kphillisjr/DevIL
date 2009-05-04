//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 05/02/2009
//
// Filename: src-IL/include/il_psd.h
//
// Description: Reads from a PhotoShop (.psd) file.
//
//-----------------------------------------------------------------------------


#ifndef PSD_H
#define PSD_H

#include "il_internal.h"

#ifdef _MSC_VER
#pragma pack(push, packed_struct, 1)
#endif
typedef struct PSDHEAD
{
	ILubyte		Signature[4];
	ILushort	Version;
	ILubyte		Reserved[6];
	ILushort	Channels;
	ILuint		Height;
	ILuint		Width;
	ILushort	Depth;
	ILushort	Mode;
} IL_PACKSTRUCT PSDHEAD;

#ifdef _MSC_VER
#pragma pack(pop,  packed_struct)
#endif

ILushort	ChannelNum;

ILboolean	iIsValidPsd(void);
ILboolean	iCheckPsd(PSDHEAD *Header);
ILboolean	iLoadPsdInternal(ILimage *Image, ILstate *State);
ILboolean	ReadPsd(ILimage *Image, PSDHEAD *Head, ILstate *State);
ILboolean	ReadGrey(ILimage *Image, PSDHEAD *Head, ILstate *State);
ILboolean	ReadIndexed(ILimage *Image, PSDHEAD *Head, ILstate *State);
ILboolean	ReadRGB(ILimage *Image, PSDHEAD *Head, ILstate *State);
ILboolean	ReadCMYK(ILimage *Image, PSDHEAD *Head, ILstate *State);
ILuint		*GetCompChanLen(PSDHEAD *Head);
ILboolean	PsdGetData(ILimage *Image, PSDHEAD *Head, void *Buffer, ILboolean Compressed);
ILboolean	ParseResources(ILimage *Image, ILuint ResourceSize, ILubyte *Resources);
ILboolean	GetSingleChannel(ILimage *Image, PSDHEAD *Head, ILubyte *Buffer, ILboolean Compressed);
ILboolean	iSavePsdInternal(ILimage *Image, ILstate *State);



#endif//PSD_H
