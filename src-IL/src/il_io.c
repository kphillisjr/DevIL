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

	if (!iStrCmp(Ext, IL_TEXT("tga")) || !iStrCmp(Ext, IL_TEXT("vda")) ||
		!iStrCmp(Ext, IL_TEXT("icb")) || !iStrCmp(Ext, IL_TEXT("vst")))
		Type = IL_TGA;
	else if (!iStrCmp(Ext, IL_TEXT("jpg")) || !iStrCmp(Ext, IL_TEXT("jpe")) ||
		!iStrCmp(Ext, IL_TEXT("jpeg")) || !iStrCmp(Ext, IL_TEXT("jif")) || !iStrCmp(Ext, IL_TEXT("jfif")))
		Type = IL_JPG;
	else if (!iStrCmp(Ext, IL_TEXT("jp2")) || !iStrCmp(Ext, IL_TEXT("jpx")) ||
		!iStrCmp(Ext, IL_TEXT("j2k")) || !iStrCmp(Ext, IL_TEXT("j2c")))
		Type = IL_JP2;
	else if (!iStrCmp(Ext, IL_TEXT("dds")))
		Type = IL_DDS;
	else if (!iStrCmp(Ext, IL_TEXT("png")))
		Type = IL_PNG;
	else if (!iStrCmp(Ext, IL_TEXT("bmp")) || !iStrCmp(Ext, IL_TEXT("dib")))
		Type = IL_BMP;
	else if (!iStrCmp(Ext, IL_TEXT("gif")))
		Type = IL_GIF;
	else if (!iStrCmp(Ext, IL_TEXT("blp")))
		Type = IL_BLP;
	else if (!iStrCmp(Ext, IL_TEXT("cut")))
		Type = IL_CUT;
	else if (!iStrCmp(Ext, IL_TEXT("dcm")) || !iStrCmp(Ext, IL_TEXT("dicom")))
		Type = IL_DICOM;
	else if (!iStrCmp(Ext, IL_TEXT("dpx")))
		Type = IL_DPX;
	else if (!iStrCmp(Ext, IL_TEXT("exr")))
		Type = IL_EXR;
	else if (!iStrCmp(Ext, IL_TEXT("fit")) || !iStrCmp(Ext, IL_TEXT("fits")))
		Type = IL_FITS;
	else if (!iStrCmp(Ext, IL_TEXT("ftx")))
		Type = IL_FTX;
	else if (!iStrCmp(Ext, IL_TEXT("hdr")))
		Type = IL_HDR;
	else if (!iStrCmp(Ext, IL_TEXT("iff")))
		Type = IL_IFF;
	else if (!iStrCmp(Ext, IL_TEXT("ico")) || !iStrCmp(Ext, IL_TEXT("cur")))
		Type = IL_ICO;
	else if (!iStrCmp(Ext, IL_TEXT("icns")))
		Type = IL_ICNS;
	else if (!iStrCmp(Ext, IL_TEXT("iwi")))
		Type = IL_IWI;
	else if (!iStrCmp(Ext, IL_TEXT("iwi")))
		Type = IL_IWI;
	else if (!iStrCmp(Ext, IL_TEXT("jng")))
		Type = IL_JNG;
	else if (!iStrCmp(Ext, IL_TEXT("lif")))
		Type = IL_LIF;
	else if (!iStrCmp(Ext, IL_TEXT("mdl")))
		Type = IL_MDL;
	else if (!iStrCmp(Ext, IL_TEXT("mng")) || !iStrCmp(Ext, IL_TEXT("jng")))
		Type = IL_MNG;
	else if (!iStrCmp(Ext, IL_TEXT("mp3")))
		Type = IL_MP3;
	else if (!iStrCmp(Ext, IL_TEXT("pcd")))
		Type = IL_PCD;
	else if (!iStrCmp(Ext, IL_TEXT("pcx")))
		Type = IL_PCX;
	else if (!iStrCmp(Ext, IL_TEXT("pic")))
		Type = IL_PIC;
	else if (!iStrCmp(Ext, IL_TEXT("pix")))
		Type = IL_PIX;
	else if (!iStrCmp(Ext, IL_TEXT("pbm")) || !iStrCmp(Ext, IL_TEXT("pgm")) ||
		!iStrCmp(Ext, IL_TEXT("pnm")) || !iStrCmp(Ext, IL_TEXT("ppm")))
		Type = IL_PNM;
	else if (!iStrCmp(Ext, IL_TEXT("psd")) || !iStrCmp(Ext, IL_TEXT("pdd")))
		Type = IL_PSD;
	else if (!iStrCmp(Ext, IL_TEXT("psp")))
		Type = IL_PSP;
	else if (!iStrCmp(Ext, IL_TEXT("pxr")))
		Type = IL_PXR;
	else if (!iStrCmp(Ext, IL_TEXT("rot")))
		Type = IL_ROT;
	else if (!iStrCmp(Ext, IL_TEXT("sgi")) || !iStrCmp(Ext, IL_TEXT("bw")) ||
		!iStrCmp(Ext, IL_TEXT("rgb")) || !iStrCmp(Ext, IL_TEXT("rgba")))
		Type = IL_SGI;
	else if (!iStrCmp(Ext, IL_TEXT("sun")) || !iStrCmp(Ext, IL_TEXT("ras")) ||
			 !iStrCmp(Ext, IL_TEXT("rs")) || !iStrCmp(Ext, IL_TEXT("im1")) ||
			 !iStrCmp(Ext, IL_TEXT("im8")) || !iStrCmp(Ext, IL_TEXT("im24")) ||
			 !iStrCmp(Ext, IL_TEXT("im32")))
		Type = IL_SUN;
	else if (!iStrCmp(Ext, IL_TEXT("texture")))
		Type = IL_TEXTURE;
	else if (!iStrCmp(Ext, IL_TEXT("tif")) || !iStrCmp(Ext, IL_TEXT("tiff")))
		Type = IL_TIF;
	else if (!iStrCmp(Ext, IL_TEXT("tpl")))
		Type = IL_TPL;
	else if (!iStrCmp(Ext, IL_TEXT("vtf")))
		Type = IL_VTF;
	else if (!iStrCmp(Ext, IL_TEXT("wal")))
		Type = IL_WAL;
	else if (!iStrCmp(Ext, IL_TEXT("wbmp")))
		Type = IL_WBMP;
	else if (!iStrCmp(Ext, IL_TEXT("wdp")) || !iStrCmp(Ext, IL_TEXT("hdp")))
		Type = IL_WDP;
	else if (!iStrCmp(Ext, IL_TEXT("xpm")))
		Type = IL_XPM;
	else
		Type = IL_TYPE_UNKNOWN;

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

	#ifndef IL_NO_JPG
	if (ilIsValidF_JPEG(File))
		return IL_JPG;
	#endif

	#ifndef IL_NO_DDS
	if (ilIsValidF_DDS(File))
		return IL_DDS;
	#endif

	#ifndef IL_NO_PNG
	if (ilIsValidF_PNG(File))
		return IL_PNG;
	#endif

	#ifndef IL_NO_BMP
	if (ilIsValidF_BMP(File))
		return IL_BMP;
	#endif

	#ifndef IL_NO_EXR
	if (ilIsValidF_EXR(File))
		return IL_EXR;
	#endif

	#ifndef IL_NO_GIF
	if (ilIsValidF_GIF(File))
		return IL_GIF;
	#endif

	#ifndef IL_NO_HDR
	if (ilIsValidF_HDR(File))
		return IL_HDR;
	#endif

	#ifndef IL_NO_ICNS
	if (ilIsValidF_ICNS(File))
		return IL_ICNS;
	#endif

	#ifndef IL_NO_IWI
	if (ilIsValidF_IWI(File))
		return IL_IWI;
	#endif

	#ifndef IL_NO_JP2
	if (ilIsValidF_JP2(File))
		return IL_JP2;
	#endif

	#ifndef IL_NO_LIF
	if (ilIsValidF_LIF(File))
		return IL_LIF;
	#endif

	#ifndef IL_NO_MDL
	if (ilIsValidF_MDL(File))
		return IL_MDL;
	#endif

	#ifndef IL_NO_MDL
	if (ilIsValidMp3F(File))
		return IL_MP3;
	#endif

	#ifndef IL_NO_PCX
	if (ilIsValidF_PCX(File))
		return IL_PCX;
	#endif

	#ifndef IL_NO_PIC
	if (ilIsValidF_PIC(File))
		return IL_PIC;
	#endif

	#ifndef IL_NO_PNM
	if (ilIsValidF_PNM(File))
		return IL_PNM;
	#endif

	#ifndef IL_NO_PSD
	if (ilIsValidF_PSD(File))
		return IL_PSD;
	#endif

	#ifndef IL_NO_PSP
	if (ilIsValidF_PSP(File))
		return IL_PSP;
	#endif

	#ifndef IL_NO_SGI
	if (ilIsValidF_SGI(File))
		return IL_SGI;
	#endif

	#ifndef IL_NO_SUN
	if (ilIsValidF_SUN(File))
		return IL_SUN;
	#endif

	#ifndef IL_NO_TIF
	if (ilIsValidF_TIFF(File))
		return IL_TIF;
	#endif

	#ifndef IL_NO_TPL
	if (ilIsValidF_TPL(File))
		return IL_TPL;
	#endif

	#ifndef IL_NO_VTF
	if (ilIsValidF_VTF(File))
		return IL_VTF;
	#endif

	#ifndef IL_NO_XPM
	if (ilIsValidF_XPM(File))
		return IL_XPM;
	#endif

	//moved tga to end of list because it has no magic number
	//in header to assure that this is really a tga... (20040218)
	#ifndef IL_NO_TGA
	if (ilIsValidF_TGA(File))
		return IL_TGA;
	#endif
	
	return IL_TYPE_UNKNOWN;
}


ILenum ILAPIENTRY ilDetermineTypeL(const void *Lump, ILuint Size)
{
	if (Lump == NULL)
		return IL_TYPE_UNKNOWN;

	#ifndef IL_NO_JPG
	if (ilIsValidL_JPEG(Lump, Size))
		return IL_JPG;
	#endif

	#ifndef IL_NO_DDS
	if (ilIsValidL_DDS(Lump, Size))
		return IL_DDS;
	#endif

	#ifndef IL_NO_PNG
	if (ilIsValidL_PNG(Lump, Size))
		return IL_PNG;
	#endif

	#ifndef IL_NO_BMP
	if (ilIsValidL_BMP(Lump, Size))
		return IL_BMP;
	#endif

	#ifndef IL_NO_EXR
	if (ilIsValidL_EXR(Lump, Size))
		return IL_EXR;
	#endif

	#ifndef IL_NO_GIF
	if (ilIsValidL_GIF(Lump, Size))
		return IL_GIF;
	#endif

	#ifndef IL_NO_HDR
	if (ilIsValidL_HDR(Lump, Size))
		return IL_HDR;
	#endif

	#ifndef IL_NO_ICNS
	if (ilIsValidL_ICNS(Lump, Size))
		return IL_ICNS;
	#endif

	#ifndef IL_NO_IWI
	if (ilIsValidL_IWI(Lump, Size))
		return IL_IWI;
	#endif

	#ifndef IL_NO_JP2
	if (ilIsValidL_JP2(Lump, Size))
		return IL_JP2;
	#endif

	#ifndef IL_NO_LIF
	if (ilIsValidL_LIF(Lump, Size))
		return IL_LIF;
	#endif

	#ifndef IL_NO_MDL
	if (ilIsValidL_MDL(Lump, Size))
		return IL_MDL;
	#endif

	#ifndef IL_NO_MP3
	if (ilIsValidMp3L(Lump, Size))
		return IL_MP3;
	#endif

	#ifndef IL_NO_PCX
	if (ilIsValidL_PCX(Lump, Size))
		return IL_PCX;
	#endif

	#ifndef IL_NO_PIC
	if (ilIsValidL_PIC(Lump, Size))
		return IL_PIC;
	#endif

	#ifndef IL_NO_PNM
	if (ilIsValidL_PNM(Lump, Size))
		return IL_PNM;
	#endif

	#ifndef IL_NO_PSD
	if (ilIsValidL_PSD(Lump, Size))
		return IL_PSD;
	#endif

	#ifndef IL_NO_PSP
	if (ilIsValidL_PSP(Lump, Size))
		return IL_PSP;
	#endif

	#ifndef IL_NO_SGI
	if (ilIsValidL_SGI(Lump, Size))
		return IL_SGI;
	#endif

	#ifndef IL_NO_SUN
	if (ilIsValidL_SUN(Lump, Size))
		return IL_SUN;
	#endif

	#ifndef IL_NO_TIF
	if (ilIsValidL_TIFF(Lump, Size))
		return IL_TIF;
	#endif

	#ifndef IL_NO_TPL
	if (ilIsValidL_TPL(Lump, Size))
		return IL_TPL;
	#endif

	#ifndef IL_NO_VTF
	if (ilIsValidL_VTF(Lump, Size))
		return IL_VTF;
	#endif

	#ifndef IL_NO_XPM
	if (ilIsValidL_XPM(Lump, Size))
		return IL_XPM;
	#endif

	//Moved Targa to end of list because it has no magic number
	// in header to assure that this is really a tga... (20040218).
	#ifndef IL_NO_TGA
	if (ilIsValidL_TGA(Lump, Size))
		return IL_TGA;
	#endif

	return IL_TYPE_UNKNOWN;
}


ILboolean ILAPIENTRY ilIsValid(ILenum Type, ILconst_string FileName)
{
	if (FileName == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	switch (Type)
	{
		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilIsValid_TGA(FileName);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilIsValid_JPEG(FileName);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilIsValid_DDS(FileName);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilIsValid_PNG(FileName);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilIsValid_BMP(FileName);
		#endif

		#ifndef IL_NO_DICOM
		case IL_DICOM:
			return ilIsValid_DICOM(FileName);
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			return ilIsValid_EXR(FileName);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilIsValid_GIF(FileName);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilIsValid_HDR(FileName);
		#endif

		#ifndef IL_NO_ICNS
		case IL_ICNS:
			return ilIsValid_ICNS(FileName);
		#endif

		#ifndef IL_NO_IWI
		case IL_IWI:
			return ilIsValid_IWI(FileName);
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilIsValid_JP2(FileName);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilIsValid_LIF(FileName);
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			return ilIsValid_MDL(FileName);
		#endif

		#ifndef IL_NO_MP3
		case IL_MP3:
			return ilIsValidMp3(FileName);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilIsValid_PCX(FileName);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilIsValid_PIC(FileName);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilIsValid_PNM(FileName);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilIsValid_PSD(FileName);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilIsValid_PSP(FileName);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilIsValid_SGI(FileName);
		#endif

		#ifndef IL_NO_SUN
		case IL_SUN:
			return ilIsValid_SUN(FileName);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilIsValid_TIFF(FileName);
		#endif

		#ifndef IL_NO_TPL
		case IL_TPL:
			return ilIsValid_TPL(FileName);
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			return ilIsValid_VTF(FileName);
		#endif

		#ifndef IL_NO_XPM
		case IL_XPM:
			return ilIsValid_XPM(FileName);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


ILboolean ILAPIENTRY ilIsValidF(ILenum Type, ILHANDLE File)
{
	if (File == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	switch (Type)
	{
		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilIsValidF_TGA(File);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilIsValidF_JPEG(File);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilIsValidF_DDS(File);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilIsValidF_PNG(File);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilIsValidF_BMP(File);
		#endif

		#ifndef IL_NO_DICOM
		case IL_DICOM:
			return ilIsValidF_DICOM(File);
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			return ilIsValidF_EXR(File);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilIsValidF_GIF(File);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilIsValidF_HDR(File);
		#endif

		#ifndef IL_NO_ICNS
		case IL_ICNS:
			return ilIsValidF_ICNS(File);
		#endif

		#ifndef IL_NO_IWI
		case IL_IWI:
			return ilIsValidF_IWI(File);
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilIsValidF_JP2(File);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilIsValidF_LIF(File);
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			return ilIsValidF_MDL(File);
		#endif

		#ifndef IL_NO_MP3
		case IL_MP3:
			return ilIsValidMp3F(File);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilIsValidF_PCX(File);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilIsValidF_PIC(File);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilIsValidF_PNM(File);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilIsValidF_PSD(File);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilIsValidF_PSP(File);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilIsValidF_SGI(File);
		#endif

		#ifndef IL_NO_SUN
		case IL_SUN:
			return ilIsValidF_SUN(File);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilIsValidF_TIFF(File);
		#endif

		#ifndef IL_NO_TPL
		case IL_TPL:
			return ilIsValidF_TPL(File);
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			return ilIsValidF_VTF(File);
		#endif

		#ifndef IL_NO_XPM
		case IL_XPM:
			return ilIsValidF_XPM(File);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


ILboolean ILAPIENTRY ilIsValidL(ILenum Type, void *Lump, ILuint Size)
{
	if (Lump == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	switch (Type)
	{
		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilIsValidL_TGA(Lump, Size);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilIsValidL_JPEG(Lump, Size);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilIsValidL_DDS(Lump, Size);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilIsValidL_PNG(Lump, Size);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilIsValidL_BMP(Lump, Size);
		#endif

		#ifndef IL_NO_DICOM
		case IL_DICOM:
			return ilIsValidL_DICOM(Lump, Size);
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			return ilIsValidL_EXR(Lump, Size);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilIsValidL_GIF(Lump, Size);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilIsValidL_HDR(Lump, Size);
		#endif

		#ifndef IL_NO_ICNS
		case IL_ICNS:
			return ilIsValidL_ICNS(Lump, Size);
		#endif

		#ifndef IL_NO_IWI
		case IL_IWI:
			return ilIsValidL_IWI(Lump, Size);
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilIsValidL_JP2(Lump, Size);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilIsValidL_LIF(Lump, Size);
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			return ilIsValidL_MDL(Lump, Size);
		#endif

		#ifndef IL_NO_MP3
		case IL_MP3:
			return ilIsValidMp3L(Lump, Size);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilIsValidL_PCX(Lump, Size);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilIsValidL_PIC(Lump, Size);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilIsValidL_PNM(Lump, Size);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilIsValidL_PSD(Lump, Size);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilIsValidL_PSP(Lump, Size);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilIsValidL_SGI(Lump, Size);
		#endif

		#ifndef IL_NO_SUN
		case IL_SUN:
			return ilIsValidL_SUN(Lump, Size);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilIsValidL_TIFF(Lump, Size);
		#endif

		#ifndef IL_NO_TPL
		case IL_TPL:
			return ilIsValidL_TPL(Lump, Size);
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			return ilIsValidL_VTF(Lump, Size);
		#endif

		#ifndef IL_NO_XPM
		case IL_XPM:
			return ilIsValidL_XPM(Lump, Size);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


//! Attempts to load an image from a file.  The file format is specified by the user.
/*! \param Type Format of this file.  Acceptable values are IL_BLP, IL_BMP, IL_CUT, IL_DCX, IL_DDS,
	IL_DICOM, IL_DOOM, IL_DOOM_FLAT, IL_DPX, IL_EXR, IL_FITS, IL_FTX, IL_GIF, IL_HDR, IL_ICO, IL_ICNS,
	IL_IFF, IL_IWI, IL_JP2, IL_JPG, IL_LIF, IL_MDL,	IL_MNG, IL_MP3, IL_PCD, IL_PCX, IL_PIX, IL_PNG,
	IL_PNM, IL_PSD, IL_PSP, IL_PXR, IL_ROT, IL_SGI, IL_SUN, IL_TEXTURE, IL_TGA, IL_TIF, IL_TPL,
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

	switch (Type)
	{
		case IL_TYPE_UNKNOWN:
			bRet = ilLoadImage(FileName);
			break;

		#ifndef IL_NO_TGA
		case IL_TGA:
			bRet = ilLoad_TARGA(FileName);
			break;
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			bRet = ilLoad_JPEG(FileName);
			break;
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			bRet = ilLoad_JP2(FileName);
			break;
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			bRet = ilLoad_DDS(FileName);
			break;
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			bRet = ilLoad_PNG(FileName);
			break;
		#endif

		#ifndef IL_NO_BLP
		case IL_BLP:
			bRet = ilLoad_BLP(FileName);
			break;
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			bRet = ilLoad_BMP(FileName);
			break;
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			bRet = ilLoad_GIF(FileName);
			break;
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			bRet = ilLoad_HDR(FileName);
			break;
		#endif

		#ifndef IL_NO_CUT
		case IL_CUT:
			bRet = ilLoad_CUT(FileName);
			break;
		#endif

		#ifndef IL_NO_DICOM
		case IL_DICOM:
			bRet = ilLoad_DICOM(FileName);
			break;
		#endif

		#ifndef IL_NO_DOOM
		case IL_DOOM:
			bRet = ilLoad_DOOM(FileName);
			break;
		case IL_DOOM_FLAT:
			bRet = ilLoad_DOOM_FLAT(FileName);
			break;
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			bRet = ilLoad_EXR(FileName);
			break;
		#endif

		#ifndef IL_NO_FITS
		case IL_FITS:
			bRet = ilLoad_FITS(FileName);
			break;
		#endif

		#ifndef IL_NO_FTX
		case IL_FTX:
			bRet = ilLoad_FTX(FileName);
			break;
		#endif

		#ifndef IL_NO_ICO
		case IL_ICO:
			bRet = ilLoad_ICON(FileName);
			break;
		#endif

		#ifndef IL_NO_ICNS
		case IL_ICNS:
			bRet = ilLoad_ICNS(FileName);
			break;
		#endif

		#ifndef IL_NO_IFF
		case IL_IFF:
			bRet = ilLoad_IFF(FileName);
			break;
		#endif

		#ifndef IL_NO_IWI
		case IL_IWI:
			bRet = ilLoad_IWI(FileName);
			break;
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			bRet = ilLoad_LIF(FileName);
			break;
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			bRet = ilLoad_MDL(FileName);
			break;
		#endif

		#ifndef IL_NO_MNG
		case IL_MNG:
			bRet = ilLoad_MNG(FileName);
			break;
		#endif

		#ifndef IL_NO_MP3
		case IL_MP3:
			bRet = ilLoadMp3(FileName);
			break;
		#endif

		#ifndef IL_NO_PCD
		case IL_PCD:
			ilLoad_PCD(FileName);
			break;
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			bRet = ilLoad_PCX(FileName);
			break;
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			bRet = ilLoad_PIC(FileName);
			break;
		#endif

		#ifndef IL_NO_PIX
		case IL_PIX:
			bRet = ilLoad_PIX(FileName);
			break;
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			bRet = ilLoad_PNM(FileName);
			break;
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			bRet = ilLoad_PSD(FileName);
			break;
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			bRet = ilLoad_PSP(FileName);
			break;
		#endif

		#ifndef IL_NO_PXR
		case IL_PXR:
			bRet = ilLoad_PXR(FileName);
			break;
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			bRet = ilLoad_RAW(FileName);
			break;
		#endif

		#ifndef IL_NO_ROT
		case IL_ROT:
			bRet = ilLoad_ROT(FileName);
			break;
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			bRet = ilLoad_SGI(FileName);
			break;
		#endif

		#ifndef IL_NO_SUN
		case IL_SUN:
			bRet = ilLoad_SUN(FileName);
			break;
		#endif

		#ifndef IL_NO_TEXTURE
		case IL_TEXTURE:
			bRet = ilLoad_TEXTURE(FileName);
			break;
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			bRet = ilLoad_TIFF(FileName);
			break;
		#endif

		#ifndef IL_NO_TPL
		case IL_TPL:
			bRet = ilLoad_TPL(FileName);
			break;
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			bRet = ilLoad_VTF(FileName);
			break;
		#endif

		#ifndef IL_NO_WAL
		case IL_WAL:
			bRet = ilLoad_WAL(FileName);
			break;
		#endif

		#ifndef IL_NO_WBMP
		case IL_WBMP:
			bRet = ilLoad_WBMP(FileName);
			break;
		#endif

		#ifndef IL_NO_XPM
		case IL_XPM:
			bRet = ilLoad_XPM(FileName);
			break;
		#endif

		#ifndef IL_NO_WDP
		case IL_WDP:
			bRet = ilLoad_WDP(FileName);
			break;
		#endif

		default:
			ilSetError(IL_INVALID_ENUM);
			bRet = IL_FALSE;
	}

	return bRet;
}


//! Attempts to load an image from a file stream.  The file format is specified by the user.
/*! \param Type Format of this file.  Acceptable values are IL_BLP, IL_BMP, IL_CUT, IL_DCX, IL_DDS,
	IL_DICOM, IL_DOOM, IL_DOOM_FLAT, IL_DPX, IL_EXR, IL_FITS, IL_FTX, IL_GIF, IL_HDR, IL_ICO, IL_ICNS,
	IL_IFF, IL_IWI, IL_JP2, IL_JPG, IL_LIF, IL_MDL,	IL_MNG, IL_MP3, IL_PCD, IL_PCX, IL_PIX, IL_PNG,
	IL_PNM, IL_PSD, IL_PSP, IL_PXR, IL_ROT, IL_SGI, IL_SUN, IL_TEXTURE, IL_TGA, IL_TIF, IL_TPL,
	IL_UTX, IL_VTF, IL_WAL, IL_WBMP, IL_XPM, IL_RAW, IL_JASC_PAL and IL_TYPE_UNKNOWN.
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
	
	switch (Type)
	{
		case IL_TYPE_UNKNOWN:
			return IL_FALSE;

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilLoadF_TARGA(File);
		#endif

		#ifndef IL_NO_JPG
			#ifndef IL_USE_IJL
			case IL_JPG:
				return ilLoadF_JPEG(File);
			#endif
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilLoadF_JP2(File);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilLoadF_DDS(File);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilLoadF_PNG(File);
		#endif

		#ifndef IL_NO_BLP
		case IL_BLP:
			return ilLoadF_BLP(File);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilLoadF_BMP(File);
		#endif

		#ifndef IL_NO_CUT
		case IL_CUT:
			return ilLoadF_CUT(File);
		#endif

		#ifndef IL_NO_DICOM
		case IL_DICOM:
			return ilLoadF_DICOM(File);
		#endif

		#ifndef IL_NO_DOOM
		case IL_DOOM:
			return ilLoadF_DOOM(File);
		case IL_DOOM_FLAT:
			return ilLoadF_DOOM_FLAT(File);
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			return ilLoadF_EXR(File);
		#endif

		#ifndef IL_NO_FITS
		case IL_FITS:
			return ilLoadF_FITS(File);
		#endif

		#ifndef IL_NO_FTX
		case IL_FTX:
			return ilLoadF_FTX(File);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilLoadF_GIF(File);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilLoadF_HDR(File);
		#endif

		#ifndef IL_NO_ICO
		case IL_ICO:
			return ilLoadF_ICON(File);
		#endif

		#ifndef IL_NO_ICNS
		case IL_ICNS:
			return ilLoadF_ICNS(File);
		#endif

		#ifndef IL_NO_IFF
		case IL_IFF:
			return ilLoadF_IFF(File);
		#endif

		#ifndef IL_NO_IWI
		case IL_IWI:
			return ilLoadF_IWI(File);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilLoadF_LIF(File);
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			return ilLoadF_MDL(File);
		#endif

		#ifndef IL_NO_MNG
		case IL_MNG:
			return ilLoadF_MNG(File);
		#endif

		#ifndef IL_NO_MP3
		case IL_MP3:
			return ilLoadMp3F(File);
		#endif

		#ifndef IL_NO_PCD
		case IL_PCD:
			return ilLoadF_PCD(File);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilLoadF_PCX(File);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilLoadF_PIC(File);
		#endif

		#ifndef IL_NO_PIX
		case IL_PIX:
			return ilLoadF_PIX(File);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilLoadF_PNM(File);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilLoadF_PSD(File);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilLoadF_PSP(File);
		#endif

		#ifndef IL_NO_PXR
		case IL_PXR:
			return ilLoadF_PXR(File);
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			return ilLoadF_RAW(File);
		#endif

		#ifndef IL_NO_ROT
		case IL_ROT:
			return ilLoadF_ROT(File);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilLoadF_SGI(File);
		#endif

		#ifndef IL_NO_SUN
		case IL_SUN:
			return ilLoadF_SUN(File);
		#endif

		#ifndef IL_NO_TEXTURE
		case IL_TEXTURE:
			return ilLoadF_TEXTURE(File);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilLoadF_TIFF(File);
		#endif

		#ifndef IL_NO_TPL
		case IL_TPL:
			return ilLoadF_TPL(File);
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			return ilLoadF_VTF(File);
		#endif

		#ifndef IL_NO_WAL
		case IL_WAL:
			return ilLoadF_WAL(File);
		#endif

		#ifndef IL_NO_WBMP
		case IL_WBMP:
			return ilLoadF_WBMP(File);
		#endif

		#ifndef IL_NO_XPM
		case IL_XPM:
			return ilLoadF_XPM(File);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


//! Attempts to load an image from a memory buffer.  The file format is specified by the user.
/*! \param Type Format of this file.  Acceptable values are IL_BLP, IL_BMP, IL_CUT, IL_DCX, IL_DDS,
	IL_DICOM, IL_DOOM, IL_DOOM_FLAT, IL_DPX, IL_EXR, IL_FITS, IL_FTX, IL_GIF, IL_HDR, IL_ICO, IL_ICNS,
	IL_IFF, IL_IWI, IL_JP2, IL_JPG, IL_LIF, IL_MDL,	IL_MNG, IL_MP3, IL_PCD, IL_PCX, IL_PIX, IL_PNG,
	IL_PNM, IL_PSD, IL_PSP, IL_PXR, IL_ROT, IL_SGI, IL_SUN, IL_TEXTURE, IL_TGA, IL_TIF, IL_TPL,
	IL_UTX, IL_VTF, IL_WAL, IL_WBMP, IL_XPM, IL_RAW, IL_JASC_PAL and IL_TYPE_UNKNOWN.
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
	
	switch (Type)
	{
		case IL_TYPE_UNKNOWN:
			return IL_FALSE;

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilLoadL_TARGA(Lump, Size);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilLoadL_JPEG(Lump, Size);
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilLoadL_JP2(Lump, Size);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilLoadL_DDS(Lump, Size);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilLoadL_PNG(Lump, Size);
		#endif

		#ifndef IL_NO_BLP
		case IL_BLP:
			return ilLoadL_BLP(Lump, Size);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilLoadL_BMP(Lump, Size);
		#endif

		#ifndef IL_NO_CUT
		case IL_CUT:
			return ilLoadL_CUT(Lump, Size);
		#endif

		#ifndef IL_NO_DICOM
		case IL_DICOM:
			return ilLoadL_DICOM(Lump, Size);
		#endif

		#ifndef IL_NO_DOOM
		case IL_DOOM:
			return ilLoadL_DOOM(Lump, Size);
		case IL_DOOM_FLAT:
			return ilLoadL_DOOM_FLAT(Lump, Size);
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			return ilLoadL_EXR(Lump, Size);
		#endif

		#ifndef IL_NO_FITS
		case IL_FITS:
			return ilLoadL_FITS(Lump, Size);
		#endif

		#ifndef IL_NO_FTX
		case IL_FTX:
			return ilLoadL_FTX(Lump, Size);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilLoadL_GIF(Lump, Size);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilLoadL_HDR(Lump, Size);
		#endif

		#ifndef IL_NO_ICO
		case IL_ICO:
			return ilLoadL_ICON(Lump, Size);
		#endif

		#ifndef IL_NO_ICNS
		case IL_ICNS:
			return ilLoadL_ICNS(Lump, Size);
		#endif

		#ifndef IL_NO_IFF
		case IL_IFF:
			return ilLoadL_IFF(Lump, Size);
		#endif

		#ifndef IL_NO_IWI
		case IL_IWI:
			return ilLoadL_IWI(Lump, Size);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilLoadL_LIF(Lump, Size);
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			return ilLoadL_MDL(Lump, Size);
		#endif

		#ifndef IL_NO_MNG
		case IL_MNG:
			return ilLoadL_MNG(Lump, Size);
		#endif

		#ifndef IL_NO_MP3
		case IL_MP3:
			return ilLoadMp3L(Lump, Size);
		#endif

		#ifndef IL_NO_PCD
		case IL_PCD:
			return ilLoadL_PCD(Lump, Size);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilLoadL_PCX(Lump, Size);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilLoadL_PIC(Lump, Size);
		#endif

		#ifndef IL_NO_PIX
		case IL_PIX:
			return ilLoadL_PIX(Lump, Size);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilLoadL_PNM(Lump, Size);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilLoadL_PSD(Lump, Size);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilLoadL_PSP(Lump, Size);
		#endif

		#ifndef IL_NO_PXR
		case IL_PXR:
			return ilLoadL_PXR(Lump, Size);
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			return ilLoadL_RAW(Lump, Size);
		#endif

		#ifndef IL_NO_ROT
		case IL_ROT:
			return ilLoadL_ROT(Lump, Size);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilLoadL_SGI(Lump, Size);
		#endif

		#ifndef IL_NO_SUN
		case IL_SUN:
			return ilLoadL_SUN(Lump, Size);
		#endif

		#ifndef IL_NO_TEXTURE
		case IL_TEXTURE:
			return ilLoadL_TEXTURE(Lump, Size);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilLoadL_TIFF(Lump, Size);
		#endif

		#ifndef IL_NO_TPL
		case IL_TPL:
			return ilLoadL_TPL(Lump, Size);
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			return ilLoadL_VTF(Lump, Size);
		#endif

		#ifndef IL_NO_WAL
		case IL_WAL:
			return ilLoadL_WAL(Lump, Size);
		#endif

		#ifndef IL_NO_WBMP
		case IL_WBMP:
			return ilLoadL_WBMP(Lump, Size);
		#endif

		#ifndef IL_NO_XPM
		case IL_XPM:
			return ilLoadL_XPM(Lump, Size);
		#endif
	}

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

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (FileName == NULL || ilStrLen(FileName) < 1) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	Ext = iGetExtension(FileName);

	// Try registered procedures first (so users can override default lib functions).
	if (Ext) {
		if (iRegisterLoad(FileName))
			return IL_TRUE;

		#ifndef IL_NO_TGA
		if (!iStrCmp(Ext, IL_TEXT("tga")) || !iStrCmp(Ext, IL_TEXT("vda")) ||
			!iStrCmp(Ext, IL_TEXT("icb")) || !iStrCmp(Ext, IL_TEXT("vst"))) {
			bRet = ilLoad_TARGA(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_JPG
		if (!iStrCmp(Ext, IL_TEXT("jpg")) || !iStrCmp(Ext, IL_TEXT("jpe")) ||
			!iStrCmp(Ext, IL_TEXT("jpeg")) || !iStrCmp(Ext, IL_TEXT("jif")) || !iStrCmp(Ext, IL_TEXT("jfif"))) {
			bRet = ilLoad_JPEG(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_JP2
		if (!iStrCmp(Ext, IL_TEXT("jp2")) || !iStrCmp(Ext, IL_TEXT("jpx")) ||
			!iStrCmp(Ext, IL_TEXT("j2k")) || !iStrCmp(Ext, IL_TEXT("j2c"))) {
			bRet = ilLoad_JP2(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_DDS
		if (!iStrCmp(Ext, IL_TEXT("dds"))) {
			bRet = ilLoad_DDS(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PNG
		if (!iStrCmp(Ext, IL_TEXT("png"))) {
			bRet = ilLoad_PNG(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_BMP
		if (!iStrCmp(Ext, IL_TEXT("bmp")) || !iStrCmp(Ext, IL_TEXT("dib"))) {
			bRet = ilLoad_BMP(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_BLP
		if (!iStrCmp(Ext, IL_TEXT("blp"))) {
			bRet = ilLoad_BLP(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_EXR
		if (!iStrCmp(Ext, IL_TEXT("exr"))) {
			bRet = ilLoad_EXR(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_GIF
		if (!iStrCmp(Ext, IL_TEXT("gif"))) {
			bRet = ilLoad_GIF(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_HDR
		if (!iStrCmp(Ext, IL_TEXT("hdr"))) {
			bRet = ilLoad_HDR(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_CUT
		if (!iStrCmp(Ext, IL_TEXT("cut"))) {
			bRet = ilLoad_CUT(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_DCX
		if (!iStrCmp(Ext, IL_TEXT("dcx"))) {
			bRet = ilLoad_DCX(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_DICOM
		if (!iStrCmp(Ext, IL_TEXT("dicom")) || !iStrCmp(Ext, IL_TEXT("dcm"))) {
			bRet = ilLoad_DICOM(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_FITS
		if (!iStrCmp(Ext, IL_TEXT("fits")) || !iStrCmp(Ext, IL_TEXT("fit"))) {
			bRet = ilLoad_FITS(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_FTX
		if (!iStrCmp(Ext, IL_TEXT("ftx"))) {
			bRet = ilLoad_FTX(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_ICO
		if (!iStrCmp(Ext, IL_TEXT("ico")) || !iStrCmp(Ext, IL_TEXT("cur"))) {
			bRet = ilLoad_ICON(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_ICNS
		if (!iStrCmp(Ext, IL_TEXT("icns"))) {
			bRet = ilLoad_ICNS(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_IFF
		if (!iStrCmp(Ext, IL_TEXT("iff"))) {
			bRet = ilLoad_IFF(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_IWI
		if (!iStrCmp(Ext, IL_TEXT("iwi"))) {
			bRet = ilLoad_IWI(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_LIF
		if (!iStrCmp(Ext, IL_TEXT("lif"))) {
			bRet = ilLoad_LIF(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_MDL
		if (!iStrCmp(Ext, IL_TEXT("mdl"))) {
			bRet = ilLoad_MDL(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_MNG
		if (!iStrCmp(Ext, IL_TEXT("mng")) || !iStrCmp(Ext, IL_TEXT("jng"))) {
			bRet = ilLoad_MNG(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_MP3
		if (!iStrCmp(Ext, IL_TEXT("mp3"))) {
			bRet = ilLoadMp3(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PCD
		if (!iStrCmp(Ext, IL_TEXT("pcd"))) {
			bRet = ilLoad_PCD(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PCX
		if (!iStrCmp(Ext, IL_TEXT("pcx"))) {
			bRet = ilLoad_PCX(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PIC
		if (!iStrCmp(Ext, IL_TEXT("pic"))) {
			bRet = ilLoad_PIC(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PIX
		if (!iStrCmp(Ext, IL_TEXT("pix"))) {
			bRet = ilLoad_PIX(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PNM
		if (!iStrCmp(Ext, IL_TEXT("pbm"))) {
			bRet = ilLoad_PNM(FileName);
			goto finish;
		}
		if (!iStrCmp(Ext, IL_TEXT("pgm"))) {
			bRet = ilLoad_PNM(FileName);
			goto finish;
		}
		if (!iStrCmp(Ext, IL_TEXT("pnm"))) {
			bRet = ilLoad_PNM(FileName);
			goto finish;
		}
		if (!iStrCmp(Ext, IL_TEXT("ppm"))) {
			bRet = ilLoad_PNM(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PSD
		if (!iStrCmp(Ext, IL_TEXT("psd")) || !iStrCmp(Ext, IL_TEXT("pdd"))) {
			bRet = ilLoad_PSD(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PSP
		if (!iStrCmp(Ext, IL_TEXT("psp"))) {
			bRet = ilLoad_PSP(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_PXR
		if (!iStrCmp(Ext, IL_TEXT("pxr"))) {
			bRet = ilLoad_PXR(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_ROT
		if (!iStrCmp(Ext, IL_TEXT("rot"))) {
			bRet = ilLoad_ROT(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_SGI
		if (!iStrCmp(Ext, IL_TEXT("sgi")) || !iStrCmp(Ext, IL_TEXT("bw")) ||
			!iStrCmp(Ext, IL_TEXT("rgb")) || !iStrCmp(Ext, IL_TEXT("rgba"))) {
			bRet = ilLoad_SGI(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_SUN
		if (!iStrCmp(Ext, IL_TEXT("sun")) || !iStrCmp(Ext, IL_TEXT("ras")) ||
			!iStrCmp(Ext, IL_TEXT("rs")) || !iStrCmp(Ext, IL_TEXT("im1")) ||
			!iStrCmp(Ext, IL_TEXT("im8")) || !iStrCmp(Ext, IL_TEXT("im24")) ||
			!iStrCmp(Ext, IL_TEXT("im32"))) {
			bRet = ilLoad_SUN(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_TEXTURE
		if (!iStrCmp(Ext, IL_TEXT("texture"))) {
			bRet = ilLoad_TEXTURE(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_TIF
		if (!iStrCmp(Ext, IL_TEXT("tif")) || !iStrCmp(Ext, IL_TEXT("tiff"))) {
			bRet = ilLoad_TIFF(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_TPL
		if (!iStrCmp(Ext, IL_TEXT("tpl"))) {
			bRet = ilLoad_TPL(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_VTF
		if (!iStrCmp(Ext, IL_TEXT("vtf"))) {
			bRet = ilLoad_VTF(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_WAL
		if (!iStrCmp(Ext, IL_TEXT("wal"))) {
			bRet = ilLoad_WAL(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_WBMP
		if (!iStrCmp(Ext, IL_TEXT("wbmp"))) {
			bRet = ilLoad_WBMP(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_WDP
		if (!iStrCmp(Ext, IL_TEXT("wdp")) || !iStrCmp(Ext, IL_TEXT("hdp")) ) {
			bRet = ilLoad_WDP(FileName);
			goto finish;
		}
		#endif

		#ifndef IL_NO_XPM
		if (!iStrCmp(Ext, IL_TEXT("xpm"))) {
			bRet = ilLoad_XPM(FileName);
			goto finish;
		}
		#endif
	}

	// As a last-ditch effort, try to identify the image
	Type = ilDetermineType(FileName);
	if (Type == IL_TYPE_UNKNOWN) {
		ilSetError(IL_INVALID_EXTENSION);
		return IL_FALSE;
	}
	return ilLoad(Type, FileName);

finish:
	return bRet;
}


//! Attempts to save an image to a file.  The file format is specified by the user.
/*! \param Type Format of this file.  Acceptable values are IL_BMP, IL_CHEAD, IL_DDS, IL_EXR,
	IL_HDR, IL_JP2, IL_JPG, IL_PCX, IL_PNG, IL_PNM, IL_PSD, IL_RAW, IL_SGI, IL_TGA, IL_TIF,
	IL_VTF, IL_WBMP and IL_JASC_PAL.
	\param FileName Ansi or Unicode string, depending on the compiled version of DevIL, that gives
	       the filename to save to.
	\return Boolean value of failure or success.  Returns IL_FALSE if saving failed.*/
ILboolean ILAPIENTRY ilSave(ILenum Type, ILconst_string FileName)
{
	switch (Type)
	{
		case IL_TYPE_UNKNOWN:
			return ilSaveImage(FileName);

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilSave_BMP(FileName);
		#endif

		#ifndef IL_NO_CHEAD
		case IL_CHEAD:
			return ilSaveCHeader(FileName, "IL_IMAGE");
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
    		return ilSave_DDS(FileName);
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
    		return ilSave_EXR(FileName);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilSave_HDR(FileName);
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilSave_JP2(FileName);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilSave_JPEG(FileName);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilSave_PCX(FileName);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilSave_PNG(FileName);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilSave_PNM(FileName);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilSave_PSD(FileName);
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			return ilSave_RAW(FileName);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilSave_SGI(FileName);
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilSave_TARGA(FileName);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilSave_TIFF(FileName);
		#endif

		#ifndef IL_NO_WBMP
		case IL_WBMP:
			return ilSave_WBMP(FileName);
		#endif

		case IL_JASC_PAL:
			return ilSavePal_JASC(FileName);
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


//! Attempts to save an image to a file stream.  The file format is specified by the user.
/*! \param Type Format of this file.  Acceptable values are IL_BMP, IL_CHEAD, IL_DDS, IL_EXR,
	IL_HDR, IL_JP2, IL_JPG, IL_PCX, IL_PNG, IL_PNM, IL_PSD, IL_RAW, IL_SGI, IL_TGA, IL_TIF,
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

	switch (Type)
	{
		#ifndef IL_NO_BMP
		case IL_BMP:
			Ret = ilSaveF_BMP(File);
			break;
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			Ret = ilSaveF_DDS(File);
			break;
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			Ret = ilSaveF_EXR(File);
			break;
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			Ret = ilSaveF_HDR(File);
			break;
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			Ret = ilSaveF_JP2(File);
			break;
		#endif

		#ifndef IL_NO_JPG
			#ifndef IL_USE_IJL
			case IL_JPG:
				Ret = ilSaveF_JPEG(File);
				break;
			#endif
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			Ret = ilSaveF_PNM(File);
			break;
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			Ret = ilSaveF_PNG(File);
			break;	
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			Ret = ilSaveF_PSD(File);
			break;
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			Ret = ilSaveF_RAW(File);
			break;
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			Ret = ilSaveF_SGI(File);
			break;
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			Ret = ilSaveF_TARGA(File);
			break;
		#endif

		#ifndef IL_NO_WBMP
		case IL_WBMP:
			Ret = ilSaveF_WBMP(File);
			break;
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			Ret = ilSaveF_TIFF(File);
			break;
		#endif

		default:
			ilSetError(IL_INVALID_ENUM);
			return 0;
	}

	if (Ret == IL_FALSE)
		return 0;

	return itellw();
}


//! Attempts to save an image to a memory buffer.  The file format is specified by the user.
/*! \param Type Format of this image file.  Acceptable values are IL_BMP, IL_CHEAD, IL_DDS, IL_EXR,
	IL_HDR, IL_JP2, IL_JPG, IL_PCX, IL_PNG, IL_PNM, IL_PSD, IL_RAW, IL_SGI, IL_TGA, IL_TIF,
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

	switch (Type)
	{
		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilSaveL_BMP(Lump, Size);
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			return ilSaveL_EXR(Lump, Size);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilSaveL_HDR(Lump, Size);
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilSaveL_JP2(Lump, Size);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilSaveL_JPEG(Lump, Size);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilSaveL_PNG(Lump, Size);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilSaveL_PNM(Lump, Size);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilSaveL_PSD(Lump, Size);
		#endif

		#ifndef IL_NO_RAW
		case IL_RAW:
			return ilSaveL_RAW(Lump, Size);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilSaveL_SGI(Lump, Size);
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilSaveL_TARGA(Lump, Size);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilSaveL_DDS(Lump, Size);
		#endif

		#ifndef IL_NO_WBMP
		case IL_WBMP:
			return ilSaveL_WBMP(Lump, Size);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilSaveL_TIFF(Lump, Size);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return 0;
}


/** Saves the current image based on the extension given in FileName.
 * \param FileName Ansi or Unicode string, depending on the compiled version of DevIL, that gives the filename to save to.
 * \return Boolean value of failure or success.  Returns IL_FALSE if saving failed.
 */
ILboolean ILAPIENTRY ilSaveImage(ILconst_string FileName)
{
	ILstring Ext;
	ILboolean	bRet = IL_FALSE;

	if (FileName == NULL || ilStrLen(FileName) < 1) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Ext = iGetExtension(FileName);
	if (Ext == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	#ifndef IL_NO_BMP
	if (!iStrCmp(Ext, IL_TEXT("bmp"))) {
		bRet = ilSave_BMP(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_CHEAD
	if (!iStrCmp(Ext, IL_TEXT("h"))) {
		bRet = ilSaveCHeader(FileName, "IL_IMAGE");
		goto finish;
	}
	#endif

	#ifndef IL_NO_DDS
	if (!iStrCmp(Ext, IL_TEXT("dds"))) {
		bRet = ilSave_DDS(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_EXR
	if (!iStrCmp(Ext, IL_TEXT("exr"))) {
		bRet = ilSave_EXR(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_HDR
	if (!iStrCmp(Ext, IL_TEXT("hdr"))) {
		bRet = ilSave_HDR(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_JP2
	if (!iStrCmp(Ext, IL_TEXT("jp2"))) {
		bRet = ilSave_JP2(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_JPG
	if (!iStrCmp(Ext, IL_TEXT("jpg")) || !iStrCmp(Ext, IL_TEXT("jpeg")) || !iStrCmp(Ext, IL_TEXT("jpe"))) {
		bRet = ilSave_JPEG(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_PCX
	if (!iStrCmp(Ext, IL_TEXT("pcx"))) {
		bRet = ilSave_PCX(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_PNG
	if (!iStrCmp(Ext, IL_TEXT("png"))) {
		bRet = ilSave_PNG(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_PNM  // Not sure if binary or ascii should be defaulted...maybe an option?
	if (!iStrCmp(Ext, IL_TEXT("pbm"))) {
		bRet = ilSave_PNM(FileName);
		goto finish;
	}
	if (!iStrCmp(Ext, IL_TEXT("pgm"))) {
		bRet = ilSave_PNM(FileName);
		goto finish;
	}
	if (!iStrCmp(Ext, IL_TEXT("ppm"))) {
		bRet = ilSave_PNM(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_PSD
	if (!iStrCmp(Ext, IL_TEXT("psd"))) {
		bRet = ilSave_PSD(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_RAW
	if (!iStrCmp(Ext, IL_TEXT("raw"))) {
		bRet = ilSave_RAW(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_SGI
	if (!iStrCmp(Ext, IL_TEXT("sgi")) || !iStrCmp(Ext, IL_TEXT("bw")) ||
		!iStrCmp(Ext, IL_TEXT("rgb")) || !iStrCmp(Ext, IL_TEXT("rgba"))) {
		bRet = ilSave_SGI(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_TGA
	if (!iStrCmp(Ext, IL_TEXT("tga"))) {
		bRet = ilSave_TARGA(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_TIF
	if (!iStrCmp(Ext, IL_TEXT("tif")) || !iStrCmp(Ext, IL_TEXT("tiff"))) {
		bRet = ilSave_TIFF(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_WBMP
	if (!iStrCmp(Ext, IL_TEXT("wbmp"))) {
		bRet = ilSave_WBMP(FileName);
		goto finish;
	}
	#endif

	#ifndef IL_NO_MNG
	if (!iStrCmp(Ext, IL_TEXT("mng"))) {
		bRet = ilSave_MNG(FileName);
		goto finish;
	}
	#endif

	// Check if we just want to save the palette.
	if (!iStrCmp(Ext, IL_TEXT("pal"))) {
		bRet = ilSave_PAL(FileName);
		goto finish;
	}

	// Try registered procedures
	if (iRegisterSave(FileName))
		return IL_TRUE;

	ilSetError(IL_INVALID_EXTENSION);
	return IL_FALSE;

finish:
	return bRet;
}

