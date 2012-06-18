//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 03/07/2009
//
// Filename: src-IL/src/il_io.c
//
// Description: Determines image types and loads/saves images
//
//-----------------------------------------------------------------------------

#include "il_internal.h"
#include "il_register.h"
#include "il_pal.h"
#include <string.h>


// Returns a widened version of a string.
// Make sure to free this after it is used.  Code help from
//  https://buildsecurityin.us-cert.gov/daisy/bsi-rules/home/g1/769-BSI.html
#if defined(_UNICODE)
wchar_t *WideFromMultiByte(const char *Multi)
{
	ILint	Length;
	wchar_t	*Temp;

	Length = (ILint)mbstowcs(NULL, (const char*)Multi, 0) + 1; // note error return of -1 is possible
	if (Length == 0) {
		ilSetError(IL_INVALID_PARAM);
		return NULL;
	}
	if (Length > ULONG_MAX/sizeof(wchar_t)) {
		ilSetError(IL_INTERNAL_ERROR);
		return NULL;
	}
	Temp = (wchar_t*)ialloc(Length * sizeof(wchar_t));
	mbstowcs(Temp, (const char*)Multi, Length); 

	return Temp;
}
#endif


ILenum ILAPIENTRY ilTypeFromExt(ILconst_string FileName)
{
	ILenum		Type;
	ILstring	Ext;

	if (FileName == NULL || ilStrLen(FileName) < 1) {
		ilSetError(IL_INVALID_PARAM);
		return IL_TYPE_UNKNOWN;
	}

	Ext = iGetExtension(FileName);
	//added 2003-08-31: fix sf bug 789535
	if (Ext == NULL) {
		return IL_TYPE_UNKNOWN;
	}

	Type = IL_TYPE_UNKNOWN;
	int i, j;
	for (i = 0; i < IL_FORMATS_COUNT && Type != IL_TYPE_UNKNOWN; i++)
	{
		if (Formats[i].Extensions == NULL)
			continue;
		for (j = 0; Formats[i].Extensions[j] != NULL; j++)
			if (!iStrCmp(Ext, Formats[i].Extensions[j]))
			{
				Type = i;
				break;
			}
	}

	return Type;
}


//changed 2003-09-17 to ILAPIENTRY
ILenum ILAPIENTRY ilDetermineType(ILconst_string FileName)
{
	ILHANDLE	File;
	ILenum		Type;

	if (FileName == NULL)
		return IL_TYPE_UNKNOWN;

	File = iopenr(FileName);
	if (File == NULL) {
		ilSetError(IL_COULD_NOT_OPEN_FILE);
		return IL_FALSE;
	}
	Type = ilDetermineTypeF(File);
	icloser(File);

	return Type;
}


ILenum ILAPIENTRY ilDetermineTypeF(ILHANDLE File)
{
	if (File == NULL)
		return IL_TYPE_UNKNOWN;

	int i;
	for (i = 0; i < IL_FORMATS_COUNT; i++)
	{
		if (Formats[i].Extensions == NULL)
			continue;
		if (Formats[i].Callbacks.ilIsValidF == NULL)
			continue;
		else if ( Formats[i].Callbacks.ilIsValidF(File) )
			return i;
	}
	
	return IL_TYPE_UNKNOWN;
}


ILenum ILAPIENTRY ilDetermineTypeL(const void *Lump, ILuint Size)
{
	if (Lump == NULL)
		return IL_TYPE_UNKNOWN;

	int i;
	for (i = 0; i < IL_FORMATS_COUNT; i++)
	{
		if (Formats[i].Extensions == NULL)
			continue;
		if (Formats[i].Callbacks.ilIsValidL == NULL)
			continue;
		else if ( Formats[i].Callbacks.ilIsValidL(Lump, Size) )
			return i;
	}

	return IL_TYPE_UNKNOWN;
}


ILboolean ILAPIENTRY ilIsValid(ILenum Type, ILconst_string FileName)
{
	if (FileName == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	if (Formats[Type].Callbacks.ilIsValid == NULL)
	{
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}
	
       	if (Formats[Type].Callbacks.ilIsValid(FileName) == IL_FALSE)
	{	
		ilSetError(IL_INVALID_ENUM);
		return IL_FALSE;
	}
	return IL_TRUE;
}


ILboolean ILAPIENTRY ilIsValidF(ILenum Type, ILHANDLE File)
{
	if (File == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	if (Formats[Type].Callbacks.ilIsValidF == NULL)
	{
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}
	return Formats[Type].Callbacks.ilIsValidF(File);

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


ILboolean ILAPIENTRY ilIsValidL(ILenum Type, void *Lump, ILuint Size)
{
	if (Lump == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	if (Formats[Type].Callbacks.ilIsValidL == NULL)
	{
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}
	return Formats[Type].Callbacks.ilIsValidL(Lump, Size);

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


//! Attempts to load an image from a file.  The file format is specified by the user.
/*! \param Type Format of this file.  Acceptable values are IL_BLP, IL_BMP, IL_CUT, IL_DCX, IL_DDS,
	IL_DICOM, IL_DOOM, IL_DOOM_FLAT, IL_DPX, IL_EXR, IL_FITS, IL_FTX, IL_GIF, IL_HDR, IL_ICO, IL_ICNS,
	IL_IFF, IL_IWI, IL_JP2, IL_JPG, IL_LIF, IL_MDL,	IL_MNG, IL_MP3, IL_PCD, IL_PCX, IL_PIX, IL_PNG,
	IL_PNM, IL_PSD, IL_PSP, IL_PXR, IL_ROT, IL_SGI, IL_SUN, IL_TEXTURE, IL_TGA, IL_TIFF, IL_TPL,
	IL_UTX, IL_VTF, IL_WAL, IL_WBMP, IL_XPM, IL_RAW, IL_JASC_PAL and IL_TYPE_UNKNOWN.
	If IL_TYPE_UNKNOWN is specified, ilLoad will try to determine the type of the file and load it.
	\param FileName Ansi or Unicode string, depending on the compiled version of DevIL, that gives
	       the filename of the file to load.
	\return Boolean value of failure or success.  Returns IL_FALSE if all three loading methods
	       have been tried and failed.*/
ILboolean ILAPIENTRY ilLoad(ILenum Type, ILconst_string FileName)
{
	ILboolean	bRet;

	if (FileName == NULL || ilStrLen(FileName) < 1) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}
	if (Formats[Type].Callbacks.ilLoad == NULL)
	{
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}
	bRet = Formats[Type].Callbacks.ilLoad(FileName);

	return bRet;
}


//! Attempts to load an image from a file stream.  The file format is specified by the user.
/*! \param Type Format of this file. You can look up acceptable values in include/IL/il.h.
	If IL_TYPE_UNKNOWN is specified, ilLoadF will try to determine the type of the file and load it.
	\param File File stream to load from.
	\return Boolean value of failure or success.  Returns IL_FALSE if loading fails.*/
ILboolean ILAPIENTRY ilLoadF(ILenum Type, ILHANDLE File)
{
	if (File == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	if (Type == IL_TYPE_UNKNOWN)
		Type = ilDetermineTypeF(File);
	
	if (Formats[Type].Callbacks.ilLoadF == NULL)
	{
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}
	return Formats[Type].Callbacks.ilLoadF(File);

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


//! Attempts to load an image from a memory buffer.  The file format is specified by the user.
/*! \param Type Format of this file. You can look up acceptable values in include/IL/il.h.
	If IL_TYPE_UNKNOWN is specified, ilLoadL will try to determine the type of the file and load it.
	\param Lump The buffer where the file data is located
	\param Size Size of the buffer
	\return Boolean value of failure or success.  Returns IL_FALSE if loading fails.*/
ILboolean ILAPIENTRY ilLoadL(ILenum Type, const void *Lump, ILuint Size)
{
	if (Lump == NULL || Size == 0) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	if (Type == IL_TYPE_UNKNOWN)
		Type = ilDetermineTypeL(Lump, Size);
	/* format name can be told in terms of its extension */
	if (Formats[Type].Callbacks.ilLoadL == NULL)
	{
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}
	return Formats[Type].Callbacks.ilLoadL(Lump, Size);

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


//! Attempts to load an image from a file with various different methods before failing - very generic.
/*! The ilLoadImage function allows a general interface to the specific internal file-loading
	routines.  First, it finds the extension and checks to see if any user-registered functions
	(registered through ilRegisterLoad) match the extension. If nothing matches, it takes the
	extension and determines which function to call based on it. Lastly, it attempts to identify
	the image based on various image header verification functions, such as ilIsValidPngF.
	If all this checking fails, IL_FALSE is returned with no modification to the current bound image.
	\param FileName Ansi or Unicode string, depending on the compiled version of DevIL, that gives
	       the filename of the file to load.
	\return Boolean value of failure or success.  Returns IL_FALSE if all three loading methods
	       have been tried and failed.*/
ILboolean ILAPIENTRY ilLoadImage(ILconst_string FileName)
{
	ILstring	Ext;
	ILenum		Type;
	ILboolean	bRet = IL_FALSE;

	LOG_ACTION_BEGIN(il_load_image, IL_LOG_INFO, "Trying to load file '%s'", FileName);

	if (iCurImage == NULL) {
		LOG_ACTION_END(il_load_image, LOG_RES_FAIL);
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (FileName == NULL || ilStrLen(FileName) < 1) {
		LOG_ACTION_END(il_load_image, LOG_RES_FAIL);
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	Ext = iGetExtension(FileName);

	// Try registered procedures first (so users can override default lib functions).
	if (iRegisterLoad(FileName))
	{
		LOG_ACTION_END(il_load_image, LOG_RES_OK);
		return IL_TRUE;
	}

	int i, j;
	for (i = 0; i < IL_FORMATS_COUNT; i++)
	{
		if (Formats[i].Extensions == NULL)
			continue;
		for (j = 0; Formats[i].Extensions[j] != NULL; j++)
			if (!iStrCmp(Ext, Formats[i].Extensions[j]))
			{
				if (Formats[i].Callbacks.ilLoad == NULL)
				{
					LOG_ACTION_END(il_load_image, LOG_RES_FAIL);
					ilSetError(IL_FORMAT_NOT_SUPPORTED);
					return IL_FALSE;
				}
				bRet = Formats[i].Callbacks.ilLoad(FileName);
				goto finish;
			}
	}

	// As a last-ditch effort, try to identify the image
	Type = ilDetermineType(FileName);
	if (Type == IL_TYPE_UNKNOWN) {
		LOG_ACTION_END(il_load_image, LOG_RES_FAIL);
		ilSetError(IL_INVALID_EXTENSION);
		return IL_FALSE;
	}
	bRet = ilLoad(Type, FileName);

finish:
	LOG_ACTION_END(il_load_image, (bRet == IL_TRUE) ? LOG_RES_OK : LOG_RES_FAIL);
	return bRet;
}


//! Attempts to save an image to a file.  The file format is specified by the user.
/*! \param Type Format of this file.  Acceptable values are IL_BMP, IL_CHEAD, IL_DDS, IL_EXR,
	IL_HDR, IL_JP2, IL_JPG, IL_PCX, IL_PNG, IL_PNM, IL_PSD, IL_RAW, IL_SGI, IL_TGA, IL_TIFF,
	IL_VTF, IL_WBMP and IL_JASC_PAL.
	\param FileName Ansi or Unicode string, depending on the compiled version of DevIL, that gives
	       the filename to save to.
	\return Boolean value of failure or success.  Returns IL_FALSE if saving failed.*/
ILboolean ILAPIENTRY ilSave(ILenum Type, ILconst_string FileName)
{
	if (Formats[Type].Callbacks.ilSave == NULL)
	{
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}
	ILboolean retval =  Formats[Type].Callbacks.ilSave(FileName);
	if (retval == IL_FALSE)
	{
		ilSetError(IL_INVALID_ENUM);
	}
	else
	return retval;
}


//! Attempts to save an image to a file stream.  The file format is specified by the user.
/*! \param Type Format of this file.  Acceptable values are IL_BMP, IL_CHEAD, IL_DDS, IL_EXR,
	IL_HDR, IL_JP2, IL_JPG, IL_PCX, IL_PNG, IL_PNM, IL_PSD, IL_RAW, IL_SGI, IL_TGA, IL_TIFF,
	IL_VTF, IL_WBMP and IL_JASC_PAL.
	\param File File stream to save to.
	\return Boolean value of failure or success.  Returns IL_FALSE if saving failed.*/
ILuint ILAPIENTRY ilSaveF(ILenum Type, ILHANDLE File)
{
	ILboolean Ret;

	if (File == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return 0;
	}

	if (Formats[Type].Callbacks.ilSaveF == NULL)
	{
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}
	return Formats[Type].Callbacks.ilSaveF(File);

	if (Ret == IL_FALSE)
		return 0;

	return itellw();
}


//! Attempts to save an image to a memory buffer.  The file format is specified by the user.
/*! \param Type Format of this image file.  Acceptable values are IL_BMP, IL_CHEAD, IL_DDS, IL_EXR,
	IL_HDR, IL_JP2, IL_JPG, IL_PCX, IL_PNG, IL_PNM, IL_PSD, IL_RAW, IL_SGI, IL_TGA, IL_TIFF,
	IL_VTF, IL_WBMP and IL_JASC_PAL.
	\param Lump Memory buffer to save to
	\param Size Size of the memory buffer
	\return Boolean value of failure or success.  Returns IL_FALSE if saving failed.*/
ILuint ILAPIENTRY ilSaveL(ILenum Type, void *Lump, ILuint Size)
{
	if (Lump == NULL) {
		if (Size != 0) {
			ilSetError(IL_INVALID_PARAM);
			return 0;
		}
		// The user wants to know how large of a buffer they need.
		else {
			return ilDetermineSize(Type);
		}
	}

	if (Formats[Type].Callbacks.ilSaveL == NULL)
	{
		ilSetError(IL_FORMAT_NOT_SUPPORTED);
		return IL_FALSE;
	}
	return Formats[Type].Callbacks.ilSaveL(Lump, Size);

	ilSetError(IL_INVALID_ENUM);
	return 0;
}


/** Saves the current image based on the extension given in FileName.
 * \param FileName Ansi or Unicode string, depending on the compiled version of DevIL, that gives the filename to save to.
 * \return Boolean value of failure or success.  Returns IL_FALSE if saving failed.
 */
ILboolean ILAPIENTRY ilSaveImage(ILconst_string FileName)
{
	LOG_ACTION_BEGIN(save_image, IL_LOG_INFO, "Trying to save image to '%s'", FileName);
	ILstring Ext;
	ILboolean	bRet = IL_FALSE;

	if (FileName == NULL || ilStrLen(FileName) < 1) {
		LOG_ACTION_END(save_image, LOG_RES_FAIL);
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	if (iCurImage == NULL) {
		LOG_ACTION_END(save_image, LOG_RES_FAIL);
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Ext = iGetExtension(FileName);
	if (Ext == NULL) {
		LOG_ACTION_END(save_image, LOG_RES_FAIL);
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	int i, j;
	for (i = 0; i < IL_FORMATS_COUNT; i++)
	{
		if (Formats[i].Extensions == NULL)
			continue;
		for (j = 0; Formats[i].Extensions[j] != NULL; j++)
			if (!iStrCmp(Ext, Formats[i].Extensions[j]))
			{
				if (Formats[i].Callbacks.ilSave == NULL)
				{
					LOG_ACTION_END(save_image, LOG_RES_FAIL);
					ilSetError(IL_FORMAT_NOT_SUPPORTED);
					return IL_FALSE;
				}
				bRet = Formats[i].Callbacks.ilSave(FileName);
				goto finish;
			}
	}

	// Try registered procedures
	if (iRegisterSave(FileName))
	{
		LOG_ACTION_END(save_image, LOG_RES_OK);
		return IL_TRUE;
	}

	LOG_ACTION_END(save_image, LOG_RES_FAIL);
	ilSetError(IL_INVALID_EXTENSION);
	return IL_FALSE;

finish:
	LOG_ACTION_END(save_image,  (bRet == IL_TRUE) ? LOG_RES_OK : LOG_RES_FAIL);
	return bRet;
}

