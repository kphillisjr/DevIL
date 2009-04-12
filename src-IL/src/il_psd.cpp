//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 04/03/2009
//
// Filename: src-IL/src/il_psd.cpp
//
// Description: Reads and writes Photoshop (.psd) files.
//
//-----------------------------------------------------------------------------


// Information about the .psd format was taken from Adobe's PhotoShop SDK at
//  http://partners.adobe.com/asn/developer/gapsdk/PhotoshopSDK.html
//  Information about the Packbits compression scheme was found at
//	http://partners.adobe.com/asn/developer/PDFS/TN/TIFF6.pdf

#include "il_internal.h"
#ifndef IL_NO_PSD
#include "il_psd.h"

static float ubyte_to_float(ILubyte val)
{
	return ((float)val) / 255.0f;
}
static float ushort_to_float(ILushort val)
{
	return ((float)val) / 65535.0f;
}

static ILubyte float_to_ubyte(float val)
{
	return (ILubyte)(val * 255.0f);
}
static ILushort float_to_ushort(float val)
{
	return (ILushort)(val * 65535.0f);
}


//! Checks if the file specified in FileName is a valid Psd file.
ILboolean ilIsValidPsd(ILconst_string FileName)
{
	ILHANDLE	PsdFile;
	ILboolean	bPsd = IL_FALSE;

	if (!iCheckExtension(FileName, IL_TEXT("psd")) &&
		!iCheckExtension(FileName, IL_TEXT("pdd"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return bPsd;
	}

	PsdFile = iopenr(FileName);
	if (PsdFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bPsd;
	}

	bPsd = ilIsValidPsdF(PsdFile);
	icloser(PsdFile);

	return bPsd;
}


//! Checks if the ILHANDLE contains a valid Psd file at the current position.
ILboolean ilIsValidPsdF(ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iIsValidPsd();
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Checks if Lump is a valid Psd lump.
ILboolean ilIsValidPsdL(const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iIsValidPsd();
}


// Internal function used to get the Psd header from the current file.
ILboolean iGetPsdHead(PSDHEAD *Header)
{
	iread(Header->Signature, 1, 4);
	Header->Version = GetBigUShort();
	iread(Header->Reserved, 1, 6);
	Header->Channels = GetBigUShort();
	Header->Height = GetBigUInt();
	Header->Width = GetBigUInt();
	Header->Depth = GetBigUShort();
	Header->Mode = GetBigUShort();

	return IL_TRUE;
}


// Internal function to get the header and check it.
ILboolean iIsValidPsd()
{
	PSDHEAD	Head;

	iGetPsdHead(&Head);
	iseek(-(ILint)sizeof(PSDHEAD), IL_SEEK_CUR);

	return iCheckPsd(&Head);
}


// Internal function used to check if the HEADER is a valid Psd header.
ILboolean iCheckPsd(PSDHEAD *Header)
{
	ILuint i;

	if (strncmp((char*)Header->Signature, "8BPS", 4))
		return IL_FALSE;
	if (Header->Version != 1)
		return IL_FALSE;
	for (i = 0; i < 6; i++) {
		if (Header->Reserved[i] != 0)
			return IL_FALSE;
	}
	if (Header->Channels < 1 || Header->Channels > 24)
		return IL_FALSE;
	if (Header->Height < 1 || Header->Width < 1)
		return IL_FALSE;
	if (Header->Depth != 1 && Header->Depth != 8 && Header->Depth != 16)
		return IL_FALSE;

	return IL_TRUE;
}


//! Reads a Psd file
ILboolean ilLoadPsd(ILimage *Image, ILconst_string FileName)
{
	ILHANDLE	PsdFile;
	ILboolean	bPsd = IL_FALSE;

	PsdFile = iopenr(FileName);
	if (PsdFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return bPsd;
	}

	bPsd = ilLoadPsdF(Image, PsdFile);
	icloser(PsdFile);

	return bPsd;
}


//! Reads an already-opened Psd file
ILboolean ilLoadPsdF(ILimage *Image, ILHANDLE File)
{
	ILuint		FirstPos;
	ILboolean	bRet;

	iSetInputFile(File);
	FirstPos = itell();
	bRet = iLoadPsdInternal(Image);
	iseek(FirstPos, IL_SEEK_SET);

	return bRet;
}


//! Reads from a memory "lump" that contains a Psd
ILboolean ilLoadPsdL(ILimage *Image, const void *Lump, ILuint Size)
{
	iSetInputLump(Lump, Size);
	return iLoadPsdInternal(Image);
}


// Internal function used to load the Psd.
ILboolean iLoadPsdInternal(ILimage *Image)
{
	PSDHEAD	Header;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	iGetPsdHead(&Header);
	if (!iCheckPsd(&Header)) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}

	if (!ReadPsd(Image, &Header))
		return IL_FALSE;
	Image->Origin = IL_ORIGIN_UPPER_LEFT;

	return ilFixImage(Image);
}


ILboolean ReadPsd(ILimage *Image, PSDHEAD *Head)
{
	switch (Head->Mode)
	{
		case 1:  // Greyscale
			return ReadGrey(Image, Head);
		case 2:  // Indexed
			return ReadIndexed(Image, Head);
		case 3:  // RGB
			return ReadRGB(Image, Head);
		case 4:  // CMYK
			return ReadCMYK(Image, Head);
	}

	ilSetError(IL_FORMAT_NOT_SUPPORTED);
	return IL_FALSE;
}


ILboolean ReadGrey(ILimage *Image, PSDHEAD *Head)
{
	ILuint		ColorMode, ResourceSize, MiscInfo;
	ILushort	Compressed;
	ILenum		Type;
	ILubyte		*Resources = NULL;

	ColorMode = GetBigUInt();  // Skip over the 'color mode data section'
	iseek(ColorMode, IL_SEEK_CUR);

	ResourceSize = GetBigUInt();  // Read the 'image resources section'
	Resources = (ILubyte*)ialloc(ResourceSize);
	if (Resources == NULL) {
		return IL_FALSE;
	}
	if (iread(Resources, 1, ResourceSize) != ResourceSize)
		goto cleanup_error;

	MiscInfo = GetBigUInt();
	iseek(MiscInfo, IL_SEEK_CUR);

	Compressed = GetBigUShort();

	ChannelNum = Head->Channels;
	Head->Channels = 1;  // Temporary to read only one channel...some greyscale .psd files have 2.
	if (Head->Channels != 1) {
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}
	switch (Head->Depth)
	{
		case 8:
			Type = IL_UNSIGNED_BYTE;
			break;
		case 16:
			Type = IL_UNSIGNED_SHORT;
			break;
		default:
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return IL_FALSE;
	}

	if (!ilTexImage(Image, Head->Width, Head->Height, 1, IL_LUMINANCE, Type, NULL))
		goto cleanup_error;
	if (!PsdGetData(Image, Head, Image->Data, (ILboolean)Compressed))
		goto cleanup_error;
	if (!ParseResources(Image, ResourceSize, Resources))
		goto cleanup_error;
	ifree(Resources);

	return IL_TRUE;

cleanup_error:
	ifree(Resources);
	return IL_FALSE;
}


ILboolean ReadIndexed(ILimage *Image, PSDHEAD *Head)
{
	ILuint		ColorMode, ResourceSize, MiscInfo, i, j, NumEnt;
	ILushort	Compressed;
	ILubyte		*Palette = NULL, *Resources = NULL;

	ColorMode = GetBigUInt();  // Skip over the 'color mode data section'
	if (ColorMode % 3 != 0) {
		ilSetError(IL_INVALID_FILE_HEADER);
		return IL_FALSE;
	}
	Palette = (ILubyte*)ialloc(ColorMode);
	if (Palette == NULL)
		return IL_FALSE;
	if (iread(Palette, 1, ColorMode) != ColorMode)
		goto cleanup_error;

	ResourceSize = GetBigUInt();  // Read the 'image resources section'
	Resources = (ILubyte*)ialloc(ResourceSize);
	if (Resources == NULL) {
		return IL_FALSE;
	}
	if (iread(Resources, 1, ResourceSize) != ResourceSize)
		goto cleanup_error;

	MiscInfo = GetBigUInt();
	if (ieof())
		goto cleanup_error;
	iseek(MiscInfo, IL_SEEK_CUR);

	Compressed = GetBigUShort();
	if (ieof())
		goto cleanup_error;

	if (Head->Channels != 1 || Head->Depth != 8) {
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		goto cleanup_error;
	}
	ChannelNum = Head->Channels;

	if (!ilTexImage(Image, Head->Width, Head->Height, 1, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE, NULL))
		goto cleanup_error;

	Image->Pal.Palette = (ILubyte*)ialloc(ColorMode);
	if (Image->Pal.Palette == NULL) {
		goto cleanup_error;
	}
	Image->Pal.PalSize = ColorMode;
	Image->Pal.PalType = IL_PAL_RGB24;

	NumEnt = Image->Pal.PalSize / 3;
	for (i = 0, j = 0; i < Image->Pal.PalSize; i += 3, j++) {
		Image->Pal.Palette[i  ] = Palette[j];
		Image->Pal.Palette[i+1] = Palette[j+NumEnt];
		Image->Pal.Palette[i+2] = Palette[j+NumEnt*2];
	}
	ifree(Palette);
	Palette = NULL;

	if (!PsdGetData(Image, Head, Image->Data, (ILboolean)Compressed))
		goto cleanup_error;

	ParseResources(Image, ResourceSize, Resources);
	ifree(Resources);
	Resources = NULL;

	return IL_TRUE;

cleanup_error:
	ifree(Palette);
	ifree(Resources);

	return IL_FALSE;
}


ILboolean ReadRGB(ILimage *Image, PSDHEAD *Head)
{
	ILuint		ColorMode, ResourceSize, MiscInfo;
	ILushort	Compressed;
	ILenum		Format, Type;
	ILubyte		*Resources = NULL;

	ColorMode = GetBigUInt();  // Skip over the 'color mode data section'
	iseek(ColorMode, IL_SEEK_CUR);

	ResourceSize = GetBigUInt();  // Read the 'image resources section'
	Resources = (ILubyte*)ialloc(ResourceSize);
	if (Resources == NULL)
		return IL_FALSE;
	if (iread(Resources, 1, ResourceSize) != ResourceSize)
		goto cleanup_error;

	MiscInfo = GetBigUInt();
	iseek(MiscInfo, IL_SEEK_CUR);

	Compressed = GetBigUShort();

	ChannelNum = Head->Channels;
	if (Head->Channels == 3)
 	{
		Format = IL_RGB;
	}
	else if (Head->Channels == 4)
	{
		Format = IL_RGBA;
	}
	else if (Head->Channels >= 5)
	{
		// Additional channels are accumulated as a single alpha channel, since
		// if an image does not have a layer set as the "background", but also
		// has a real alpha channel, there will be 5 channels (or more).
		Format = IL_RGBA;
	}
	else
	{
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}

	switch (Head->Depth)
	{
		case 8:
			Type = IL_UNSIGNED_BYTE;
			break;
		case 16:
			Type = IL_UNSIGNED_SHORT;
			break;
		default:
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return IL_FALSE;
	}
	if (!ilTexImage(Image, Head->Width, Head->Height, 1, Format, Type, NULL))
		goto cleanup_error;
	if (!PsdGetData(Image, Head, Image->Data, (ILboolean)Compressed))
		goto cleanup_error;
	if (!ParseResources(Image, ResourceSize, Resources))
		goto cleanup_error;
	ifree(Resources);

	return IL_TRUE;

cleanup_error:
	ifree(Resources);
	return IL_FALSE;
}


ILboolean ReadCMYK(ILimage *Image, PSDHEAD *Head)
{
	ILuint		ColorMode, ResourceSize, MiscInfo, Size, i, j;
	ILushort	Compressed;
	ILenum		Format, Type;
	ILubyte		*Resources = NULL, *KChannel = NULL;

	ColorMode = GetBigUInt();  // Skip over the 'color mode data section'
	iseek(ColorMode, IL_SEEK_CUR);

	ResourceSize = GetBigUInt();  // Read the 'image resources section'
	Resources = (ILubyte*)ialloc(ResourceSize);
	if (Resources == NULL) {
		return IL_FALSE;
	}
	if (iread(Resources, 1, ResourceSize) != ResourceSize)
		goto cleanup_error;

	MiscInfo = GetBigUInt();
	iseek(MiscInfo, IL_SEEK_CUR);

	Compressed = GetBigUShort();

	switch (Head->Channels)
	{
		case 4:
			Format = IL_RGB;
			ChannelNum = 4;
			Head->Channels = 3;
			break;
		case 5:
			Format = IL_RGBA;
			ChannelNum = 5;
			Head->Channels = 4;
			break;
		default:
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return IL_FALSE;
	}
	switch (Head->Depth)
	{
		case 8:
			Type = IL_UNSIGNED_BYTE;
			break;
		case 16:
			Type = IL_UNSIGNED_SHORT;
			break;
		default:
			ilSetError(IL_FORMAT_NOT_SUPPORTED);
			return IL_FALSE;
	}
	if (!ilTexImage(Image, Head->Width, Head->Height, 1, Format, Type, NULL))
		goto cleanup_error;
	if (!PsdGetData(Image, Head, Image->Data, (ILboolean)Compressed))
		goto cleanup_error;

	Size = Image->Bpc * Image->Width * Image->Height;
	KChannel = (ILubyte*)ialloc(Size);
	if (KChannel == NULL)
		goto cleanup_error;
	if (!GetSingleChannel(Image, Head, KChannel, (ILboolean)Compressed))
		goto cleanup_error;

	if (Format == IL_RGB) {
		for (i = 0, j = 0; i < Image->SizeOfData; i += 3, j++) {
			Image->Data[i  ] = (Image->Data[i  ] * KChannel[j]) >> 8;
			Image->Data[i+1] = (Image->Data[i+1] * KChannel[j]) >> 8;
			Image->Data[i+2] = (Image->Data[i+2] * KChannel[j]) >> 8;
		}
	}
	else {  // IL_RGBA
		// The KChannel array really holds the alpha channel on this one.
		for (i = 0, j = 0; i < Image->SizeOfData; i += 4, j++) {
			Image->Data[i  ] = (Image->Data[i  ] * Image->Data[i+3]) >> 8;
			Image->Data[i+1] = (Image->Data[i+1] * Image->Data[i+3]) >> 8;
			Image->Data[i+2] = (Image->Data[i+2] * Image->Data[i+3]) >> 8;
			Image->Data[i+3] = KChannel[j];  // Swap 'K' with alpha channel.
		}
	}

	if (!ParseResources(Image, ResourceSize, Resources))
		goto cleanup_error;

	ifree(Resources);
	ifree(KChannel);

	return IL_TRUE;

cleanup_error:
	ifree(Resources);
	ifree(KChannel);
	return IL_FALSE;
}


ILuint *GetCompChanLen(PSDHEAD *Head)
{
	ILushort	*RleTable;
	ILuint		*ChanLen, c, i, j;

	RleTable = (ILushort*)ialloc(Head->Height * ChannelNum * sizeof(ILushort));
	ChanLen = (ILuint*)ialloc(ChannelNum * sizeof(ILuint));
	if (RleTable == NULL || ChanLen == NULL) {
		return NULL;
	}

	if (iread(RleTable, sizeof(ILushort), Head->Height * ChannelNum) != Head->Height * ChannelNum) {
		ifree(RleTable);
		ifree(ChanLen);
		return NULL;
	}
#ifdef __LITTLE_ENDIAN__
	for (i = 0; i < Head->Height * ChannelNum; i++) {
		iSwapUShort(&RleTable[i]);
	}
#endif

	imemclear(ChanLen, ChannelNum * sizeof(ILuint));
	for (c = 0; c < ChannelNum; c++) {
		j = c * Head->Height;
		for (i = 0; i < Head->Height; i++) {
			ChanLen[c] += RleTable[i + j];
		}
	}

	ifree(RleTable);

	return ChanLen;
}



static const ILuint READ_COMPRESSED_SUCCESS					= 0;
static const ILuint READ_COMPRESSED_ERROR_FILE_CORRUPT		= 1;
static const ILuint READ_COMPRESSED_ERROR_FILE_READ_ERROR	= 2;

static ILuint ReadCompressedChannel(const ILuint ChanLen, ILuint Size, ILubyte* Channel)
{
	ILuint		i;
	ILint		Run;
	ILboolean	PreCache = IL_FALSE;
	ILbyte		HeadByte;

	if (iGetHint(IL_MEM_SPEED_HINT) == IL_FASTEST)
		PreCache = IL_TRUE;

	if (PreCache)
		iPreCache(ChanLen);
	for (i = 0; i < Size; ) {
		HeadByte = igetc();

		if (HeadByte >= 0) {  //  && HeadByte <= 127
			if (i + HeadByte > Size)
			{
				if (PreCache)
					iUnCache();
				return READ_COMPRESSED_ERROR_FILE_CORRUPT;
			}
			if (iread(Channel + i, HeadByte + 1, 1) != 1)
			{
				if (PreCache)
					iUnCache();
				return READ_COMPRESSED_ERROR_FILE_READ_ERROR;
			}

			i += HeadByte + 1;
		}
		if (HeadByte >= -127 && HeadByte <= -1) {
			Run = igetc();
			if (Run == IL_EOF)
			{
				if (PreCache)
					iUnCache();
				return READ_COMPRESSED_ERROR_FILE_READ_ERROR;
			}
			if (i + (-HeadByte + 1) > Size)
			{
				if (PreCache)
					iUnCache();
				return READ_COMPRESSED_ERROR_FILE_CORRUPT;
			}

			memset(Channel + i, Run, -HeadByte + 1);
			i += -HeadByte + 1;
		}
		if (HeadByte == -128)
		{ }  // Noop
	}
	if (PreCache)
		iUnCache();

	return READ_COMPRESSED_SUCCESS;
}


ILboolean PsdGetData(ILimage *Image, PSDHEAD *Head, void *Buffer, ILboolean Compressed)
{
	ILuint		c, x, y, i, Size, ReadResult, NumChan;
	ILubyte		*Channel = NULL;
	ILushort	*ShortPtr;
	ILuint		*ChanLen = NULL;

	// Added 01-07-2009: This is needed to correctly load greyscale and
	//  paletted images.
	switch (Head->Mode)
	{
		case 1:
		case 2:
			NumChan = 1;
			break;
		default:
			NumChan = 3;
	}

	Channel = (ILubyte*)ialloc(Head->Width * Head->Height * Image->Bpc);
	if (Channel == NULL) {
		return IL_FALSE;
	}
	ShortPtr = (ILushort*)Channel;

	// @TODO: Add support for this in, though I have yet to run across a .psd
	//	file that uses this.
	if (Compressed && Image->Type == IL_UNSIGNED_SHORT) {
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}

	if (!Compressed) {
		if (Image->Bpc == 1) {
			for (c = 0; c < NumChan; c++) {
				i = 0;
				if (iread(Channel, Head->Width * Head->Height, 1) != 1) {
					ifree(Channel);
					return IL_FALSE;
				}
				for (y = 0; y < Head->Height * Image->Bps; y += Image->Bps) {
					for (x = 0; x < Image->Bps; x += Image->Bpp, i++) {
						Image->Data[y + x + c] = Channel[i];
					}
				}
			}
			// Accumulate any remaining channels into a single alpha channel
			//@TODO: This needs to be changed for greyscale images.
			for (; c < Head->Channels; c++) {
				i = 0;
				if (iread(Channel, Head->Width * Head->Height, 1) != 1) {
					ifree(Channel);
					return IL_FALSE;
				}
				for (y = 0; y < Head->Height * Image->Bps; y += Image->Bps) {
					for (x = 0; x < Image->Bps; x += Image->Bpp, i++) {
						float curVal = ubyte_to_float(Image->Data[y + x + 3]);
						float newVal = ubyte_to_float(Channel[i]);
						Image->Data[y + x + 3] = float_to_ubyte(curVal * newVal);
					}
				}
			}
		}
		else {  // Image->Bpc == 2
			for (c = 0; c < NumChan; c++) {
				i = 0;
				if (iread(Channel, Head->Width * Head->Height * 2, 1) != 1) {
					ifree(Channel);
					return IL_FALSE;
				}
				Image->Bps /= 2;
				for (y = 0; y < Head->Height * Image->Bps; y += Image->Bps) {
					for (x = 0; x < Image->Bps; x += Image->Bpp, i++) {
					 #ifndef WORDS_BIGENDIAN
						iSwapUShort(ShortPtr+i);
					 #endif
						((ILushort*)Image->Data)[y + x + c] = ShortPtr[i];
					}
				}
				Image->Bps *= 2;
			}
			// Accumulate any remaining channels into a single alpha channel
			//@TODO: This needs to be changed for greyscale images.
			for (; c < Head->Channels; c++) {
				i = 0;
				if (iread(Channel, Head->Width * Head->Height * 2, 1) != 1) {
					ifree(Channel);
					return IL_FALSE;
				}
				Image->Bps /= 2;
				for (y = 0; y < Head->Height * Image->Bps; y += Image->Bps) {
					for (x = 0; x < Image->Bps; x += Image->Bpp, i++) {
						float curVal = ushort_to_float(((ILushort*)Image->Data)[y + x + 3]);
						float newVal = ushort_to_float(ShortPtr[i]);
						((ILushort*)Image->Data)[y + x + 3] = float_to_ushort(curVal * newVal);
					}
				}
				Image->Bps *= 2;
			}
		}
	}
	else {
		ChanLen = GetCompChanLen(Head);

		Size = Head->Width * Head->Height;
		for (c = 0; c < NumChan; c++) {
			ReadResult = ReadCompressedChannel(ChanLen[c], Size, Channel);
			if (ReadResult == READ_COMPRESSED_ERROR_FILE_CORRUPT)
				goto file_corrupt;
			else if (ReadResult == READ_COMPRESSED_ERROR_FILE_READ_ERROR)
				goto file_read_error;

			i = 0;
			for (y = 0; y < Head->Height * Image->Bps; y += Image->Bps) {
				for (x = 0; x < Image->Bps; x += Image->Bpp, i++) {
					Image->Data[y + x + c] = Channel[i];
				}
			}
		}

		// Initialize the alpha channel to solid
		//@TODO: This needs to be changed for greyscale images.
		if (Head->Channels >= 4) {
			for (y = 0; y < Head->Height * Image->Bps; y += Image->Bps) {
				for (x = 0; x < Image->Bps; x += Image->Bpp) {
					Image->Data[y + x + 3] = 255;
				}
			}
					
			for (; c < Head->Channels; c++) {
				ReadResult = ReadCompressedChannel(ChanLen[c], Size, Channel);
				if (ReadResult == READ_COMPRESSED_ERROR_FILE_CORRUPT)
					goto file_corrupt;
				else if (ReadResult == READ_COMPRESSED_ERROR_FILE_READ_ERROR)
					goto file_read_error;

				i = 0;
				for (y = 0; y < Head->Height * Image->Bps; y += Image->Bps) {
					for (x = 0; x < Image->Bps; x += Image->Bpp, i++) {
						float curVal = ubyte_to_float(Image->Data[y + x + 3]);
						float newVal = ubyte_to_float(Channel[i]);
						Image->Data[y + x + 3] = float_to_ubyte(curVal * newVal);
					}
				}
			}
		}

		ifree(ChanLen);
	}

	ifree(Channel);

	return IL_TRUE;

file_corrupt:
	ifree(ChanLen);
	ifree(Channel);
	ilSetError(IL_ILLEGAL_FILE_VALUE);
	return IL_FALSE;

file_read_error:
	ifree(ChanLen);
	ifree(Channel);
	return IL_FALSE;
}


ILboolean ParseResources(ILimage *Image, ILuint ResourceSize, ILubyte *Resources)
{
	ILushort	ID;
	ILubyte		NameLen;
	ILuint		Size;

	if (Resources == NULL) {
		ilSetError(IL_INTERNAL_ERROR);
		return IL_FALSE;
	}

	while (ResourceSize > 13) {  // Absolutely has to be larger than this.
		if (strncmp("8BIM", (const char*)Resources, 4)) {
			//return IL_FALSE;
			return IL_TRUE;  // 05-30-2002: May not necessarily mean corrupt data...
		}
		Resources += 4;

		ID = *((ILushort*)Resources);
		BigUShort(&ID);
		Resources += 2;

		NameLen = *Resources++;
		// NameLen + the byte it occupies must be padded to an even number, so NameLen must be odd.
		NameLen = NameLen + (NameLen & 1 ? 0 : 1);
		Resources += NameLen;

		// Get the resource data size.
		Size = *((ILuint*)Resources);
		BigUInt(&Size);
		Resources += 4;

		ResourceSize -= (4 + 2 + 1 + NameLen + 4);

		switch (ID)
		{
			case 0x040F:  // ICC Profile
				if (Size > ResourceSize) {  // Check to make sure we are not going past the end of Resources.
					ilSetError(IL_ILLEGAL_FILE_VALUE);
					return IL_FALSE;
				}
				Image->Profile = (ILubyte*)ialloc(Size);
				if (Image->Profile == NULL) {
					return IL_FALSE;
				}
				memcpy(Image->Profile, Resources, Size);
				Image->ProfileSize = Size;
				break;

			default:
				break;
		}

		if (Size & 1)  // Must be an even number.
			Size++;
		ResourceSize -= Size;
		Resources += Size;
	}

	return IL_TRUE;
}


ILboolean GetSingleChannel(ILimage *Image, PSDHEAD *Head, ILubyte *Buffer, ILboolean Compressed)
{
	ILuint		i;
	ILushort	*ShortPtr;
	ILbyte		HeadByte;
	ILint		Run;

	ShortPtr = (ILushort*)Buffer;

	if (!Compressed) {
		if (Image->Bpc == 1) {
			if (iread(Buffer, Head->Width * Head->Height, 1) != 1)
				return IL_FALSE;
		}
		else {  // Image->Bpc == 2
			if (iread(Buffer, Head->Width * Head->Height * 2, 1) != 1)
				return IL_FALSE;
		}
	}
	else {
		for (i = 0; i < Head->Width * Head->Height; ) {
			HeadByte = igetc();

			if (HeadByte >= 0) {  //  && HeadByte <= 127
				if (iread(Buffer + i, HeadByte + 1, 1) != 1)
					return IL_FALSE;
				i += HeadByte + 1;
			}
			if (HeadByte >= -127 && HeadByte <= -1) {
				Run = igetc();
				if (Run == IL_EOF)
					return IL_FALSE;
				memset(Buffer + i, Run, -HeadByte + 1);
				i += -HeadByte + 1;
			}
			if (HeadByte == -128)
			{ }  // Noop
		}
	}

	return IL_TRUE;
}



//! Writes a Psd file
ILboolean ilSavePsd(ILimage *Image, const ILstring FileName)
{
	ILHANDLE	PsdFile;
	ILuint		PsdSize;

	PsdFile = iopenw(FileName);
	if (PsdFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return IL_FALSE;
	}

	PsdSize = ilSavePsdF(Image, PsdFile);
	iclosew(PsdFile);

	if (PsdSize == 0)
		return IL_FALSE;
	return IL_TRUE;
}


//! Writes a Psd to an already-opened file
ILuint ilSavePsdF(ILimage *Image, ILHANDLE File)
{
	ILuint Pos;
	iSetOutputFile(File);
	Pos = itellw();
	if (iSavePsdInternal(Image) == IL_FALSE)
		return 0;  // Error occurred
	return itellw() - Pos;  // Return the number of bytes written.
}


//! Writes a Psd to a memory "lump"
ILuint ilSavePsdL(ILimage *Image, void *Lump, ILuint Size)
{
	ILuint Pos;
	iSetOutputLump(Lump, Size);
	Pos = itellw();
	if (iSavePsdInternal(Image) == IL_FALSE)
		return 0;  // Error occurred
	return itellw() - Pos;  // Return the number of bytes written.
}


// Internal function used to save the Psd.
ILboolean iSavePsdInternal(ILimage *Image)
{
	ILubyte		*Signature = (ILubyte*)"8BPS";
	ILimage		*TempImage;
	ILpal		*TempPal;
	ILuint		c, i;
	ILubyte		*TempData;
	ILushort	*ShortPtr;
	ILenum		Format, Type;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Format = Image->Format;
	Type = Image->Type;

	// All of these comprise the actual signature.
	iwrite(Signature, 1, 4);
	SaveBigShort(1);
	SaveBigInt(0);
	SaveBigShort(0);

	SaveBigShort(Image->Bpp);
	SaveBigInt(Image->Height);
	SaveBigInt(Image->Width);
	if (Image->Bpc > 2)
		Type = IL_UNSIGNED_SHORT;

	if (Image->Format == IL_BGR)
		Format = IL_RGB;
	else if (Image->Format == IL_BGRA)
		Format = IL_RGBA;

	if (Format != Image->Format || Type != Image->Type) {
		TempImage = iConvertImage(Image, Format, Type);
		if (TempImage == NULL)
			return IL_FALSE;
	}
	else {
		TempImage = Image;
	}
	SaveBigShort((ILushort)(TempImage->Bpc * 8));

	// @TODO:  Put the other formats here.
	switch (TempImage->Format)
	{
		case IL_COLOUR_INDEX:
			SaveBigShort(2);
			break;
		case IL_LUMINANCE:
			SaveBigShort(1);
			break;
		case IL_RGB:
		case IL_RGBA:
			SaveBigShort(3);
			break;
		default:
			ilSetError(IL_INTERNAL_ERROR);
			return IL_FALSE;
	}

	if (TempImage->Format == IL_COLOUR_INDEX) {
		// @TODO: We're currently making a potentially fatal assumption that
		//	iConvertImage was not called if the format is IL_COLOUR_INDEX.
		TempPal = iConvertPal(&TempImage->Pal, IL_PAL_RGB24);
		if (TempPal == NULL)
			return IL_FALSE;
		SaveBigInt(768);

		// Have to save the palette in a planar format.
		for (c = 0; c < 3; c++) {
			for (i = c; i < TempPal->PalSize; i += 3) {
				iputc(TempPal->Palette[i]);
			}
		}

		ifree(TempPal->Palette);
	}
	else {
		SaveBigInt(0);  // No colour mode data.
	}

	SaveBigInt(0);  // No image resources.
	SaveBigInt(0);  // No layer information.
	SaveBigShort(0);  // Psd data, no compression.

	// @TODO:  Add RLE compression.

	if (TempImage->Origin == IL_ORIGIN_LOWER_LEFT) {
		TempData = iGetFlipped(TempImage);
		if (TempData == NULL) {
			ilCloseImage(TempImage);
			return IL_FALSE;
		}
	}
	else {
		TempData = TempImage->Data;
	}

	if (TempImage->Bpc == 1) {
		for (c = 0; c < TempImage->Bpp; c++) {
			for (i = c; i < TempImage->SizeOfPlane; i += TempImage->Bpp) {
				iputc(TempData[i]);
			}
		}
	}
	else {  // TempImage->Bpc == 2
		ShortPtr = (ILushort*)TempData;
		TempImage->SizeOfPlane /= 2;
		for (c = 0; c < TempImage->Bpp; c++) {
			for (i = c; i < TempImage->SizeOfPlane; i += TempImage->Bpp) {
				SaveBigUShort(ShortPtr[i]);
			}
		}
		TempImage->SizeOfPlane *= 2;
	}

	if (TempData != TempImage->Data)
		ifree(TempData);

	if (TempImage != Image)
		ilCloseImage(TempImage);


	return IL_TRUE;
}


#endif//IL_NO_PSD
