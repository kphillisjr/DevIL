//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 05/12/2001 <--Y2K Compliant! =]
//
// Filename: il/il_bmp.c
//
// Description: Reads from and writes to a bitmap (.bmp) file.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_BMP
#include "il_bmp.h"
#include "il_manip.h"
#include "il_endian.h"

#define	IL_USE_POINTER 1

//! Checks if the file specified in FileName is a valid .bmp file.
ILboolean ilIsValidBmp(const ILstring FileName)
{
	ILHANDLE	BitmapFile;
	ILboolean	bBitmap = IL_FALSE;

	if (!iCheckExtension(FileName, IL_TEXT("bmp"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bBitmap;
	}

	BitmapFile = iopenr(FileName);
	if (BitmapFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bBitmap;
	}

	bBitmap = ilIsValidBmpF(BitmapFile);
	icloser(BitmapFile);

	return bBitmap;
}


//! Checks if the ILHANDLE contains a valid .bmp file at the current position.
ILboolean ilIsValidBmpF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidBmp();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Checks if Lump is a valid .bmp lump.
ILboolean ilIsValidBmpL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidBmp();
}


// Internal function used to get the .bmp header from the current file.
ILboolean iGetBmpHead(BMPHEAD *Header)
{
	if (iread(Header, sizeof(BMPHEAD), 1) != 1)
		return IL_FALSE;

	Header->bfType			= UShort(Header->bfType);
	Header->bfSize			= Int(Header->bfSize);
	Header->bfReserved		= UInt(Header->bfReserved);
	Header->bfDataOff		= Int(Header->bfDataOff);
	Header->biSize			= Int(Header->biSize);
	Header->biWidth			= Int(Header->biWidth);
	Header->biHeight		= Int(Header->biHeight);
	Header->biPlanes		= Short(Header->biPlanes);
	Header->biBitCount		= Short(Header->biBitCount);
	Header->biCompression	= Int(Header->biCompression);
	Header->biSizeImage		= Int(Header->biSizeImage);
	Header->biXPelsPerMeter	= Int(Header->biXPelsPerMeter);
	Header->biYPelsPerMeter	= Int(Header->biYPelsPerMeter);
	Header->biClrUsed		= Int(Header->biClrUsed);
	Header->biClrImportant	= Int(Header->biClrImportant);

	return IL_TRUE;
}


ILboolean iGetOS2Head(OS2_HEAD *Header)
{
	if (iread(Header, sizeof(OS2_HEAD), 1) != 1)
		return IL_FALSE;

	Header->bfType		= UShort(Header->bfType);
	Header->biSize		= UInt(Header->biSize);
	Header->xHotspot	= Short(Header->xHotspot);
	Header->yHotspot	= Short(Header->yHotspot);
	Header->DataOff		= UInt(Header->DataOff);
	Header->cbFix		= UInt(Header->cbFix);
	Header->cx			= UShort(Header->cx);
	Header->cy			= UShort(Header->cy);
	Header->cPlanes		= UShort(Header->cPlanes);
	Header->cBitCount	= UShort(Header->cBitCount);

	iseek((ILint)Header->cbFix - 12, IL_SEEK_CUR);  // Skip rest of header, if any.

	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidBmp()
{
	BMPHEAD		Head;
	OS2_HEAD	Os2Head;
	ILboolean	IsValid;

	iGetBmpHead(&Head);
	iseek(-(ILint)sizeof(BMPHEAD), IL_SEEK_CUR);  // Go ahead and restore to previous state

	IsValid = iCheckBmp(&Head);
	if (!IsValid) {
		iGetOS2Head(&Os2Head);
		iseek(-(ILint)sizeof(BMPHEAD), IL_SEEK_CUR);
		IsValid = iCheckOS2(&Os2Head);
	}

	return IsValid;
}


// Internal function used to check if the HEADER is a valid .bmp header.
ILboolean iCheckBmp(BMPHEAD *Header)
{
	//if ((Header->bfType != ('B'|('M'<<8))) || ((Header->biSize != 0x28) && (Header->biSize != 0x0C)))
	if ((Header->bfType != ('B'|('M'<<8))) || (Header->biSize != 0x28))
		return IL_FALSE;
	if (Header->biHeight == 0 || Header->biWidth < 1)
		return IL_FALSE;
	if (Header->biPlanes > 1)  // Not sure...
		return IL_FALSE;
	if (Header->biBitCount != 8 && /*Header->biBitCount != 16 &&*/ Header->biBitCount != 24 &&
		Header->biBitCount != 32 && Header->biBitCount != 4 && Header->biBitCount != 1)
		return IL_FALSE;

	return IL_TRUE;
}


ILboolean iCheckOS2(OS2_HEAD *Header)
{
	if ((Header->bfType != ('B'|('M'<<8))) || (Header->DataOff < 26) || (Header->cbFix < 12))
		return IL_FALSE;
	if (Header->cPlanes != 1)
		return IL_FALSE;
	if (Header->cx == 0 || Header->cy == 0)
		return IL_FALSE;
	if (Header->cBitCount != 1 && Header->cBitCount != 4 && Header->cBitCount != 8 &&
		 Header->cBitCount != 24 && Header->cBitCount != 32)
		 return IL_FALSE;

	return IL_TRUE;
}


//! Reads a .bmp file
ILboolean ilLoadBmp(const ILstring FileName)
{
	ILHANDLE	BitmapFile;
	ILboolean	bBitmap = IL_FALSE;

	BitmapFile = iopenr(FileName);
	if (BitmapFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bBitmap;
	}

	bBitmap = ilLoadBmpF(BitmapFile);
	icloser(BitmapFile);

	return bBitmap;
}


//! Reads an already-opened .bmp file
ILboolean ilLoadBmpF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadBitmapInternal();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a .bmp
ILboolean ilLoadBmpL(ILvoid *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadBitmapInternal();
}


// Internal function used to load the .bmp.
ILboolean iLoadBitmapInternal()
{
	BMPHEAD		Header;
	OS2_HEAD	Os2Head;
	ILboolean	bBitmap;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	iGetBmpHead(&Header);
	if (!iCheckBmp(&Header)) {
		iseek(-(ILint)sizeof(BMPHEAD), IL_SEEK_CUR);
		iGetOS2Head(&Os2Head);
		if (!iCheckOS2(&Os2Head)) {
			ilSetError(IL_INVALID_FILE_HEADER);
			return IL_FALSE;
		}
		else {
			return iGetOS2Bmp(&Os2Head);
		}
	}

	// Don't know what to do if it has more than one plane...
	if (Header.biPlanes != 1) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	switch (Header.biCompression)
	{
		case 0:  // No compression
			bBitmap = ilReadUncompBmp(&Header);
			break;
		case 1:  //	RLE 8-bit / pixel (BI_RLE4)
			bBitmap = ilReadRLE8Bmp(&Header);
			break;
		case 2:  // RLE 4-bit / pixel (BI_RLE8)
			bBitmap = ilReadRLE4Bmp(&Header);
			break;
		default:
			ilSetError(IL_INVALID_FILE_HEADER);
			return IL_FALSE;
	}

	ilFixImage();

	return bBitmap;
}


// Reads an uncompressed .bmp
//	One of the absolute ugliest functions I've ever written!
//	Note:  I'm not sure, but I don't think .bmp files can be 16 or 32 bpp...
ILboolean ilReadUncompBmp(BMPHEAD *Header)
{
	ILuint i, j, k, c, Read;
	ILubyte Bpp, ByteData, PadSize, Padding[4];

	if (Header->biBitCount < 8)
		Bpp = 1;  // We can't have an integral number less than one and greater than 0
	else
		Bpp = (ILubyte)(Header->biBitCount >> 3);  // Convert to bytes per pixel

	// Update the current image with the new dimensions
	if (!ilTexImage(Header->biWidth, abs(Header->biHeight), 1, Bpp, 0, IL_UNSIGNED_BYTE, NULL)) {
		return IL_FALSE;
	}
	iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	switch (Header->biBitCount)
	{
		case 1:
			//iCurImage->Format = IL_LUMINANCE;
			iCurImage->Format = IL_COLOUR_INDEX;
			iCurImage->Pal.PalType = IL_PAL_BGR32;
			iCurImage->Pal.PalSize = 2 * 4;
			iCurImage->Pal.Palette = (ILubyte*)ialloc(iCurImage->Pal.PalSize);
			if (iCurImage->Pal.Palette == NULL) {
				return IL_FALSE;
			}
			break;

		case 4:
		case 8:
			iCurImage->Format = IL_COLOUR_INDEX;
			iCurImage->Pal.PalType = IL_PAL_BGR32;
			//iCurImage->Pal.PalSize = Header->biClrUsed * 4;  // 256 * 4 for most images
			iCurImage->Pal.PalSize = 256 * 4;
			if (Header->biBitCount == 4)  // biClrUsed is 0 for 4-bit bitmaps
				iCurImage->Pal.PalSize = 16 * 4;
			iCurImage->Pal.Palette = (ILubyte*)ialloc(iCurImage->Pal.PalSize);
			if (iCurImage->Pal.Palette == NULL) {
				return IL_FALSE;
			}
			break;

		case 16:
		case 24:
			iCurImage->Format = IL_BGR;
			break;
		case 32:
			iCurImage->Format = IL_BGRA;
			break;

		default:
			ilSetError(IL_ILLEGAL_FILE_VALUE);
			return IL_FALSE;
	}

	// A height of 0 is illegal
	if (Header->biHeight == 0) {
		ilSetError(IL_ILLEGAL_FILE_VALUE);
		if (iCurImage->Pal.Palette)
			ifree(iCurImage->Pal.Palette);
		return IL_FALSE;
	}

	// If the image height is negative, then the image is flipped
	//	(Normal is IL_ORIGIN_LOWER_LEFT)
	if (Header->biHeight < 0) {
		iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;
	}
	else {
		iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;
	}

	// Read the palette
	iseek(sizeof(BMPHEAD), IL_SEEK_SET);
	if (iread(iCurImage->Pal.Palette, 1, iCurImage->Pal.PalSize) != iCurImage->Pal.PalSize)
		return IL_FALSE;

	// Seek to the data from the "beginning" of the file
	iseek(Header->bfDataOff, IL_SEEK_SET);

	// We have to handle 1 and 4-bit images separately, because we have to expand them.
	if (Header->biBitCount < 8) {
		switch (Header->biBitCount)
		{
			case 1:
				if (iGetHint(IL_MEM_SPEED_HINT) == IL_FASTEST) {
					iPreCache(iCurImage->Width / 8 * iCurImage->Height);

					PadSize = ((32 - (iCurImage->Width % 32)) / 8) % 4;  // Has to truncate
					for (j = 0; j < iCurImage->Height; j++) {
						Read = 0;
						for (i = 0; i < iCurImage->Width; ) {
							if (iread(&ByteData, 1, 1) != 1) {
								iUnCache();
								return IL_FALSE;
							}
							Read++;
							k = 128;
							for (c = 0; c < 8; c++) {
								iCurImage->Data[j * iCurImage->Width + i] = 
									(!!(ByteData & k) == 1 ? 255 : 0);
								k >>= 1;
								if (++i >= iCurImage->Width)
									break;
							}
						}
						//iseek(PadSize, IL_SEEK_CUR);
						iread(Padding, 1, PadSize);
					}

					iUnCache();
				}
				else {
					PadSize = ((32 - (iCurImage->Width % 32)) / 8) % 4;  // Has to truncate
					for (j = 0; j < iCurImage->Height; j++) {
						Read = 0;
						for (i = 0; i < iCurImage->Width; ) {
							if (iread(&ByteData, 1, 1) != 1)
								return IL_FALSE;
							Read++;
							k = 128;
							for (c = 0; c < 8; c++) {
								iCurImage->Data[j * iCurImage->Width + i] = 
									(!!(ByteData & k) == 1 ? 255 : 0);
								k >>= 1;
								if (++i >= iCurImage->Width)
									break;
							}
						}
						iseek(PadSize, IL_SEEK_CUR);
					}
				}

				break;

			case 4:
				if (iGetHint(IL_MEM_SPEED_HINT) == IL_FASTEST) {
					iPreCache(iCurImage->Width / 2 * iCurImage->Height);

					PadSize = ((8 - (iCurImage->Width % 8)) / 2) % 4;  // Has to truncate
					for (j = 0; j < iCurImage->Height; j++) {
						for (i = 0; i < iCurImage->Width; i++) {
							if (iread(&ByteData, 1, 1) != 1) {
								iUnCache();
								return IL_FALSE;
							}
							iCurImage->Data[j * iCurImage->Width + i] = ByteData >> 4;
							if (++i == iCurImage->Width)
								break;
							iCurImage->Data[j * iCurImage->Width + i] = ByteData & 0x0F;
						}
						//iread(Padding, 1, PadSize);iseek(PadSize, IL_SEEK_CUR);
					}

					iUnCache();
				}
				else {
					PadSize = ((8 - (iCurImage->Width % 8)) / 2) % 4;  // Has to truncate
					for (j = 0; j < iCurImage->Height; j++) {
						for (i = 0; i < iCurImage->Width; i++) {
							if (iread(&ByteData, 1, 1) != 1)
								return IL_FALSE;
							iCurImage->Data[j * iCurImage->Width + i] = ByteData >> 4;
							if (++i == iCurImage->Width)
								break;
							iCurImage->Data[j * iCurImage->Width + i] = ByteData & 0x0F;
						}
						iseek(PadSize, IL_SEEK_CUR);
					}
				}

				break;
		}
	}

	else {
		PadSize = (4 - (iCurImage->Bps % 4)) % 4;
		if (PadSize == 0) {
			if (iread(iCurImage->Data, 1, iCurImage->SizeOfPlane) != iCurImage->SizeOfPlane)
				return IL_FALSE;
		}
		else {  // M$ requires lines to be padded if the widths aren't multiples of 4.
			if (iGetHint(IL_MEM_SPEED_HINT) == IL_FASTEST) {
				iPreCache(iCurImage->Width * iCurImage->Height);

				PadSize = (4 - (iCurImage->Bps % 4));
				for (i = 0; i < iCurImage->SizeOfPlane; i += iCurImage->Bps) {
					if (iread(iCurImage->Data + i, 1, iCurImage->Bps) != iCurImage->Bps) {
						iUnCache();
						return IL_FALSE;
					}
					//iseek(PadSize, IL_SEEK_CUR);
					iread(Padding, 1, PadSize);
				}

				iUnCache();
			}
			else {
				PadSize = (4 - (iCurImage->Bps % 4));
				for (i = 0; i < iCurImage->SizeOfPlane; i += iCurImage->Bps) {
					if (iread(iCurImage->Data + i, 1, iCurImage->Bps) != iCurImage->Bps)
						return IL_FALSE;
					iseek(PadSize, IL_SEEK_CUR);
				}
			}
		}
	}

	return IL_TRUE;
}

#if ( IL_USE_POINTER == 0 )

ILboolean ilReadRLE8Bmp(BMPHEAD *Header)
{
	ILubyte		Bytes[2];
	ILuint		x, y;

	// Update the current image with the new dimensions
	if (!ilTexImage(Header->biWidth, abs(Header->biHeight), 1, 1, 0, IL_UNSIGNED_BYTE, NULL))
		return IL_FALSE;

	iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	// A height of 0 is illegal
	if (Header->biHeight == 0)
		return IL_FALSE;

	iCurImage->Format = IL_COLOUR_INDEX;
	iCurImage->Pal.PalType = IL_PAL_BGR32;
	iCurImage->Pal.PalSize = Header->biClrUsed * 4;  // 256 * 4 for most images
	if (iCurImage->Pal.PalSize == 0)
		iCurImage->Pal.PalSize = 256 * 4;
	iCurImage->Pal.Palette = (ILubyte*)ialloc(iCurImage->Pal.PalSize);
	if (iCurImage->Pal.Palette == NULL)
		return IL_FALSE;

	// If the image height is negative, then the image is flipped
	//	(Normal is IL_ORIGIN_LOWER_LEFT)
	if (Header->biHeight < 0)
		iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;
	else
		iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	// Read the palette
	iseek(sizeof(BMPHEAD), IL_SEEK_SET);
	if (iread(iCurImage->Pal.Palette, iCurImage->Pal.PalSize, 1) != 1)
		return IL_FALSE;

	// Seek to the data from the "beginning" of the file
	iseek(Header->bfDataOff, IL_SEEK_SET);

	for (y = 0 ; y < iCurImage->Height; y++)
	{
		x = 0;
		while (1)
		{
			if (iread(Bytes, sizeof(Bytes), 1) != 1)
				return IL_FALSE;
			if (Bytes[0] == 0x0) {  // Escape sequence
				if (Bytes[1] == 0x0)  // End of line
					break;
				else if (Bytes[1] == 0x1) {  // End of bitmap
					y = iCurImage->Height;
					break;
				}
				else if (Bytes[1] == 0x2) {
					if (iread(Bytes, sizeof(Bytes), 1) != 1)
						return IL_FALSE;
					x += Bytes[0];
					y += Bytes[1];
					if (y >= iCurImage->Height)
						break;
				}
				else {  // Run of pixels
					if ((ILint)iCurImage->Width - (ILint)x < (ILint)Bytes[1])
						return IL_FALSE;
					if (iread(iCurImage->Data + (y * iCurImage->Width + x),
								IL_MIN(((ILint)iCurImage->Width-(ILint)x), Bytes[1]), 1) != 1)
						return IL_FALSE;
					x += Bytes[1];
					if (Bytes[1] % 2)  // Must be on a word boundary
						if (iread( &Bytes[0], 1, 1) != 1)
							return IL_FALSE;
				}
			}
			else
			{
				memset( iCurImage->Data + y * iCurImage->Width + x, Bytes[1], Bytes[0] );
				x += Bytes[0];
			}
		}
	}
	return IL_TRUE;
}

#else		// if IL_USE_POINTER == 1

ILboolean ilReadRLE8Bmp(BMPHEAD *Header)
{
	ILubyte	Bytes[2];
	ILubyte	*start, *end;

	// Update the current image with the new dimensions
	if (!ilTexImage( Header->biWidth, abs(Header->biHeight), 1, 1, 0, IL_UNSIGNED_BYTE, NULL))
		return IL_FALSE;

	iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	// A height of 0 is illegal
	if ( Header->biHeight == 0 )
	{
		ilSetError( IL_ILLEGAL_FILE_VALUE );
		return IL_FALSE;
	}

	iCurImage->Format = IL_COLOUR_INDEX;
	iCurImage->Pal.PalType = IL_PAL_BGR32;
	iCurImage->Pal.PalSize = Header->biClrUsed * 4;  // 256 * 4 for most images
	if ( iCurImage->Pal.PalSize == 0 )
		iCurImage->Pal.PalSize = 256 * 4;
	iCurImage->Pal.Palette = (ILubyte*)ialloc(iCurImage->Pal.PalSize);
	if ( iCurImage->Pal.Palette == NULL )
		return IL_FALSE;	

	// If the image height is negative, then the image is flipped
	//	(Normal is IL_ORIGIN_LOWER_LEFT)
	if ( Header->biHeight < 0 )
		iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;
	else
		iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	// Read the palette
	iseek( sizeof(BMPHEAD), IL_SEEK_SET );
	if ( iread( iCurImage->Pal.Palette, iCurImage->Pal.PalSize, 1 ) != 1 )
		return IL_FALSE;

	// Seek to the data from the "beginning" of the file
	iseek( Header->bfDataOff, IL_SEEK_SET );

	start = iCurImage->Data;
	end =  iCurImage->Data + iCurImage->SizeOfData;

	while ( start < end )
	{
		if ( iread( Bytes, sizeof(Bytes), 1 ) != 1 )
			return IL_FALSE;
		if ( Bytes[0] == 0x0 )  // Escape sequence
		{
			if ( Bytes[1] == 0x0 )			// End of line
			{
				int		x = (end - start) % iCurImage->Width;

				if ( x )
					start += iCurImage->Width - x;
			}
			else if ( Bytes[1] == 0x1 )  // End of bitmap
				break;
			else if ( Bytes[1] == 0x2 ) {
				if (iread(&Bytes[0], sizeof(Bytes), 1) != 1)
					return IL_FALSE;
				start += iCurImage->Width * Bytes[1] + Bytes[0];
			}
			else  { // Run of pixels
				if ( iread( start, Bytes[1], 1 ) != 1 )
					return IL_FALSE;
				start += Bytes[1];

				if (Bytes[1] % 2)	// Must be on a dword boundary
					if (iread(&Bytes[0], sizeof(Bytes[0]), 1) != 1)
						return IL_FALSE;
			}
		}
		else {
			memset(start, Bytes[1], IL_MIN((end-start), Bytes[0]));
			start += Bytes[0];
		}
	}
	return IL_TRUE;
}

#endif

#if ( IL_USE_POINTER == 0 )

ILboolean ilReadRLE4Bmp(BMPHEAD *Header)
{
	ILubyte	Bytes[2], Upper, Lower;
	ILuint	x, y, i;

	// Update the current image with the new dimensions
	if ( ! ilTexImage( Header->biWidth, abs(Header->biHeight), 1, 1, 0, IL_UNSIGNED_BYTE, NULL ) )
		return IL_FALSE;
	iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	// A height of 0 is illegal
	if ( Header->biHeight == 0 )
	{
		ilSetError( IL_ILLEGAL_FILE_VALUE );
		return IL_FALSE;
	}

	iCurImage->Format = IL_COLOUR_INDEX;
	iCurImage->Pal.PalType = IL_PAL_BGR32;
	iCurImage->Pal.PalSize = 16 * 4; //Header->biClrUsed * 4;  // 16 * 4 for most images
	iCurImage->Pal.Palette = (ILubyte*)ialloc(iCurImage->Pal.PalSize);
	if ( iCurImage->Pal.Palette == NULL )
		return IL_FALSE;

	// If the image height is negative, then the image is flipped
	//	(Normal is IL_ORIGIN_LOWER_LEFT)
	if ( Header->biHeight < 0 )
		iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;
	else
		iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	// Read the palette
	iseek( sizeof(BMPHEAD), IL_SEEK_SET );

	if ( iread( iCurImage->Pal.Palette, iCurImage->Pal.PalSize, 1 ) != 1 )
		return IL_FALSE;

	// Seek to the data from the "beginning" of the file
	iseek( Header->bfDataOff, IL_SEEK_SET );

	for ( y = 0 ; y < iCurImage->Height ; y++ )
	{
		x = 0;
		while ( 1 )
		{
			if ( iread( &Bytes[0], sizeof(Bytes), 1 ) != 1 )
				return IL_FALSE;
			if ( Bytes[0] == 0x0 )				// Escape sequence
			{
				if ( Bytes[1] == 0x0 )			// End of line
					break;
				else if ( Bytes[1] == 0x1 )		// End of bitmap
				{
					y = iCurImage->Height;
					break;
				}
				else if ( Bytes[1] == 0x2 )
				{
					if ( iread( &Bytes[0], sizeof(Bytes), 1 ) != 1 )
						return IL_FALSE;
					x += Bytes[0];
					y += Bytes[1];
					if ( y >= iCurImage->Height )
						break;
				}
				else	  // Run of pixels
				{
					int		align;

					for ( i = 0 ; i < Bytes[1] && x < iCurImage->Width ; i++ )
					{
						int		byte;

						if ( (i & 0x01) == 0 )
						{
							if ( iread( &Bytes[0], sizeof(Bytes[0]), 1 ) != 1 )
								return IL_FALSE;
							byte = (Bytes[0] >> 4);
						}
						else
							byte = (Bytes[0] & 0x0F);
						iCurImage->Data[y * iCurImage->Width + x++] = byte;
					}

					align = Bytes[1] % 4;

					if ( align == 1 || align == 2 )	// Must be on a dword boundary
						if ( iread( &Bytes[0], sizeof(Bytes[0]), 1 ) != 1 )
							return IL_FALSE;
				}
			}
			else
			{
				Upper = (Bytes[1] >> 4);
				Lower = Bytes[1] & 0x0F;

				for ( i = 0 ; i < Bytes[0] && x < iCurImage->Width ; i++ )
					iCurImage->Data[y * iCurImage->Width + x++] = (i & 0x01) == 0 ? Upper : Lower;
			}
		}
	}
	return IL_TRUE;
}

#else		// if IL_USE_POINTER == 1

ILboolean ilReadRLE4Bmp(BMPHEAD *Header)
{
	ILubyte		Bytes[2];
	ILubyte     *start, *end;
	ILubyte     i, upper, lower;

	// Update the current image with the new dimensions
	if ( ! ilTexImage( Header->biWidth, abs(Header->biHeight), 1, 1, 0, IL_UNSIGNED_BYTE, NULL ) )
		return IL_FALSE;

	iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	// A height of 0 is illegal
	if ( Header->biHeight == 0 )
	{
		ilSetError( IL_ILLEGAL_FILE_VALUE );
		return IL_FALSE;
	}

	iCurImage->Format = IL_COLOUR_INDEX;
	iCurImage->Pal.PalType = IL_PAL_BGR32;
	iCurImage->Pal.PalSize = 16 * 4; //Header->biClrUsed * 4;  // 16 * 4 for most images
	iCurImage->Pal.Palette = (ILubyte*)ialloc(iCurImage->Pal.PalSize);
	if ( iCurImage->Pal.Palette == NULL )
		return IL_FALSE;

	// If the image height is negative, then the image is flipped
	//	(Normal is IL_ORIGIN_LOWER_LEFT)
	if ( Header->biHeight < 0 )
		iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;
	else
		iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	// Read the palette
	iseek( sizeof(BMPHEAD), IL_SEEK_SET );
	if ( iread( iCurImage->Pal.Palette, iCurImage->Pal.PalSize, 1 ) != 1 )
		return IL_FALSE;

	// Seek to the data from the "beginning" of the file
	iseek( Header->bfDataOff, IL_SEEK_SET );

	start = iCurImage->Data;
	end =  iCurImage->Data + iCurImage->SizeOfData;

	while ( start < end )
	{
		if ( iread( &Bytes[0], sizeof(Bytes), 1 ) != 1 )
			return IL_FALSE;
		if ( Bytes[0] == 0x0 )		  		// Escape sequence
		{
			if ( Bytes[1] == 0x0 )			// End of line
			{
				int		x = (end - start) % iCurImage->Width;

				if ( x )
					start += iCurImage->Width - x;
			}
			else if ( Bytes[1] == 0x1 )	// End of bitmap
				break;
			else if ( Bytes[1] == 0x2 )
			{
				if ( iread( &Bytes[0], sizeof(Bytes), 1 ) != 1 )
					return IL_FALSE;
				start += iCurImage->Width * Bytes[1] + Bytes[0];
			}
			else						// Run of pixels
			{
				int		align;

				for ( i = 0 ; i < Bytes[1] && start < end ; i++ )
				{
					if ( (i & 0x01) == 0 )
					{
						if ( iread( &Bytes[0], sizeof(Bytes[0]), 1 ) != 1 )
							return IL_FALSE;
						*start++ = (ILubyte)(Bytes[0] >> 4);
					}
					else
						*start++ = (Bytes[0] & (ILubyte)0x0F);
				}

				align = Bytes[1] % 4;
				if ( align == 1 || align == 2 )	// Must be on a dword boundary
					if ( iread( &Bytes[0], sizeof(Bytes[0]), 1 ) != 1 )
						return IL_FALSE;
			}
		}
		else
		{
			upper = (ILubyte)(Bytes[1] >> 4);
			lower = (Bytes[1] & (ILubyte)0x0F);

			for ( i = 0 ; i < Bytes[0] && start < end ; i++ )
				*start++ = (i & 0x01) == 0 ? upper : lower;
		}
	}
	return IL_TRUE;
}

#endif

ILboolean iGetOS2Bmp(OS2_HEAD *Header)
{
	ILuint	PadSize, Read, i, j, k, c;
	ILubyte	ByteData;

	if (Header->cBitCount == 1) {
		if (!ilTexImage(Header->cx, Header->cy, 1, 1, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE, NULL)) {
			return IL_FALSE;
		}
		iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

		iCurImage->Pal.Palette = (ILubyte*)ialloc(2 * 3);
		if (iCurImage->Pal.Palette == NULL) {
			return IL_FALSE;
		}
		iCurImage->Pal.PalSize = 2 * 3;
		iCurImage->Pal.PalType = IL_PAL_BGR24;

		if (iread(iCurImage->Pal.Palette, 1, 2 * 3) != 6)
			return IL_FALSE;

		PadSize = ((32 - (iCurImage->Width % 32)) / 8) % 4;  // Has to truncate.
		iseek(Header->DataOff, IL_SEEK_SET);

		for (j = 0; j < iCurImage->Height; j++) {
			Read = 0;
			for (i = 0; i < iCurImage->Width; ) {
				if (iread(&ByteData, 1, 1) != 1)
					return IL_FALSE;
				Read++;
				k = 128;
				for (c = 0; c < 8; c++) {
					iCurImage->Data[j * iCurImage->Width + i] =
						(!!(ByteData & k) == 1 ? 255 : 0);
					k >>= 1;
					if (++i >= iCurImage->Width)
						break;
				}
			}
			iseek(PadSize, IL_SEEK_CUR);
		}
		return IL_TRUE;
	}

	if (Header->cBitCount == 4) {
		if (!ilTexImage(Header->cx, Header->cy, 1, 1, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE, NULL)) {
			return IL_FALSE;
		}
		iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

		iCurImage->Pal.Palette = (ILubyte*)ialloc(16 * 3);
		if (iCurImage->Pal.Palette == NULL) {
			return IL_FALSE;
		}
		iCurImage->Pal.PalSize = 16 * 3;
		iCurImage->Pal.PalType = IL_PAL_BGR24;

		if (iread(iCurImage->Pal.Palette, 1, 16 * 3) != 16*3)
			return IL_FALSE;

		PadSize = ((8 - (iCurImage->Width % 8)) / 2) % 4;  // Has to truncate
		iseek(Header->DataOff, IL_SEEK_SET);

		for (j = 0; j < iCurImage->Height; j++) {
			for (i = 0; i < iCurImage->Width; i++) {
				if (iread(&ByteData, 1, 1) != 1)
					return IL_FALSE;
				iCurImage->Data[j * iCurImage->Width + i] = ByteData >> 4;
				if (++i == iCurImage->Width)
					break;
				iCurImage->Data[j * iCurImage->Width + i] = ByteData & 0x0F;
			}
			iseek(PadSize, IL_SEEK_CUR);
		}

		return IL_TRUE;
	}

	if (Header->cBitCount == 8) {
		iCurImage->Format = IL_COLOUR_INDEX;
		iCurImage->Pal.Palette = (ILubyte*)ialloc(256 * 3);
		if (iCurImage->Pal.Palette == NULL) {
			return IL_FALSE;
		}
		iCurImage->Pal.PalSize = 256 * 3;
		iCurImage->Pal.PalType = IL_PAL_BGR24;

		if (iread(iCurImage->Pal.Palette, 1, 256 * 3) != 256*3)
			return IL_FALSE;
	}

	if (!ilTexImage(Header->cx, Header->cy, 1, (ILubyte)(Header->cBitCount >> 3), IL_BGR, IL_UNSIGNED_BYTE, NULL)) {
		return IL_FALSE;
	}
	iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;
	if (Header->cBitCount == 32) {
		iCurImage->Format = IL_BGRA;
	}

	iseek(Header->DataOff, IL_SEEK_SET);

	PadSize = PadSize = (4 - (iCurImage->Bps % 4)) % 4;
	if (PadSize == 0) {
		if (iread(iCurImage->Data, 1, iCurImage->SizeOfData) != iCurImage->SizeOfData)
			return IL_FALSE;
	}
	else {
		for (i = 0; i < iCurImage->Height; i++) {
			if (iread(iCurImage->Data + i * iCurImage->Bps, 1, iCurImage->Bps) != iCurImage->Bps)
				return IL_FALSE;
			iseek(PadSize, IL_SEEK_CUR);
		}
	}

	return IL_TRUE;
}


//! Writes a .bmp file
ILboolean ilSaveBmp(const ILstring FileName)
{
	ILHANDLE	BitmapFile;
	ILboolean	bBitmap = IL_FALSE;

	if (ilGetBoolean(IL_FILE_MODE) == IL_FALSE) {
		if (iFileExists(FileName)) {
			ilSetError(IL_FILE_ALREADY_EXISTS);
			return IL_FALSE;
		}
	}

	BitmapFile = iopenw(FileName);
	if (BitmapFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bBitmap;
	}

	bBitmap = ilSaveBmpF(BitmapFile);
	iclosew(BitmapFile);

	return bBitmap;
}


//! Writes a .bmp to an already-opened file
ILboolean ilSaveBmpF(ILHANDLE File)
{
	iSetOutputFile(File);
	return iSaveBitmapInternal();
}


//! Writes a .bmp to a memory "lump"
ILboolean ilSaveBmpL(ILvoid *Lump, ILuint Size)
{
	iSetOutputLump(Lump, Size);
	return iSaveBitmapInternal();
}


// Internal function used to save the .bmp.
ILboolean iSaveBitmapInternal()
{
	ILuint	FileSize, i, PadSize, Padding = 0;
	ILimage	*TempImage = NULL;
	ILpal	*TempPal;
	ILubyte	*TempData;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	iputc('B');  // Comprises the
	iputc('M');  //  "signature"
	SaveLittleUInt(0);  // Will come back and change later in this function (filesize)
	SaveLittleUInt(0);  // Reserved

	// If the current image has a palette, take care of it
	TempPal = &iCurImage->Pal;
	if (iCurImage->Pal.PalSize && iCurImage->Pal.Palette && iCurImage->Pal.PalType != IL_PAL_NONE) {
		// If the palette in .bmp format, write it directly
		if (iCurImage->Pal.PalType == IL_PAL_BGR32) {
			TempPal = &iCurImage->Pal;
		}
		else {
			TempPal = iConvertPal(&iCurImage->Pal, IL_PAL_BGR32);
			if (TempPal == NULL) {
				return IL_FALSE;
			}
		}
	}

	SaveLittleUInt(54 + TempPal->PalSize);  // Offset of the data

	SaveLittleUInt(0x28);  // Header size
	SaveLittleUInt(iCurImage->Width);

	// Removed because some image viewers don't like the negative height.
	/*if (iCurImage->Origin == IL_ORIGIN_UPPER_LEFT)
		SaveLittleInt(-(ILint)iCurImage->Height);
	else*/
		SaveLittleInt(iCurImage->Height);

	SaveLittleUShort(1);  // Number of planes
	SaveLittleUShort((ILushort)((ILushort)iCurImage->Bpp << 3));  // Bpp
	SaveLittleInt(0);  // Compression
	SaveLittleInt(0);  // Size of image (Obsolete)
	SaveLittleInt(0);  // (Obsolete)
	SaveLittleInt(0);  // (Obsolete)

	if (iCurImage->Pal.PalType != IL_PAL_NONE)
		SaveLittleInt(ilGetInteger(IL_PALETTE_NUM_COLS));  // Num colours used
	else
		SaveLittleInt(0);
	SaveLittleInt(0);  // Important colour (none)

	if (iCurImage->Format != IL_BGR && iCurImage->Format != IL_BGRA && iCurImage->Format != IL_COLOUR_INDEX) {
		if (iCurImage->Format == IL_RGBA)
			TempImage = iConvertImage(iCurImage, IL_BGRA, IL_UNSIGNED_BYTE);
		else
			TempImage = iConvertImage(iCurImage, IL_BGR, IL_UNSIGNED_BYTE);

		if (TempImage == NULL)
			return IL_FALSE;
	}
	else if (iCurImage->Bpc > 1) {
		TempImage = iConvertImage(iCurImage, iCurImage->Format, IL_UNSIGNED_BYTE);
		if (TempImage == NULL)
			return IL_FALSE;
	}
	else {
		TempImage = iCurImage;
	}

	if (TempImage->Origin != IL_ORIGIN_LOWER_LEFT)
		TempData = iGetFlipped(TempImage);
	else
		TempData = TempImage->Data;

	iwrite(TempPal->Palette, 1, TempPal->PalSize);

	PadSize = PadSize = (4 - (TempImage->Bps % 4)) % 4;
	// No padding, so write data directly.
	if (PadSize == 0) {
		iwrite(TempData, 1, TempImage->SizeOfPlane);
	}
	else {  // Odd width, so we must pad each line.
		for (i = 0; i < TempImage->SizeOfPlane; i += TempImage->Bps) {
			iwrite(TempData + i, 1, TempImage->Bps);
			// Write pad byte(s)
			iwrite(&Padding, 1, PadSize);
		}
	}

	// Write the filesize
	FileSize = itellw();
	iseekw(2, IL_SEEK_SET);
	SaveLittleUInt(FileSize);


	if (TempPal != &iCurImage->Pal) {
		ifree(TempPal->Palette);
		ifree(TempPal);
	}
	if (TempData != TempImage->Data)
		ifree(TempData);
	if (TempImage != iCurImage)
		ilCloseImage(TempImage);

	return IL_TRUE;
}


#endif//IL_NO_BMP
