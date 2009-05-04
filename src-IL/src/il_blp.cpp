//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 05/02/2009
//
// Filename: src-IL/src/il_blp.cpp
//
// Description: Reads from a Blizzard Texture (.blp).
//                Specifications were found at http://www.wowwiki.com/BLP_files
//				  for BLP2 and from
// http://web.archive.org/web/20080117120549/magos.thejefffiles.com/War3ModelEditor/MagosBlpFormat.txt
//				  for BLP1.
//
//-----------------------------------------------------------------------------

//@TODO: Add support for the BLP1 format as well.

#include "il_internal.h"
#ifndef IL_NO_BLP
#include "il_dds.h"


typedef struct BLP1HEAD
{
	char	Sig[4];
	ILuint	Compression;	// Texture type: 0 = JPG, 1 = Paletted
	ILuint	Flags;			// #8 - Uses alpha channel (?)
	ILuint	Width;			// Image width in pixels
	ILuint	Height;			// Image height in pixels
	ILuint	PictureType;	// 3 - Uncompressed index list + alpha list
							// 4 - Uncompressed index list + alpha list
							// 5 - Uncompressed index list
	ILuint	PictureSubType;	// 1 - ???
	ILuint	MipOffsets[16]; // The file offsets of each mipmap, 0 for unused
	ILuint	MipLengths[16]; // The length of each mipmap data block
} BLP1HEAD;

typedef struct BLP2HEAD
{
	char	Sig[4];         // "BLP2" signature
	ILuint	Type;           // Texture type: 0 = JPG, 1 = DXTC
	ILubyte	Compression;    // Compression mode: 1 = raw, 2 = DXTC
	ILubyte	AlphaBits;      // 0, 1, or 8
	ILubyte	AlphaType;      // 0, 1, 7 or 8
	ILubyte	HasMips;        // 0 = no mips levels, 1 = has mips (number of levels determined by image size)
	ILuint	Width;          // Image width in pixels
	ILuint	Height;         // Image height in pixels
	ILuint	MipOffsets[16]; // The file offsets of each mipmap, 0 for unused
	ILuint	MipLengths[16]; // The length of each mipmap data block
} BLP2HEAD;

// Data formats
#define BLP_TYPE_JPG		0
#define BLP_TYPE_DXTC_RAW	1
#define BLP_RAW				1
#define BLP_DXTC			2

#define BLP_RAW_PLUS_ALPHA1	3
#define BLP_RAW_PLUS_ALPHA2	4
#define BLP_RAW_NO_ALPHA	5


ILboolean iIsValidBlp2(void);
ILboolean iCheckBlp2(BLP2HEAD *Header);
ILboolean iLoadBlpInternal(ILimage *Image, ILstate *State);
ILboolean iLoadBlp1(ILimage *Image, ILstate *State);
ILboolean iCheckBlp1(BLP1HEAD *Header);
ILboolean iGetBlp1Head(BLP1HEAD *Header);


//! Checks if the file specified in FileName is a valid BLP file.
ILboolean ilIsValidBlp(ILconst_string FileName, ILstate *State)
{
	ILHANDLE	BlpFile;
	ILboolean	bBlp = IL_FALSE;
	
	if (!iCheckExtension(FileName, IL_TEXT("blp"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bBlp;
	}
	
	BlpFile = iopenr(FileName);
	if (BlpFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bBlp;
	}
	
	bBlp = ilIsValidBlpF(BlpFile, State);
	icloser(BlpFile);
	
	return bBlp;
}


//! Checks if the ILHANDLE contains a valid BLP file at the current position.
ILboolean ilIsValidBlpF(ILHANDLE File, ILstate *State)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidBlp2();
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Checks if Lump is a valid BLP lump.
ILboolean ilIsValidBlpL(const void *Lump, ILuint Size, ILstate *State)
{
	iSetInputLump(Lump, Size);
	return iIsValidBlp2();
}


// Internal function used to get the BLP header from the current file.
ILboolean iGetBlp2Head(BLP2HEAD *Header)
{
	ILuint i;

	iread(Header->Sig, 1, 4);
	Header->Type = GetLittleUInt();
	Header->Compression = igetc();
	Header->AlphaBits = igetc();
	Header->AlphaType = igetc();
	Header->HasMips = igetc();
	Header->Width = GetLittleUInt();
	Header->Height = GetLittleUInt();
	for (i = 0; i < 16; i++)
		Header->MipOffsets[i] = GetLittleUInt();
	for (i = 0; i < 16; i++)
		Header->MipLengths[i] = GetLittleUInt();

	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidBlp2(void)
{
	BLP2HEAD Header;

	if (!iGetBlp2Head(&Header))
		return IL_FALSE;
	iseek(-148, IL_SEEK_CUR);
	
	return iCheckBlp2(&Header);
}


// Internal function used to check if the HEADER is a valid BLP header.
ILboolean iCheckBlp2(BLP2HEAD *Header)
{
	// The file signature is 'BLP2'.
	if (strncmp(Header->Sig, "BLP2", 4))
		return IL_FALSE;
	// Valid types are JPEG and DXTC.  JPEG is not common, though.
	//  WoW only uses DXTC.
	if (Header->Type != BLP_TYPE_JPG && Header->Type != BLP_TYPE_DXTC_RAW)
		return IL_FALSE;
	// For BLP_TYPE_DXTC_RAW, we can have RAW and DXTC compression.
	if (Header->Compression != BLP_RAW && Header->Compression != BLP_DXTC)
		return IL_FALSE;
	// Alpha bits can only be 0, 1 and 8.
	if (Header->AlphaBits != 0 && Header->AlphaBits != 1 && Header->AlphaBits != 8)
		return IL_FALSE;
	// Alpha type can only be 0, 1, 7 and 8.
	if (Header->AlphaType != 0 && Header->AlphaType != 1 && Header->AlphaType != 7 && Header->AlphaType != 8)
		return IL_FALSE;
	// Width or height of 0 makes no sense.
	if (Header->Width == 0 || Header->Height == 0)
		return IL_FALSE;

	return IL_TRUE;
}


//! Reads a BLP file
ILboolean ilLoadBlp(ILimage *Image, ILconst_string FileName, ILstate *State)
{
	ILHANDLE	BlpFile;
	ILboolean	bBlp = IL_FALSE;
	
	BlpFile = iopenr(FileName);
	if (BlpFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bBlp;
	}

	bBlp = ilLoadBlpF(Image, BlpFile, State);
	icloser(BlpFile);

	return bBlp;
}


//! Reads an already-opened BLP file
ILboolean ilLoadBlpF(ILimage *Image, ILHANDLE File, ILstate *State)
{
	ILuint		FirstPos;
	ILboolean	bRet;
	
	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadBlpInternal(Image, State);
	iseek(FirstPos, IL_SEEK_SET);
	
	return bRet;
}


//! Reads from a memory "lump" that contains a BLP
ILboolean ilLoadBlpL(ILimage *Image, const void *Lump, ILuint Size, ILstate *State)
{
	iSetInputLump(Lump, Size);
	return iLoadBlpInternal(Image, State);
}


// Internal function used to load the BLP.
ILboolean iLoadBlpInternal(ILimage *Image, ILstate *State)
{
	BLP2HEAD	Header;
	ILubyte		*CompData;
	ILimage		*CurImage;
	ILuint		Mip, j, x, CompSize, AlphaSize, AlphaOff;
	ILint		y;
	ILboolean	BaseCreated = IL_FALSE;
	ILubyte		*DataAndAlpha = NULL, *Palette = NULL, AlphaMask; //, *JpegHeader, *JpegData;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (!iGetBlp2Head(&Header)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}
	if (!iCheckBlp2(&Header)) {
		goto check_blp1;
	}

//@TODO: Remove this!
	if (Header.Type != BLP_TYPE_DXTC_RAW)
		return IL_FALSE;

	switch (Header.Compression)
	{
		case BLP_RAW:
			for (Mip = 0; Mip < 16; Mip++) {  // Possible maximum of 16 mipmaps
				if (BaseCreated) {
					if (Header.HasMips == 0)  // Does not have mipmaps, so we are done.
						break;
					if (CurImage->Width == 1 && CurImage->Height == 1)  // Already at the smallest mipmap (1x1), so we are done.
						break;
					if (Header.MipOffsets[Mip] == 0 || Header.MipLengths == 0)  // No more mipmaps in the file.
						break;
				}

				switch (Header.AlphaBits)
				{
					case 0:
						if (!BaseCreated) {  // Have not created the base image yet, so use ilTexImage.
							if (!ilTexImage(Image, Header.Width, Header.Height, 1, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE, NULL, State))
								return IL_FALSE;
							CurImage = Image;
							BaseCreated = IL_TRUE;

							CurImage->Pal.Palette = (ILubyte*)ialloc(256 * 4);  // 256 entries of ARGB8888 values (1024).
							if (CurImage->Pal.Palette == NULL)
								return IL_FALSE;
							CurImage->Pal.PalSize = 1024;
							CurImage->Pal.PalType = IL_PAL_BGRA32;  //@TODO: Find out if this is really BGRA data.
							if (iread(CurImage->Pal.Palette, 1, 1024) != 1024)  // Read in the palette.
								return IL_FALSE;
						}
						else {
							CurImage->Mipmaps = ilNewImage(CurImage->Width >> 1, CurImage->Height >> 1, 1, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE, NULL, State);
							if (CurImage->Mipmaps == NULL)
								return IL_FALSE;

							// Copy the palette from the first image before we change our CurImage pointer.
							iCopyPalette(&CurImage->Mipmaps->Pal, &CurImage->Pal);
							// Move to the next mipmap in the linked list.
							CurImage = CurImage->Mipmaps;
						}
						// The origin should be in the upper left.
						CurImage->Origin = IL_ORIGIN_UPPER_LEFT;

						// These two should be the same (tells us how much data is in the file for this mipmap level).
						if (Header.MipLengths[Mip] != CurImage->SizeOfData) {
							ilSetError(IL_INVALID_FILE_HEADER);
							return IL_FALSE;
						}
						// Finally read in the image data.
						iseek(Header.MipOffsets[Mip], IL_SEEK_SET);
						if (iread(CurImage->Data, 1, CurImage->SizeOfData) != CurImage->SizeOfData)
							return IL_FALSE;
						break;

					case 1:
						if (!BaseCreated) {  // Have not created the base image yet, so use ilTexImage.
							if (!ilTexImage(Image, Header.Width, Header.Height, 1, IL_BGRA, IL_UNSIGNED_BYTE, NULL, State))
								return IL_FALSE;
							CurImage = Image;
							BaseCreated = IL_TRUE;

							Palette = (ILubyte*)ialloc(256 * 4);
							if (Palette == NULL)
								return IL_FALSE;

							// Read in the palette.
							if (iread(Palette, 1, 1024) != 1024) {
								ifree(Palette);
								return IL_FALSE;
							}

							// We only allocate this once and reuse this buffer with every mipmap (since successive ones are smaller).
							DataAndAlpha = (ILubyte*)ialloc(CurImage->Width * CurImage->Height);
							if (DataAndAlpha == NULL) {
								ifree(DataAndAlpha);
								ifree(Palette);
								return IL_FALSE;
							}
						}
						else {
							CurImage->Mipmaps = ilNewImage(CurImage->Width >> 1, CurImage->Height >> 1, 1, IL_BGRA, IL_UNSIGNED_BYTE, NULL, State);
							if (CurImage->Mipmaps == NULL)
								return IL_FALSE;

							// Move to the next mipmap in the linked list.
							CurImage = CurImage->Mipmaps;
						}
						// The origin should be in the upper left.
						CurImage->Origin = IL_ORIGIN_UPPER_LEFT;

						// Determine the size of the alpha data following the color indices.
						AlphaSize = CurImage->Width * CurImage->Height / 8;
						if (AlphaSize == 0)
							AlphaSize = 1;  // Should never be 0.
						// These two should be the same (tells us how much data is in the file for this mipmap level).
						if (Header.MipLengths[Mip] != CurImage->SizeOfData / 4 + AlphaSize) {
							ilSetError(IL_INVALID_FILE_HEADER);
							return IL_FALSE;
						}

						// Seek to the data and read it.
						iseek(Header.MipOffsets[Mip], IL_SEEK_SET);						
						if (iread(DataAndAlpha, CurImage->Width * CurImage->Height, 1) != 1) {
							ifree(DataAndAlpha);
							ifree(Palette);
							return IL_FALSE;
						}

						// Convert the color-indexed data to BGRX.
						for (j = 0; j < CurImage->Width * CurImage->Height; j++) {
							CurImage->Data[j*4]   = Palette[DataAndAlpha[j]*4];
							CurImage->Data[j*4+1] = Palette[DataAndAlpha[j]*4+1];
							CurImage->Data[j*4+2] = Palette[DataAndAlpha[j]*4+2];
						}

						// Read in the alpha list.
						if (iread(DataAndAlpha, AlphaSize, 1) != 1) {
							ifree(DataAndAlpha);
							ifree(Palette);
							return IL_FALSE;
						}

						AlphaMask = 0x01;  // Lowest bit
						AlphaOff = 0;
						// The really strange thing about this alpha data is that it is upside-down when compared to the
						//   regular color-indexed data, so we have to flip it.
						for (y = CurImage->Height - 1; y >= 0; y--) {
							for (x = 0; x < CurImage->Width; x++) {
								if (AlphaMask == 0) {  // Shifting it past the highest bit makes it 0, since we only have 1 byte.
									AlphaOff++;        // Move along the alpha buffer.
									AlphaMask = 0x01;  // Reset the alpha mask.
								}
								// This is just 1-bit alpha, so it is either on or off.
								CurImage->Data[CurImage->Bps * y + x * 4 + 3] = DataAndAlpha[AlphaOff] & AlphaMask ? 0xFF : 0x00;
								AlphaMask <<= 1;
							}
						}

						break;

					default:
						//@TODO: Accept any other alpha values?
						ilSetError(IL_INVALID_FILE_HEADER);
						return IL_FALSE;
				}
			}

			// Done, so we can finally free these two.
			ifree(DataAndAlpha);
			ifree(Palette);

			break;

		case BLP_DXTC:
			for (Mip = 0; Mip < 16; Mip++) {  // Possible maximum of 16 mipmaps
				//@TODO: Other formats
				//if (Header.AlphaBits == 0)
				//	if (!ilTexImage(Header.Width, Header.Height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL))
				//	return IL_FALSE;
				if (!BaseCreated) {  // Have not created the base image yet, so use ilTexImage.
					if (!ilTexImage(Image, Header.Width, Header.Height, 1, IL_RGBA, IL_UNSIGNED_BYTE, NULL, State))
						return IL_FALSE;
					CurImage = Image;
					BaseCreated = IL_TRUE;
				}
				else {
					if (Header.HasMips == 0)  // Does not have mipmaps, so we are done.
						break;
					if (CurImage->Width == 1 && CurImage->Height == 1)  // Already at the smallest mipmap (1x1), so we are done.
						break;
					if (Header.MipOffsets[Mip] == 0 || Header.MipLengths[Mip] == 0)  // No more mipmaps in the file.
						break;

					//@TODO: Other formats
					// ilNewImageFull automatically changes widths and heights of 0 to 1, so we do not have to worry about it.
					CurImage->Mipmaps = ilNewImage(CurImage->Width >> 1, CurImage->Height >> 1, 1, IL_RGBA, IL_UNSIGNED_BYTE, NULL, State);
					if (CurImage->Mipmaps == NULL)
						return IL_FALSE;
					CurImage = CurImage->Mipmaps;
				}
				// The origin should be in the upper left.
				CurImage->Origin = IL_ORIGIN_UPPER_LEFT;

				//@TODO: Only do the allocation once.
				CompData = (ILubyte*)ialloc(Header.MipLengths[Mip]);
				if (CompData == NULL)
					return IL_FALSE;

				// Read in the compressed mipmap data.
				iseek(Header.MipOffsets[Mip], IL_SEEK_SET);
				if (iread(CompData, 1, Header.MipLengths[Mip]) != Header.MipLengths[Mip]) {
					ifree(CompData);
					return IL_FALSE;
				}

				switch (Header.AlphaBits)
				{
					case 0:  // DXT1 without alpha
					case 1:  // DXT1 with alpha
						// Check to make sure that the MipLength reported is the size needed, so that
						//  DecompressDXT1 does not crash.
						CompSize = ((CurImage->Width + 3) / 4) * ((CurImage->Height + 3) / 4) * 8;
						if (CompSize != Header.MipLengths[Mip]) {
							ilSetError(IL_INVALID_FILE_HEADER);
							ifree(CompData);
							return IL_FALSE;
						}
						if (!DecompressDXT1(CurImage, CompData)) {
							ifree(CompData);
							return IL_FALSE;
						}
						break;

					case 8:
						// Check to make sure that the MipLength reported is the size needed, so that
						//  DecompressDXT3/5 do not crash.
						CompSize = ((CurImage->Width + 3) / 4) * ((CurImage->Height + 3) / 4) * 16;
						if (CompSize != Header.MipLengths[Mip]) {
							ifree(CompData);
							ilSetError(IL_INVALID_FILE_HEADER);
							return IL_FALSE;
						}
						switch (Header.AlphaType)
						{
							case 0:  // All three of
							case 1:  //  these refer to
							case 8:  //  DXT3...
								if (!DecompressDXT3(CurImage, CompData)) {
									ifree(CompData);
									return IL_FALSE;
								}
								break;

							case 7:  // DXT5 compression
								if (!DecompressDXT5(CurImage, CompData)) {
									ifree(CompData);
									return IL_FALSE;
								}
								break;

							//default:  // Should already be checked by iCheckBlp2.
						}
						break;
					//default:  // Should already be checked by iCheckBlp2.
				}
				//@TODO: Save DXTC data.
				ifree(CompData);
			}
			break;
		//default:
	}

	return ilFixImage(Image);

check_blp1:
	iseek(-148, IL_SEEK_CUR);  // Go back the size of the BLP2 header, since we tried reading it.
	return iLoadBlp1(Image, State);
}


ILboolean iGetBlp1Head(BLP1HEAD *Header)
{
	ILuint i;

	iread(Header->Sig, 1, 4);
	Header->Compression = GetLittleUInt();
	Header->Flags = GetLittleUInt();
	Header->Width = GetLittleUInt();
	Header->Height = GetLittleUInt();
	Header->PictureType = GetLittleUInt();
	Header->PictureSubType = GetLittleUInt();
	for (i = 0; i < 16; i++)
		Header->MipOffsets[i] = GetLittleUInt();
	for (i = 0; i < 16; i++)
		Header->MipLengths[i] = GetLittleUInt();

	return IL_TRUE;
}


ILboolean iCheckBlp1(BLP1HEAD *Header)
{
	// The file signature is 'BLP1'.
	if (strncmp(Header->Sig, "BLP1", 4))
		return IL_FALSE;
	// Valid types are JPEG and RAW.  JPEG is not common, though.
	if (Header->Compression != BLP_TYPE_JPG && Header->Compression != BLP_RAW)
		return IL_FALSE;
//@TODO: Find out what Flags is for.

	// PictureType determines whether this has an alpha list.
	if (Header->PictureType != BLP_RAW_PLUS_ALPHA1 && Header->PictureType != BLP_RAW_PLUS_ALPHA2
		&& Header->PictureType != BLP_RAW_NO_ALPHA)
		return IL_FALSE;
	// Width or height of 0 makes no sense.
	if (Header->Width == 0 || Header->Height == 0)
		return IL_FALSE;

	return IL_TRUE;
}


ILboolean iLoadBlp1(ILimage *Image, ILstate *State)
{
	BLP1HEAD	Header;
	ILubyte		*DataAndAlpha, *Palette;
	ILuint		i;
	ILimage		*CurImage = Image;
	ILboolean	BaseCreated = IL_FALSE;
#ifndef IL_NO_JPG
	ILubyte		*JpegHeader, *JpegData;
	ILuint		JpegHeaderSize;
#endif//IL_NO_JPG

	if (!iGetBlp1Head(&Header))
		return IL_FALSE;
	if (!iCheckBlp1(&Header)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	//@TODO: Remove this.
	i = 0;

	switch (Header.Compression)
	{
		case BLP_TYPE_JPG:
#ifdef IL_NO_JPG
			// We can only do the Jpeg decoding if we do not have IL_NO_JPEG defined.
			return IL_FALSE;
#else
			JpegHeaderSize = GetLittleUInt();
			JpegHeader = (ILubyte*)ialloc(JpegHeaderSize);
			if (JpegHeader == NULL)
				return IL_FALSE;
			// Read the shared Jpeg header.
			if (iread(JpegHeader, 1, JpegHeaderSize) != JpegHeaderSize) {
				ifree(JpegHeader);
				return IL_FALSE;
			}

			//for (i = 0; i < 16; i++) {  // Possible maximum of 16 mipmaps
				//@TODO: Check return value?
				iseek(Header.MipOffsets[i], IL_SEEK_SET);
				JpegData = (ILubyte*)ialloc(JpegHeaderSize + Header.MipLengths[i]);
				if (JpegData == NULL) {
					ifree(JpegHeader);
					return IL_FALSE;
				}
				memcpy(JpegData, JpegHeader, JpegHeaderSize);
				if (iread(JpegData + JpegHeaderSize, Header.MipLengths[i], 1) != 1)
					return IL_FALSE;

				// Just send the data straight to the Jpeg loader.
				if (!ilLoadJpegL(CurImage, JpegData, JpegHeaderSize + Header.MipLengths[i], State))
					return IL_FALSE;

				// The image data is in BGR(A) order, even though it is Jpeg-compressed.
				if (CurImage->Format == IL_RGBA)
					CurImage->Format = IL_BGRA;
				if (CurImage->Format == IL_RGB)
					CurImage->Format = IL_BGR;

				ifree(JpegData);
			//}
			ifree(JpegHeader);
#endif//IL_NO_JPG
			break;

		case BLP_RAW:
			switch (Header.PictureType)
			{
				// There is no alpha list, so we just read like a normal indexed image.
				case BLP_RAW_NO_ALPHA:
					for (i = 0; i < 16; i++) {  // Possible maximum of 16 mipmaps
						if (!BaseCreated) {  // Have not created the base image yet, so use ilTexImage.
							if (!ilTexImage(Image, Header.Width, Header.Height, 1, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE, NULL, State))
								return IL_FALSE;
							CurImage = Image;
							BaseCreated = IL_TRUE;

							// We have a BGRA palette.
							CurImage->Pal.Palette = (ILubyte*)ialloc(256 * 4);
							if (CurImage->Pal.Palette == NULL)
								return IL_FALSE;
							CurImage->Pal.PalSize = 1024;
							CurImage->Pal.PalType = IL_PAL_BGRA32;

							// Read in the palette ...
							if (iread(CurImage->Pal.Palette, 1, 1024) != 1024)
								return IL_FALSE;
						}
						else {
							if (CurImage->Width == 1 && CurImage->Height == 1)  // Already at the smallest mipmap (1x1), so we are done.
								break;
							if (Header.MipOffsets[i] == 0 || Header.MipLengths[i] == 0)  // No more mipmaps in the file.
								break;

							CurImage->Mipmaps = ilNewImage(CurImage->Width >> 1, CurImage->Height >> 1, 1, IL_COLOR_INDEX, IL_UNSIGNED_BYTE, NULL, State);
							if (CurImage->Mipmaps == NULL)
								return IL_FALSE;

							// Copy the palette from the first image before we change our CurImage pointer.
							CurImage->Mipmaps->Pal.Palette = (ILubyte*)ialloc(256 * 4);
							if (CurImage->Mipmaps->Pal.Palette == NULL)
								return IL_FALSE;
							CurImage->Mipmaps->Pal.PalSize = 1024;
							CurImage->Mipmaps->Pal.PalType = IL_PAL_BGRA32;
							memcpy(CurImage->Mipmaps->Pal.Palette, CurImage->Pal.Palette, 1024);

							// Move to the next mipmap in the linked list.
							CurImage = CurImage->Mipmaps;
						}
						// The origin should be in the upper left.
						CurImage->Origin = IL_ORIGIN_UPPER_LEFT;

						// Seek to the data and read it.
						iseek(Header.MipOffsets[i], IL_SEEK_SET);						
						if (iread(CurImage->Data, 1, CurImage->SizeOfData) != CurImage->SizeOfData)
							return IL_FALSE;
					}
					break;

				// These cases are identical and have an alpha list following the image data.
				case BLP_RAW_PLUS_ALPHA1:
				case BLP_RAW_PLUS_ALPHA2:
					// Create the image.
					if (!ilTexImage(Image, Header.Width, Header.Height, 1, IL_BGRA, IL_UNSIGNED_BYTE, NULL, State))
						return IL_FALSE;

					DataAndAlpha = (ILubyte*)ialloc(Header.Width * Header.Height);
					Palette = (ILubyte*)ialloc(256 * 4);
					if (DataAndAlpha == NULL || Palette == NULL) {
						ifree(DataAndAlpha);
						ifree(Palette);
						return IL_FALSE;
					}

					// Read in the data and the palette.
					if (iread(Palette, 1, 1024) != 1024) {
						ifree(Palette);
						return IL_FALSE;
					}
					// Seek to the data and read it.
					iseek(Header.MipOffsets[i], IL_SEEK_SET);						
					if (iread(DataAndAlpha, Header.Width * Header.Height, 1) != 1) {
						ifree(DataAndAlpha);
						ifree(Palette);
						return IL_FALSE;
					}

					// Convert the color-indexed data to BGRX.
					for (i = 0; i < Header.Width * Header.Height; i++) {
						CurImage->Data[i*4]   = Palette[DataAndAlpha[i]*4];
						CurImage->Data[i*4+1] = Palette[DataAndAlpha[i]*4+1];
						CurImage->Data[i*4+2] = Palette[DataAndAlpha[i]*4+2];
					}

					// Read in the alpha list.
					if (iread(DataAndAlpha, Header.Width * Header.Height, 1) != 1) {
						ifree(DataAndAlpha);
						ifree(Palette);
						return IL_FALSE;
					}
					// Finally put the alpha data into the image data.
					for (i = 0; i < Header.Width * Header.Height; i++) {
						CurImage->Data[i*4+3] = DataAndAlpha[i];
					}

					ifree(DataAndAlpha);
					ifree(Palette);
					break;
			}
			break;

		//default:  // Should already be checked by iCheckBlp1.
	}

	// Set the origin (always upper left).
	CurImage->Origin = IL_ORIGIN_UPPER_LEFT;

	return ilFixImage(Image);
}


#endif//IL_NO_BLP
