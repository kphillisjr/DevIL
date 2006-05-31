#include "il_internal.h"
#include "il_endian.h"

INLINE ILvoid iSwapUShort(ILushort *s)  {
	#ifdef USE_WIN32_ASM
		__asm {
			mov ebx, s
			mov al, [ebx+1]
			mov ah, [ebx  ]
			mov [ebx], ax
		}
	#else
	#ifdef GCC_x86_ROR
		asm("ror $8,%0"
			: 
			: "g"  (*s) )
	#else
		*s = ((*s)>>8) | ((*s)<<8);
	#endif
	#endif
}

INLINE ILvoid iSwapShort(ILshort *s) {
	iSwapUShort((ILushort*)s);
}

INLINE ILvoid iSwapUInt(ILuint *i) {
	#ifdef USE_WIN32_ASM
		__asm {
			mov ebx, i
			mov eax, [ebx]
			bswap eax
			mov [ebx], eax
		}
	#else
	#ifdef GCC_x86_BSWAP
			asm("bswap  %0;"
	    		: "=r" (*i) 
	    		: "r"  (*i) );
	#else
		*i = ((*i)>>24) | (((*i)>>8) & 0xff00) | (((*i)<<8) & 0xff0000) | ((*i)<<24);
	#endif
	#endif//USE_WIN32_ASM
}

INLINE ILvoid iSwapInt(ILint *i) {
	iSwapUInt((ILuint*)i);
}

INLINE ILvoid iSwapFloat(ILfloat *f) {
	iSwapUInt((ILuint*)f);
}



INLINE ILvoid iSwapDouble(ILdouble *d) {
	ILubyte t,*b = (ILubyte*)d;
	#define dswap(x,y) t=b[x];b[x]=b[y];b[y]=b[x];
	dswap(0,7);
	dswap(1,6);
	dswap(2,5);
	dswap(3,4);
	#undef dswap
}

INLINE ILushort GetLittleUShort() {
	ILushort s;
	iread(&s, sizeof(ILushort), 1);
#ifdef __BIG_ENDIAN__
	iSwapUShort(&s);
#endif
	return s;
}


INLINE ILshort GetLittleShort() {
	ILshort s;
	iread(&s, sizeof(ILshort), 1);
#ifdef __BIG_ENDIAN__
	iSwapShort(&s);
#endif
	return s;
}


INLINE ILuint GetLittleUInt() {
	ILuint i;
	iread(&i, sizeof(ILuint), 1);
#ifdef __BIG_ENDIAN__
	iSwapUInt(&i);
#endif
	return i;
}


INLINE ILint GetLittleInt() {
	ILint i;
	iread(&i, sizeof(ILint), 1);
#ifdef __BIG_ENDIAN__
	iSwapInt(&i);
#endif
	return i;
}


INLINE ILfloat GetLittleFloat() {
	ILfloat f;
	iread(&f, sizeof(ILfloat), 1);
#ifdef __BIG_ENDIAN__
	iSwapFloat(&f);
#endif
	return f;
}


INLINE ILdouble GetLittleDouble() {
	ILdouble d;
	iread(&d, sizeof(ILdouble), 1);
#ifdef __BIG_ENDIAN__
	iSwapDouble(&d);
#endif
	return d;
}


INLINE ILushort GetBigUShort() {
	ILushort s;
	iread(&s, sizeof(ILushort), 1);
#ifdef __LITTLE_ENDIAN__
	iSwapUShort(&s);
#endif
	return s;
}


INLINE ILshort GetBigShort() {
	ILshort s;
	iread(&s, sizeof(ILshort), 1);
#ifdef __LITTLE_ENDIAN__
	iSwapShort(&s);
#endif
	return s;
}


INLINE ILuint GetBigUInt() {
	ILuint i;
	iread(&i, sizeof(ILuint), 1);
#ifdef __LITTLE_ENDIAN__
	iSwapUInt(&i);
#endif
	return i;
}


INLINE ILint GetBigInt() {
	ILint i;
	iread(&i, sizeof(ILint), 1);
#ifdef __LITTLE_ENDIAN__
	iSwapInt(&i);
#endif
	return i;
}


INLINE ILfloat GetBigFloat() {
	ILfloat f;
	iread(&f, sizeof(ILfloat), 1);
#ifdef __LITTLE_ENDIAN__
	iSwapFloat(&f);
#endif
	return f;
}


INLINE ILdouble GetBigDouble() {
	ILdouble d;
	iread(&d, sizeof(ILdouble), 1);
#ifdef __LITTLE_ENDIAN__
	iSwapDouble(&d);
#endif
	return d;
}


INLINE ILubyte SaveLittleUShort(ILushort s) {
#ifdef __BIG_ENDIAN__
	iSwapUShort(&s);
#endif
	return iwrite(&s, sizeof(ILushort), 1);
}


INLINE ILubyte SaveLittleShort(ILshort s) {
#ifdef __BIG_ENDIAN__
	iSwapShort(&s);
#endif
	return iwrite(&s, sizeof(ILshort), 1);
}


INLINE ILubyte SaveLittleUInt(ILuint i) {
#ifdef __BIG_ENDIAN__
	iSwapUInt(&i);
#endif
	return iwrite(&i, sizeof(ILuint), 1);
}


INLINE ILubyte SaveLittleInt(ILint i) {
#ifdef __BIG_ENDIAN__
	iSwapInt(&i);
#endif
	return iwrite(&i, sizeof(ILint), 1);
}


INLINE ILubyte SaveLittleFloat(ILfloat f) {
#ifdef __BIG_ENDIAN__
	iSwapFloat(&f);
#endif
	return iwrite(&f, sizeof(ILfloat), 1);
}


INLINE ILubyte SaveLittleDouble(ILdouble d) {
#ifdef __BIG_ENDIAN__
	iSwapDouble(&d);
#endif
	return iwrite(&d, sizeof(ILdouble), 1);
}


INLINE ILubyte SaveBigUShort(ILushort s) {
#ifdef __LITTLE_ENDIAN__
	iSwapUShort(&s);
#endif
	return iwrite(&s, sizeof(ILushort), 1);
}


INLINE ILubyte SaveBigShort(ILshort s) {
#ifdef __LITTLE_ENDIAN__
	iSwapShort(&s);
#endif
	return iwrite(&s, sizeof(ILshort), 1);
}


INLINE ILubyte SaveBigUInt(ILuint i) {
#ifdef __LITTLE_ENDIAN__
	iSwapUInt(&i);
#endif
	return iwrite(&i, sizeof(ILuint), 1);
}


INLINE ILubyte SaveBigInt(ILint i) {
#ifdef __LITTLE_ENDIAN__
	iSwapInt(&i);
#endif
	return iwrite(&i, sizeof(ILint), 1);
}


INLINE ILubyte SaveBigFloat(ILfloat f) {
#ifdef __LITTLE_ENDIAN__
	iSwapFloat(&f);
#endif
	return iwrite(&f, sizeof(ILfloat), 1);
}


INLINE ILubyte SaveBigDouble(ILdouble d) {
#ifdef __LITTLE_ENDIAN__
	iSwapDouble(&d);
#endif
	return iwrite(&d, sizeof(ILdouble), 1);
}


ILvoid EndianSwapData(void *_Image) {
	ILuint		i;
	ILubyte		*temp, *s, *d;
	ILushort	*ShortS, *ShortD;
	ILuint		*IntS, *IntD;
	ILfloat		*FltS, *FltD;
	ILdouble	*DblS, *DblD;

	ILimage *Image = (ILimage*)_Image;

	switch (Image->Type)
	{
		case IL_BYTE:
		case IL_UNSIGNED_BYTE:
			switch (Image->Bpp)
			{
				case 3:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					s = Image->Data;
					d = temp;

					for( i = Image->Width * Image->Height; i > 0; i-- ) {
						*d++ = *(s+2);
						*d++ = *(s+1);
						*d++ = *s;
						s += 3;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;

				case 4:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					s = Image->Data;
					d = temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						*d++ = *(s+3);
						*d++ = *(s+2);
						*d++ = *(s+1);
						*d++ = *s;
						s += 4;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;
			}
			break;

		case IL_SHORT:
		case IL_UNSIGNED_SHORT:
			switch (Image->Bpp)
			{
				case 3:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					ShortS = (ILushort*)Image->Data;
					ShortD = (ILushort*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						*ShortD++ = SwapShort(*(ShortS+2));
						*ShortD++ = SwapShort(*(ShortS+1));
						*ShortD++ = SwapShort(*ShortS);
						ShortS += 3;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;

				case 4:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					ShortS = (ILushort*)Image->Data;
					ShortD = (ILushort*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						*ShortD++ = SwapShort(*(ShortS+3));
						*ShortD++ = SwapShort(*(ShortS+2));
						*ShortD++ = SwapShort(*(ShortS+1));
						*ShortD++ = SwapShort(*ShortS);
						ShortS += 4;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;
			}
			break;

		case IL_INT:
		case IL_UNSIGNED_INT:
			switch (Image->Bpp)
			{
				case 3:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					IntS = (ILuint*)Image->Data;
					IntD = (ILuint*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						*IntD++ = SwapInt(*(IntS+2));
						*IntD++ = SwapInt(*(IntS+1));
						*IntD++ = SwapInt(*IntS);
						IntS += 3;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;

				case 4:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					IntS = (ILuint*)Image->Data;
					IntD = (ILuint*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						*IntD++ = SwapInt(*(IntS+3));
						*IntD++ = SwapInt(*(IntS+2));
						*IntD++ = SwapInt(*(IntS+1));
						*IntD++ = SwapInt(*IntS);
						IntS += 4;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;
			}
			break;

		case IL_FLOAT:
			switch (Image->Bpp)
			{
				case 3:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					FltS = (ILfloat*)Image->Data;
					FltD = (ILfloat*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						iSwapFloat(FltS);
						iSwapFloat(FltS+1);
						iSwapFloat(FltS+2);
						*FltD++ = *(FltS+2);
						*FltD++ = *(FltS+1);
						*FltD++ = *FltS;
						FltS += 3;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;

				case 4:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					FltS = (ILfloat*)Image->Data;
					FltD = (ILfloat*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						iSwapFloat(FltS);
						iSwapFloat(FltS+1);
						iSwapFloat(FltS+2);
						iSwapFloat(FltS+3);
						*FltD++ = *(FltS+3);
						*FltD++ = *(FltS+2);
						*FltD++ = *(FltS+1);
						*FltD++ = *FltS;
						FltS += 4;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;
			}
			break;

		case IL_DOUBLE:
			switch (Image->Bpp)
			{
				case 3:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					DblS = (ILdouble*)Image->Data;
					DblD = (ILdouble*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						iSwapDouble(DblS);
						iSwapDouble(DblS+1);
						iSwapDouble(DblS+2);
						*DblD++ = *(DblS+2);
						*DblD++ = *(DblS+1);
						*DblD++ = *DblS;
						DblS += 3;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;

				case 4:
					temp = ialloc(Image->SizeOfData);
					if (temp == NULL)
						return;
					DblS = (ILdouble*)Image->Data;
					DblD = (ILdouble*)temp;

					for (i = Image->Width * Image->Height; i > 0; i--) {
						iSwapDouble(DblS);
						iSwapDouble(DblS+1);
						iSwapDouble(DblS+2);
						iSwapDouble(DblS+3);
						*DblD++ = *(DblS+3);
						*DblD++ = *(DblS+2);
						*DblD++ = *(DblS+1);
						*DblD++ = *DblS;
						DblS += 4;
					}

					ifree(Image->Data);
					Image->Data = temp;
					break;
			}
			break;
	}

	if (iCurImage->Format == IL_COLOUR_INDEX) {
		switch (iCurImage->Pal.PalType)
		{
			case IL_PAL_RGB24:
			case IL_PAL_BGR24:
				temp = ialloc(Image->Pal.PalSize);
				if (temp == NULL)
					return;
				s = Image->Pal.Palette;
				d = temp;

				for (i = Image->Pal.PalSize / 3; i > 0; i--) {
					*d++ = *(s+2);
					*d++ = *(s+1);
					*d++ = *s;
					s += 3;
				}

				ifree(Image->Pal.Palette);
				Image->Pal.Palette = temp;
				break;

			case IL_PAL_RGBA32:
			case IL_PAL_RGB32:
			case IL_PAL_BGRA32:
			case IL_PAL_BGR32:
				temp = ialloc(Image->Pal.PalSize);
				if (temp == NULL)
					return;
				s = Image->Pal.Palette;
				d = temp;

				for (i = Image->Pal.PalSize / 4; i > 0; i--) {
					*d++ = *(s+3);
					*d++ = *(s+2);
					*d++ = *(s+1);
					*d++ = *s;
					s += 4;
				}

				ifree(Image->Pal.Palette);
				Image->Pal.Palette = temp;
				break;
		}
	}

	return;
}
