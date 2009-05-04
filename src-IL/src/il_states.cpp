//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 05/03/2009
//
// Filename: src-IL/src/il_states.cpp
//
// Description: State machine
//
//
// 20040223 XIX : now has a ilPngAlphaIndex member, so we can spit out png files with a transparent index, set to -1 for none
//
//-----------------------------------------------------------------------------


#include "il_internal.h"
#include "il_states.h"
//#include <malloc.h>
#include <stdlib.h>

ILconst_string _ilVendor		= IL_TEXT("Abysmal Software");
ILconst_string _ilVersion		= IL_TEXT("Developer's Image Library (DevIL) 1.7.8");


char* _ilLoadExt		= "" IL_BLP_EXT IL_BMP_EXT IL_CUT_EXT IL_DCX_EXT IL_DDS_EXT
									IL_DCM_EXT IL_DPX_EXT IL_EXR_EXT IL_FITS_EXT IL_FTX_EXT
									IL_GIF_EXT IL_HDR_EXT IL_ICNS_EXT IL_ICO_EXT IL_IFF_EXT
									IL_IWI_EXT IL_JPG_EXT IL_JP2_EXT IL_LIF_EXT IL_MDL_EXT
									IL_MNG_EXT IL_MP3_EXT IL_PCD_EXT IL_PCX_EXT IL_PIC_EXT
									IL_PIX_EXT IL_PNG_EXT IL_PNM_EXT IL_PSD_EXT IL_PSP_EXT
									IL_PXR_EXT IL_RAW_EXT IL_ROT_EXT IL_SGI_EXT IL_SUN_EXT
									IL_TEX_EXT IL_TGA_EXT IL_TIF_EXT IL_TPL_EXT IL_UTX_EXT
									IL_VTF_EXT IL_WAL_EXT IL_WDP_EXT IL_XPM_EXT;

char* _ilSaveExt		= "" IL_BMP_EXT IL_CHEAD_EXT IL_DDS_EXT IL_EXR_EXT
									IL_HDR_EXT IL_JP2_EXT IL_JPG_EXT IL_PCX_EXT
									IL_PNG_EXT IL_PNM_EXT IL_PSD_EXT IL_RAW_EXT
									IL_SGI_EXT IL_TGA_EXT IL_TIF_EXT IL_VTF_EXT
									IL_WBMP_EXT;


ILstate DefaultState;


//! Set all states to their defaults.
void ilDefaultStates(ILstate *State)
{
	//@TODO: Setup default states.
	if (State == NULL)
		State = &DefaultState;

	State->ilOriginSet = IL_FALSE;
	State->ilOriginMode = IL_ORIGIN_LOWER_LEFT;
	State->ilFormatSet = IL_FALSE;
	State->ilFormatMode = IL_BGRA;
	State->ilTypeSet = IL_FALSE;
	State->ilTypeMode = IL_UNSIGNED_BYTE;
	State->ilAutoConvPal = IL_FALSE;
	State->ilDefaultOnFail = IL_FALSE;
	State->ilUseKeyColour = IL_FALSE;
	State->ilBlitBlend = IL_TRUE;
	State->ilCompression = IL_COMPRESS_ZLIB;
	State->ilInterlace = IL_FALSE;

	State->ilTgaCreateStamp = IL_FALSE;
	State->ilJpgQuality = 99;
	State->ilPngInterlace = IL_FALSE;
	State->ilTgaRle = IL_FALSE;
	State->ilBmpRle = IL_FALSE;
	State->ilSgiRle = IL_FALSE;
	State->ilJpgFormat = IL_JFIF;
	State->ilJpgProgressive = IL_FALSE;
	State->ilDxtcFormat = IL_DXT1;
	State->ilPcdPicNum = 2;
	State->ilPngAlphaIndex = -1;
	State->ilVtfCompression = IL_DXT_NO_COMP;

	State->ilTgaId = NULL;
	State->ilTgaAuthName = NULL;
	State->ilTgaAuthComment = NULL;
	State->ilPngAuthName = NULL;
	State->ilPngTitle = NULL;
	State->ilPngDescription = NULL;

	//2003-09-01: added tiff strings
	State->ilTifDescription = NULL;
	State->ilTifHostComputer = NULL;
	State->ilTifDocumentName = NULL;
	State->ilTifAuthName = NULL;
	State->ilCHeader = NULL;

	State->ilQuantMode = IL_WU_QUANT;
	State->ilNeuSample = 15;
	State->ilQuantMaxIndexs = 256;

	State->ilKeepDxtcData = IL_FALSE;
	State->ilUseNVidiaDXT = IL_FALSE;
	State->ilUseSquishDXT = IL_FALSE;

	State->MemVsSpeedHint = IL_FASTEST;
	State->CompressHint = IL_USE_COMPRESSION;

	while (ilGetError() != IL_NO_ERROR);

	return;
}


//! Returns a constant string detailing aspects about this library.
ILconst_string ILAPIENTRY ilGetString(ILenum StringName, ILstate *State)
{
	switch (StringName)
	{
		case IL_VENDOR:
			return (ILconst_string)_ilVendor;
		case IL_VERSION_NUM: //changed 2003-08-30: IL_VERSION changes									//switch define ;-)
			return (ILconst_string)_ilVersion;
		case IL_LOAD_EXT:
			return (ILconst_string)_ilLoadExt;
		case IL_SAVE_EXT:
			return (ILconst_string)_ilSaveExt;
		case IL_TGA_ID_STRING:
			return (ILconst_string)State->ilTgaId;
		case IL_TGA_AUTHNAME_STRING:
			return (ILconst_string)State->ilTgaAuthName;
		case IL_TGA_AUTHCOMMENT_STRING:
			return (ILconst_string)State->ilTgaAuthComment;
		case IL_PNG_AUTHNAME_STRING:
			return (ILconst_string)State->ilPngAuthName;
		case IL_PNG_TITLE_STRING:
			return (ILconst_string)State->ilPngTitle;
		case IL_PNG_DESCRIPTION_STRING:
			return (ILconst_string)State->ilPngDescription;
		//2003-08-31: added tif strings
		case IL_TIF_DESCRIPTION_STRING:
			return (ILconst_string)State->ilTifDescription;
		case IL_TIF_HOSTCOMPUTER_STRING:
			return (ILconst_string)State->ilTifHostComputer;
		case IL_TIF_DOCUMENTNAME_STRING:
			return (ILconst_string)State->ilTifDocumentName;
		case IL_TIF_AUTHNAME_STRING:
			return (ILconst_string)State->ilTifAuthName;
		case IL_CHEAD_HEADER_STRING:
			return (ILconst_string)State->ilCHeader;
		default:
			ilSetError(IL_INVALID_ENUM);
			break;
	}
	return NULL;
}


// Clips a string to a certain length and returns a new string.
char *iClipString(char *String, ILuint MaxLen)
{
	char	*Clipped;
	ILuint	Length;

	if (String == NULL)
		return NULL;

	Length = ilCharStrLen(String);  //ilStrLen(String);

	Clipped = (char*)ialloc((MaxLen + 1) * sizeof(char) /*sizeof(ILchar)*/);  // Terminating NULL makes it +1.
	if (Clipped == NULL) {
		return NULL;
	}

	memcpy(Clipped, String, MaxLen * sizeof(char) /*sizeof(ILchar)*/);
	Clipped[Length] = 0;

	return Clipped;
}


// Returns format-specific strings, truncated to MaxLen (not counting the terminating NULL).
char *iGetString(ILenum StringName, ILstate *State)
{
	switch (StringName)
	{
		case IL_TGA_ID_STRING:
			return iClipString(State->ilTgaId, 254);
		case IL_TGA_AUTHNAME_STRING:
			return iClipString(State->ilTgaAuthName, 40);
		case IL_TGA_AUTHCOMMENT_STRING:
			return iClipString(State->ilTgaAuthComment, 80);
		case IL_PNG_AUTHNAME_STRING:
			return iClipString(State->ilPngAuthName, 255);
		case IL_PNG_TITLE_STRING:
			return iClipString(State->ilPngTitle, 255);
		case IL_PNG_DESCRIPTION_STRING:
			return iClipString(State->ilPngDescription, 255);

		//changed 2003-08-31...here was a serious copy and paste bug ;-)
		case IL_TIF_DESCRIPTION_STRING:
			return iClipString(State->ilTifDescription, 255);
		case IL_TIF_HOSTCOMPUTER_STRING:
			return iClipString(State->ilTifHostComputer, 255);
		case IL_TIF_DOCUMENTNAME_STRING:
			return iClipString(State->ilTifDocumentName, 255);
		case IL_TIF_AUTHNAME_STRING:
			return iClipString(State->ilTifAuthName, 255);
		case IL_CHEAD_HEADER_STRING:
			return iClipString(State->ilCHeader, 32);
		default:
			ilSetError(IL_INVALID_ENUM);
	}
	return NULL;
}


//! Enables a mode
ILboolean ILAPIENTRY ilEnable(ILenum Mode, ILstate *State)
{
	return ilAble(Mode, IL_TRUE, State);
}


//! Disables a mode
ILboolean ILAPIENTRY ilDisable(ILenum Mode, ILstate *State)
{
	return ilAble(Mode, IL_FALSE, State);
}


// Internal function that sets the Mode equal to Flag
ILboolean ilAble(ILenum Mode, ILboolean Flag, ILstate *State)
{
	switch (Mode)
	{
		case IL_ORIGIN_SET:
			State->ilOriginSet = Flag;
			break;
		case IL_FORMAT_SET:
			State->ilFormatSet = Flag;
			break;
		case IL_TYPE_SET:
			State->ilTypeSet = Flag;
			break;
		case IL_CONV_PAL:
			State->ilAutoConvPal = Flag;
			break;
		case IL_DEFAULT_ON_FAIL:
			State->ilDefaultOnFail = Flag;
			break;
		case IL_USE_KEY_COLOUR:
			State->ilUseKeyColour = Flag;
			break;
		case IL_BLIT_BLEND:
			State->ilBlitBlend = Flag;
			break;
		case IL_SAVE_INTERLACED:
			State->ilInterlace = Flag;
			break;
		case IL_JPG_PROGRESSIVE:
			State->ilJpgProgressive = Flag;
			break;
		case IL_NVIDIA_COMPRESS:
			State->ilUseNVidiaDXT = Flag;
			break;
		case IL_SQUISH_COMPRESS:
			State->ilUseSquishDXT = Flag;
			break;

		default:
			ilSetError(IL_INVALID_ENUM);
			return IL_FALSE;
	}

	return IL_TRUE;
}


//! Checks whether the mode is enabled.
ILboolean ILAPIENTRY ilIsEnabled(ILenum Mode, ILstate *State)
{
	switch (Mode)
	{
		case IL_ORIGIN_SET:
			return State->ilOriginSet;
		case IL_FORMAT_SET:
			return State->ilFormatSet;
		case IL_TYPE_SET:
			return State->ilTypeSet;
		case IL_CONV_PAL:
			return State->ilAutoConvPal;
		case IL_DEFAULT_ON_FAIL:
			return State->ilDefaultOnFail;
		case IL_USE_KEY_COLOUR:
			return State->ilUseKeyColour;
		case IL_BLIT_BLEND:
			return State->ilBlitBlend;
		case IL_SAVE_INTERLACED:
			return State->ilInterlace;
		case IL_JPG_PROGRESSIVE:
			return State->ilJpgProgressive;
		case IL_NVIDIA_COMPRESS:
			return State->ilUseNVidiaDXT;
		case IL_SQUISH_COMPRESS:
			return State->ilUseSquishDXT;

		default:
			ilSetError(IL_INVALID_ENUM);
	}

	return IL_FALSE;
}


//! Checks whether the mode is disabled.
ILboolean ILAPIENTRY ilIsDisabled(ILenum Mode, ILstate *State)
{
	return !ilIsEnabled(Mode, State);
}


//! Sets Param equal to the current value of the Mode
void ILAPIENTRY ilGetBooleanv(ILenum Mode, ILboolean *Param, ILstate *State)
{
	if (Param == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return;
	}

	*Param = ilGetInteger(Mode, State);

	return;
}


//! Returns the current value of the Mode
ILboolean ILAPIENTRY ilGetBoolean(ILenum Mode, ILstate *State)
{
	ILboolean Temp;
	Temp = IL_FALSE;
	ilGetBooleanv(Mode, &Temp, State);
	return Temp;
}


ILimage *iGetBaseImage(void);

//! Internal function to figure out where we are in an image chain.
//@TODO: This may get much more complex with mipmaps under faces, etc.
ILuint iGetActiveNum(ILenum Type)
{
	//ILimage *BaseImage;
	//ILuint Num = 0;

	//if (iCurImage == NULL) {
	//	ilSetError(IL_ILLEGAL_OPERATION);
	//	return 0;
	//}

	//BaseImage = iGetBaseImage();
	//if (BaseImage == iCurImage)
	//	return 0;

	//switch (Type)
	//{
	//	case IL_ACTIVE_IMAGE:
	//		BaseImage = BaseImage->Next;
	//		do {
	//			if (BaseImage == NULL)
	//				return 0;
	//			Num++;
	//			if (BaseImage == iCurImage)
	//				return Num;
	//		} while ((BaseImage = BaseImage->Next));
	//		break;
	//	case IL_ACTIVE_MIPMAP:
	//		BaseImage = BaseImage->Mipmaps;
	//		do {
	//			if (BaseImage == NULL)
	//				return 0;
	//			Num++;
	//			if (BaseImage == iCurImage)
	//				return Num;
	//		} while ((BaseImage = BaseImage->Mipmaps));
	//		break;
	//	case IL_ACTIVE_LAYER:
	//		BaseImage = BaseImage->Layers;
	//		do {
	//			if (BaseImage == NULL)
	//				return 0;
	//			Num++;
	//			if (BaseImage == iCurImage)
	//				return Num;
	//		} while ((BaseImage = BaseImage->Layers));
	//		break;
	//	case IL_ACTIVE_FACE:
	//		BaseImage = BaseImage->Faces;
	//		do {
	//			if (BaseImage == NULL)
	//				return 0;
	//			Num++;
	//			if (BaseImage == iCurImage)
	//				return Num;
	//		} while ((BaseImage = BaseImage->Faces));
	//		break;
	//}

	//@TODO: Any error needed here?

	return 0;
}


//! Sets Param equal to the current value of the Mode
void ILAPIENTRY ilGetIntegerv(ILenum Mode, ILint *Param, ILstate *State)
{
	if (Param == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return;
	}

	*Param = 0;

	switch (Mode) {
		// Integer values
		case IL_COMPRESS_MODE:
			*Param = State->ilCompression;
			break;
		//case IL_CUR_IMAGE:
		//	if (iCurImage == NULL) {
		//		ilSetError(IL_ILLEGAL_OPERATION);
		//		break;
		//	}
		//	*Param = ilGetCurName();
		//	break;
		case IL_FORMAT_MODE:
			*Param = State->ilFormatMode;
			break;
		case IL_INTERLACE_MODE:
			*Param = State->ilInterlace;
			break;
		case IL_KEEP_DXTC_DATA:
			*Param = State->ilKeepDxtcData;
			break;
		case IL_ORIGIN_MODE:
			*Param = State->ilOriginMode;
			break;
		case IL_MAX_QUANT_INDICES:
			*Param = State->ilQuantMaxIndexs;
			break;
		case IL_NEU_QUANT_SAMPLE:
			*Param = State->ilNeuSample;
			break;
		case IL_QUANTIZATION_MODE:
			*Param = State->ilQuantMode;
			break;
		case IL_TYPE_MODE:
			*Param = State->ilTypeMode;
			break;
		case IL_VERSION_NUM:
			*Param = IL_VERSION;
			break;

		// Image specific values
		case IL_ACTIVE_IMAGE:
		case IL_ACTIVE_MIPMAP:
		case IL_ACTIVE_LAYER:
			*Param = iGetActiveNum(Mode);
			break;

		// Format-specific values
		case IL_BMP_RLE:
			*Param = State->ilBmpRle;
			break;
		case IL_DXTC_FORMAT:
			*Param = State->ilDxtcFormat;
			break;
		case IL_JPG_QUALITY:
			*Param = State->ilJpgQuality;
			break;
		case IL_JPG_SAVE_FORMAT:
			*Param = State->ilJpgFormat;
			break;
		case IL_PCD_PICNUM:
			*Param = State->ilPcdPicNum;
			break;
		case IL_PNG_ALPHA_INDEX:
			*Param = State->ilPngAlphaIndex;
			break;
		case IL_PNG_INTERLACE:
			*Param = State->ilPngInterlace;
			break;
		case IL_SGI_RLE:
			*Param = State->ilSgiRle;
			break;
		case IL_TGA_CREATE_STAMP:
			*Param = State->ilTgaCreateStamp;
			break;
		case IL_TGA_RLE:
			*Param = State->ilTgaRle;
			break;
		case IL_VTF_COMP:
			*Param = State->ilVtfCompression;
			break;

		// Boolean values
		case IL_CONV_PAL:
			*Param = State->ilAutoConvPal;
			break;
		case IL_DEFAULT_ON_FAIL:
			*Param = State->ilDefaultOnFail;
			break;
		case IL_FORMAT_SET:
			*Param = State->ilFormatSet;
			break;
		case IL_ORIGIN_SET:
			*Param = State->ilOriginSet;
			break;
		case IL_TYPE_SET:
			*Param = State->ilTypeSet;
			break;
		case IL_USE_KEY_COLOUR:
			*Param = State->ilUseKeyColour;
			break;
		case IL_BLIT_BLEND:
			*Param = State->ilBlitBlend;
			break;
		case IL_JPG_PROGRESSIVE:
			*Param = State->ilJpgProgressive;
			break;
		case IL_NVIDIA_COMPRESS:
			*Param = State->ilUseNVidiaDXT;
			break;
		case IL_SQUISH_COMPRESS:
			*Param = State->ilUseSquishDXT;
			break;

		default:
			break;
            //iGetIntegervImage(iCurImage, Mode, Param);
	}

	return;
}


ILint ILAPIENTRY ilImageInfo(ILimage *Image, ILenum Mode)
{
	ILint Param;
	iGetIntegervImage(Image, Mode, &Param);
	return Param;
}


//@TODO rename to ilGetImageIntegerv for 1.6.9 and make it public
//! Sets Param equal to the current value of the Mode
void ILAPIENTRY iGetIntegervImage(ILimage *Image, ILenum Mode, ILint *Param)
{
    ILimage *SubImage;
    if (Image == NULL) {
        ilSetError(IL_ILLEGAL_OPERATION);
        return;
    }
    if (Param == NULL) {
        ilSetError(IL_INVALID_PARAM);
        return;
    }
    *Param = 0;

    switch (Mode)
    {
        case IL_DXTC_DATA_FORMAT:
            if (Image->DxtcData == NULL || Image->DxtcSize == 0) {
                 *Param = IL_DXT_NO_COMP;
                 break;
            }
            *Param = Image->DxtcFormat;
            break;
            ////
        case IL_IMAGE_BITS_PER_PIXEL:
            //changed 20040610 to channel count (Bpp) times Bytes per channel
            *Param = (Image->Bpp << 3)*Image->Bpc;
            break;
        case IL_IMAGE_BYTES_PER_PIXEL:
            //changed 20040610 to channel count (Bpp) times Bytes per channel
            *Param = Image->Bpp*Image->Bpc;
            break;
        case IL_IMAGE_BPC:
            *Param = Image->Bpc;
            break;
        case IL_IMAGE_CHANNELS:
            *Param = Image->Bpp;
            break;
        case IL_IMAGE_CUBEFLAGS:
            *Param = Image->CubeFlags;
            break;
        case IL_IMAGE_DEPTH:
            *Param = Image->Depth;
            break;
        case IL_IMAGE_DURATION:
            *Param = Image->Duration;
            break;
        case IL_IMAGE_FORMAT:
            *Param = Image->Format;
            break;
        case IL_IMAGE_HEIGHT:
            *Param = Image->Height;
            break;
        case IL_IMAGE_SIZE_OF_DATA:
            *Param = Image->SizeOfData;

            break;
        case IL_IMAGE_OFFX:
            *Param = Image->OffX;
            break;
        case IL_IMAGE_OFFY:
            *Param = Image->OffY;
            break;
        case IL_IMAGE_ORIGIN:
            *Param = Image->Origin;
            break;
        case IL_IMAGE_PLANESIZE:
            *Param = Image->SizeOfPlane;
            break;
        case IL_IMAGE_TYPE:
            *Param = Image->Type;
            break;
        case IL_IMAGE_WIDTH:
            *Param = Image->Width;
            break;
        case IL_NUM_FACES:
            for (SubImage = Image->Faces; SubImage; SubImage = SubImage->Faces)
                (*Param)++;
            break;
        case IL_NUM_IMAGES:
            for (SubImage = Image->Next; SubImage; SubImage = SubImage->Next)
                (*Param)++;
            break;
        case IL_NUM_LAYERS:
            for (SubImage = Image->Layers; SubImage; SubImage = SubImage->Layers)
                (*Param)++;
            break;
        case IL_NUM_MIPMAPS:
			for (SubImage = Image->Mipmaps; SubImage; SubImage = SubImage->Mipmaps)
                (*Param)++;
            break;

        case IL_PALETTE_TYPE:
             *Param = Image->Pal.PalType;
             break;
        case IL_PALETTE_BPP:
             *Param = ilGetBppPal(Image->Pal.PalType);
             break;
        case IL_PALETTE_NUM_COLS:
             if (!Image->Pal.Palette || !Image->Pal.PalSize || Image->Pal.PalType == IL_PAL_NONE)
                  *Param = 0;
             else
                  *Param = Image->Pal.PalSize / ilGetBppPal(Image->Pal.PalType);
             break;
        case IL_PALETTE_BASE_TYPE:
             switch (Image->Pal.PalType)
             {
                  case IL_PAL_RGB24:
                      *Param = IL_RGB;
                  case IL_PAL_RGB32:
                      *Param = IL_RGBA; // Not sure
                  case IL_PAL_RGBA32:
                      *Param = IL_RGBA;
                  case IL_PAL_BGR24:
                      *Param = IL_BGR;
                  case IL_PAL_BGR32:
                      *Param = IL_BGRA; // Not sure
                  case IL_PAL_BGRA32:
                      *Param = IL_BGRA;
             }
             break;
        default:
             ilSetError(IL_INVALID_ENUM);
    }
}


ILuint ILAPIENTRY ilGetWidth(ILimage *Image)
{
	return Image->Width;
}

ILuint ILAPIENTRY ilGetHeight(ILimage *Image)
{
	return Image->Height;
}

ILuint ILAPIENTRY ilGetDepth(ILimage *Image)
{
	return Image->Depth;
}

ILenum ILAPIENTRY ilGetFormat(ILimage *Image)
{
	return Image->Format;
}

ILenum ILAPIENTRY ilGetType(ILimage *Image)
{
	return Image->Type;
}

ILuint ILAPIENTRY ilGetImageSize(ILimage *Image)
{
	return Image->SizeOfData;
}





//! Returns the current value of the Mode
ILint ILAPIENTRY ilGetInteger(ILenum Mode, ILstate *State)
{
	ILint Temp;
	Temp = 0;
	ilGetIntegerv(Mode, &Temp, State);
	return Temp;
}


//! Sets the default origin to be used.
ILboolean ILAPIENTRY ilOriginFunc(ILenum Mode, ILstate *State)
{
	switch (Mode)
	{
		case IL_ORIGIN_LOWER_LEFT:
		case IL_ORIGIN_UPPER_LEFT:
			State->ilOriginMode = Mode;
			break;
		default:
			ilSetError(IL_INVALID_PARAM);
			return IL_FALSE;
	}
	return IL_TRUE;
}


//! Sets the default format to be used.
ILboolean ILAPIENTRY ilFormatFunc(ILenum Mode, ILstate *State)
{
	switch (Mode)
	{
		//case IL_COLOUR_INDEX:
		case IL_RGB:
		case IL_RGBA:
		case IL_BGR:
		case IL_BGRA:
		case IL_LUMINANCE:
		case IL_LUMINANCE_ALPHA:
			State->ilFormatMode = Mode;
			break;
		default:
			ilSetError(IL_INVALID_PARAM);
			return IL_FALSE;
	}
	return IL_TRUE;
}


//! Sets the default type to be used.
ILboolean ILAPIENTRY ilTypeFunc(ILenum Mode, ILstate *State)
{
	switch (Mode)
	{
		case IL_BYTE:
		case IL_UNSIGNED_BYTE:
		case IL_SHORT:
		case IL_UNSIGNED_SHORT:
		case IL_INT:
		case IL_UNSIGNED_INT:
		case IL_FLOAT:
		case IL_DOUBLE:
			State->ilTypeMode = Mode;
			break;
		default:
			ilSetError(IL_INVALID_PARAM);
			return IL_FALSE;
	}
	return IL_TRUE;
}


ILboolean ILAPIENTRY ilCompressFunc(ILenum Mode, ILstate *State)
{
	switch (Mode)
	{
		case IL_COMPRESS_NONE:
		case IL_COMPRESS_RLE:
		//case IL_COMPRESS_LZO:
		case IL_COMPRESS_ZLIB:
			State->ilCompression = Mode;
			break;
		default:
			ilSetError(IL_INVALID_PARAM);
			return IL_FALSE;
	}
	return IL_TRUE;
}


//! Specifies implementation-dependent performance hints
void ILAPIENTRY ilHint(ILenum Target, ILenum Mode, ILstate *State)
{
	switch (Target)
	{
		case IL_MEM_SPEED_HINT:
			switch (Mode)
			{
				case IL_FASTEST:
					State->MemVsSpeedHint = Mode;
					break;
				case IL_LESS_MEM:
					State->MemVsSpeedHint = Mode;
					break;
				case IL_DONT_CARE:
					State->MemVsSpeedHint = IL_FASTEST;
					break;
				default:
					ilSetError(IL_INVALID_ENUM);
					return;
			}
			break;

		case IL_COMPRESSION_HINT:
			switch (Mode)
			{
				case IL_USE_COMPRESSION:
					State->CompressHint = Mode;
					break;
				case IL_NO_COMPRESSION:
					State->CompressHint = Mode;
					break;
				case IL_DONT_CARE:
					State->CompressHint = IL_NO_COMPRESSION;
					break;
				default:
					ilSetError(IL_INVALID_ENUM);
					return;
			}
			break;

			
		default:
			ilSetError(IL_INVALID_ENUM);
			return;
	}

	return;
}


ILenum iGetHint(ILenum Target, ILstate *State)
{
	switch (Target)
	{
		case IL_MEM_SPEED_HINT:
			return State->MemVsSpeedHint;
		case IL_COMPRESSION_HINT:
			return State->CompressHint;
		default:
			ilSetError(IL_INTERNAL_ERROR);
			return 0;
	}
}


void ILAPIENTRY ilSetString(ILenum Mode, const char *String, ILstate *State)
{
	if (String == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return;
	}

	switch (Mode)
	{
		case IL_TGA_ID_STRING:
			if (State->ilTgaId)
				ifree(State->ilTgaId);
			State->ilTgaId = strdup(String);
			break;
		case IL_TGA_AUTHNAME_STRING:
			if (State->ilTgaAuthName)
				ifree(State->ilTgaAuthName);
			State->ilTgaAuthName = strdup(String);
			break;
		case IL_TGA_AUTHCOMMENT_STRING:
			if (State->ilTgaAuthComment)
				ifree(State->ilTgaAuthComment);
			State->ilTgaAuthComment = strdup(String);
			break;
		case IL_PNG_AUTHNAME_STRING:
			if (State->ilPngAuthName)
				ifree(State->ilPngAuthName);
			State->ilPngAuthName = strdup(String);
			break;
		case IL_PNG_TITLE_STRING:
			if (State->ilPngTitle)
				ifree(State->ilPngTitle);
			State->ilPngTitle = strdup(String);
			break;
		case IL_PNG_DESCRIPTION_STRING:
			if (State->ilPngDescription)
				ifree(State->ilPngDescription);
			State->ilPngDescription = strdup(String);
			break;

		//2003-09-01: added tif strings
		case IL_TIF_DESCRIPTION_STRING:
			if (State->ilTifDescription)
				ifree(State->ilTifDescription);
			State->ilTifDescription = strdup(String);
			break;
		case IL_TIF_HOSTCOMPUTER_STRING:
			if (State->ilTifHostComputer)
				ifree(State->ilTifHostComputer);
			State->ilTifHostComputer = strdup(String);
			break;
		case IL_TIF_DOCUMENTNAME_STRING:
						if (State->ilTifDocumentName)
				ifree(State->ilTifDocumentName);
			State->ilTifDocumentName = strdup(String);
			break;
		case IL_TIF_AUTHNAME_STRING:
			if (State->ilTifAuthName)
				ifree(State->ilTifAuthName);
			State->ilTifAuthName = strdup(String);
			break;

		case IL_CHEAD_HEADER_STRING:
			if (State->ilCHeader)
				ifree(State->ilCHeader);
			State->ilCHeader = strdup(String);
			break;

		default:
			ilSetError(IL_INVALID_ENUM);
	}

	return;
}


void ILAPIENTRY ilSetInteger(ILenum Mode, ILint Param, ILstate *State)
{
	switch (Mode)
	{
		// Integer values
		case IL_FORMAT_MODE:
			ilFormatFunc(Param, State);
			return;
		case IL_KEEP_DXTC_DATA:
			if (Param == IL_FALSE || Param == IL_TRUE) {
				State->ilKeepDxtcData = Param;
				return;
			}
			break;
		case IL_MAX_QUANT_INDICES:
			if (Param >= 2 && Param <= 256) {
				State->ilQuantMaxIndexs = Param;
				return;
			}
			break;
		case IL_NEU_QUANT_SAMPLE:
			if (Param >= 1 && Param <= 30) {
				State->ilNeuSample = Param;
				return;
			}
			break;
		case IL_ORIGIN_MODE:
			ilOriginFunc(Param, State);
			return;
		case IL_QUANTIZATION_MODE:
			if (Param == IL_WU_QUANT || Param == IL_NEU_QUANT) {
				State->ilQuantMode = Param;
				return;
			}
			break;
		case IL_TYPE_MODE:
			ilTypeFunc(Param, State);
			return;

		// Image specific values
		//case IL_IMAGE_DURATION:
		//	if (iCurImage == NULL) {
		//		ilSetError(IL_ILLEGAL_OPERATION);
		//		break;
		//	}
		//	iCurImage->Duration = Param;
		//	return;
		//case IL_IMAGE_OFFX:
		//	if (iCurImage == NULL) {
		//		ilSetError(IL_ILLEGAL_OPERATION);
		//		break;
		//	}
		//	iCurImage->OffX = Param;
		//	return;
		//case IL_IMAGE_OFFY:
		//	if (iCurImage == NULL) {
		//		ilSetError(IL_ILLEGAL_OPERATION);
		//		break;
		//	}
		//	iCurImage->OffY = Param;
		//	return;
		//case IL_IMAGE_CUBEFLAGS:
		//	if (iCurImage == NULL) {
		//		ilSetError(IL_ILLEGAL_OPERATION);
		//		break;
		//	}
		//	iCurImage->CubeFlags = Param;
		//	break;
 
		// Format specific values
		case IL_BMP_RLE:
			if (Param == IL_FALSE || Param == IL_TRUE) {
				State->ilBmpRle = Param;
				return;
			}
			break;
		case IL_DXTC_FORMAT:
			if (Param >= IL_DXT1 || Param <= IL_DXT5 || Param == IL_DXT1A) {
				State->ilDxtcFormat = Param;
				return;
			}
			break;
		case IL_JPG_SAVE_FORMAT:
			if (Param == IL_JFIF || Param == IL_EXIF) {
				State->ilJpgFormat = Param;
				return;
			}
			break;
		case IL_JPG_QUALITY:
			if (Param >= 0 && Param <= 99) {
				State->ilJpgQuality = Param;
				return;
			}
			break;
		case IL_PNG_INTERLACE:
			if (Param == IL_FALSE || Param == IL_TRUE) {
				State->ilPngInterlace = Param;
				return;
			}
			break;
		case IL_PCD_PICNUM:
			if (Param >= 0 || Param <= 2) {
				State->ilPcdPicNum = Param;
				return;
			}
			break;
		case IL_PNG_ALPHA_INDEX:
			if (Param >= -1 || Param <= 255) {
				State->ilPngAlphaIndex=Param;
				return;
			}
			break;
		case IL_SGI_RLE:
			if (Param == IL_FALSE || Param == IL_TRUE) {
				State->ilSgiRle = Param;
				return;
			}
			break;
		case IL_TGA_CREATE_STAMP:
			if (Param == IL_FALSE || Param == IL_TRUE) {
				State->ilTgaCreateStamp = Param;
				return;
			}
			break;
		case IL_TGA_RLE:
			if (Param == IL_FALSE || Param == IL_TRUE) {
				State->ilTgaRle = Param;
				return;
			}
			break;
		case IL_VTF_COMP:
			if (Param == IL_DXT1 || Param == IL_DXT5 || Param == IL_DXT3 || Param == IL_DXT1A || Param == IL_DXT_NO_COMP) {
				State->ilVtfCompression = Param;
				return;
			}
			break;

		default:
			ilSetError(IL_INVALID_ENUM);
			return;
	}

	ilSetError(IL_INVALID_PARAM);  // Parameter not in valid bounds.
	return;
}



ILint iGetInt(ILenum Mode, ILstate *State)
{
	//like ilGetInteger(), but sets another error on failure

	//call ilGetIntegerv() for more robust code
	ILenum err;
	ILint r = -1;

	ilGetIntegerv(Mode, &r, State);

	//check if an error occured, set another error
	err = ilGetError();
	if (r == -1 && err == IL_INVALID_ENUM)
		ilSetError(IL_INTERNAL_ERROR);
	else
		ilSetError(err); //restore error

	return r;
}
