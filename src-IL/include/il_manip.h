//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2001-2009 by Denton Woods
// Last modified: 03/13/2009
//
// Filename: src-IL/include/il_manip.h
//
// Description: Image manipulation
//
//-----------------------------------------------------------------------------

#ifndef MANIP_H
#define MANIP_H

/*#ifdef _cplusplus
extern "C" {
#endif*/

ILboolean ilFlipImage(ILimage *Image);
ILboolean ilMirrorImage(ILimage *Image);	//@JASON New routine created 03/28/2001

//-----------------------------------------------
// Overflow handler for float-to-half conversion;
// generates a hardware floating-point overflow,
// which may be trapped by the operating system.
//-----------------------------------------------

#if defined(_MSC_VER)
	#pragma warning(push)
	#pragma warning(disable : 4756)  // Disables 'named type definition in parentheses' warning
#endif
/*INLINE*/ ILfloat /*ILAPIENTRY*/ ilFloatToHalfOverflow();
#if defined(_MSC_VER)
	#pragma warning(pop)
#endif

/*INLINE*/ ILuint ILAPIENTRY ilHalfToFloat (ILushort y);
/*INLINE*/ ILushort ILAPIENTRY ilFloatToHalf(ILuint i);

/*#ifdef _cplusplus
}
#endif*/

#endif//MANIP_H
