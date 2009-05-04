//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 05/01/2009
//
// Filename: src-IL/src/il_stack.cpp
//
// Description: The main image stack
//
//-----------------------------------------------------------------------------

#include "il_internal.h"
#include "il_stack.h"

//! Creates Num images and puts their index in Images - similar to glGenTextures().
void ILAPIENTRY ilGenImages(ILsizei Num, ILimage **Images, ILstate *State)
{
	//ILsizei	Index = 0;
	//iFree	*TempFree = FreeNames;

	if (Num < 1 || Images == NULL) {
		ilSetError(IL_INVALID_PARAM);
		return;
	}

	// No images have been generated yet, so create the image stack.
	/*if (ImageStack == NULL)
		if (!iEnlargeStack())
			return;

	do {
		if (FreeNames != NULL) {  // If any have been deleted, then reuse their image names.
                        TempFree = (iFree*)FreeNames->Next;
                        Images[Index] = FreeNames->Name;
                        ImageStack[FreeNames->Name] = ilNewImage(1, 1, 1, IL_LUMINANCE, IL_UNSIGNED_BYTE, NULL);
                        ifree(FreeNames);
                        FreeNames = TempFree;
		} else {
                        if (LastUsed >= StackSize)
				if (!iEnlargeStack())
					return;
			Images[Index] = LastUsed;
			// Must be all 1's instead of 0's, because some functions would divide by 0.
			ImageStack[LastUsed] = ilNewImage(1, 1, 1, IL_LUMINANCE, IL_UNSIGNED_BYTE, NULL);
			LastUsed++;
		}
	} while (++Index < Num);*/

	for (ILsizei i = 0; i < Num; i++) {
		Images[i] = ilNewImage(1, 1, 1, IL_LUMINANCE, IL_UNSIGNED_BYTE, NULL, State);
	}

	return;
}

ILimage* ILAPIENTRY ilGenImage(ILstate *State)
{
    return ilNewImage(1, 1, 1, IL_LUMINANCE, IL_UNSIGNED_BYTE, NULL, State);
}

//! Makes Image the current active image - similar to glBindTexture().
void ILAPIENTRY ilBindImage(ILuint Image)
{
	//if (ImageStack == NULL || StackSize == 0) {
	//	if (!iEnlargeStack()) {
	//		return;
	//	}
	//}

	//// If the user requests a high image name.
	//while (Image >= StackSize) {
	//	if (!iEnlargeStack()) {
	//		return;
	//	}
	//}

	//if (ImageStack[Image] == NULL) {
	//	ImageStack[Image] = ilNewImage(1, 1, 1, IL_LUMINANCE, IL_UNSIGNED_BYTE, NULL);
	//	if (Image >= LastUsed) // >= ?
	//		LastUsed = Image + 1;
	//}

	//iCurImage = ImageStack[Image];
	//CurName = Image;

	//ParentImage = IL_TRUE;

	return;
}


//! Deletes Num images from the image stack - similar to glDeleteTextures().
void ILAPIENTRY ilDeleteImages(ILsizei Num, ILimage **Images)
{
	//iFree	*Temp = FreeNames;
	//ILuint	Index = 0;

	//if (Num < 1) {
	//	//ilSetError(IL_INVALID_VALUE);
	//	return;
	//}
	//if (StackSize == 0)
	//	return;

	//do {
	//	if (Images[Index] > 0 && Images[Index] < LastUsed) {  // <= ?
	//		/*if (FreeNames != NULL) {  // Terribly inefficient
	//			Temp = FreeNames;
	//			do {
	//				if (Temp->Name == Images[Index]) {
	//					continue;  // Sufficient?
	//				}
	//			} while ((Temp = Temp->Next));
	//		}*/

	//		// Already has been deleted or was never used.
	//		if (ImageStack[Images[Index]] == NULL)
	//			continue;

	//		// Find out if current image - if so, set to default image zero.
	//		if (Images[Index] == CurName || Images[Index] == 0) {
	//			iCurImage = ImageStack[0];
	//			CurName = 0;
	//		}
	//		
	//		// Should *NOT* be NULL here!
	//		ilCloseImage(ImageStack[Images[Index]]);
	//		ImageStack[Images[Index]] = NULL;

	//		// Add to head of list - works for empty and non-empty lists
	//		Temp = (iFree*)ialloc(sizeof(iFree));
	//		if (!Temp) {
	//			return;
	//		}
	//		Temp->Name = Images[Index];
	//		Temp->Next = FreeNames;
	//		FreeNames = Temp;
	//	}
	//	/*else {  // Shouldn't set an error...just continue onward.
	//		ilSetError(IL_ILLEGAL_OPERATION);
	//	}*/
	//} while (++Index < (ILuint)Num);
}


void ILAPIENTRY ilDeleteImage(ILimage *Image)
{
    ilCloseImage(Image);
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


ILAPI ILboolean ILAPIENTRY ilIsValidPal(ILpal *Palette)
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
	if (!ilIsValidPal(Palette))
		return;
	ifree(Palette->Palette);
	ifree(Palette);
	return;
}


ILimage *iGetBaseImage()
{
	//return ImageStack[ilGetCurName()];
	return NULL;
}


//! Sets the current mipmap level
ILimage* ILAPIENTRY ilGetMipmap(ILimage *Image, ILuint Number)
{
	ILimage *SubImage;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return NULL;
	}

	if (Number == 0) {
		return Image;
	}

	SubImage = Image->Mipmaps;
	if (SubImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return NULL;
	}

	Number--;  // Skip 0 (parent image)
	for (ILuint Current = 0; Current < Number; Current++) {
		SubImage = SubImage->Mipmaps;
		if (SubImage == NULL) {
			ilSetError(IL_ILLEGAL_OPERATION);
			return NULL;
		}
	}

	return SubImage;
}


//! Used for setting the current image if it is an animation.
ILimage* ILAPIENTRY ilGetImage(ILimage *Image, ILuint Number)
{
	ILimage *SubImage;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return NULL;
	}

	if (Number == 0) {
		return Image;
	}

	SubImage = Image->Next;
	if (SubImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return NULL;
	}

	Number--;  // Skip 0 (parent image)
	for (ILuint Current = 0; Current < Number; Current++) {
		SubImage = SubImage->Next;
		if (SubImage == NULL) {
			ilSetError(IL_ILLEGAL_OPERATION);
			return NULL;
		}
	}

	return SubImage;
}


//! Used for setting the current face if it is a cubemap.
ILimage* ILAPIENTRY ilGetFace(ILimage *Image, ILuint Number)
{
	ILimage *SubImage;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return NULL;
	}

	if (Number == 0) {
		return Image;
	}

	SubImage = Image->Faces;
	if (SubImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return NULL;
	}

	Number--;  // Skip 0 (parent image)
	for (ILuint Current = 0; Current < Number; Current++) {
		SubImage = SubImage->Faces;
		if (SubImage == NULL) {
			ilSetError(IL_ILLEGAL_OPERATION);
			return NULL;
		}
	}

	return SubImage;
}



//! Used for setting the current layer if layers exist.
ILimage* ILAPIENTRY ilGetLayer(ILimage *Image, ILuint Number)
{
	ILimage *SubImage;

	if (Image == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return NULL;
	}

	if (Number == 0) {
		return Image;
	}

	SubImage = Image->Layers;
	if (SubImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return NULL;
	}

	Number--;  // Skip 0 (parent image)
	for (ILuint Current = 0; Current < Number; Current++) {
		SubImage = SubImage->Layers;
		if (SubImage == NULL) {
			ilSetError(IL_ILLEGAL_OPERATION);
			return NULL;
		}
	}

	return SubImage;
}


ILuint ILAPIENTRY ilCreateSubImage(ILenum Type, ILuint Num)
{
	//ILimage	*SubImage;
	//ILuint	Count;  // Create one before we go in the loop.

	//if (iCurImage == NULL) {
	//	ilSetError(IL_ILLEGAL_OPERATION);
	//	return 0;
	//}
	//if (Num == 0)  {
	//	return 0;
	//}

	//switch (Type)
	//{
	//	case IL_SUB_NEXT:
	//		if (iCurImage->Next)
	//			ilCloseImage(iCurImage->Next);
	//		iCurImage->Next = ilNewImage(1, 1, 1, 1, 1);
	//		SubImage = iCurImage->Next;
	//		break;

	//	case IL_SUB_MIPMAP:
	//		if (iCurImage->Mipmaps)
	//			ilCloseImage(iCurImage->Mipmaps);
	//		iCurImage->Mipmaps = ilNewImage(1, 1, 1, 1, 1);
	//		SubImage = iCurImage->Mipmaps;
	//		break;

	//	case IL_SUB_LAYER:
	//		if (iCurImage->Layers)
	//			ilCloseImage(iCurImage->Layers);
	//		iCurImage->Layers = ilNewImage(1, 1, 1, 1, 1);
	//		SubImage = iCurImage->Layers;
	//		break;

	//	default:
	//		ilSetError(IL_INVALID_ENUM);
	//		return IL_FALSE;
	//}

	//if (SubImage == NULL) {
	//	return 0;
	//}

	//for (Count = 1; Count < Num; Count++) {
	//	SubImage->Next = ilNewImage(1, 1, 1, 1, 1);
	//	SubImage = SubImage->Next;
	//	if (SubImage == NULL)
	//		return Count;
	//}

	//return Count;
	return 0;
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

void iShutDown()
{
	return ilShutDown();
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
			atexit(iShutDown);
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

// ONLY call at startup.
void ILAPIENTRY ilInit()
{
	// if it is already initialized skip initialization
	if (IsInit == IL_TRUE ) 
		return;
	
	//ilSetMemory(NULL, NULL);  Now useless 3/4/2006 (due to modification in il_alloc.c)
	ilSetError(IL_NO_ERROR);
	ilDefaultStates(NULL);  // Set states to their defaults.
	// Sets default file-reading callbacks.
	ilResetRead();
	ilResetWrite();
#if (!defined(_WIN32_WCE)) && (!defined(IL_STATIC_LIB))
	atexit(ilRemoveRegistered);
#endif
	//_WIN32_WCE
	//ilShutDown();
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
	IsInit = IL_FALSE;
	return;
}


// Initializes the image stack's first entry (default image) -- ONLY CALL ONCE!
void iSetImage0()
{
	//if (ImageStack == NULL)
	//	if (!iEnlargeStack())
	//		return;

	//LastUsed = 1;
	//CurName = 0;
	//ParentImage = IL_TRUE;
	//if (!ImageStack[0])
	//	ImageStack[0] = ilNewImage(1, 1, 1, 1, 1);
	//iCurImage = ImageStack[0];
	//ilDefaultImage();

	return;
}


ILAPI void ILAPIENTRY iBindImageTemp()
{
	//if (ImageStack == NULL || StackSize <= 1)
	//	if (!iEnlargeStack())
	//		return;

	//if (LastUsed < 2)
	//	LastUsed = 2;
	//CurName = 1;
	//ParentImage = IL_TRUE;
	//if (!ImageStack[1])
	//	ImageStack[1] = ilNewImage(1, 1, 1, 1, 1);
	//iCurImage = ImageStack[1];

	return;
}
