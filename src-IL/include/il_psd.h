//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2001 by Denton Woods
// Last modified: 01/23/2001 <--Y2K Compliant! =]
//
// Filename: src-IL/include/il_il_psd.c
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
ILboolean	iLoadPsdInternal(ILimage *Image);
ILboolean	ReadPsd(ILimage *Image, PSDHEAD *Head);
ILboolean	ReadGrey(ILimage *Image, PSDHEAD *Head);
ILboolean	ReadIndexed(ILimage *Image, PSDHEAD *Head);
ILboolean	ReadRGB(ILimage *Image, PSDHEAD *Head);
ILboolean	ReadCMYK(ILimage *Image, PSDHEAD *Head);
ILuint		*GetCompChanLen(PSDHEAD *Head);
ILboolean	PsdGetData(ILimage *Image, PSDHEAD *Head, void *Buffer, ILboolean Compressed);
ILboolean	ParseResources(ILimage *Image, ILuint ResourceSize, ILubyte *Resources);
ILboolean	GetSingleChannel(ILimage *Image, PSDHEAD *Head, ILubyte *Buffer, ILboolean Compressed);
ILboolean	iSavePsdInternal(ILimage *Image);



#endif//PSD_H
