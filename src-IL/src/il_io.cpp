//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 05/03/2009
//
// Filename: src-IL/src/il_io.cpp
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
	else if (!iStrCmp(Ext, IL_TEXT("ilbm")) || !iStrCmp(Ext, IL_TEXT("lbm")) ||
        !iStrCmp(Ext, IL_TEXT("ham")))
		Type = IL_ILBM;
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
	else if (!iStrCmp(Ext, IL_TEXT("swl")))
		Type = IL_SIN;
	else if (!iStrCmp(Ext, IL_TEXT("sun")) || !iStrCmp(Ext, IL_TEXT("ras")) ||
			 !iStrCmp(Ext, IL_TEXT("rs")) || !iStrCmp(Ext, IL_TEXT("im1")) ||
			 !iStrCmp(Ext, IL_TEXT("im8")) || !iStrCmp(Ext, IL_TEXT("im24")) ||
			 !iStrCmp(Ext, IL_TEXT("im32")))
		Type = IL_SUN;
	else if (!iStrCmp(Ext, IL_TEXT("tex")))
		Type = IL_TEX;
	else if (!iStrCmp(Ext, IL_TEXT("texture")))
		Type = IL_TEXTURE;
	else if (!iStrCmp(Ext, IL_TEXT("tif")) || !iStrCmp(Ext, IL_TEXT("tiff")))
		Type = IL_TIF;
	else if (!iStrCmp(Ext, IL_TEXT("tpl")))
		Type = IL_TPL;
	else if (!iStrCmp(Ext, IL_TEXT("utx")))
		Type = IL_UTX;
	else if (!iStrCmp(Ext, IL_TEXT("vtf")))
		Type = IL_VTF;
	else if (!iStrCmp(Ext, IL_TEXT("wad")))
		Type = IL_WAD;
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
	if (ilIsValidJpegF(File))
		return IL_JPG;
	#endif

	#ifndef IL_NO_DDS
	if (ilIsValidDdsF(File))
		return IL_DDS;
	#endif

	#ifndef IL_NO_PNG
	if (ilIsValidPngF(File))
		return IL_PNG;
	#endif

	#ifndef IL_NO_BMP
	if (ilIsValidBmpF(File))
		return IL_BMP;
	#endif

	#ifndef IL_NO_EXR
	if (ilIsValidExrF(File))
		return IL_EXR;
	#endif

	#ifndef IL_NO_GIF
	if (ilIsValidGifF(File))
		return IL_GIF;
	#endif

	#ifndef IL_NO_HDR
	if (ilIsValidHdrF(File))
		return IL_HDR;
	#endif

	#ifndef IL_NO_ICNS
	if (ilIsValidIcnsF(File))
		return IL_ICNS;
	#endif

	#ifndef IL_NO_ILBM
	if (ilIsValidIlbmF(File))
		return IL_ILBM;
	#endif

	#ifndef IL_NO_IWI
	if (ilIsValidIwiF(File))
		return IL_IWI;
	#endif

	#ifndef IL_NO_JP2
	if (ilIsValidJp2F(File))
		return IL_JP2;
	#endif

	#ifndef IL_NO_LIF
	if (ilIsValidLifF(File))
		return IL_LIF;
	#endif

	#ifndef IL_NO_MDL
	if (ilIsValidMdlF(File))
		return IL_MDL;
	#endif

	#ifndef IL_NO_MP3
	if (ilIsValidMp3F(File))
		return IL_MP3;
	#endif

	#ifndef IL_NO_PCX
	if (ilIsValidPcxF(File))
		return IL_PCX;
	#endif

	#ifndef IL_NO_PIC
	if (ilIsValidPicF(File))
		return IL_PIC;
	#endif

	#ifndef IL_NO_PNM
	if (ilIsValidPnmF(File))
		return IL_PNM;
	#endif

	#ifndef IL_NO_PSD
	if (ilIsValidPsdF(File))
		return IL_PSD;
	#endif

	#ifndef IL_NO_PSP
	if (ilIsValidPspF(File))
		return IL_PSP;
	#endif

	#ifndef IL_NO_SGI
	if (ilIsValidSgiF(File))
		return IL_SGI;
	#endif

	#ifndef IL_NO_SUN
	if (ilIsValidSunF(File))
		return IL_SUN;
	#endif

	#ifndef IL_NO_TEX
	if (ilIsValidTexF(File))
		return IL_TEX;
	#endif

	#ifndef IL_NO_TIF
	if (ilIsValidTiffF(File))
		return IL_TIF;
	#endif

	#ifndef IL_NO_TPL
	if (ilIsValidTplF(File))
		return IL_TPL;
	#endif

	#ifndef IL_NO_VTF
	if (ilIsValidVtfF(File))
		return IL_VTF;
	#endif

	#ifndef IL_NO_XPM
	if (ilIsValidXpmF(File))
		return IL_XPM;
	#endif

	// Moved tga to end of list, because it has no magic number
	//  in header to assure that this is really a tga. (20040218)
	#ifndef IL_NO_TGA
	if (ilIsValidTgaF(File))
		return IL_TGA;
	#endif
	
	return IL_TYPE_UNKNOWN;
}


ILenum ILAPIENTRY ilDetermineTypeL(const void *Lump, ILuint Size)
{
	if (Lump == NULL)
		return IL_TYPE_UNKNOWN;

	#ifndef IL_NO_JPG
	if (ilIsValidJpegL(Lump, Size))
		return IL_JPG;
	#endif

	#ifndef IL_NO_DDS
	if (ilIsValidDdsL(Lump, Size))
		return IL_DDS;
	#endif

	#ifndef IL_NO_PNG
	if (ilIsValidPngL(Lump, Size))
		return IL_PNG;
	#endif

	#ifndef IL_NO_BMP
	if (ilIsValidBmpL(Lump, Size))
		return IL_BMP;
	#endif

	#ifndef IL_NO_EXR
	if (ilIsValidExrL(Lump, Size))
		return IL_EXR;
	#endif

	#ifndef IL_NO_GIF
	if (ilIsValidGifL(Lump, Size))
		return IL_GIF;
	#endif

	#ifndef IL_NO_HDR
	if (ilIsValidHdrL(Lump, Size))
		return IL_HDR;
	#endif

	#ifndef IL_NO_ICNS
	if (ilIsValidIcnsL(Lump, Size))
		return IL_ICNS;
	#endif

	#ifndef IL_NO_IWI
	if (ilIsValidIwiL(Lump, Size))
		return IL_IWI;
	#endif

	#ifndef IL_NO_ILBM
	if (ilIsValidIlbmL(Lump,Size))
		return IL_ILBM;
	#endif

	#ifndef IL_NO_JP2
	if (ilIsValidJp2L(Lump, Size))
		return IL_JP2;
	#endif

	#ifndef IL_NO_LIF
	if (ilIsValidLifL(Lump, Size))
		return IL_LIF;
	#endif

	#ifndef IL_NO_MDL
	if (ilIsValidMdlL(Lump, Size))
		return IL_MDL;
	#endif

	#ifndef IL_NO_MP3
	if (ilIsValidMp3L(Lump, Size))
		return IL_MP3;
	#endif

	#ifndef IL_NO_PCX
	if (ilIsValidPcxL(Lump, Size))
		return IL_PCX;
	#endif

	#ifndef IL_NO_PIC
	if (ilIsValidPicL(Lump, Size))
		return IL_PIC;
	#endif

	#ifndef IL_NO_PNM
	if (ilIsValidPnmL(Lump, Size))
		return IL_PNM;
	#endif

	#ifndef IL_NO_PSD
	if (ilIsValidPsdL(Lump, Size))
		return IL_PSD;
	#endif

	#ifndef IL_NO_PSP
	if (ilIsValidPspL(Lump, Size))
		return IL_PSP;
	#endif

	#ifndef IL_NO_SGI
	if (ilIsValidSgiL(Lump, Size))
		return IL_SGI;
	#endif

	#ifndef IL_NO_SUN
	if (ilIsValidSunL(Lump, Size))
		return IL_SUN;
	#endif

	#ifndef IL_NO_TEX
	if (ilIsValidTexL(Lump, Size))
		return IL_TEX;
	#endif

	#ifndef IL_NO_TIF
	if (ilIsValidTiffL(Lump, Size))
		return IL_TIF;
	#endif

	#ifndef IL_NO_TPL
	if (ilIsValidTplL(Lump, Size))
		return IL_TPL;
	#endif

	#ifndef IL_NO_VTF
	if (ilIsValidVtfL(Lump, Size))
		return IL_VTF;
	#endif

	#ifndef IL_NO_XPM
	if (ilIsValidXpmL(Lump, Size))
		return IL_XPM;
	#endif

	// Moved Targa to end of list, because it has no magic number
	//  in header to assure that this is really a tga. (20040218).
	#ifndef IL_NO_TGA
	if (ilIsValidTgaL(Lump, Size))
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
			return ilIsValidTga(FileName);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilIsValidJpeg(FileName);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilIsValidDds(FileName);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilIsValidPng(FileName);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilIsValidBmp(FileName);
		#endif

		#ifndef IL_NO_DICOM
		case IL_DICOM:
			return ilIsValidDicom(FileName);
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			return ilIsValidExr(FileName);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilIsValidGif(FileName);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilIsValidHdr(FileName);
		#endif

		#ifndef IL_NO_ICNS
		case IL_ICNS:
			return ilIsValidIcns(FileName);
		#endif

		#ifndef IL_NO_IWI
		case IL_IWI:
			return ilIsValidIwi(FileName);
		#endif

    	#ifndef IL_NO_ILBM
        case IL_ILBM:
            return ilIsValidIlbm(FileName);
	    #endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilIsValidJp2(FileName);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilIsValidLif(FileName);
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			return ilIsValidMdl(FileName);
		#endif

		#ifndef IL_NO_MP3
		case IL_MP3:
			return ilIsValidMp3(FileName);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilIsValidPcx(FileName);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilIsValidPic(FileName);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilIsValidPnm(FileName);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilIsValidPsd(FileName);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilIsValidPsp(FileName);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilIsValidSgi(FileName);
		#endif

		#ifndef IL_NO_SUN
		case IL_SUN:
			return ilIsValidSun(FileName);
		#endif

		#ifndef IL_NO_TEX
		case IL_TEX:
			return ilIsValidTex(FileName);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilIsValidTiff(FileName);
		#endif

		#ifndef IL_NO_TPL
		case IL_TPL:
			return ilIsValidTpl(FileName);
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			return ilIsValidVtf(FileName);
		#endif

		#ifndef IL_NO_XPM
		case IL_XPM:
			return ilIsValidXpm(FileName);
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
			return ilIsValidTgaF(File);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilIsValidJpegF(File);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilIsValidDdsF(File);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilIsValidPngF(File);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilIsValidBmpF(File);
		#endif

		#ifndef IL_NO_DICOM
		case IL_DICOM:
			return ilIsValidDicomF(File);
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			return ilIsValidExrF(File);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilIsValidGifF(File);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilIsValidHdrF(File);
		#endif

		#ifndef IL_NO_ICNS
		case IL_ICNS:
			return ilIsValidIcnsF(File);
		#endif

		#ifndef IL_NO_IWI
		case IL_IWI:
			return ilIsValidIwiF(File);
		#endif

    	#ifndef IL_NO_ILBM
        case IL_ILBM:
            return ilIsValidIlbmF(File);
	    #endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilIsValidJp2F(File);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilIsValidLifF(File);
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			return ilIsValidMdlF(File);
		#endif

		#ifndef IL_NO_MP3
		case IL_MP3:
			return ilIsValidMp3F(File);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilIsValidPcxF(File);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilIsValidPicF(File);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilIsValidPnmF(File);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilIsValidPsdF(File);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilIsValidPspF(File);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilIsValidSgiF(File);
		#endif

		#ifndef IL_NO_SUN
		case IL_SUN:
			return ilIsValidSunF(File);
		#endif

		#ifndef IL_NO_TEX
		case IL_TEX:
			return ilIsValidTexF(File);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilIsValidTiffF(File);
		#endif

		#ifndef IL_NO_TPL
		case IL_TPL:
			return ilIsValidTplF(File);
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			return ilIsValidVtfF(File);
		#endif

		#ifndef IL_NO_XPM
		case IL_XPM:
			return ilIsValidXpmF(File);
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
			return ilIsValidTgaL(Lump, Size);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilIsValidJpegL(Lump, Size);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilIsValidDdsL(Lump, Size);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilIsValidPngL(Lump, Size);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilIsValidBmpL(Lump, Size);
		#endif

		#ifndef IL_NO_DICOM
		case IL_DICOM:
			return ilIsValidDicomL(Lump, Size);
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			return ilIsValidExrL(Lump, Size);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilIsValidGifL(Lump, Size);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilIsValidHdrL(Lump, Size);
		#endif

		#ifndef IL_NO_ICNS
		case IL_ICNS:
			return ilIsValidIcnsL(Lump, Size);
		#endif

		#ifndef IL_NO_IWI
		case IL_IWI:
			return ilIsValidIwiL(Lump, Size);
		#endif

    	#ifndef IL_NO_ILBM
        case IL_ILBM:
            return ilIsValidIlbmL(Lump, Size);
	    #endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilIsValidJp2L(Lump, Size);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilIsValidLifL(Lump, Size);
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			return ilIsValidMdlL(Lump, Size);
		#endif

		#ifndef IL_NO_MP3
		case IL_MP3:
			return ilIsValidMp3L(Lump, Size);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilIsValidPcxL(Lump, Size);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilIsValidPicL(Lump, Size);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilIsValidPnmL(Lump, Size);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilIsValidPsdL(Lump, Size);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilIsValidPspL(Lump, Size);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilIsValidSgiL(Lump, Size);
		#endif

		#ifndef IL_NO_SUN
		case IL_SUN:
			return ilIsValidSunL(Lump, Size);
		#endif

		#ifndef IL_NO_TEX
		case IL_TEX:
			return ilIsValidTexL(Lump, Size);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilIsValidTiffL(Lump, Size);
		#endif

		#ifndef IL_NO_TPL
		case IL_TPL:
			return ilIsValidTplL(Lump, Size);
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			return ilIsValidVtfL(Lump, Size);
		#endif

		#ifndef IL_NO_XPM
		case IL_XPM:
			return ilIsValidXpmL(Lump, Size);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


//! Attempts to load an image from a file.  The file format is specified by the user.
/*! \param Type Format of this file.  Acceptable values are IL_BLP, IL_BMP, IL_CUT, IL_DCX, IL_DDS,
	IL_DICOM, IL_DOOM, IL_DOOM_FLAT, IL_DPX, IL_EXR, IL_FITS, IL_FTX, IL_GIF, IL_HDR, IL_ICO, IL_ICNS,
	IL_IFF, IL_IWI, IL_JP2, IL_JPG, IL_LIF, IL_MDL,	IL_MNG, IL_MP3, IL_PCD, IL_PCX, IL_PIX, IL_PNG,
	IL_PNM, IL_PSD, IL_PSP, IL_PXR, IL_ROT, IL_SGI, IL_SIN, IL_SUN, IL_TEX, IL_TEXTURE, IL_TGA, IL_TIF,
	IL_TPL,	IL_UTX, IL_VTF, IL_WAD, IL_WAL, IL_WBMP, IL_XPM, IL_JASC_PAL and IL_TYPE_UNKNOWN.
	If IL_TYPE_UNKNOWN is specified, ilLoad will try to determine the type of the file and load it.
	\param FileName Ansi or Unicode string, depending on the compiled version of DevIL, that gives
	       the filename of the file to load.
	\return Boolean value of failure or success.  Returns IL_FALSE if all three loading methods
	       have been tried and failed.*/
ILboolean ILAPIENTRY ilLoad(ILimage *Image, ILenum Type, ILconst_string FileName, ILstate *State)
{
	CheckState();
	if (FileName == NULL || ilStrLen(FileName) < 1) {
		ilSetError(IL_INVALID_PARAM);
		return NULL;
	}

	switch (Type)
	{
		case IL_TYPE_UNKNOWN:
			return ilLoadImage(Image, FileName, State);

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilLoadTarga(Image, FileName, State);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilLoadJpeg(Image, FileName, State);
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilLoadJp2(Image, FileName, State);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilLoadDds(Image, FileName, State);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilLoadPng(Image, FileName, State);
		#endif

		#ifndef IL_NO_BLP
		case IL_BLP:
			return ilLoadBlp(Image, FileName, State);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilLoadBmp(Image, FileName, State);
		#endif

		#ifndef IL_NO_DPX
		case IL_DPX:
			return ilLoadDpx(Image, FileName, State);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilLoadGif(Image, FileName, State);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilLoadHdr(Image, FileName, State);
		#endif

		#ifndef IL_NO_CUT
		case IL_CUT:
			return ilLoadCut(Image, FileName, State);
		#endif

		#ifndef IL_NO_DICOM
		case IL_DICOM:
			return ilLoadDicom(Image, FileName, State);
		#endif

		#ifndef IL_NO_DOOM
		case IL_DOOM:
			return ilLoadDoom(Image, FileName, State);
		case IL_DOOM_FLAT:
			return ilLoadDoomFlat(Image, FileName, State);
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			return ilLoadExr(Image, FileName, State);
		#endif

		#ifndef IL_NO_FITS
		case IL_FITS:
			return ilLoadFits(Image, FileName, State);
		#endif

		#ifndef IL_NO_FTX
		case IL_FTX:
			return ilLoadFtx(Image, FileName, State);
		#endif

		#ifndef IL_NO_ICO
		case IL_ICO:
			return ilLoadIcon(Image, FileName, State);
		#endif

		#ifndef IL_NO_ICNS
		case IL_ICNS:
			return ilLoadIcns(Image, FileName, State);
		#endif

		#ifndef IL_NO_IFF
		case IL_IFF:
			return ilLoadIff(Image, FileName, State);
		#endif

		#ifndef IL_NO_ILBM
		case IL_ILBM:
			return ilLoadIlbm(Image, FileName, State);
		#endif

		#ifndef IL_NO_IWI
		case IL_IWI:
			return ilLoadIwi(Image, FileName, State);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilLoadLif(Image, FileName, State);
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			return ilLoadMdl(Image, FileName, State);
		#endif

		#ifndef IL_NO_MNG
		case IL_MNG:
			return ilLoadMng(Image, FileName, State);
		#endif

		#ifndef IL_NO_MP3
		case IL_MP3:
			return ilLoadMp3(Image, FileName, State);
		#endif

		#ifndef IL_NO_PCD
		case IL_PCD:
			ilLoadPcd(Image, FileName, State);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilLoadPcx(Image, FileName, State);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilLoadPic(Image, FileName, State);
		#endif

		#ifndef IL_NO_PIX
		case IL_PIX:
			return ilLoadPix(Image, FileName, State);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilLoadPnm(Image, FileName, State);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilLoadPsd(Image, FileName, State);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilLoadPsp(Image, FileName, State);
		#endif

		#ifndef IL_NO_PXR
		case IL_PXR:
			return ilLoadPxr(Image, FileName, State);
		#endif

		#ifndef IL_NO_ROT
		case IL_ROT:
			return ilLoadRot(Image, FileName, State);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilLoadSgi(Image, FileName, State);
		#endif

		#ifndef IL_NO_SIN
		case IL_SIN:
			return ilLoadSin(Image, FileName, State);
		#endif

		#ifndef IL_NO_SUN
		case IL_SUN:
			return ilLoadSun(Image, FileName, State);
		#endif

		#ifndef IL_NO_TEX
		case IL_TEX:
			return ilLoadTex(Image, FileName, State);
		#endif

		#ifndef IL_NO_TEXTURE
		case IL_TEXTURE:
			return ilLoadTexture(Image, FileName, State);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilLoadTiff(Image, FileName, State);
		#endif

		#ifndef IL_NO_TPL
		case IL_TPL:
			return ilLoadTpl(Image, FileName, State);
		#endif

		#ifndef IL_NO_UTX
		case IL_UTX:
			return ilLoadUtx(Image, FileName, State);
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			return ilLoadVtf(Image, FileName, State);
		#endif

		#ifndef IL_NO_WAD
		case IL_WAD:
			return ilLoadWad(Image, FileName, State);
		#endif

		#ifndef IL_NO_WAL
		case IL_WAL:
			return ilLoadWal(Image, FileName, State);
		#endif

		#ifndef IL_NO_WBMP
		case IL_WBMP:
			return ilLoadWbmp(Image, FileName, State);
		#endif

		#ifndef IL_NO_XPM
		case IL_XPM:
			return ilLoadXpm(Image, FileName, State);
		#endif

		#ifndef IL_NO_WDP
		case IL_WDP:
			return ilLoadWdp(Image, FileName, State);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return NULL;
}


//! Attempts to load an image from a file stream.  The file format is specified by the user.
/*! \param Type Format of this file.  Acceptable values are IL_BLP, IL_BMP, IL_CUT, IL_DCX, IL_DDS,
	IL_DICOM, IL_DOOM, IL_DOOM_FLAT, IL_DPX, IL_EXR, IL_FITS, IL_FTX, IL_GIF, IL_HDR, IL_ICO, IL_ICNS,
	IL_IFF, IL_IWI, IL_JP2, IL_JPG, IL_LIF, IL_MDL,	IL_MNG, IL_MP3, IL_PCD, IL_PCX, IL_PIX, IL_PNG,
	IL_PNM, IL_PSD, IL_PSP, IL_PXR, IL_ROT, IL_SGI, IL_SIN, IL_SUN, IL_TEX, IL_TEXTURE, IL_TGA, IL_TIF,
	IL_TPL,	IL_UTX, IL_VTF, IL_WAD, IL_WAL, IL_WBMP, IL_XPM, IL_JASC_PAL and IL_TYPE_UNKNOWN.
	If IL_TYPE_UNKNOWN is specified, ilLoadF will try to determine the type of the file and load it.
	\param File File stream to load from.
	\return Boolean value of failure or success.  Returns IL_FALSE if loading fails.*/
ILboolean ILAPIENTRY ilLoadF(ILimage *Image, ILenum Type, ILHANDLE File, ILstate *State)
{
	CheckState();
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
			return ilLoadTargaF(Image, File, State);
		#endif

		#ifndef IL_NO_JPG
			#ifndef IL_USE_IJL
			case IL_JPG:
				return ilLoadJpegF(Image, File, State);
			#endif
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilLoadJp2F(Image, File, State);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilLoadDdsF(Image, File, State);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilLoadPngF(Image, File, State);
		#endif

		#ifndef IL_NO_BLP
		case IL_BLP:
			return ilLoadBlpF(Image, File, State);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilLoadBmpF(Image, File, State);
		#endif

		#ifndef IL_NO_CUT
		case IL_CUT:
			return ilLoadCutF(Image, File, State);
		#endif

		#ifndef IL_NO_DICOM
		case IL_DICOM:
			return ilLoadDicomF(Image, File, State);
		#endif

		#ifndef IL_NO_DOOM
		case IL_DOOM:
			return ilLoadDoomF(Image, File, State);
		case IL_DOOM_FLAT:
			return ilLoadDoomFlatF(Image, File, State);
		#endif

		#ifndef IL_NO_DPX
		case IL_DPX:
			return ilLoadDpxF(Image, File, State);
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			return ilLoadExrF(Image, File, State);
		#endif

		#ifndef IL_NO_FITS
		case IL_FITS:
			return ilLoadFitsF(Image, File, State);
		#endif

		#ifndef IL_NO_FTX
		case IL_FTX:
			return ilLoadFtxF(Image, File, State);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilLoadGifF(Image, File, State);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilLoadHdrF(Image, File, State);
		#endif

		#ifndef IL_NO_ICO
		case IL_ICO:
			return ilLoadIconF(Image, File, State);
		#endif

		#ifndef IL_NO_ICNS
		case IL_ICNS:
			return ilLoadIcnsF(Image, File, State);
		#endif

		#ifndef IL_NO_IFF
		case IL_IFF:
			return ilLoadIffF(Image, File, State);
		#endif

		#ifndef IL_NO_ILBM
		case IL_ILBM:
			return ilLoadIlbmF(Image, File, State);
		#endif

		#ifndef IL_NO_IWI
		case IL_IWI:
			return ilLoadIwiF(Image, File, State);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilLoadLifF(Image, File, State);
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			return ilLoadMdlF(Image, File, State);
		#endif

		#ifndef IL_NO_MNG
		case IL_MNG:
			return ilLoadMngF(Image, File, State);
		#endif

		#ifndef IL_NO_MP3
		case IL_MP3:
			return ilLoadMp3F(Image, File, State);
		#endif

		#ifndef IL_NO_PCD
		case IL_PCD:
			return ilLoadPcdF(Image, File, State);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilLoadPcxF(Image, File, State);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilLoadPicF(Image, File, State);
		#endif

		#ifndef IL_NO_PIX
		case IL_PIX:
			return ilLoadPixF(Image, File, State);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilLoadPnmF(Image, File, State);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilLoadPsdF(Image, File, State);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilLoadPspF(Image, File, State);
		#endif

		#ifndef IL_NO_PXR
		case IL_PXR:
			return ilLoadPxrF(Image, File, State);
		#endif

		#ifndef IL_NO_ROT
		case IL_ROT:
			return ilLoadRotF(Image, File, State);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilLoadSgiF(Image, File, State);
		#endif

		#ifndef IL_NO_SIN
		case IL_SIN:
			return ilLoadSinF(Image, File, State);
		#endif

		#ifndef IL_NO_SUN
		case IL_SUN:
			return ilLoadSunF(Image, File, State);
		#endif

		#ifndef IL_NO_TEX
		case IL_TEX:
			return ilLoadTexF(Image, File, State);
		#endif

		#ifndef IL_NO_TEXTURE
		case IL_TEXTURE:
			return ilLoadTextureF(Image, File, State);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilLoadTiffF(Image, File, State);
		#endif

		#ifndef IL_NO_TPL
		case IL_TPL:
			return ilLoadTplF(Image, File, State);
		#endif

		#ifndef IL_NO_UTX
		case IL_UTX:
			return ilLoadUtxF(Image, File, State);
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			return ilLoadVtfF(Image, File, State);
		#endif

		#ifndef IL_NO_WAD
		case IL_WAD:
			return ilLoadWadF(Image, File, State);
		#endif

		#ifndef IL_NO_WAL
		case IL_WAL:
			return ilLoadWalF(Image, File, State);
		#endif

		#ifndef IL_NO_WBMP
		case IL_WBMP:
			return ilLoadWbmpF(Image, File, State);
		#endif

		#ifndef IL_NO_XPM
		case IL_XPM:
			return ilLoadXpmF(Image, File, State);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


//! Attempts to load an image from a memory buffer.  The file format is specified by the user.
/*! \param Type Format of this file.  Acceptable values are IL_BLP, IL_BMP, IL_CUT, IL_DCX, IL_DDS,
	IL_DICOM, IL_DOOM, IL_DOOM_FLAT, IL_DPX, IL_EXR, IL_FITS, IL_FTX, IL_GIF, IL_HDR, IL_ICO, IL_ICNS,
	IL_IFF, IL_IWI, IL_JP2, IL_JPG, IL_LIF, IL_MDL,	IL_MNG, IL_MP3, IL_PCD, IL_PCX, IL_PIX, IL_PNG,
	IL_PNM, IL_PSD, IL_PSP, IL_PXR, IL_ROT, IL_SGI, IL_SIN, IL_SUN, IL_TEX, IL_TEXTURE, IL_TGA, IL_TIF,
	IL_TPL,	IL_UTX, IL_VTF, IL_WAD, IL_WAL, IL_WBMP, IL_XPM, IL_JASC_PAL and IL_TYPE_UNKNOWN.
	If IL_TYPE_UNKNOWN is specified, ilLoadL will try to determine the type of the file and load it.
	\param Lump The buffer where the file data is located
	\param Size Size of the buffer
	\return Boolean value of failure or success.  Returns IL_FALSE if loading fails.*/
ILboolean ILAPIENTRY ilLoadL(ILimage *Image, ILenum Type, const void *Lump, ILuint Size, ILstate *State)
{
	CheckState();
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
			return ilLoadTargaL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilLoadJpegL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilLoadJp2L(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilLoadDdsL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilLoadPngL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_BLP
		case IL_BLP:
			return ilLoadBlpL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilLoadBmpL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_CUT
		case IL_CUT:
			return ilLoadCutL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_DICOM
		case IL_DICOM:
			return ilLoadDicomL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_DOOM
		case IL_DOOM:
			return ilLoadDoomL(Image, Lump, Size, State);
		case IL_DOOM_FLAT:
			return ilLoadDoomFlatL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_DPX
		case IL_DPX:
			return ilLoadDpxL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			return ilLoadExrL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_FITS
		case IL_FITS:
			return ilLoadFitsL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_FTX
		case IL_FTX:
			return ilLoadFtxL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_GIF
		case IL_GIF:
			return ilLoadGifL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilLoadHdrL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_ICO
		case IL_ICO:
			return ilLoadIconL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_ICNS
		case IL_ICNS:
			return ilLoadIcnsL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_IFF
		case IL_IFF:
			return ilLoadIffL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_ILBM
		case IL_ILBM:
			return ilLoadIlbmL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_IWI
		case IL_IWI:
			return ilLoadIwiL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_LIF
		case IL_LIF:
			return ilLoadLifL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_MDL
		case IL_MDL:
			return ilLoadMdlL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_MNG
		case IL_MNG:
			return ilLoadMngL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_MP3
		case IL_MP3:
			return ilLoadMp3L(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_PCD
		case IL_PCD:
			return ilLoadPcdL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilLoadPcxL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_PIC
		case IL_PIC:
			return ilLoadPicL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_PIX
		case IL_PIX:
			return ilLoadPixL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilLoadPnmL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilLoadPsdL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_PSP
		case IL_PSP:
			return ilLoadPspL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_PXR
		case IL_PXR:
			return ilLoadPxrL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_ROT
		case IL_ROT:
			return ilLoadRotL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilLoadSgiL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_SIN
		case IL_SIN:
			return ilLoadSinL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_SUN
		case IL_SUN:
			return ilLoadSunL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_TEX
		case IL_TEX:
			return ilLoadTexL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_TEXTURE
		case IL_TEXTURE:
			return ilLoadTextureL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilLoadTiffL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_TPL
		case IL_TPL:
			return ilLoadTplL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_UTX
		case IL_UTX:
			return ilLoadUtxL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			return ilLoadVtfL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_WAD
		case IL_WAD:
			return ilLoadWadL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_WAL
		case IL_WAL:
			return ilLoadWalL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_WBMP
		case IL_WBMP:
			return ilLoadWbmpL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_XPM
		case IL_XPM:
			return ilLoadXpmL(Image, Lump, Size, State);
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
ILboolean ILAPIENTRY ilLoadImage(ILimage *Image, ILconst_string FileName, ILstate *State)
{
	ILstring	Ext;
	ILenum		Type;

	CheckState();
	if (FileName == NULL || ilStrLen(FileName) < 1) {
		ilSetError(IL_INVALID_PARAM);
		return NULL;
	}

	Ext = iGetExtension(FileName);

	// Try registered procedures first (so users can override default lib functions).
	if (Ext) {
		if (iRegisterLoad(FileName))
			return NULL;

		#ifndef IL_NO_TGA
		if (!iStrCmp(Ext, IL_TEXT("tga")) || !iStrCmp(Ext, IL_TEXT("vda")) ||
			!iStrCmp(Ext, IL_TEXT("icb")) || !iStrCmp(Ext, IL_TEXT("vst"))) {
			return ilLoadTarga(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_JPG
		if (!iStrCmp(Ext, IL_TEXT("jpg")) || !iStrCmp(Ext, IL_TEXT("jpe")) ||
			!iStrCmp(Ext, IL_TEXT("jpeg")) || !iStrCmp(Ext, IL_TEXT("jif")) || !iStrCmp(Ext, IL_TEXT("jfif"))) {
			return ilLoadJpeg(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_JP2
		if (!iStrCmp(Ext, IL_TEXT("jp2")) || !iStrCmp(Ext, IL_TEXT("jpx")) ||
			!iStrCmp(Ext, IL_TEXT("j2k")) || !iStrCmp(Ext, IL_TEXT("j2c"))) {
			return ilLoadJp2(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_DDS
		if (!iStrCmp(Ext, IL_TEXT("dds"))) {
			return ilLoadDds(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_PNG
		if (!iStrCmp(Ext, IL_TEXT("png"))) {
			return ilLoadPng(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_BMP
		if (!iStrCmp(Ext, IL_TEXT("bmp")) || !iStrCmp(Ext, IL_TEXT("dib"))) {
			return ilLoadBmp(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_BLP
		if (!iStrCmp(Ext, IL_TEXT("blp"))) {
			return ilLoadBlp(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_DOOM
		if (!iStrCmp(Ext, IL_TEXT("lmp"))) {
			return ilLoadDoom(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_DPX
		if (!iStrCmp(Ext, IL_TEXT("dpx"))) {
			return ilLoadDpx(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_EXR
		if (!iStrCmp(Ext, IL_TEXT("exr"))) {
			return ilLoadExr(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_GIF
		if (!iStrCmp(Ext, IL_TEXT("gif"))) {
			return ilLoadGif(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_HDR
		if (!iStrCmp(Ext, IL_TEXT("hdr"))) {
			return ilLoadHdr(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_CUT
		if (!iStrCmp(Ext, IL_TEXT("cut"))) {
			return ilLoadCut(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_DCX
		if (!iStrCmp(Ext, IL_TEXT("dcx"))) {
			return ilLoadDcx(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_DICOM
		if (!iStrCmp(Ext, IL_TEXT("dicom")) || !iStrCmp(Ext, IL_TEXT("dcm"))) {
			return ilLoadDicom(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_FITS
		if (!iStrCmp(Ext, IL_TEXT("fits")) || !iStrCmp(Ext, IL_TEXT("fit"))) {
			return ilLoadFits(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_FTX
		if (!iStrCmp(Ext, IL_TEXT("ftx"))) {
			return ilLoadFtx(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_ICO
		if (!iStrCmp(Ext, IL_TEXT("ico")) || !iStrCmp(Ext, IL_TEXT("cur"))) {
			return ilLoadIcon(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_ICNS
		if (!iStrCmp(Ext, IL_TEXT("icns"))) {
			return ilLoadIcns(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_IFF
		if (!iStrCmp(Ext, IL_TEXT("iff"))) {
			return ilLoadIff(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_ILBM
		if (!iStrCmp(Ext, IL_TEXT("ilbm")) || !iStrCmp(Ext, IL_TEXT("lbm")) ||
            !iStrCmp(Ext, IL_TEXT("ham")) ) {
			return ilLoadIlbm(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_IWI
		if (!iStrCmp(Ext, IL_TEXT("iwi"))) {
			return ilLoadIwi(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_LIF
		if (!iStrCmp(Ext, IL_TEXT("lif"))) {
			return ilLoadLif(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_MDL
		if (!iStrCmp(Ext, IL_TEXT("mdl"))) {
			return ilLoadMdl(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_MNG
		if (!iStrCmp(Ext, IL_TEXT("mng")) || !iStrCmp(Ext, IL_TEXT("jng"))) {
			return ilLoadMng(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_MP3
		if (!iStrCmp(Ext, IL_TEXT("mp3"))) {
			return ilLoadMp3(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_PCD
		if (!iStrCmp(Ext, IL_TEXT("pcd"))) {
			return ilLoadPcd(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_PCX
		if (!iStrCmp(Ext, IL_TEXT("pcx"))) {
			return ilLoadPcx(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_PIC
		if (!iStrCmp(Ext, IL_TEXT("pic"))) {
			return ilLoadPic(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_PIX
		if (!iStrCmp(Ext, IL_TEXT("pix"))) {
			return ilLoadPix(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_PNM
		if (!iStrCmp(Ext, IL_TEXT("pbm")) || !iStrCmp(Ext, IL_TEXT("pgm")) ||
			!iStrCmp(Ext, IL_TEXT("pnm")) || !iStrCmp(Ext, IL_TEXT("ppm"))) {
			return ilLoadPnm(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_PSD
		if (!iStrCmp(Ext, IL_TEXT("psd")) || !iStrCmp(Ext, IL_TEXT("pdd"))) {
			return ilLoadPsd(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_PSP
		if (!iStrCmp(Ext, IL_TEXT("psp"))) {
			return ilLoadPsp(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_PXR
		if (!iStrCmp(Ext, IL_TEXT("pxr"))) {
			return ilLoadPxr(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_ROT
		if (!iStrCmp(Ext, IL_TEXT("rot"))) {
			return ilLoadRot(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_SIN
		if (!iStrCmp(Ext, IL_TEXT("swl"))) {
			return ilLoadSin(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_SGI
		if (!iStrCmp(Ext, IL_TEXT("sgi")) || !iStrCmp(Ext, IL_TEXT("bw")) ||
			!iStrCmp(Ext, IL_TEXT("rgb")) || !iStrCmp(Ext, IL_TEXT("rgba"))) {
			return ilLoadSgi(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_SUN
		if (!iStrCmp(Ext, IL_TEXT("sun")) || !iStrCmp(Ext, IL_TEXT("ras")) ||
			!iStrCmp(Ext, IL_TEXT("rs")) || !iStrCmp(Ext, IL_TEXT("im1")) ||
			!iStrCmp(Ext, IL_TEXT("im8")) || !iStrCmp(Ext, IL_TEXT("im24")) ||
			!iStrCmp(Ext, IL_TEXT("im32"))) {
			return ilLoadSun(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_TEX
		if (!iStrCmp(Ext, IL_TEXT("tex"))) {
			return ilLoadTex(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_TEXTURE
		if (!iStrCmp(Ext, IL_TEXT("texture"))) {
			return ilLoadTexture(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_TIF
		if (!iStrCmp(Ext, IL_TEXT("tif")) || !iStrCmp(Ext, IL_TEXT("tiff"))) {
			return ilLoadTiff(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_TPL
		if (!iStrCmp(Ext, IL_TEXT("tpl"))) {
			return ilLoadTpl(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_UTX
		if (!iStrCmp(Ext, IL_TEXT("utx"))) {
			return ilLoadUtx(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_VTF
		if (!iStrCmp(Ext, IL_TEXT("vtf"))) {
			return ilLoadVtf(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_WAD
		if (!iStrCmp(Ext, IL_TEXT("wad"))) {
			return ilLoadWad(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_WAL
		if (!iStrCmp(Ext, IL_TEXT("wal"))) {
			return ilLoadWal(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_WBMP
		if (!iStrCmp(Ext, IL_TEXT("wbmp"))) {
			return ilLoadWbmp(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_WDP
		if (!iStrCmp(Ext, IL_TEXT("wdp")) || !iStrCmp(Ext, IL_TEXT("hdp")) ) {
			return ilLoadWdp(Image, FileName, State);
		}
		#endif

		#ifndef IL_NO_XPM
		if (!iStrCmp(Ext, IL_TEXT("xpm"))) {
			return ilLoadXpm(Image, FileName, State);
		}
		#endif
	}

	// As a last-ditch effort, try to identify the image
	Type = ilDetermineType(FileName);
	if (Type == IL_TYPE_UNKNOWN) {
		ilSetError(IL_INVALID_EXTENSION);
		return NULL;
	}
	return ilLoad(Image, Type, FileName, State);
}


//! Attempts to save an image to a file.  The file format is specified by the user.
/*! \param Type Format of this file.  Acceptable values are IL_BMP, IL_CHEAD, IL_DDS, IL_EXR,
	IL_HDR, IL_JP2, IL_JPG, IL_PCX, IL_PNG, IL_PNM, IL_PSD, IL_SGI, IL_TGA, IL_TIF,
	IL_VTF, IL_WBMP and IL_JASC_PAL.
	\param FileName Ansi or Unicode string, depending on the compiled version of DevIL, that gives
	       the filename to save to.
	\return Boolean value of failure or success.  Returns IL_FALSE if saving failed.*/
ILboolean ILAPIENTRY ilSave(ILimage *Image, ILenum Type, ILconst_string FileName, ILstate *State)
{
	switch (Type)
	{
		case IL_TYPE_UNKNOWN:
			return ilSaveImage(Image, FileName, State);

		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilSaveBmp(Image, FileName, State);
		#endif

		#ifndef IL_NO_CHEAD
		case IL_CHEAD:
			return ilSaveCHeader(Image, FileName, "IL_IMAGE", State);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
    		return ilSaveDds(Image, FileName, State);
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
    		return ilSaveExr(Image, FileName, State);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilSaveHdr(Image, FileName, State);
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilSaveJp2(Image, FileName, State);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilSaveJpeg(Image, FileName, State);
		#endif

		#ifndef IL_NO_PCX
		case IL_PCX:
			return ilSavePcx(Image, FileName, State);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilSavePng(Image, FileName, State);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilSavePnm(Image, FileName, State);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilSavePsd(Image, FileName, State);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilSaveSgi(Image, FileName, State);
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilSaveTarga(Image, FileName, State);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilSaveTiff(Image, FileName, State);
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			return ilSaveVtf(Image, FileName, State);
		#endif

		#ifndef IL_NO_WBMP
		case IL_WBMP:
			return ilSaveWbmp(Image, FileName, State);
		#endif

		case IL_JASC_PAL:
			return ilSaveJascPal(Image, FileName, State);
	}

	ilSetError(IL_INVALID_ENUM);
	return IL_FALSE;
}


//! Attempts to save an image to a file stream.  The file format is specified by the user.
/*! \param Type Format of this file.  Acceptable values are IL_BMP, IL_CHEAD, IL_DDS, IL_EXR,
	IL_HDR, IL_JP2, IL_JPG, IL_PCX, IL_PNG, IL_PNM, IL_PSD, IL_SGI, IL_TGA, IL_TIF,
	IL_VTF, IL_WBMP and IL_JASC_PAL.
	\param File File stream to save to.
	\return Boolean value of failure or success.  Returns IL_FALSE if saving failed.*/
ILuint ILAPIENTRY ilSaveF(ILimage *Image, ILenum Type, ILHANDLE File, ILstate *State)
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
			Ret = ilSaveBmpF(Image, File, State);
			break;
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			Ret = ilSaveDdsF(Image, File, State);
			break;
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			Ret = ilSaveExrF(Image, File, State);
			break;
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			Ret = ilSaveHdrF(Image, File, State);
			break;
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			Ret = ilSaveJp2F(Image, File, State);
			break;
		#endif

		#ifndef IL_NO_JPG
			#ifndef IL_USE_IJL
			case IL_JPG:
				Ret = ilSaveJpegF(Image, File, State);
				break;
			#endif
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			Ret = ilSavePnmF(Image, File, State);
			break;
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			Ret = ilSavePngF(Image, File, State);
			break;	
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			Ret = ilSavePsdF(Image, File, State);
			break;
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			Ret = ilSaveSgiF(Image, File, State);
			break;
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			Ret = ilSaveTargaF(Image, File, State);
			break;
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			Ret = ilSaveVtfF(Image, File, State);
			break;
		#endif

		#ifndef IL_NO_WBMP
		case IL_WBMP:
			Ret = ilSaveWbmpF(Image, File, State);
			break;
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			Ret = ilSaveTiffF(Image, File, State);
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
	IL_HDR, IL_JP2, IL_JPG, IL_PCX, IL_PNG, IL_PNM, IL_PSD, IL_SGI, IL_TGA, IL_TIF,
	IL_VTF, IL_WBMP and IL_JASC_PAL.
	\param Lump Memory buffer to save to
	\param Size Size of the memory buffer
	\return Boolean value of failure or success.  Returns IL_FALSE if saving failed.*/
ILuint ILAPIENTRY ilSaveL(ILimage *Image, ILenum Type, void *Lump, ILuint Size, ILstate *State)
{
	if (Lump == NULL) {
		if (Size != 0) {
			ilSetError(IL_INVALID_PARAM);
			return 0;
		}
		// The user wants to know how large of a buffer they need.
		else {
			return ilDetermineSize(Image, Type, State);
		}
	}

	switch (Type)
	{
		#ifndef IL_NO_BMP
		case IL_BMP:
			return ilSaveBmpL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_EXR
		case IL_EXR:
			return ilSaveExrL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_HDR
		case IL_HDR:
			return ilSaveHdrL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_JP2
		case IL_JP2:
			return ilSaveJp2L(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_JPG
		case IL_JPG:
			return ilSaveJpegL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_PNG
		case IL_PNG:
			return ilSavePngL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_PNM
		case IL_PNM:
			return ilSavePnmL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_PSD
		case IL_PSD:
			return ilSavePsdL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_SGI
		case IL_SGI:
			return ilSaveSgiL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_TGA
		case IL_TGA:
			return ilSaveTargaL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_DDS
		case IL_DDS:
			return ilSaveDdsL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_VTF
		case IL_VTF:
			return ilSaveVtfL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_WBMP
		case IL_WBMP:
			return ilSaveWbmpL(Image, Lump, Size, State);
		#endif

		#ifndef IL_NO_TIF
		case IL_TIF:
			return ilSaveTiffL(Image, Lump, Size, State);
		#endif
	}

	ilSetError(IL_INVALID_ENUM);
	return 0;
}


//! Saves the current image based on the extension given in FileName.
/*! \param FileName Ansi or Unicode string, depending on the compiled version of DevIL, that gives
	       the filename to save to.
	\return Boolean value of failure or success.  Returns IL_FALSE if saving failed.*/
ILboolean ILAPIENTRY ilSaveImage(ILimage *Image, ILconst_string FileName, ILstate *State)
{
	ILstring	Ext;
	ILboolean	bRet = IL_FALSE;

	CheckState();

	if (FileName == NULL || ilStrLen(FileName) < 1) {
		ilSetError(IL_INVALID_PARAM);
		return IL_FALSE;
	}

	if (Image == NULL) {
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
		bRet = ilSaveBmp(Image, FileName, State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_CHEAD
	if (!iStrCmp(Ext, IL_TEXT("h"))) {
		bRet = ilSaveCHeader(Image, FileName, "IL_IMAGE", State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_DDS
	if (!iStrCmp(Ext, IL_TEXT("dds"))) {
		bRet = ilSaveDds(Image, FileName, State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_EXR
	if (!iStrCmp(Ext, IL_TEXT("exr"))) {
		bRet = ilSaveExr(Image, FileName, State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_HDR
	if (!iStrCmp(Ext, IL_TEXT("hdr"))) {
		bRet = ilSaveHdr(Image, FileName, State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_JP2
	if (!iStrCmp(Ext, IL_TEXT("jp2"))) {
		bRet = ilSaveJp2(Image, FileName, State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_JPG
	if (!iStrCmp(Ext, IL_TEXT("jpg")) || !iStrCmp(Ext, IL_TEXT("jpeg")) || !iStrCmp(Ext, IL_TEXT("jpe"))) {
		bRet = ilSaveJpeg(Image, FileName, State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_PCX
	if (!iStrCmp(Ext, IL_TEXT("pcx"))) {
		bRet = ilSavePcx(Image, FileName, State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_PNG
	if (!iStrCmp(Ext, IL_TEXT("png"))) {
		bRet = ilSavePng(Image, FileName, State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_PNM  // Not sure if binary or ascii should be defaulted...maybe an option?
	if (!iStrCmp(Ext, IL_TEXT("pbm"))) {
		bRet = ilSavePnm(Image, FileName, State);
		goto finish;
	}
	if (!iStrCmp(Ext, IL_TEXT("pgm"))) {
		bRet = ilSavePnm(Image, FileName, State);
		goto finish;
	}
	if (!iStrCmp(Ext, IL_TEXT("pnm"))) {
		bRet = ilSavePnm(Image, FileName, State);
		goto finish;
	}
	if (!iStrCmp(Ext, IL_TEXT("ppm"))) {
		bRet = ilSavePnm(Image, FileName, State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_PSD
	if (!iStrCmp(Ext, IL_TEXT("psd"))) {
		bRet = ilSavePsd(Image, FileName, State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_SGI
	if (!iStrCmp(Ext, IL_TEXT("sgi")) || !iStrCmp(Ext, IL_TEXT("bw")) ||
		!iStrCmp(Ext, IL_TEXT("rgb")) || !iStrCmp(Ext, IL_TEXT("rgba"))) {
		bRet = ilSaveSgi(Image, FileName, State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_TGA
	if (!iStrCmp(Ext, IL_TEXT("tga"))) {
		bRet = ilSaveTarga(Image, FileName, State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_TIF
	if (!iStrCmp(Ext, IL_TEXT("tif")) || !iStrCmp(Ext, IL_TEXT("tiff"))) {
		bRet = ilSaveTiff(Image, FileName, State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_VTF
	if (!iStrCmp(Ext, IL_TEXT("vtf"))) {
		bRet = ilSaveVtf(Image, FileName, State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_WBMP
	if (!iStrCmp(Ext, IL_TEXT("wbmp"))) {
		bRet = ilSaveWbmp(Image, FileName, State);
		goto finish;
	}
	#endif

	#ifndef IL_NO_MNG
	if (!iStrCmp(Ext, IL_TEXT("mng"))) {
		bRet = ilSaveMng(Image, FileName, State);
		goto finish;
	}
	#endif

	// Check if we just want to save the palette.
	if (!iStrCmp(Ext, IL_TEXT("pal"))) {
		bRet = ilSavePal(Image, FileName, State);
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
