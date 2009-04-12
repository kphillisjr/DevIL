//-----------------------------------------------------------------------------
//
// ImageLib Utility Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 03/14/2009
//
// Filename: src-ILU/src/ilu_utilities.c
//
// Description: Utility functions
//
//-----------------------------------------------------------------------------


#include "ilu_internal.h"


//! Retrieves information about the current bound image.
void ILAPIENTRY iluGetImageInfo(ILimage *Image, ILinfo *Info)
{
	if (Image == NULL || Info == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return;
	}

	Info->Data			= ilGetData(Image);
	Info->Width			= Image->Width;
	Info->Height		= Image->Height;
	Info->Depth			= Image->Depth;
	Info->Bpp			= Image->Bpp;
	Info->SizeOfData	= Image->SizeOfData;
	Info->Format		= Image->Format;
	Info->Type			= Image->Type;
	Info->Origin		= Image->Origin;
	Info->Palette		= Image->Pal.Palette;
	Info->PalType		= Image->Pal.PalType;
	Info->PalSize		= Image->Pal.PalSize;
	iGetIntegervImage(Image, IL_NUM_IMAGES,             
	                        (ILint*)&Info->NumNext);
	iGetIntegervImage(Image, IL_NUM_MIPMAPS, 
	                        (ILint*)&Info->NumMips);
	iGetIntegervImage(Image, IL_NUM_LAYERS, 
	                        (ILint*)&Info->NumLayers);
	
	return;
}