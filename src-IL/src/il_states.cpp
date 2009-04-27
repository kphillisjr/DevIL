//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 03/07/2009
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


//! Set all states to their defaults.
void ilDefaultStates(ILstate *State)
{
	State[ilCurrentPos].ilOriginSet = IL_FALSE;
	State[ilCurrentPos].ilOriginMode = IL_ORIGIN_LOWER_LEFT;
	State[ilCurrentPos].ilFormatSet = IL_FALSE;
	State[ilCurrentPos].ilFormatMode = IL_BGRA;
	State[ilCurrentPos].ilTypeSet = IL_FALSE;
	State[ilCurrentPos].ilTypeMode = IL_UNSIGNED_BYTE;
	State[ilCurrentPos].ilAutoConvPal = IL_FALSE;
	State[ilCurrentPos].ilDefaultOnFail = IL_FALSE;
	State[ilCurrentPos].ilUseKeyColour = IL_FALSE;
	State[ilCurrentPos].ilBlitBlend = IL_TRUE;
	State[ilCurrentPos].ilCompression = IL_COMPRESS_ZLIB;
	State[ilCurrentPos].ilInterlace = IL_FALSE;

	State[ilCurrentPos].ilTgaCreateStamp = IL_FALSE;
	State[ilCurrentPos].ilJpgQuality = 99;
	State[ilCurrentPos].ilPngInterlace = IL_FALSE;
	State[ilCurrentPos].ilTgaRle = IL_FALSE;
	State[ilCurrentPos].ilBmpRle = IL_FALSE;
	State[ilCurrentPos].ilSgiRle = IL_FALSE;
	State[ilCurrentPos].ilJpgFormat = IL_JFIF;
	State[ilCurrentPos].ilJpgProgressive = IL_FALSE;
	State[ilCurrentPos].ilDxtcFormat = IL_DXT1;
	State[ilCurrentPos].ilPcdPicNum = 2;
	State[ilCurrentPos].ilPngAlphaIndex = -1;
	State[ilCurrentPos].ilVtfCompression = IL_DXT_NO_COMP;

	State[ilCurrentPos].ilTgaId = NULL;
	State[ilCurrentPos].ilTgaAuthName = NULL;
	State[ilCurrentPos].ilTgaAuthComment = NULL;
	State[ilCurrentPos].ilPngAuthName = NULL;
	State[ilCurrentPos].ilPngTitle = NULL;
	State[ilCurrentPos].ilPngDescription = NULL;

	//2003-09-01: added tiff strings
	State[ilCurrentPos].ilTifDescription = NULL;
	State[ilCurrentPos].ilTifHostComputer = NULL;
	State[ilCurrentPos].ilTifDocumentName = NULL;
	State[ilCurrentPos].ilTifAuthName = NULL;
	State[ilCurrentPos].ilCHeader = NULL;

	State[ilCurrentPos].ilQuantMode = IL_WU_QUANT;
	State[ilCurrentPos].ilNeuSample = 15;
	State[ilCurrentPos].ilQuantMaxIndexs = 256;

	State[ilCurrentPos].ilKeepDxtcData = IL_FALSE;
	State[ilCurrentPos].ilUseNVidiaDXT = IL_FALSE;
	State[ilCurrentPos].ilUseSquishDXT = IL_FALSE;




	State[ilCurrentPos].MemVsSpeedHint = IL_FASTEST;
	State[ilCurrentPos].CompressHint = IL_USE_COMPRESSION;

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
			return (ILconst_string)State[ilCurrentPos].ilTgaId;
		case IL_TGA_AUTHNAME_STRING:
			return (ILconst_string)State[ilCurrentPos].ilTgaAuthName;
		case IL_TGA_AUTHCOMMENT_STRING:
			return (ILconst_string)State[ilCurrentPos].ilTgaAuthComment;
		case IL_PNG_AUTHNAME_STRING:
			return (ILconst_string)State[ilCurrentPos].ilPngAuthName;
		case IL_PNG_TITLE_STRING:
			return (ILconst_string)State[ilCurrentPos].ilPngTitle;
		case IL_PNG_DESCRIPTION_STRING:
			return (ILconst_string)State[ilCurrentPos].ilPngDescription;
		//2003-08-31: added tif strings
		case IL_TIF_DESCRIPTION_STRING:
			return (ILconst_string)State[ilCurrentPos].ilTifDescription;
		case IL_TIF_HOSTCOMPUTER_STRING:
			return (ILconst_string)State[ilCurrentPos].ilTifHostComputer;
		case IL_TIF_DOCUMENTNAME_STRING:
			return (ILconst_string)State[ilCurrentPos].ilTifDocumentName;
		case IL_TIF_AUTHNAME_STRING:
			return (ILconst_string)State[ilCurrentPos].ilTifAuthName;
		case IL_CHEAD_HEADER_STRING:
			return (ILconst_string)State[ilCurrentPos].ilCHeader;
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
			return iClipString(State[ilCurrentPos].ilTgaId, 254);
		case IL_TGA_AUTHNAME_STRING:
			return iClipString(State[ilCurrentPos].ilTgaAuthName, 40);
		case IL_TGA_AUTHCOMMENT_STRING:
			return iClipString(State[ilCurrentPos].ilTgaAuthComment, 80);
		case IL_PNG_AUTHNAME_STRING:
			return iClipString(State[ilCurrentPos].ilPngAuthName, 255);
		case IL_PNG_TITLE_STRING:
			return iClipString(State[ilCurrentPos].ilPngTitle, 255);
		case IL_PNG_DESCRIPTION_STRING:
			return iClipString(State[ilCurrentPos].ilPngDescription, 255);

		//changed 2003-08-31...here was a serious copy and paste bug ;-)
		case IL_TIF_DESCRIPTION_STRING:
			return iClipString(State[ilCurrentPos].ilTifDescription, 255);
		case IL_TIF_HOSTCOMPUTER_STRING:
			return iClipString(State[ilCurrentPos].ilTifHostComputer, 255);
		case IL_TIF_DOCUMENTNAME_STRING:
			return iClipString(State[ilCurrentPos].ilTifDocumentName, 255);
		case IL_TIF_AUTHNAME_STRING:
			return iClipString(State[ilCurrentPos].ilTifAuthName, 255);
		case IL_CHEAD_HEADER_STRING:
			return iClipString(State[ilCurrentPos].ilCHeader, 32);
		default:
			ilSetError(IL_INVALID_ENUM);
	}
	return NULL;
}


//! Enables a mode
ILboolean ILAPIENTRY ilEnable(ILenum Mode)
{
	return ilAble(Mode, IL_TRUE);
}


//! Disables a mode
ILboolean ILAPIENTRY ilDisable(ILenum Mode)
{
	return ilAble(Mode, IL_FALSE);
}


// Internal function that sets the Mode equal to Flag
ILboolean ilAble(ILenum Mode, ILboolean Flag, ILstate *State)
{
	switch (Mode)
	{
		case IL_ORIGIN_SET:
			State[ilCurrentPos].ilOriginSet = Flag;
			break;
		case IL_FORMAT_SET:
			State[ilCurrentPos].ilFormatSet = Flag;
			break;
		case IL_TYPE_SET:
			State[ilCurrentPos].ilTypeSet = Flag;
			break;
		case IL_CONV_PAL:
			State[ilCurrentPos].ilAutoConvPal = Flag;
			break;
		case IL_DEFAULT_ON_FAIL:
			State[ilCurrentPos].ilDefaultOnFail = Flag;
			break;
		case IL_USE_KEY_COLOUR:
			State[ilCurrentPos].ilUseKeyColour = Flag;
			break;
		case IL_BLIT_BLEND:
			State[ilCurrentPos].ilBlitBlend = Flag;
			break;
		case IL_SAVE_INTERLACED:
			State[ilCurrentPos].ilInterlace = Flag;
			break;
		case IL_JPG_PROGRESSIVE:
			State[ilCurrentPos].ilJpgProgressive = Flag;
			break;
		case IL_NVIDIA_COMPRESS:
			State[ilCurrentPos].ilUseNVidiaDXT = Flag;
			break;
		case IL_SQUISH_COMPRESS:
			State[ilCurrentPos].ilUseSquishDXT = Flag;
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
			return State[ilCurrentPos].ilOriginSet;
		case IL_FORMAT_SET:
			return State[ilCurrentPos].ilFormatSet;
		case IL_TYPE_SET:
			return State[ilCurrentPos].ilTypeSet;
		case IL_CONV_PAL:
			return State[ilCurrentPos].ilAutoConvPal;
		case IL_DEFAULT_ON_FAIL:
			return State[ilCurrentPos].ilDefaultOnFail;
		case IL_USE_KEY_COLOUR:
			return State[ilCurrentPos].ilUseKeyColour;
		case IL_BLIT_BLEND:
			return State[ilCurrentPos].ilBlitBlend;
		case IL_SAVE_INTERLACED:
			return State[ilCurrentPos].ilInterlace;
		case IL_JPG_PROGRESSIVE:
			return State[ilCurrentPos].ilJpgProgressive;
		case IL_NVIDIA_COMPRESS:
			return State[ilCurrentPos].ilUseNVidiaDXT;
		case IL_SQUISH_COMPRESS:
			return State[ilCurrentPos].ilUseSquishDXT;

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
	ilGetBooleanv(Mode, &Temp);
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
			*Param = State[ilCurrentPos].ilCompression;
			break;
		//case IL_CUR_IMAGE:
		//	if (iCurImage == NULL) {
		//		ilSetError(IL_ILLEGAL_OPERATION);
		//		break;
		//	}
		//	*Param = ilGetCurName();
		//	break;
		case IL_FORMAT_MODE:
			*Param = State[ilCurrentPos].ilFormatMode;
			break;
		case IL_INTERLACE_MODE:
			*Param = State[ilCurrentPos].ilInterlace;
			break;
		case IL_KEEP_DXTC_DATA:
			*Param = State[ilCurrentPos].ilKeepDxtcData;
			break;
		case IL_ORIGIN_MODE:
			*Param = State[ilCurrentPos].ilOriginMode;
			break;
		case IL_MAX_QUANT_INDICES:
			*Param = State[ilCurrentPos].ilQuantMaxIndexs;
			break;
		case IL_NEU_QUANT_SAMPLE:
			*Param = State[ilCurrentPos].ilNeuSample;
			break;
		case IL_QUANTIZATION_MODE:
			*Param = State[ilCurrentPos].ilQuantMode;
			break;
		case IL_TYPE_MODE:
			*Param = State[ilCurrentPos].ilTypeMode;
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
			*Param = State[ilCurrentPos].ilBmpRle;
			break;
		case IL_DXTC_FORMAT:
			*Param = State[ilCurrentPos].ilDxtcFormat;
			break;
		case IL_JPG_QUALITY:
			*Param = State[ilCurrentPos].ilJpgQuality;
			break;
		case IL_JPG_SAVE_FORMAT:
			*Param = State[ilCurrentPos].ilJpgFormat;
			break;
		case IL_PCD_PICNUM:
			*Param = State[ilCurrentPos].ilPcdPicNum;
			break;
		case IL_PNG_ALPHA_INDEX:
			*Param = State[ilCurrentPos].ilPngAlphaIndex;
			break;
		case IL_PNG_INTERLACE:
			*Param = State[ilCurrentPos].ilPngInterlace;
			break;
		case IL_SGI_RLE:
			*Param = State[ilCurrentPos].ilSgiRle;
			break;
		case IL_TGA_CREATE_STAMP:
			*Param = State[ilCurrentPos].ilTgaCreateStamp;
			break;
		case IL_TGA_RLE:
			*Param = State[ilCurrentPos].ilTgaRle;
			break;
		case IL_VTF_COMP:
			*Param = State[ilCurrentPos].ilVtfCompression;
			break;

		// Boolean values
		case IL_CONV_PAL:
			*Param = State[ilCurrentPos].ilAutoConvPal;
			break;
		case IL_DEFAULT_ON_FAIL:
			*Param = State[ilCurrentPos].ilDefaultOnFail;
			break;
		case IL_FORMAT_SET:
			*Param = State[ilCurrentPos].ilFormatSet;
			break;
		case IL_ORIGIN_SET:
			*Param = State[ilCurrentPos].ilOriginSet;
			break;
		case IL_TYPE_SET:
			*Param = State[ilCurrentPos].ilTypeSet;
			break;
		case IL_USE_KEY_COLOUR:
			*Param = State[ilCurrentPos].ilUseKeyColour;
			break;
		case IL_BLIT_BLEND:
			*Param = State[ilCurrentPos].ilBlitBlend;
			break;
		case IL_JPG_PROGRESSIVE:
			*Param = State[ilCurrentPos].ilJpgProgressive;
			break;
		case IL_NVIDIA_COMPRESS:
			*Param = State[ilCurrentPos].ilUseNVidiaDXT;
			break;
		case IL_SQUISH_COMPRESS:
			*Param = State[ilCurrentPos].ilUseSquishDXT;
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
			State[ilCurrentPos].ilOriginMode = Mode;
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
			State[ilCurrentPos].ilFormatMode = Mode;
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
			State[ilCurrentPos].ilTypeMode = Mode;
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
			State[ilCurrentPos].ilCompression = Mode;
			break;
		default:
			ilSetError(IL_INVALID_PARAM);
			return IL_FALSE;
	}
	return IL_TRUE;
}


//! Pushes the states indicated by Bits onto the state stack
void ILAPIENTRY ilPushAttrib(ILuint Bits, ILstate *State)
{
	// Should we check here to see if ilCurrentPos is negative?

	if (ilCurrentPos >= IL_ATTRIB_STACK_MAX - 1) {
		ilCurrentPos = IL_ATTRIB_STACK_MAX - 1;
		ilSetError(IL_STACK_OVERFLOW);
		return;
	}

	ilCurrentPos++;

	//	memcpy(&State[ilCurrentPos], &State[ilCurrentPos - 1], sizeof(IL_STATES));

	ilDefaultStates();

	if (Bits & IL_ORIGIN_BIT) {
		State[ilCurrentPos].ilOriginMode = State[ilCurrentPos-1].ilOriginMode;
		State[ilCurrentPos].ilOriginSet  = State[ilCurrentPos-1].ilOriginSet;
	}
	if (Bits & IL_FORMAT_BIT) {
		State[ilCurrentPos].ilFormatMode = State[ilCurrentPos-1].ilFormatMode;
		State[ilCurrentPos].ilFormatSet  = State[ilCurrentPos-1].ilFormatSet;
	}
	if (Bits & IL_TYPE_BIT) {
		State[ilCurrentPos].ilTypeMode = State[ilCurrentPos-1].ilTypeMode;
		State[ilCurrentPos].ilTypeSet  = State[ilCurrentPos-1].ilTypeSet;
	}
	if (Bits & IL_PAL_BIT) {
		State[ilCurrentPos].ilAutoConvPal = State[ilCurrentPos-1].ilAutoConvPal;
	}
	if (Bits & IL_LOADFAIL_BIT) {
		State[ilCurrentPos].ilDefaultOnFail = State[ilCurrentPos-1].ilDefaultOnFail;
	}
	if (Bits & IL_COMPRESS_BIT) {
		State[ilCurrentPos].ilCompression = State[ilCurrentPos-1].ilCompression;
	}
	if (Bits & IL_FORMAT_SPECIFIC_BIT) {
		State[ilCurrentPos].ilTgaCreateStamp = State[ilCurrentPos-1].ilTgaCreateStamp;
		State[ilCurrentPos].ilJpgQuality = State[ilCurrentPos-1].ilJpgQuality;
		State[ilCurrentPos].ilPngInterlace = State[ilCurrentPos-1].ilPngInterlace;
		State[ilCurrentPos].ilTgaRle = State[ilCurrentPos-1].ilTgaRle;
		State[ilCurrentPos].ilBmpRle = State[ilCurrentPos-1].ilBmpRle;
		State[ilCurrentPos].ilSgiRle = State[ilCurrentPos-1].ilSgiRle;
		State[ilCurrentPos].ilJpgFormat = State[ilCurrentPos-1].ilJpgFormat;
		State[ilCurrentPos].ilDxtcFormat = State[ilCurrentPos-1].ilDxtcFormat;
		State[ilCurrentPos].ilPcdPicNum = State[ilCurrentPos-1].ilPcdPicNum;

		State[ilCurrentPos].ilPngAlphaIndex = State[ilCurrentPos-1].ilPngAlphaIndex;

		// Strings
		if (State[ilCurrentPos].ilTgaId)
			ifree(State[ilCurrentPos].ilTgaId);
		if (State[ilCurrentPos].ilTgaAuthName)
			ifree(State[ilCurrentPos].ilTgaAuthName);
		if (State[ilCurrentPos].ilTgaAuthComment)
			ifree(State[ilCurrentPos].ilTgaAuthComment);
		if (State[ilCurrentPos].ilPngAuthName)
			ifree(State[ilCurrentPos].ilPngAuthName);
		if (State[ilCurrentPos].ilPngTitle)
			ifree(State[ilCurrentPos].ilPngTitle);
		if (State[ilCurrentPos].ilPngDescription)
			ifree(State[ilCurrentPos].ilPngDescription);

		//2003-09-01: added tif strings
		if (State[ilCurrentPos].ilTifDescription)
			ifree(State[ilCurrentPos].ilTifDescription);
		if (State[ilCurrentPos].ilTifHostComputer)
			ifree(State[ilCurrentPos].ilTifHostComputer);
		if (State[ilCurrentPos].ilTifDocumentName)
			ifree(State[ilCurrentPos].ilTifDocumentName);
		if (State[ilCurrentPos].ilTifAuthName)
			ifree(State[ilCurrentPos].ilTifAuthName);

		if (State[ilCurrentPos].ilCHeader)
			ifree(State[ilCurrentPos].ilCHeader);

		State[ilCurrentPos].ilTgaId = strdup(State[ilCurrentPos-1].ilTgaId);
		State[ilCurrentPos].ilTgaAuthName = strdup(State[ilCurrentPos-1].ilTgaAuthName);
		State[ilCurrentPos].ilTgaAuthComment = strdup(State[ilCurrentPos-1].ilTgaAuthComment);
		State[ilCurrentPos].ilPngAuthName = strdup(State[ilCurrentPos-1].ilPngAuthName);
		State[ilCurrentPos].ilPngTitle = strdup(State[ilCurrentPos-1].ilPngTitle);
		State[ilCurrentPos].ilPngDescription = strdup(State[ilCurrentPos-1].ilPngDescription);

		//2003-09-01: added tif strings
		State[ilCurrentPos].ilTifDescription = strdup(State[ilCurrentPos-1].ilTifDescription);
		State[ilCurrentPos].ilTifHostComputer = strdup(State[ilCurrentPos-1].ilTifHostComputer);
		State[ilCurrentPos].ilTifDocumentName = strdup(State[ilCurrentPos-1].ilTifDocumentName);
		State[ilCurrentPos].ilTifAuthName = strdup(State[ilCurrentPos-1].ilTifAuthName);

		State[ilCurrentPos].ilCHeader = strdup(State[ilCurrentPos-1].ilCHeader);
	}

	return;
}


// @TODO:  Find out how this affects strings!!!

//! Pops the last entry off the state stack into the current states
void ILAPIENTRY ilPopAttrib()
{
	if (ilCurrentPos <= 0) {
		ilCurrentPos = 0;
		ilSetError(IL_STACK_UNDERFLOW);
		return;
	}

	// Should we check here to see if ilCurrentPos is too large?
	ilCurrentPos--;

	return;
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
			if (State[ilCurrentPos].ilTgaId)
				ifree(State[ilCurrentPos].ilTgaId);
			State[ilCurrentPos].ilTgaId = strdup(String);
			break;
		case IL_TGA_AUTHNAME_STRING:
			if (State[ilCurrentPos].ilTgaAuthName)
				ifree(State[ilCurrentPos].ilTgaAuthName);
			State[ilCurrentPos].ilTgaAuthName = strdup(String);
			break;
		case IL_TGA_AUTHCOMMENT_STRING:
			if (State[ilCurrentPos].ilTgaAuthComment)
				ifree(State[ilCurrentPos].ilTgaAuthComment);
			State[ilCurrentPos].ilTgaAuthComment = strdup(String);
			break;
		case IL_PNG_AUTHNAME_STRING:
			if (State[ilCurrentPos].ilPngAuthName)
				ifree(State[ilCurrentPos].ilPngAuthName);
			State[ilCurrentPos].ilPngAuthName = strdup(String);
			break;
		case IL_PNG_TITLE_STRING:
			if (State[ilCurrentPos].ilPngTitle)
				ifree(State[ilCurrentPos].ilPngTitle);
			State[ilCurrentPos].ilPngTitle = strdup(String);
			break;
		case IL_PNG_DESCRIPTION_STRING:
			if (State[ilCurrentPos].ilPngDescription)
				ifree(State[ilCurrentPos].ilPngDescription);
			State[ilCurrentPos].ilPngDescription = strdup(String);
			break;

		//2003-09-01: added tif strings
		case IL_TIF_DESCRIPTION_STRING:
			if (State[ilCurrentPos].ilTifDescription)
				ifree(State[ilCurrentPos].ilTifDescription);
			State[ilCurrentPos].ilTifDescription = strdup(String);
			break;
		case IL_TIF_HOSTCOMPUTER_STRING:
			if (State[ilCurrentPos].ilTifHostComputer)
				ifree(State[ilCurrentPos].ilTifHostComputer);
			State[ilCurrentPos].ilTifHostComputer = strdup(String);
			break;
		case IL_TIF_DOCUMENTNAME_STRING:
						if (State[ilCurrentPos].ilTifDocumentName)
				ifree(State[ilCurrentPos].ilTifDocumentName);
			State[ilCurrentPos].ilTifDocumentName = strdup(String);
			break;
		case IL_TIF_AUTHNAME_STRING:
			if (State[ilCurrentPos].ilTifAuthName)
				ifree(State[ilCurrentPos].ilTifAuthName);
			State[ilCurrentPos].ilTifAuthName = strdup(String);
			break;

		case IL_CHEAD_HEADER_STRING:
			if (State[ilCurrentPos].ilCHeader)
				ifree(State[ilCurrentPos].ilCHeader);
			State[ilCurrentPos].ilCHeader = strdup(String);
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
			ilFormatFunc(Param);
			return;
		case IL_KEEP_DXTC_DATA:
			if (Param == IL_FALSE || Param == IL_TRUE) {
				State[ilCurrentPos].ilKeepDxtcData = Param;
				return;
			}
			break;
		case IL_MAX_QUANT_INDICES:
			if (Param >= 2 && Param <= 256) {
				State[ilCurrentPos].ilQuantMaxIndexs = Param;
				return;
			}
			break;
		case IL_NEU_QUANT_SAMPLE:
			if (Param >= 1 && Param <= 30) {
				State[ilCurrentPos].ilNeuSample = Param;
				return;
			}
			break;
		case IL_ORIGIN_MODE:
			ilOriginFunc(Param);
			return;
		case IL_QUANTIZATION_MODE:
			if (Param == IL_WU_QUANT || Param == IL_NEU_QUANT) {
				State[ilCurrentPos].ilQuantMode = Param;
				return;
			}
			break;
		case IL_TYPE_MODE:
			ilTypeFunc(Param);
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
				State[ilCurrentPos].ilBmpRle = Param;
				return;
			}
			break;
		case IL_DXTC_FORMAT:
			if (Param >= IL_DXT1 || Param <= IL_DXT5 || Param == IL_DXT1A) {
				State[ilCurrentPos].ilDxtcFormat = Param;
				return;
			}
			break;
		case IL_JPG_SAVE_FORMAT:
			if (Param == IL_JFIF || Param == IL_EXIF) {
				State[ilCurrentPos].ilJpgFormat = Param;
				return;
			}
			break;
		case IL_JPG_QUALITY:
			if (Param >= 0 && Param <= 99) {
				State[ilCurrentPos].ilJpgQuality = Param;
				return;
			}
			break;
		case IL_PNG_INTERLACE:
			if (Param == IL_FALSE || Param == IL_TRUE) {
				State[ilCurrentPos].ilPngInterlace = Param;
				return;
			}
			break;
		case IL_PCD_PICNUM:
			if (Param >= 0 || Param <= 2) {
				State[ilCurrentPos].ilPcdPicNum = Param;
				return;
			}
			break;
		case IL_PNG_ALPHA_INDEX:
			if (Param >= -1 || Param <= 255) {
				State[ilCurrentPos].ilPngAlphaIndex=Param;
				return;
			}
			break;
		case IL_SGI_RLE:
			if (Param == IL_FALSE || Param == IL_TRUE) {
				State[ilCurrentPos].ilSgiRle = Param;
				return;
			}
			break;
		case IL_TGA_CREATE_STAMP:
			if (Param == IL_FALSE || Param == IL_TRUE) {
				State[ilCurrentPos].ilTgaCreateStamp = Param;
				return;
			}
			break;
		case IL_TGA_RLE:
			if (Param == IL_FALSE || Param == IL_TRUE) {
				State[ilCurrentPos].ilTgaRle = Param;
				return;
			}
			break;
		case IL_VTF_COMP:
			if (Param == IL_DXT1 || Param == IL_DXT5 || Param == IL_DXT3 || Param == IL_DXT1A || Param == IL_DXT_NO_COMP) {
				State[ilCurrentPos].ilVtfCompression = Param;
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
