//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 03/13/2009
//
// Filename: src-IL/src/il_header.cpp
//
// Description: Generates a C-style header file for the current image.
//
//-----------------------------------------------------------------------------

#ifndef IL_NO_CHEAD

#include "il_internal.h"

// Just a guess...let's see what's purty!
#define MAX_LINE_WIDTH 14

//! Generates a C-style header file for the current image.
ILboolean ilSaveCHeader(ILimage *Image, ILconst_string FileName, char *InternalName)
{
	FILE		*HeadFile;
	ILuint		i = 0, j;
	ILimage		*TempImage;
	const char	*Name;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Name = iGetString(IL_CHEAD_HEADER_STRING);
	if (Name == NULL)
		Name = InternalName;

	if (FileName == NULL || Name == NULL ||
		ilStrLen(FileName) < 1 || ilCharStrLen(Name) < 1) {
		ilSetError(IL_INVALID_VALUE);
		return IL_FALSE;
	}

	if (!iCheckExtension(FileName, IL_TEXT("h"))) {
		ilSetError(IL_INVALID_EXTENSION);
		return IL_FALSE;
	}

	if (ilGetBoolean(IL_FILE_MODE) == IL_FALSE) {
		if (iFileExists(FileName)) {
			ilSetError(IL_FILE_ALREADY_EXISTS);
            return IL_FALSE;
		}
	}

	if (Image->Bpc > 1) {
		TempImage = iConvertImage(Image, Image->Format, IL_UNSIGNED_BYTE);
		if (TempImage == NULL)
           return IL_FALSE;
	} else {
		TempImage = Image;
	}

#ifndef _UNICODE
	HeadFile = fopen(FileName, "wb");
#else
	#ifdef _WIN32
		HeadFile = _wfopen(FileName, L"rb");
	#else
		HeadFile = fopen((char*)FileName, "rb");
	#endif

#endif//_UNICODE

	if (HeadFile == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
        return IL_FALSE;
	}

	fprintf(HeadFile, "//#include <il/il.h>\n");
	fprintf(HeadFile, "// C Image Header:\n\n\n");
	fprintf(HeadFile, "// IMAGE_BPP is in bytes per pixel, *not* bits\n");
    fprintf(HeadFile, "#define IMAGE_BPP %d\n",Image->Bpp);
	fprintf(HeadFile, "#define IMAGE_WIDTH   %d\n", Image->Width);
	fprintf(HeadFile, "#define IMAGE_HEIGHT  %d\n", Image->Height);	
	fprintf(HeadFile, "#define IMAGE_DEPTH   %d\n\n\n", Image->Depth);
	fprintf(HeadFile, "#define IMAGE_TYPE    0x%X\n", Image->Type);
	fprintf(HeadFile, "#define IMAGE_FORMAT  0x%X\n\n\n", Image->Format);
    fprintf(HeadFile, "ILubyte %s[] = {\n", Name);
        

	for (; i < TempImage->SizeOfData; i += MAX_LINE_WIDTH) {
		fprintf(HeadFile, "\t");
		for (j = 0; j < MAX_LINE_WIDTH; j++) {
			if (i + j >= TempImage->SizeOfData - 1) {
				fprintf(HeadFile, "%4d", TempImage->Data[i+j]);
				break;
			}
			else
				fprintf(HeadFile, "%4d,", TempImage->Data[i+j]);
		}
		fprintf(HeadFile, "\n");
	}
	if (TempImage != Image)
		ilCloseImage(TempImage);

	fprintf(HeadFile, "};\n");


	if (Image->Pal.Palette && Image->Pal.PalSize && Image->Pal.PalType != IL_PAL_NONE) {
		fprintf(HeadFile, "\n\n");
		fprintf(HeadFile, "#define IMAGE_PALSIZE %u\n\n", Image->Pal.PalSize);
		fprintf(HeadFile, "#define IMAGE_PALTYPE 0x%X\n\n", Image->Pal.PalType);
        fprintf(HeadFile, "ILubyte %sPal[] = {\n", Name);
		for (i = 0; i < Image->Pal.PalSize; i += MAX_LINE_WIDTH) {
			fprintf(HeadFile, "\t");
			for (j = 0; j < MAX_LINE_WIDTH; j++) {
				if (i + j >= Image->Pal.PalSize - 1) {
					fprintf(HeadFile, " %4d", Image->Pal.Palette[i+j]);
					break;
				}
				else
					fprintf(HeadFile, " %4d,", Image->Pal.Palette[i+j]);
			}
			fprintf(HeadFile, "\n");
		}

		fprintf(HeadFile, "};\n");
	}
	fclose(HeadFile);
	return IL_TRUE;
}



#endif//IL_NO_CHEAD
