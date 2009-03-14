//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 01/29/2009
//
// Filename: src-IL/include/il_endian.h
//
// Description: Handles Endian-ness
//
//-----------------------------------------------------------------------------

#ifndef IL_ENDIAN_H
#define IL_ENDIAN_H

#include "il_internal.h"

#ifdef WORDS_BIGENDIAN  // This is defined by ./configure.
	#ifndef __BIG_ENDIAN__
	#define __BIG_ENDIAN__ 1
	#endif
#endif

#if (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __BIG_ENDIAN__) \
  || (defined(__BIG_ENDIAN__) && !defined(__LITTLE_ENDIAN__))
 	#undef __LITTLE_ENDIAN__
	#define Short(s) iSwapShort(s)
	#define UShort(s) iSwapUShort(s)
	#define Int(i) iSwapInt(i)
	#define UInt(i) iSwapUInt(i)
	#define Float(f) iSwapFloat(f)
	#define Double(d) iSwapDouble(d)
 
	#define BigShort(s)  
	#define BigUShort(s)  
	#define BigInt(i)  
	#define BigUInt(i)  
	#define BigFloat(f)  
	#define BigDouble(d)  
#else
	#undef __BIG_ENDIAN__
	#undef __LITTLE_ENDIAN__  // Not sure if it's defined by any compiler...
	#define __LITTLE_ENDIAN__
	#define Short(s)  
	#define UShort(s)  
	#define Int(i)  
	#define UInt(i)  
	#define Float(f)  
	#define Double(d)  

	#define BigShort(s) iSwapShort(s)
	#define BigUShort(s) iSwapUShort(s)
	#define BigInt(i) iSwapInt(i)
	#define BigUInt(i) iSwapUInt(i)
	#define BigFloat(f) iSwapFloat(f)
	#define BigDouble(d) iSwapDouble(d)
#endif

void   iSwapUShort(ILushort *s);
void   iSwapShort(ILshort *s);
void   iSwapUInt(ILuint *i);
void   iSwapInt(ILint *i);
void   iSwapFloat(ILfloat *f);
void   iSwapDouble(ILdouble *d);
ILushort GetLittleUShort();
ILshort  GetLittleShort();
ILuint   GetLittleUInt();
ILint    GetLittleInt();
ILfloat  GetLittleFloat();
ILdouble GetLittleDouble();
ILushort GetBigUShort();
ILshort  GetBigShort();
ILuint   GetBigUInt();
ILint    GetBigInt();
ILfloat  GetBigFloat();
ILdouble GetBigDouble();
ILubyte SaveLittleUShort(ILushort s);
ILubyte SaveLittleShort(ILshort s);
ILubyte SaveLittleUInt(ILuint i);
ILubyte SaveLittleInt(ILint i);
ILubyte SaveLittleFloat(ILfloat f);
ILubyte SaveLittleDouble(ILdouble d);
ILubyte SaveBigUShort(ILushort s);
ILubyte SaveBigShort(ILshort s);
ILubyte SaveBigUInt(ILuint i);
ILubyte SaveBigInt(ILint i);
ILubyte SaveBigFloat(ILfloat f);
ILubyte SaveBigDouble(ILdouble d);

#ifdef IL_ENDIAN_C
#undef NOINLINE
#undef INLINE
#define INLINE
#endif

//#ifndef NOINLINE
INLINE void iSwapUShort(ILushort *s);
INLINE void iSwapShort(ILshort *s);
INLINE void iSwapUInt(ILuint *i);
INLINE void iSwapInt(ILint *i);
INLINE void iSwapFloat(ILfloat *f);
INLINE void iSwapDouble(ILdouble *d);
INLINE ILushort GetLittleUShort();
INLINE ILshort GetLittleShort();
INLINE ILuint GetLittleUInt();
INLINE ILint GetLittleInt();
INLINE ILfloat GetLittleFloat();
INLINE ILdouble GetLittleDouble();
INLINE ILushort GetBigUShort();
INLINE ILshort GetBigShort();
INLINE ILuint GetBigUInt();
INLINE ILint GetBigInt();
INLINE ILfloat GetBigFloat();
INLINE ILdouble GetBigDouble();
INLINE ILubyte SaveLittleUShort(ILushort s);
INLINE ILubyte SaveLittleShort(ILshort s);
INLINE ILubyte SaveLittleUInt(ILuint i);
INLINE ILubyte SaveLittleInt(ILint i);
INLINE ILubyte SaveLittleFloat(ILfloat f);
INLINE ILubyte SaveLittleDouble(ILdouble d);
INLINE ILubyte SaveBigUShort(ILushort s);
INLINE ILubyte SaveBigShort(ILshort s);
INLINE ILubyte SaveBigUInt(ILuint i);
INLINE ILubyte SaveBigInt(ILint i);
INLINE ILubyte SaveBigFloat(ILfloat f);
INLINE ILubyte SaveBigDouble(ILdouble d);
//#endif//NOINLINE

void		EndianSwapData(void *_Image);

#endif//ENDIAN_H
