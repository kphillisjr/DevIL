//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 12/27/2008
//
// Filename: src-IL/src/il_vtf.c
//
// Description: Reads from and writes to a Valve Texture Format (.vtf) file.
//                These are used in Valve's Source games.  VTF specs available
//                from http://developer.valvesoftware.com/wiki/VTF.
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#ifndef IL_NO_VTF
#include "il_vtf.h"
#include "il_dds.h"


//@TODO: Get rid of these globals.
static VTFHEAD	Head;


//! Checks if the file specified in FileName is a valid VTF file.
ILboolean ilIsValidVtf(ILconst_string FileName)
{
	ILHANDLE	VtfFile;
	ILboolean	bVtf = IL_FALSE;
	
	if (!iCheckExtension(FileName, IL_TEXT("vtf"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bVtf;
	}
	
	VtfFile = iopenr(FileName);
	if (VtfFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bVtf;
	}
	
	bVtf = ilIsValidVtfF(VtfFile);
	icloser(VtfFile);
	
	return bVtf;
}


//! Checks if the ILHANDLE contains a valid VTF file at the current position.
ILboolean ilIsValidVtfF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidVtf();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Checks if Lump is a valid VTF lump.
ILboolean ilIsValidVtfL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidVtf();
}


// Internal function used to get the VTF header from the current file.
ILboolean iGetVtfHead(VTFHEAD *Header)
{
	iread(Header->Signature, 1, 4);
	Header->Version[0] = GetLittleUInt();
	Header->Version[1] = GetLittleUInt();
	Header->HeaderSize = GetLittleUInt();
	Header->Width = GetLittleUShort();
	Header->Height = GetLittleUShort();
	Header->Flags = GetLittleUInt();
	Header->Frames = GetLittleUShort();
	Header->FirstFrame = GetLittleUShort();
	iseek(4, IL_SEEK_CUR);  // Padding
	Header->Reflectivity[0] = GetLittleFloat();
	Header->Reflectivity[1] = GetLittleFloat();
	Header->Reflectivity[2] = GetLittleFloat();
	iseek(4, IL_SEEK_CUR);  // Padding
	Header->BumpmapScale = GetLittleFloat();
	Header->HighResImageFormat = GetLittleUInt();
	Header->MipmapCount = (ILubyte)igetc();
	Header->LowResImageFormat = GetLittleUInt();
	Header->LowResImageWidth = (ILubyte)igetc();
	Header->LowResImageHeight = (ILubyte)igetc();
	Header->Depth = GetLittleUShort();

	iseek(Header->HeaderSize - sizeof(VTFHEAD), IL_SEEK_CUR);

	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidVtf()
{
	if (!iGetVtfHead(&Head))
		return IL_FALSE;
	iseek(-(ILint)sizeof(VTFHEAD), IL_SEEK_CUR);
	
	return iCheckVtf(&Head);
}


//@TODO: Add more checks.
// Should we check for Frames, MipmapCount and Depth != 0?

// Internal function used to check if the HEADER is a valid VTF header.
ILboolean iCheckVtf(VTFHEAD *Header)
{
	// The file signature is "VTF\0".
	if ((Header->Signature[0] != 'V') || (Header->Signature[1] != 'T') || (Header->Signature[2] != 'F')
		|| (Header->Signature[3] != 0))
		return IL_FALSE;
	// Are there other versions available yet?
	if (Header->Version[0] != 7)
		return IL_FALSE;
	// We have 7.0 through 7.4 as of 12/27/2008.
	if (Header->Version[1] > 4)
		return IL_FALSE;
	// May change in future version of the specifications.
	//  80 is through version 7.2, and 96 is through 7.4.
	if ((Header->HeaderSize != 80) && (Header->HeaderSize != 104))
		return IL_FALSE;

	// 0 is an invalid dimension
	if (Header->Width == 0 || Header->Height == 0)
		return IL_FALSE;
	// Width and Height must be powers of 2.
	if ((ilNextPower2(Header->Width) != Header->Width) || (ilNextPower2(Header->Height) != Header->Height))
		return IL_FALSE;
	if ((ilNextPower2(Header->LowResImageWidth) != Header->LowResImageWidth)
		|| (ilNextPower2(Header->LowResImageHeight) != Header->LowResImageHeight))
		return IL_FALSE;
	// In addition, the LowResImage has to have dimensions no greater than 16.
	if ((Header->LowResImageWidth > 16) || (Header->LowResImageHeight > 16)
		|| (Header->LowResImageWidth == 0) || (Header->LowResImageHeight == 0))
		return IL_FALSE;
	// And the LowResImage has to have dimensions less than or equal to the main image.
	if ((Header->LowResImageWidth > Header->Width) || (Header->LowResImageHeight > Header->Height))
		return IL_FALSE;
	// The LowResImage must be in DXT1 format.
	if (Header->LowResImageFormat != IMAGE_FORMAT_DXT1)
		return IL_FALSE;
	
	return IL_TRUE;
}


//! Reads a VTF file
ILboolean ilLoadVtf(ILconst_string FileName)
{
	ILHANDLE	VtfFile;
	ILboolean	bVtf = IL_FALSE;
	
	VtfFile = iopenr(FileName);
	if (VtfFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bVtf;
	}

	bVtf = ilLoadVtfF(VtfFile);
	icloser(VtfFile);

	return bVtf;
}


//! Reads an already-opened VTF file
ILboolean ilLoadVtfF(ILHANDLE File) {
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadVtfInternal();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Reads from a memory "lump" that contains a VTF
ILboolean ilLoadVtfL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadVtfInternal();
}


// Internal function used to load the VTF.
ILboolean iLoadVtfInternal()
{
	ILboolean	bVtf = IL_TRUE;
	ILimage		*Image;
	ILuint		Width, Height, Depth;
	ILenum		Format, Type;
	ILint		i, j;
	ILuint		Channels, Bpc;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}
	
	if (!iGetVtfHead(&Head))
		return IL_FALSE;
	if (!iCheckVtf(&Head)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}
	
	Width = Head.Width;
	Height = Head.Height;
	Depth = Head.Depth;

	//@TODO: Take care of volume textures soon.
	if (Head.Depth != 1) {
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}

	if (!ilTexImage(Head.LowResImageWidth, Head.LowResImageHeight, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL))
		return IL_FALSE;
	// The origin should be in the upper left.
	iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;

	// Read the low resolution image first.  This is just a thumbnail.
	//  Just read it and discard it for right now.
	if (!VtfDecompressDXT1(iCurImage))
		return IL_FALSE;

	// Make this a helper function that set channels, bpc and format.
	switch (Head.HighResImageFormat)
	{
		case IMAGE_FORMAT_DXT1:
			Channels = 4;
			Bpc = 1;
			Format = IL_RGBA;
			Type = IL_UNSIGNED_BYTE;
			break;
		case IMAGE_FORMAT_DXT5:
			Channels = 4;
			Bpc = 1;
			Format = IL_RGBA;
			Type = IL_UNSIGNED_BYTE;
			break;

		default:
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return IL_FALSE;
	}

//@TODO: Change...!
	if (!ilTexImage(Head.Width, Head.Height, Head.Depth, Channels, Format, Type, NULL))
		return IL_FALSE;
	// The origin should be in the upper left.
	iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;
	Image = iCurImage;

	// Next comes the high resolution images.  The mipmaps are in order from smallest to largest in the file.
	//  We want them the other way around.
	Width = iCurImage->Width;  Height = iCurImage->Height;  Depth = iCurImage->Depth;
	for (i = 0; i < Head.MipmapCount - 1; i++) {
		// 1 is the smallest dimension possible.
		Width = (Width >> 1) == 0 ? 1 : (Width >> 1);
		Height = (Height >> 1) == 0 ? 1 : (Height >> 1);
		Depth = (Depth >> 1) == 0 ? 1 : (Depth >> 1);

		Image->Next = ilNewImage(Width, Height, Depth, Channels, Bpc);
		if (Image->Next == NULL)  //@TODO: Do we need to do any cleanup here?
			return IL_FALSE;
		Image->Origin = IL_ORIGIN_UPPER_LEFT;
		Image = Image->Next;
	}

	for (i = 0; i < Head.MipmapCount; i++) {
		Image = iCurImage;
		// We want to put the smallest mipmap at the end, but it is first in the file.
		for (j = 1; j < Head.MipmapCount - i; j++) {
			Image = Image->Next;  // Move down the linked list.
		}

		switch (Head.HighResImageFormat)
		{
			case IMAGE_FORMAT_DXT1:
				bVtf = VtfDecompressDXT1(Image);
				if (bVtf == IL_FALSE)  //@TODO: Do we need to do any cleanup here?
					return IL_FALSE;
				break;
			case IMAGE_FORMAT_DXT5:
				bVtf = VtfDecompressDXT5(Image);
				if (bVtf == IL_FALSE)  //@TODO: Do we need to do any cleanup here?
					return IL_FALSE;
				break;
		}
	}



//@TODO: Cannot be like this for animation chains!
	iCurImage->Mipmaps = iCurImage->Next;
	iCurImage->Next = NULL;

	ilBindImage(ilGetCurName());  // Set to parent image first.
	return ilFixImage();
}


//@TODO: Taken from il_dds.c.  Make the code more modular.
ILboolean VtfDecompressDXT1(ILimage *Image)
{
	ILuint		x, y, z, i, j, k, Select;
	Color8888	colours[4], *col;
	ILushort	color_0, color_1;
	ILuint		bitmask, Offset;

	if (iGetHint(IL_MEM_SPEED_HINT) == IL_FASTEST)
		// Precache a minimum of 64-bits (8 bytes).
		//iPreCache(max(Image->Width * Image->Height * 4 / 6), 8);  // Gives 6:1 compression.
		iPreCache(Image->Width * Image->Height * 4 / 6);  // Gives 6:1 compression.

	colours[0].a = 0xFF;
	colours[1].a = 0xFF;
	colours[2].a = 0xFF;
	//colours[3].a = 0xFF;
	for (z = 0; z < Image->Depth; z++) {
		for (y = 0; y < Image->Height; y += 4) {
			for (x = 0; x < Image->Width; x += 4) {
				color_0 = GetLittleUShort();
				color_1 = GetLittleUShort();
				DxtcReadColor(color_0, colours);
				DxtcReadColor(color_1, colours + 1);
				bitmask = GetLittleUInt();

				if (color_0 > color_1) {
					// Four-color block: derive the other two colors.
					// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
					// These 2-bit codes correspond to the 2-bit fields 
					// stored in the 64-bit block.
					colours[2].b = (2 * colours[0].b + colours[1].b + 1) / 3;
					colours[2].g = (2 * colours[0].g + colours[1].g + 1) / 3;
					colours[2].r = (2 * colours[0].r + colours[1].r + 1) / 3;
					//colours[2].a = 0xFF;

					colours[3].b = (colours[0].b + 2 * colours[1].b + 1) / 3;
					colours[3].g = (colours[0].g + 2 * colours[1].g + 1) / 3;
					colours[3].r = (colours[0].r + 2 * colours[1].r + 1) / 3;
					colours[3].a = 0xFF;
				}
				else { 
					// Three-color block: derive the other color.
					// 00 = color_0,  01 = color_1,  10 = color_2,
					// 11 = transparent.
					// These 2-bit codes correspond to the 2-bit fields 
					// stored in the 64-bit block. 
					colours[2].b = (colours[0].b + colours[1].b) / 2;
					colours[2].g = (colours[0].g + colours[1].g) / 2;
					colours[2].r = (colours[0].r + colours[1].r) / 2;
					//colours[2].a = 0xFF;

					colours[3].b = (colours[0].b + 2 * colours[1].b + 1) / 3;
					colours[3].g = (colours[0].g + 2 * colours[1].g + 1) / 3;
					colours[3].r = (colours[0].r + 2 * colours[1].r + 1) / 3;
					colours[3].a = 0x00;
				}

				for (j = 0, k = 0; j < 4; j++) {
					for (i = 0; i < 4; i++, k++) {

						Select = (bitmask & (0x03 << k*2)) >> k*2;
						col = &colours[Select];

						if (((x + i) < Image->Width) && ((y + j) < Image->Height)) {
							Offset = z * Image->SizeOfPlane + (y + j) * Image->Bps + (x + i) * Image->Bpp;
							Image->Data[Offset + 0] = col->r;
							Image->Data[Offset + 1] = col->g;
							Image->Data[Offset + 2] = col->b;
							Image->Data[Offset + 3] = col->a;
						}
					}
				}
			}
		}
	}

	iUnCache();

	return IL_TRUE;
}


ILboolean VtfDecompressDXT5(ILimage *Image)
{
	ILuint		x, y, z, i, j, k, Select;
	Color8888	colours[4], *col;
	ILuint		bitmask, Offset;
	ILubyte		alphas[8], alphamask[6];
	ILuint		bits;
	ILubyte		Data[4];  //@TODO: Change

	for (z = 0; z < Image->Depth; z++) {
		for (y = 0; y < Image->Height; y += 4) {
			for (x = 0; x < Image->Width; x += 4) {
				if (y >= Image->Height || x >= Image->Width)
					break;
				alphas[0] = igetc();
				alphas[1] = igetc();
				iread(alphamask, 1, 6);

				iread(Data, 1, 4);
				DxtcReadColors(Data, colours);
				bitmask = GetLittleUInt();

				// Four-color block: derive the other two colors.    
				// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
				// These 2-bit codes correspond to the 2-bit fields 
				// stored in the 64-bit block.
				colours[2].b = (2 * colours[0].b + colours[1].b + 1) / 3;
				colours[2].g = (2 * colours[0].g + colours[1].g + 1) / 3;
				colours[2].r = (2 * colours[0].r + colours[1].r + 1) / 3;
				//colours[2].a = 0xFF;

				colours[3].b = (colours[0].b + 2 * colours[1].b + 1) / 3;
				colours[3].g = (colours[0].g + 2 * colours[1].g + 1) / 3;
				colours[3].r = (colours[0].r + 2 * colours[1].r + 1) / 3;
				//colours[3].a = 0xFF;

				k = 0;
				for (j = 0; j < 4; j++) {
					for (i = 0; i < 4; i++, k++) {
						Select = (bitmask & (0x03 << k*2)) >> k*2;
						col = &colours[Select];

						// only put pixels out < width or height
						if (((x + i) < Image->Width) && ((y + j) < Image->Height)) {
							Offset = z * Image->SizeOfPlane + (y + j) * Image->Bps + (x + i) * Image->Bpp;
							Image->Data[Offset + 0] = col->r;
							Image->Data[Offset + 1] = col->g;
							Image->Data[Offset + 2] = col->b;
						}
					}
				}

				// 8-alpha or 6-alpha block?    
				if (alphas[0] > alphas[1]) {    
					// 8-alpha block:  derive the other six alphas.    
					// Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
					alphas[2] = (6 * alphas[0] + 1 * alphas[1] + 3) / 7;	// bit code 010
					alphas[3] = (5 * alphas[0] + 2 * alphas[1] + 3) / 7;	// bit code 011
					alphas[4] = (4 * alphas[0] + 3 * alphas[1] + 3) / 7;	// bit code 100
					alphas[5] = (3 * alphas[0] + 4 * alphas[1] + 3) / 7;	// bit code 101
					alphas[6] = (2 * alphas[0] + 5 * alphas[1] + 3) / 7;	// bit code 110
					alphas[7] = (1 * alphas[0] + 6 * alphas[1] + 3) / 7;	// bit code 111
				}
				else {
					// 6-alpha block.
					// Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
					alphas[2] = (4 * alphas[0] + 1 * alphas[1] + 2) / 5;	// Bit code 010
					alphas[3] = (3 * alphas[0] + 2 * alphas[1] + 2) / 5;	// Bit code 011
					alphas[4] = (2 * alphas[0] + 3 * alphas[1] + 2) / 5;	// Bit code 100
					alphas[5] = (1 * alphas[0] + 4 * alphas[1] + 2) / 5;	// Bit code 101
					alphas[6] = 0x00;										// Bit code 110
					alphas[7] = 0xFF;										// Bit code 111
				}

				// Note: Have to separate the next two loops,
				//	it operates on a 6-byte system.

				// First three bytes
				//bits = *((ILint*)alphamask);
				bits = (alphamask[0]) | (alphamask[1] << 8) | (alphamask[2] << 16);
				for (j = 0; j < 2; j++) {
					for (i = 0; i < 4; i++) {
						// only put pixels out < width or height
						if (((x + i) < Image->Width) && ((y + j) < Image->Height)) {
							Offset = z * Image->SizeOfPlane + (y + j) * Image->Bps + (x + i) * Image->Bpp + 3;
							Image->Data[Offset] = alphas[bits & 0x07];
						}
						bits >>= 3;
					}
				}

				// Last three bytes
				//bits = *((ILint*)&alphamask[3]);
				bits = (alphamask[3]) | (alphamask[4] << 8) | (alphamask[5] << 16);
				for (j = 2; j < 4; j++) {
					for (i = 0; i < 4; i++) {
						// only put pixels out < width or height
						if (((x + i) < Image->Width) && ((y + j) < Image->Height)) {
							Offset = z * Image->SizeOfPlane + (y + j) * Image->Bps + (x + i) * Image->Bpp + 3;
							Image->Data[Offset] = alphas[bits & 0x07];
						}
						bits >>= 3;
					}
				}
			}
		}
	}

	return IL_TRUE;
}




#endif//IL_NO_VTF
