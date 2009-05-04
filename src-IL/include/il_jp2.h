//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 05/02/2009
//
// Filename: src-IL/src/il_jp2.h
//
// Description: Jpeg-2000 (.jp2) functions
//
//-----------------------------------------------------------------------------

#ifndef JP2_H
#define JP2_H

#include "il_internal.h"

ILboolean		iLoadJp2Internal(jas_stream_t *Stream, ILimage *Image, ILstate *State);
ILboolean		iSaveJp2Internal(ILimage *Image, ILstate *State);
jas_stream_t	*iJp2ReadStream();

#endif//JP2_H
