//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 05/03/2009
//
// Filename: src-IL/src/il_states.h
//
// Description: State machine
//
//-----------------------------------------------------------------------------


#ifndef STATES_H
#define STATES_H

//#include "il_internal.h"


// It is requiring us to declare this twice: once in il.h and once here.
typedef void*     ILHANDLE;


//
// Various states
//

typedef struct
{
	// Origin states
	ILboolean	ilOriginSet;
	ILenum		ilOriginMode;
	// Format and type states
	ILboolean	ilFormatSet;
	ILboolean	ilTypeSet;
	ILenum		ilFormatMode;
	ILenum		ilTypeMode;
	// Palette states
	ILboolean	ilAutoConvPal;
	// Load fail states
	ILboolean	ilDefaultOnFail;
	// Key colour states
	ILboolean	ilUseKeyColour;
	// Alpha blend states
	ILboolean	ilBlitBlend;
	// Compression states
	ILenum		ilCompression;
	// Interlace states
	ILenum		ilInterlace;
	// Quantization states
	ILenum		ilQuantMode;
	ILuint		ilNeuSample;
	ILuint		ilQuantMaxIndexs;
	// DXTC states
	ILboolean	ilKeepDxtcData;
	ILboolean	ilUseNVidiaDXT;
	ILboolean	ilUseSquishDXT;


	//
	// Format-specific states
	//

	ILboolean	ilTgaCreateStamp;
	ILuint		ilJpgQuality;
	ILboolean	ilPngInterlace;
	ILboolean	ilTgaRle;
	ILboolean	ilBmpRle;
	ILboolean	ilSgiRle;
	ILenum		ilJpgFormat;
	ILboolean	ilJpgProgressive;
	ILenum		ilDxtcFormat;
	ILenum		ilPcdPicNum;

	ILint		ilPngAlphaIndex;	// this index should be treated as an alpha key (most formats use this rather than having alpha in the palette), -1 for none
									// currently only used when writing out .png files and should obviously be set to -1 most of the time
	ILenum		ilVtfCompression;


	//
	// Format-specific strings
	//

	char*		ilTgaId;
	char*		ilTgaAuthName;
	char*		ilTgaAuthComment;
	char*		ilPngAuthName;
	char*		ilPngTitle;
	char*		ilPngDescription;
	char*		ilTifDescription;
	char*		ilTifHostComputer;
	char*		ilTifDocumentName;
	char*		ilTifAuthName;
	char*		ilCHeader;


	// Memory vs. Speed trade-off
	ILenum		MemVsSpeedHint;
	// Compression hints
	ILenum		CompressHint;


	// File loading
	void		SetInput(ILHANDLE File);
	void		SetInput(const void *Lump, ILuint Size);
	ILboolean	(ILAPIENTRY *eof)(void);
	ILHANDLE	(ILAPIENTRY *openr)(ILconst_string);
	void		(ILAPIENTRY *closer)(ILHANDLE);
	ILint		(ILAPIENTRY *getc)(void);
	ILuint		(ILAPIENTRY *read)(void *Buffer, ILuint Size, ILuint Number);
	ILint		(ILAPIENTRY *seek)(ILint Offset, ILuint Mode);
	ILuint		(ILAPIENTRY *tell)(void);

	// File saving
	void		SetOutput(ILHANDLE File);
	void		SetOutput(void *Lump, ILuint Size);
	void		SetOutput(void);
	void		(ILAPIENTRY *closew)(ILHANDLE);
	ILHANDLE	(ILAPIENTRY *openw)(ILconst_string);
	ILint		(ILAPIENTRY *putc)(ILubyte Char);
	ILint		(ILAPIENTRY *seekw)(ILint Offset, ILuint Mode);
	ILuint		(ILAPIENTRY *tellw)(void);
	ILint		(ILAPIENTRY *write)(const void *Buffer, ILuint Size, ILuint Number);

} ILstate;

// Defines to make it quicker to read/write instead of having to explicitly use
//  pointers from States everytime.
//#define ieof States->read
//#define iopenr States->openr
//#define icloser States->closer
//#define igetc States->getc
//#define iread States->read
//#define iseek States->seek
//#define itell States->tell
//
//#define iclosew States->closew
//#define iopenw States->openw
//#define iputc States->putc
//#define iseekw States->seekw
//#define itellw States->tellw
//#define iwrite States->write


// Used if the user specifies the State parameter as NULL.
extern ILstate DefaultState;

ILboolean ilAble(ILenum Mode, ILboolean Flag, ILstate *State);


#ifndef IL_NO_BLP
	#define IL_BLP_EXT "blp "
#else
	#define IL_BLP_EXT ""
#endif

#ifndef IL_NO_BMP
	#define IL_BMP_EXT "bmp dib "
#else
	#define IL_BMP_EXT ""
#endif

#ifndef IL_NO_CHEAD
	#define IL_CHEAD_EXT "h "
#else
	#define IL_CHEAD_EXT ""
#endif

#ifndef IL_NO_CUT
	#define IL_CUT_EXT "cut "
#else
	#define IL_CUT_EXT ""
#endif

#ifndef IL_NO_DCX
	#define IL_DCX_EXT "dcx "
#else
	#define IL_DCX_EXT ""
#endif

#ifndef IL_NO_DCM
	#define IL_DCM_EXT "dicom dcm"
#else
	#define IL_DCM_EXT ""
#endif

#ifndef IL_NO_DDS
	#define IL_DDS_EXT "dds "
#else
	#define IL_DDS_EXT ""
#endif

#ifndef IL_NO_DPX
	#define IL_DPX_EXT "dds "
#else
	#define IL_DPX_EXT ""
#endif

#ifndef IL_NO_EXR
	#define IL_EXR_EXT "exr "
#else
	#define IL_EXR_EXT ""
#endif

#ifndef IL_NO_FITS
	#define IL_FITS_EXT "fit fits "
#else
	#define IL_FITS_EXT ""
#endif

#ifndef IL_NO_FTX
	#define IL_FTX_EXT "ftx "
#else
	#define IL_FTX_EXT ""
#endif

#ifndef IL_NO_GIF
	#define IL_GIF_EXT "gif "
#else
	#define IL_GIF_EXT ""
#endif

#ifndef IL_NO_HDR
	#define IL_HDR_EXT "hdr "
#else
	#define IL_HDR_EXT ""
#endif

#ifndef IL_NO_ICNS
	#define IL_ICNS_EXT "icns "
#else
	#define IL_ICNS_EXT ""
#endif

#ifndef IL_NO_ICO
	#define IL_ICO_EXT "ico cur "
#else
	#define IL_ICO_EXT ""
#endif

#ifndef IL_NO_IFF
	#define IL_IFF_EXT "iff "
#else
	#define IL_IFF_EXT ""
#endif

#ifndef IL_NO_IWI
	#define IL_IWI_EXT "iwi "
#else
	#define IL_IWI_EXT ""
#endif

#ifndef IL_NO_JP2
	#define IL_JP2_EXT "jp2 "
#else
	#define IL_JP2_EXT ""
#endif

#ifndef IL_NO_JPG
	#define IL_JPG_EXT "jpg jpe jpeg "
#else
	#define IL_JPG_EXT ""
#endif

#ifndef IL_NO_LIF
	#define IL_LIF_EXT "lif "
#else
	#define IL_LIF_EXT ""
#endif

#ifndef IL_NO_MDL
	#define IL_MDL_EXT "mdl "
#else
	#define IL_MDL_EXT ""
#endif

#ifndef IL_NO_MNG
	#define IL_MNG_EXT "mng jng "
#else
	#define IL_MNG_EXT ""
#endif

#ifndef IL_NO_MP3
	#define IL_MP3_EXT "mp3 "
#else
	#define IL_MP3_EXT ""
#endif

#ifndef IL_NO_PCD
	#define IL_PCD_EXT "pcd "
#else
	#define IL_PCD_EXT ""
#endif

#ifndef IL_NO_PCX
	#define IL_PCX_EXT "pcx "
#else
	#define IL_PCX_EXT ""
#endif

#ifndef IL_NO_PIC
	#define IL_PIC_EXT "pic "
#else
	#define IL_PIC_EXT ""
#endif

#ifndef IL_NO_PIX
	#define IL_PIX_EXT "pix "
#else
	#define IL_PIX_EXT ""
#endif

#ifndef IL_NO_PNG
	#define IL_PNG_EXT "png "
#else
	#define IL_PNG_EXT ""
#endif

#ifndef IL_NO_PNM
	#define IL_PNM_EXT "pbm pgm pnm ppm "
#else
	#define IL_PNM_EXT ""
#endif

#ifndef IL_NO_PSD
	#define IL_PSD_EXT "psd pdd "
#else
	#define IL_PSD_EXT ""
#endif

#ifndef IL_NO_PSP
	#define IL_PSP_EXT "psp "
#else
	#define IL_PSP_EXT ""
#endif

#ifndef IL_NO_PXR
	#define IL_PXR_EXT "pxr "
#else
	#define IL_PXR_EXT ""
#endif

#ifndef IL_NO_RAW
	#define IL_RAW_EXT "raw "
#else
	#define IL_RAW_EXT ""
#endif

#ifndef IL_NO_ROT
	#define IL_ROT_EXT "rot "
#else
	#define IL_ROT_EXT ""
#endif

#ifndef IL_NO_SGI
	#define IL_SGI_EXT "sgi bw rgb rgba "
#else
	#define IL_SGI_EXT ""
#endif

#ifndef IL_NO_SUN
	#define IL_SUN_EXT "sun ras rs im1 im8 im24 im32 "
#else
	#define IL_SUN_EXT ""
#endif

#ifndef IL_NO_TEXTURE
	#define IL_TEX_EXT "texture "
#else
	#define IL_TEX_EXT ""
#endif

#ifndef IL_NO_TGA
	#define IL_TGA_EXT "tga vda icb vst "
#else
	#define IL_TGA_EXT ""
#endif

#ifndef IL_NO_TIF
	#define IL_TIF_EXT "tif tiff "
#else
	#define IL_TIF_EXT ""
#endif

#ifndef IL_NO_TPL
	#define IL_TPL_EXT "tpl "
#else
	#define IL_TPL_EXT ""
#endif

#ifndef IL_NO_UTX
	#define IL_UTX_EXT "utx "
#else
	#define IL_UTX_EXT ""
#endif

#ifndef IL_NO_VTF
	#define IL_VTF_EXT "vtf "
#else
	#define IL_VTF_EXT ""
#endif

#ifndef IL_NO_WAL
	#define IL_WAL_EXT "wal "
#else
	#define IL_WAL_EXT ""
#endif

#ifndef IL_NO_WBMP
	#define IL_WBMP_EXT "wbmp "
#else
	#define IL_WBMP_EXT ""
#endif

#ifndef IL_NO_WDP
	#define IL_WDP_EXT "wdp "
#else
	#define IL_WDP_EXT ""
#endif

#ifndef IL_NO_XPM
	#define IL_XPM_EXT "xpm "
#else
	#define IL_XPM_EXT ""
#endif



#endif//STATES_H
