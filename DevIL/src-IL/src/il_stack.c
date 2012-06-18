//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 12/17/2008
//
// Filename: src-IL/src/il_stack.c
//
// Description: The main image stack
//
//-----------------------------------------------------------------------------

// Credit goes to John Villar (johnny@reliaschev.com) for making the suggestion
//	of not letting the user use ILimage structs but instead binding images
//	like OpenGL.

#include "il_internal.h"
#include "il_logging.h"
#include "il_stack.h"

//! Creates Num images and puts their index in Images - similar to glGenTextures().
void ILAPIENTRY ilGenImages(ILsizei Num, ILuint *Images)
{
	ILsizei	Index = 0;
	iFree	*TempFree = FreeNames;

	if (Num < 1 || Images == NULL) {
		ilSetError(IL_INVALID_VALUE);
		return;
	}

	// No images have been generated yet, so create the image stack.
	if (ImageStack == NULL)
		if (!iEnlargeStack())
			return;

	do {
		if (FreeNames != NULL) {  // If any have been deleted, then reuse their image names.
                        TempFree = (iFree*)FreeNames->Next;
                        Images[Index] = FreeNames->Name;
                        ImageStack[FreeNames->Name] = ilNewImage(1, 1, 1, 1, 1);
                        ifree(FreeNames);
                        FreeNames = TempFree;
		} else {
                        if (LastUsed >= StackSize)
				if (!iEnlargeStack())
					return;
			Images[Index] = LastUsed;
			// Must be all 1's instead of 0's, because some functions would divide by 0.
			ImageStack[LastUsed] = ilNewImage(1, 1, 1, 1, 1);
			LastUsed++;
		}
	} while (++Index < Num);
	LOG_ADVANCED(IL_LOG_INFO, _("ilGenImages: Generated %u images, the first is %u"), Num, Images[0]);
	return;
}

ILuint ILAPIENTRY ilGenImage()
{
    ILuint i;
    ilGenImages(1,&i);
    return i;
}

//! Makes Image the current active image - similar to glBindTexture().
void ILAPIENTRY ilBindImage(ILuint Image)
{
	if (ImageStack == NULL || StackSize == 0) {
		if (!iEnlargeStack()) {
			return;
		}
	}

	// If the user requests a high image name.
	while (Image >= StackSize) {
		if (!iEnlargeStack()) {
			return;
		}
	}

	if (ImageStack[Image] == NULL) {
		ImageStack[Image] = ilNewImage(1, 1, 1, 1, 1);
		if (Image >= LastUsed) // >= ?
			LastUsed = Image + 1;
	}

	iCurImage = ImageStack[Image];
	CurName = Image;

	LOG_ADVANCED(IL_LOG_VERBOSE, _("ilBindImage: The current image is now %u"), Image);
	ParentImage = IL_TRUE;

	return;
}


//! Deletes Num images from the image stack - similar to glDeleteTextures().
void ILAPIENTRY ilDeleteImages(ILsizei Num, const ILuint *Images)
{
	iFree	*Temp = FreeNames;
	ILuint	Index = 0;

	if (Num < 1) {
		//ilSetError(IL_INVALID_VALUE);
		return;
	}
	if (StackSize == 0)
		return;

	LOG_ADVANCED(IL_LOG_INFO, _("ilDeleteImages: Deleted %u images, the first one was %u"), Num, * Images);
	do {
		if (Images[Index] > 0 && Images[Index] < LastUsed) {  // <= ?
			/*if (FreeNames != NULL) {  // Terribly inefficient
				Temp = FreeNames;
				do {
					if (Temp->Name == Images[Index]) {
						continue;  // Sufficient?
					}
				} while ((Temp = Temp->Next));
			}*/

			// Already has been deleted or was never used.
			if (ImageStack[Images[Index]] == NULL)
				continue;

			// Find out if current image - if so, set to default image zero.
			if (Images[Index] == CurName || Images[Index] == 0) {
				iCurImage = ImageStack[0];
				CurName = 0;
			}
			
			// Should *NOT* be NULL here!
			ilCloseImage(ImageStack[Images[Index]]);
			ImageStack[Images[Index]] = NULL;

			// Add to head of list - works for empty and non-empty lists
			Temp = (iFree*)ialloc(sizeof(iFree));
			if (!Temp) {
				return;
			}
			Temp->Name = Images[Index];
			Temp->Next = FreeNames;
			FreeNames = Temp;
		}
		/*else {  // Shouldn't set an error...just continue onward.
			ilSetError(IL_ILLEGAL_OPERATION);
		}*/
	} while (++Index < (ILuint)Num);
}


void ILAPIENTRY ilDeleteImage(const ILuint Num) {
    ilDeleteImages(1,&Num);
}

//! Checks if Image is a valid ilGenImages-generated image (like glIsTexture()).
ILboolean ILAPIENTRY ilIsImage(ILuint Image)
{
	//iFree *Temp = FreeNames;

	if (ImageStack == NULL)
		return IL_FALSE;
	if (Image >= LastUsed || Image == 0)
		return IL_FALSE;

	/*do {
		if (Temp->Name == Image)
			return IL_FALSE;
	} while ((Temp = Temp->Next));*/

	if (ImageStack[Image] == NULL)  // Easier check.
		return IL_FALSE;

	return IL_TRUE;
}


//! Closes Image and frees all memory associated with it.
ILAPI void ILAPIENTRY ilCloseImage(ILimage *Image)
{
	if (Image == NULL)
		return;

	if (Image->Data != NULL) {
		ifree(Image->Data);
		Image->Data = NULL;
	}

	if (Image->Pal.Palette != NULL && Image->Pal.PalSize > 0 && Image->Pal.PalType != IL_PAL_NONE) {
		ifree(Image->Pal.Palette);
		Image->Pal.Palette = NULL;
	}

	if (Image->Next != NULL) {
		ilCloseImage(Image->Next);
		Image->Next = NULL;
	}

	if (Image->Faces != NULL) {
		ilCloseImage(Image->Faces);
		Image->Mipmaps = NULL;
	}

	if (Image->Mipmaps != NULL) {
		ilCloseImage(Image->Mipmaps);
		Image->Mipmaps = NULL;
	}

	if (Image->Layers != NULL) {
		ilCloseImage(Image->Layers);
		Image->Layers = NULL;
	}

	if (Image->AnimList != NULL && Image->AnimSize != 0) {
		ifree(Image->AnimList);
		Image->AnimList = NULL;
	}

	if (Image->Profile != NULL && Image->ProfileSize != 0) {
		ifree(Image->Profile);
		Image->Profile = NULL;
		Image->ProfileSize = 0;
	}

	if (Image->DxtcData != NULL && Image->DxtcFormat != IL_DXT_NO_COMP) {
		ifree(Image->DxtcData);
		Image->DxtcData = NULL;
		Image->DxtcFormat = IL_DXT_NO_COMP;
		Image->DxtcSize = 0;
	}

	ifree(Image);
	Image = NULL;

	return;
}


ILAPI ILboolean ILAPIENTRY ilIsValid_PAL(ILpal *Palette)
{
	if (Palette == NULL)
		return IL_FALSE;
	if (Palette->PalSize == 0 || Palette->Palette == NULL)
		return IL_FALSE;
	switch (Palette->PalType)
	{
		case IL_PAL_RGB24:
		case IL_PAL_RGB32:
		case IL_PAL_RGBA32:
		case IL_PAL_BGR24:
		case IL_PAL_BGR32:
		case IL_PAL_BGRA32:
			return IL_TRUE;
	}
	return IL_FALSE;
}


//! Closes Palette and frees all memory associated with it.
ILAPI void ILAPIENTRY ilClosePal(ILpal *Palette)
{
	if (Palette == NULL)
		return;
	if (!ilIsValid_PAL(Palette))
		return;
	ifree(Palette->Palette);
	ifree(Palette);
	return;
}


ILimage *iGetBaseImage()
{
	return ImageStack[ilGetCurName()];
}


//! Sets the current mipmap level
ILboolean ILAPIENTRY ilActiveMipmap(ILuint Number)
{
	ILuint Current;
    ILimage *iTempImage;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (Number == 0) {
		return IL_TRUE;
	}

    iTempImage = iCurImage;
	iCurImage = iCurImage->Mipmaps;
	if (iCurImage == NULL) {
		iCurImage = iTempImage;
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	for (Current = 1; Current < Number; Current++) {
		iCurImage = iCurImage->Mipmaps;
		if (iCurImage == NULL) {
			ilSetError(IL_ILLEGAL_OPERATION);
			iCurImage = iTempImage;
			return IL_FALSE;
		}
	}

	ParentImage = IL_FALSE;

	return IL_TRUE;
}


//! Used for setting the current image if it is an animation.
ILboolean ILAPIENTRY ilActiveImage(ILuint Number)
{
	ILuint Current;
    ILimage *iTempImage;
    
	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (Number == 0) {
		return IL_TRUE;
	}

    iTempImage = iCurImage;
	iCurImage = iCurImage->Next;
	if (iCurImage == NULL) {
		iCurImage = iTempImage;
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Number--;  // Skip 0 (parent image)
	for (Current = 0; Current < Number; Current++) {
		iCurImage = iCurImage->Next;
		if (iCurImage == NULL) {
			ilSetError(IL_ILLEGAL_OPERATION);
			iCurImage = iTempImage;
			return IL_FALSE;
		}
	}

	ParentImage = IL_FALSE;

	return IL_TRUE;
}


//! Used for setting the current face if it is a cubemap.
ILboolean ILAPIENTRY ilActiveFace(ILuint Number)
{
	ILuint Current;
    ILimage *iTempImage;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (Number == 0) {
		return IL_TRUE;
	}

    iTempImage = iCurImage;
	iCurImage = iCurImage->Faces;
	if (iCurImage == NULL) {
		iCurImage = iTempImage;
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	//Number--;  // Skip 0 (parent image)
	for (Current = 1; Current < Number; Current++) {
		iCurImage = iCurImage->Faces;
		if (iCurImage == NULL) {
			ilSetError(IL_ILLEGAL_OPERATION);
			iCurImage = iTempImage;
			return IL_FALSE;
		}
	}

	ParentImage = IL_FALSE;

	return IL_TRUE;
}



//! Used for setting the current layer if layers exist.
ILboolean ILAPIENTRY ilActiveLayer(ILuint Number)
{
	ILuint Current;
    ILimage *iTempImage;

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (Number == 0) {
		return IL_TRUE;
	}

    iTempImage = iCurImage;
	iCurImage = iCurImage->Layers;
	if (iCurImage == NULL) {
		iCurImage = iTempImage;
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	//Number--;  // Skip 0 (parent image)
	for (Current = 1; Current < Number; Current++) {
		iCurImage = iCurImage->Layers;
		if (iCurImage == NULL) {
			ilSetError(IL_ILLEGAL_OPERATION);
			iCurImage = iTempImage;
			return IL_FALSE;
		}
	}

	ParentImage = IL_FALSE;

	return IL_TRUE;
}


ILuint ILAPIENTRY ilCreateSubImage(ILenum Type, ILuint Num)
{
	ILimage	*SubImage;
	ILuint	Count ;  // Create one before we go in the loop.

	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return 0;
	}
	if (Num == 0)  {
		return 0;
	}

	switch (Type)
	{
		case IL_SUB_NEXT:
			if (iCurImage->Next)
				ilCloseImage(iCurImage->Next);
			iCurImage->Next = ilNewImage(1, 1, 1, 1, 1);
			SubImage = iCurImage->Next;
			break;

		case IL_SUB_MIPMAP:
			if (iCurImage->Mipmaps)
				ilCloseImage(iCurImage->Mipmaps);
			iCurImage->Mipmaps = ilNewImage(1, 1, 1, 1, 1);
			SubImage = iCurImage->Mipmaps;
			break;

		case IL_SUB_LAYER:
			if (iCurImage->Layers)
				ilCloseImage(iCurImage->Layers);
			iCurImage->Layers = ilNewImage(1, 1, 1, 1, 1);
			SubImage = iCurImage->Layers;
			break;

		default:
			ilSetError(IL_INVALID_ENUM);
			return IL_FALSE;
	}

	if (SubImage == NULL) {
		return 0;
	}

	for (Count = 1; Count < Num; Count++) {
		SubImage->Next = ilNewImage(1, 1, 1, 1, 1);
		SubImage = SubImage->Next;
		if (SubImage == NULL)
			return Count;
	}

	return Count;
}


// Returns the current index.
ILAPI ILuint ILAPIENTRY ilGetCurName()
{
	if (iCurImage == NULL || ImageStack == NULL || StackSize == 0)
		return 0;
	return CurName;
}


// Returns the current image.
ILAPI ILimage* ILAPIENTRY ilGetCurImage()
{
	return iCurImage;
}


// To be only used when the original image is going to be set back almost immediately.
ILAPI void ILAPIENTRY ilSetCurImage(ILimage *Image)
{
	iCurImage = Image;
	return;
}


// Completely replaces the current image and the version in the image stack.
ILAPI void ILAPIENTRY ilReplaceCurImage(ILimage *Image)
{
	if (iCurImage) {
		ilActiveImage(0);
		ilCloseImage(iCurImage);
	}
	ImageStack[ilGetCurName()] = Image;
	iCurImage = Image;
	ParentImage = IL_TRUE;
	return;
}


// Like realloc but sets new memory to 0.
void* ILAPIENTRY ilRecalloc(void *Ptr, ILuint OldSize, ILuint NewSize)
{
	void *Temp = ialloc(NewSize);
	ILuint CopySize = (OldSize < NewSize) ? OldSize : NewSize;

	if (Temp != NULL) {
		if (Ptr != NULL) {
			memcpy(Temp, Ptr, CopySize);
			ifree(Ptr);
		}

		Ptr = Temp;

		if (OldSize < NewSize)
			imemclear((ILubyte*)Temp + OldSize, NewSize - OldSize);
	}

	return Temp;
}


// Internal function to enlarge the image stack by I_STACK_INCREMENT members.
ILboolean iEnlargeStack()
{
	// 02-05-2001:  Moved from ilGenImages().
	// Puts the cleanup function on the exit handler once.
	if (!OnExit) {
		#ifdef _MEM_DEBUG
			AddToAtexit();  // So iFreeMem doesn't get called after unfreed information.
		#endif//_MEM_DEBUG
#if (!defined(_WIN32_WCE)) && (!defined(IL_STATIC_LIB))
			atexit((void*)ilShutDown);
#endif
		OnExit = IL_TRUE;
	}

	if (!(ImageStack = (ILimage**)ilRecalloc(ImageStack, StackSize * sizeof(ILimage*), (StackSize + I_STACK_INCREMENT) * sizeof(ILimage*)))) {
		return IL_FALSE;
	}
	StackSize += I_STACK_INCREMENT;
	return IL_TRUE;
}


static ILboolean IsInit = IL_FALSE;

/** Call at startup.
 *
 * What exactly this function does can depends on whether DevIL has been built with modular support or static.
 */
void ILAPIENTRY ilInit()
{
	// if it is already initialized skip initialization
	if (IsInit == IL_TRUE ) 
		return;
	INIT_LOGGING_IFNEEDED;
	INIT_LTDL_IFNEEDED;
	/* Some gettext (internationalization support)  */
	BIND_TEXTDOMAIN_IFNEEDED;
	/* OK, let's load modules and do the stuff */
	//Modules * mods = CREATE_MODULES_IFNEEDED;
	CREATE_MODULES_IFNEEDED(mods);
	/* Then let's fill in the formats */
	/* This is PARSED by the ./configure script during build, so don't write those lines in funny ways
	 * unless you have malicious intentions. 
	 * Doing like
	 * SET_FORMAT(& Formats[IL_BMP], mods, BMP, "bmp"); 
	 * should be OK 
	 */
	SET_FORMAT(& Formats[IL_BMP],	mods, BMP, "bmp");
	SET_FORMAT(& Formats[IL_CUT],	mods, CUT, "cut");
	SET_FORMAT(& Formats[IL_CHEAD],	mods, CHEAD, "h");
	SET_FORMAT(& Formats[IL_DCX],	mods, DCX, "dcx");
	SET_FORMAT(& Formats[IL_DDS],	mods, DDS, "dds");
	SET_FORMAT(& Formats[IL_DOOM],	mods, DOOM, "wad");
	SET_FORMAT(& Formats[IL_EXR],	mods, EXR, "exr");
	SET_FORMAT(& Formats[IL_FITS],	mods, FITS, "fits fit");
	SET_FORMAT(& Formats[IL_GIF],	mods, GIF, "gif");
	SET_FORMAT(& Formats[IL_HDR],	mods, HDR, "hdr");
	SET_FORMAT(& Formats[IL_ICNS],	mods, ICNS, "icns");
	SET_FORMAT(& Formats[IL_ICON],	mods, ICON, "ico cur");
	SET_FORMAT(& Formats[IL_IFF],	mods, IFF, "iff ilbm lbm");
	SET_FORMAT(& Formats[IL_ILBM],	mods, ILBM, "ilbm");
	SET_FORMAT(& Formats[IL_JPEG],	mods, JPEG, "jpeg jpg jpe");
	SET_FORMAT(& Formats[IL_JP2],	mods, JP2, "jp2");
	SET_FORMAT(& Formats[IL_LIF],	mods, LIF, "lif");
	SET_FORMAT(& Formats[IL_MDL],	mods, MDL, "mdl");
	SET_FORMAT(& Formats[IL_MNG],	mods, MNG, "mng");
	SET_FORMAT(& Formats[IL_PCX],	mods, PCX, "pcx");
	SET_FORMAT(& Formats[IL_PCD],	mods, PCD, "pcd");
	SET_FORMAT(& Formats[IL_PIC],	mods, PIC, "pic");
	SET_FORMAT(& Formats[IL_PIX],	mods, PIX, "pix");
	SET_FORMAT(& Formats[IL_PNG],	mods, PNG, "png");
	SET_FORMAT(& Formats[IL_PNM],	mods, PNM, "pnm pbm pgm ppm");
	SET_FORMAT(& Formats[IL_PSD],	mods, PSD, "psd");
	SET_FORMAT(& Formats[IL_PSP],	mods, PSP, "psp");
	SET_FORMAT(& Formats[IL_PXR],	mods, PXR, "pxr");
	SET_FORMAT(& Formats[IL_RAW],	mods, RAW, "raw");
	SET_FORMAT(& Formats[IL_ROT],	mods, ROT, "rot");
	SET_FORMAT(& Formats[IL_SGI],	mods, SGI, "sgi bw rgb rgba");
	SET_FORMAT(& Formats[IL_SUN],	mods, SUN, "sun ras rs im1 im8 im24 im32");
	SET_FORMAT(& Formats[IL_TEXTURE],	mods, TEXTURE, "texture");
	SET_FORMAT(& Formats[IL_TGA],	mods, TGA, "tga vda icb vst");
	SET_FORMAT(& Formats[IL_TIFF],	mods, TIFF, "tiff tif");
	SET_FORMAT(& Formats[IL_TPL],	mods, TPL, "tpl");
	SET_FORMAT(& Formats[IL_UTX],	mods, UTX, "utx");
	SET_FORMAT(& Formats[IL_VTF],	mods, VTF, "vtf");
	SET_FORMAT(& Formats[IL_WAL],	mods, WAL, "wal");
	SET_FORMAT(& Formats[IL_WBMP],	mods, WBMP, "wbmp");
	SET_FORMAT(& Formats[IL_WDP],	mods, WDP, "wdp");
	SET_FORMAT(& Formats[IL_XPM],	mods, XPM, "xpm");
	SET_FORMAT(& Formats[IL_BLP],	mods, BLP, "blp");
	SET_FORMAT(& Formats[IL_IWI],	mods, IWI, "iwi");
	SET_FORMAT(& Formats[IL_FTX],	mods, FTX, "ftx");
	SET_FORMAT(& Formats[IL_DICOM],	mods, DICOM, "dicom dcm");

	/* The original ilInit continues... */
	//ilSetMemory(NULL, NULL);  Now useless 3/4/2006 (due to modification in il_alloc.c)
	ilSetError(IL_NO_ERROR);
	ilDefaultStates();  // Set states to their defaults.
	// Sets default file-reading callbacks.
	ilResetRead();
	ilResetWrite();
#if (!defined(_WIN32_WCE)) && (!defined(IL_STATIC_LIB))
	atexit((void*)ilRemoveRegistered);
#endif
	//_WIN32_WCE
	//ilShutDown();
	iSetImage0();  // Beware!  Clears all existing textures!
	iBindImageTemp();  // Go ahead and create the temporary image.
	IsInit = IL_TRUE;
	return;
}


// Frees any extra memory in the stack.
//	- Called on exit
void ILAPIENTRY ilShutDown()
{
	// if it is not initialized do not shutdown
	iFree* TempFree = (iFree*)FreeNames;
	ILuint i;
	
	if (!IsInit)
		return;

	if (!IsInit) {  // Prevent from being called when not initialized.
		ilSetError(IL_ILLEGAL_OPERATION);
		return;
	}

	while (TempFree != NULL) {
		FreeNames = (iFree*)TempFree->Next;
		ifree(TempFree);
		TempFree = FreeNames;
	}

	//for (i = 0; i < LastUsed; i++) {
	for (i = 0; i < StackSize; i++) {
		if (ImageStack[i] != NULL)
			ilCloseImage(ImageStack[i]);
	}

	if (ImageStack)
		ifree(ImageStack);
	ImageStack = NULL;
	LastUsed = 0;
	StackSize = 0;
	IL_LOG_IFNEEDED(_("Thank you for closing DevIL properly, bye bye."), IL_LOG_INFO);
	IsInit = IL_FALSE;
	return;
}


// Initializes the image stack's first entry (default image) -- ONLY CALL ONCE!
void iSetImage0()
{
	if (ImageStack == NULL)
		if (!iEnlargeStack())
			return;

	LastUsed = 1;
	CurName = 0;
	ParentImage = IL_TRUE;
	if (!ImageStack[0])
		ImageStack[0] = ilNewImage(1, 1, 1, 1, 1);
	iCurImage = ImageStack[0];
	ilDefaultImage();

	return;
}


ILAPI void ILAPIENTRY iBindImageTemp()
{
	if (ImageStack == NULL || StackSize <= 1)
		if (!iEnlargeStack())
			return;

	if (LastUsed < 2)
		LastUsed = 2;
	CurName = 1;
	ParentImage = IL_TRUE;
	if (!ImageStack[1])
		ImageStack[1] = ilNewImage(1, 1, 1, 1, 1);
	iCurImage = ImageStack[1];

	return;
}
