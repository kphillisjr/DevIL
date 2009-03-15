
#include "ilu_internal.h"
#include "ilu_states.h"
#include <float.h>
#include <limits.h>


ILboolean iluCrop2D(ILimage *Image, ILuint XOff, ILuint YOff, ILuint Width, ILuint Height)
{
	ILuint	x, y, c, OldBps;
	ILubyte	*Data;
	ILenum	Origin;

	if (Image == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	// Uh-oh, what about 0 dimensions?!
	if (Width > Image->Width || Height > Image->Height) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Data = (ILubyte*)ialloc(Image->SizeOfData);
	if (Data == NULL) {
		return IL_FALSE;
	}

	OldBps = Image->Bps;
	Origin = Image->Origin;
	ilCopyPixels(Image, 0, 0, 0, Image->Width, Image->Height, 1, Image->Format, Image->Type, Data);
	if (!ilTexImage(Image, Width, Height, Image->Depth, Image->Format, Image->Type, NULL)) {
		free(Data);
		return IL_FALSE;
	}
	Image->Origin = Origin;

	// @TODO:  Optimize!  (Especially XOff * Image->Bpp...get rid of it!)
	for (y = 0; y < Image->Height; y++) {
		for (x = 0; x < Image->Bps; x += Image->Bpp) {
			for (c = 0; c < Image->Bpp; c++) {
				Image->Data[y * Image->Bps + x + c] = 
					Data[(y + YOff) * OldBps + x + XOff * Image->Bpp + c];
			}
		}
	}

	ifree(Data);

	return IL_TRUE;
}


ILboolean iluCrop3D(ILimage *Image, ILuint XOff, ILuint YOff, ILuint ZOff, ILuint Width, ILuint Height, ILuint Depth)
{
	ILuint	x, y, z, c, OldBps, OldPlane;
	ILubyte	*Data;
	ILenum	Origin;

	if (Image == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	// Uh-oh, what about 0 dimensions?!
	if (Width > Image->Width || Height > Image->Height || Depth > Image->Depth) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Data = (ILubyte*)ialloc(Image->SizeOfData);
	if (Data == NULL) {
		return IL_FALSE;
	}

	OldBps = Image->Bps;
	OldPlane = Image->SizeOfPlane;
	Origin = Image->Origin;
	ilCopyPixels(Image, 0, 0, 0, Image->Width, Image->Height, Image->Depth, Image->Format, Image->Type, Data);
	if (!ilTexImage(Image, Width - XOff, Height - YOff, Depth - ZOff, Image->Format, Image->Type, NULL)) {
		ifree(Data);
	}
	Image->Origin = Origin;

	for (z = 0; z < Image->Depth; z++) {
		for (y = 0; y < Image->Height; y++) {
			for (x = 0; x < Image->Bps; x += Image->Bpp) {
				for (c = 0; c < Image->Bpp; c++) {
					Image->Data[z * Image->SizeOfPlane + y * Image->Bps + x + c] = 
						Data[(z + ZOff) * OldPlane + (y + YOff) * OldBps + (x + XOff) + c];
				}
			}
		}
	}

	ifree(Data);

	return IL_TRUE;
}


ILboolean ILAPIENTRY iluCrop(ILimage *Image, ILuint XOff, ILuint YOff, ILuint ZOff, ILuint Width, ILuint Height, ILuint Depth)
{
	if (ZOff <= 1)
		return iluCrop2D(Image, XOff, YOff, Width, Height);
	return iluCrop3D(Image, XOff, YOff, ZOff, Width, Height, Depth);
}


//! Enlarges the canvas
ILboolean ILAPIENTRY iluEnlargeCanvas(ILimage *Image, ILuint Width, ILuint Height, ILuint Depth)
{
	ILubyte	*Data/*, Clear[4]*/;
	ILuint	x, y, z, OldBps, OldH, OldD, OldPlane, AddX, AddY;
	ILenum	Origin;

	if (Image == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	// Uh-oh, what about 0 dimensions?!
	if (Width < Image->Width || Height < Image->Height || Depth < Image->Depth) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (Image->Origin == IL_ORIGIN_LOWER_LEFT) {
		switch (iluPlacement)
		{
			case ILU_LOWER_LEFT:
				AddX = 0;
				AddY = 0;
				break;
			case ILU_LOWER_RIGHT:
				AddX = Width - Image->Width;
				AddY = 0;
				break;
			case ILU_UPPER_LEFT:
				AddX = 0;
				AddY = Height - Image->Height;
				break;
			case ILU_UPPER_RIGHT:
				AddX = Width - Image->Width;
				AddY = Height - Image->Height;
				break;
			case ILU_CENTER:
				AddX = (Width - Image->Width) >> 1;
				AddY = (Height - Image->Height) >> 1;
				break;
			default:
				ilSetError(ILU_INVALID_PARAM);
				return IL_FALSE;
		}
	}
	else {  // IL_ORIGIN_UPPER_LEFT
		switch (iluPlacement)
		{
			case ILU_LOWER_LEFT:
				AddX = 0;
				AddY = Height - Image->Height;
				break;
			case ILU_LOWER_RIGHT:
				AddX = Width - Image->Width;
				AddY = Height - Image->Height;
				break;
			case ILU_UPPER_LEFT:
				AddX = 0;
				AddY = 0;
				break;
			case ILU_UPPER_RIGHT:
				AddX = Width - Image->Width;
				AddY = 0;
				break;
			case ILU_CENTER:
				AddX = (Width - Image->Width) >> 1;
				AddY = (Height - Image->Height) >> 1;
				break;
			default:
				ilSetError(ILU_INVALID_PARAM);
				return IL_FALSE;
		}
	}

	AddX *= Image->Bpp;

	Data = (ILubyte*)ialloc(Image->SizeOfData);
	if (Data == NULL) {
		return IL_FALSE;
	}

	// Preserve old data.
	OldPlane = Image->SizeOfPlane;
	OldBps   = Image->Bps;
	OldH     = Image->Height;
	OldD     = Image->Depth;
	Origin   = Image->Origin;
	ilCopyPixels(Image, 0, 0, 0, Image->Width, Image->Height, OldD, Image->Format, Image->Type, Data);

	ilTexImage(Image, Width, Height, Depth, Image->Format, Image->Type, NULL);
	Image->Origin = Origin;

	ilClearImage(Image);
	/*ilGetClear(Clear);
	if (Image->Bpp == 1) {
		memset(Image->Data, Clear[3], Image->SizeOfData);
	}
	else {
		for (x = 0; x < Image->SizeOfData; x += Image->Bpp) {
			for (y = 0; y < Image->Bpp; y++) {
				Image->Data[y] = Clear[y];
			}
		}
	}*/

	for (z = 0; z < OldD; z++) {
		for (y = 0; y < OldH; y++) {
			for (x = 0; x < OldBps; x++) {
				Image->Data[z * Image->SizeOfPlane + (y + AddY) * Image->Bps + x + AddX] =
					Data[z * OldPlane + y * OldBps + x];
			}
		}
	}

	ifree(Data);

	return IL_TRUE;
}

//! Flips an image over its x axis
ILboolean ILAPIENTRY iluFlipImage(ILimage *Image)
{
	//ILubyte *StartPtr, *EndPtr;
	//ILuint y, d;

	if (Image == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	iFlipBuffer(Image->Data, Image->Depth, Image->Bps, Image->Height);
	/*
	for( d = 0; d < image->Depth; d++ ) {
		StartPtr = image->Data + d * image->SizeOfPlane;
		EndPtr   = image->Data + d * image->SizeOfPlane 
						+ image->SizeOfPlane;

		for( y = 0; y < (image->Height/2); y++ ) {
			EndPtr -= image->Bps; 
			iMemSwap(StartPtr,EndPtr,image->Bps);
			StartPtr += image->Bps;
		}
	}
	*/
	return IL_TRUE;
}


//! Mirrors an image over its y axis
ILboolean ILAPIENTRY iluMirror(ILimage *Image)
{
	return iMirror(Image);
}


//! Inverts the alpha in the image
ILboolean ILAPIENTRY iluInvertAlpha(ILimage *Image)
{
	ILuint		i, *IntPtr, NumPix;
	ILubyte		*Data;
	ILushort	*ShortPtr;
	ILfloat		*FltPtr;
	ILdouble	*DblPtr;
	ILubyte		Bpp;

	if (Image == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (Image->Format != IL_RGBA &&
		Image->Format != IL_BGRA &&
		Image->Format != IL_LUMINANCE_ALPHA) {
			ilSetError(ILU_ILLEGAL_OPERATION);
			return IL_FALSE;
	}

	Data = Image->Data;
	Bpp = Image->Bpp;
	NumPix = Image->Width * Image->Height * Image->Depth;

	switch (Image->Type)
	{
		case IL_BYTE:
		case IL_UNSIGNED_BYTE:
			Data += (Bpp - 1);
			for( i = Bpp - 1; i < NumPix; i++, Data += Bpp )
				*(Data) = ~*(Data);
			break;

		case IL_SHORT:
		case IL_UNSIGNED_SHORT:
			ShortPtr = ((ILushort*)Data) + Bpp-1;	
			for (i = Bpp - 1; i < NumPix; i++, ShortPtr += Bpp)
				*(ShortPtr) = ~*(ShortPtr);
			break;

		case IL_INT:
		case IL_UNSIGNED_INT:
			IntPtr = ((ILuint*)Data) + Bpp-1;
			for (i = Bpp - 1; i < NumPix; i++, IntPtr += Bpp)
				*(IntPtr) = ~*(IntPtr);
			break;

		case IL_FLOAT:
			FltPtr = ((ILfloat*)Data) + Bpp - 1;
			for (i = Bpp - 1; i < NumPix; i++, FltPtr += Bpp)
				*(FltPtr) = 1.0f - *(FltPtr);
			break;

		case IL_DOUBLE:
			DblPtr = ((ILdouble*)Data) + Bpp - 1;
			for (i = Bpp - 1; i < NumPix; i++, DblPtr += Bpp)
				*(DblPtr) = 1.0f - *(DblPtr);
			break;
	}

	return IL_TRUE;
}


//! Inverts the colours in the image
ILboolean ILAPIENTRY iluNegative(ILimage *Image)
{
	ILuint		i, j, c, *IntPtr, NumPix, Bpp;
	ILubyte		*Data;
	ILushort	*ShortPtr;
	ILubyte		*RegionMask;

	if (Image == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (Image->Format == IL_COLOUR_INDEX) {
		if (!Image->Pal.Palette || !Image->Pal.PalSize || Image->Pal.PalType == IL_PAL_NONE) {
			ilSetError(ILU_ILLEGAL_OPERATION);
			return IL_FALSE;
		}
		Data = Image->Pal.Palette;
		i = Image->Pal.PalSize;
	}
	else {
		Data = Image->Data;
		i = Image->SizeOfData;
	}

	RegionMask = iScanFill(Image);
	
	// @TODO:  Optimize this some.

	NumPix = i / Image->Bpc;
	Bpp = Image->Bpp;

	if (RegionMask) {
		switch (Image->Bpc)
		{
			case 1:
				for (j = 0, i = 0; j < NumPix; j += Bpp, i++, Data += Bpp) {
					for (c = 0; c < Bpp; c++) {
						if (RegionMask[i])
							*(Data+c) = ~*(Data+c);
					}
				}
				break;

			case 2:
				ShortPtr = (ILushort*)Data;
				for (j = 0, i = 0; j < NumPix; j += Bpp, i++, ShortPtr += Bpp) {
					for (c = 0; c < Bpp; c++) {
						if (RegionMask[i])
							*(ShortPtr+c) = ~*(ShortPtr+c);
					}
				}
				break;

			case 4:
				IntPtr = (ILuint*)Data;
				for (j = 0, i = 0; j < NumPix; j += Bpp, i++, IntPtr += Bpp) {
					for (c = 0; c < Bpp; c++) {
						if (RegionMask[i])
							*(IntPtr+c) = ~*(IntPtr+c);
					}
				}
				break;
		}
	}
	else {
		switch (Image->Bpc)
		{
			case 1:
				for (j = 0; j < NumPix; j++, Data++) {
					*(Data) = ~*(Data);
				}
				break;

			case 2:
				ShortPtr = (ILushort*)Data;
				for (j = 0; j < NumPix; j++, ShortPtr++) {
					*(ShortPtr) = ~*(ShortPtr);
				}
				break;

			case 4:
				IntPtr = (ILuint*)Data;
				for (j = 0; j < NumPix; j++, IntPtr++) {
					*(IntPtr) = ~*(IntPtr);
				}
				break;
		}
	}

	ifree(RegionMask);

	return IL_TRUE;
}


// Taken from
//	http://www-classic.be.com/aboutbe/benewsletter/volume_III/Issue2.html#Insight
//	Hope they don't mind too much. =]
ILboolean ILAPIENTRY iluWave(ILimage *Image, ILfloat Angle)
{
	ILint	Delta;
	ILuint	y;
	ILubyte	*DataPtr, *TempBuff;

	if (Image == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	TempBuff = (ILubyte*)ialloc(Image->SizeOfData);
	if (TempBuff == NULL) {
		return IL_FALSE;
	}

	for (y = 0; y < Image->Height; y++) {
		Delta = (ILint)
			(30 * sin((10 * Angle + y) * IL_DEGCONV) +
			 15 * sin(( 7 * Angle + 3 * y) * IL_DEGCONV));

		DataPtr = Image->Data + y * Image->Bps;

		if (Delta < 0) {
			Delta = -Delta;
			memcpy(TempBuff, DataPtr, Image->Bpp * Delta);
			memcpy(DataPtr, DataPtr + Image->Bpp * Delta, Image->Bpp * (Image->Width - Delta));
			memcpy(DataPtr + Image->Bpp * (Image->Width - Delta), TempBuff, Image->Bpp * Delta);
		}
		else if (Delta > 0) {
			memcpy(TempBuff, DataPtr, Image->Bpp * (Image->Width - Delta));
			memcpy(DataPtr, DataPtr + Image->Bpp * (Image->Width - Delta), Image->Bpp * Delta);
			memcpy(DataPtr + Image->Bpp * Delta, TempBuff, Image->Bpp * (Image->Width - Delta));
		}
	}

	ifree(TempBuff);

	return IL_TRUE;
}


// Swaps the colour order of the current image (rgb(a)->bgr(a) or vice-versa).
//	Must be either an 8, 24 or 32-bit (coloured) image (or palette).
ILboolean ILAPIENTRY iluSwapColours(ILimage *Image)
{
	// Use ilConvert or other like that to convert the data?
	// and extend that function to work even on paletted data
	
	if (Image == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (Image->Bpp == 1) {
		if (ilGetBppPal(Image->Pal.PalType) == 0 || Image->Format != IL_COLOUR_INDEX) {
			ilSetError(ILU_ILLEGAL_OPERATION);  // Can be luminance.
			return IL_FALSE;
		}
		
		switch (Image->Pal.PalType)
		{
			case IL_PAL_RGB24:
				return ilConvertPal(Image, IL_PAL_BGR24);
			case IL_PAL_RGB32:
				return ilConvertPal(Image, IL_PAL_BGR32);
			case IL_PAL_RGBA32:
				return ilConvertPal(Image, IL_PAL_BGRA32);
			case IL_PAL_BGR24:
				return ilConvertPal(Image, IL_PAL_RGB24);
			case IL_PAL_BGR32:
				return ilConvertPal(Image, IL_PAL_RGB32);
			case IL_PAL_BGRA32:
				return ilConvertPal(Image, IL_PAL_RGBA32);
			default:
				ilSetError(ILU_INTERNAL_ERROR);
				return IL_FALSE;
		}
	}

	switch (Image->Format)
	{
		case IL_RGB:
			return ilConvertImage(Image, IL_BGR, Image->Type);
		case IL_RGBA:
			return ilConvertImage(Image, IL_BGRA, Image->Type);
		case IL_BGR:
			return ilConvertImage(Image, IL_RGB, Image->Type);
		case IL_BGRA:
			return ilConvertImage(Image, IL_RGBA, Image->Type);
	}

	ilSetError(ILU_INTERNAL_ERROR);
	return IL_FALSE;
}


typedef struct BUCKET { ILubyte Colours[4];  struct BUCKET *Next; } BUCKET;

ILuint ILAPIENTRY iluColoursUsed(ILimage *Image)
{
	ILuint i, c, Bpp, ColVal, SizeData, BucketPos = 0, NumCols = 0;
	BUCKET Buckets[8192], *Temp;
	ILubyte ColTemp[4];
	ILboolean Matched;
	BUCKET *Heap[9];
	ILuint HeapPos = 0, HeapPtr = 0, HeapSize;

	imemclear(Buckets, sizeof(BUCKET) * 8192);
	for (c = 0; c < 9; c++) {
		Heap[c] = 0;
	}

	if (Image == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return 0;
	}

	Bpp = Image->Bpp;
	SizeData = Image->SizeOfData;

	// Create our miniature memory heap.
	// I have determined that the average number of colours versus
	//	the number of pixels is about a 1:8 ratio, so divide by 8.
	HeapSize = IL_MAX(1, Image->SizeOfData / Image->Bpp / 8);
	Heap[0] = (BUCKET*)ialloc(HeapSize * sizeof(BUCKET));
	if (Heap[0] == NULL)
		return IL_FALSE;

	for (i = 0; i < SizeData; i += Bpp) {
		*(ILuint*)ColTemp = 0;
		/*for (c = 0; c < Bpp; c++) {
			ColTemp[c] = Image->Data[i + c];
		}*/
		ColTemp[0] = Image->Data[i];
		if (Bpp > 1) {
			ColTemp[1] = Image->Data[i + 1];
			ColTemp[2] = Image->Data[i + 2];
		}
		if (Bpp > 3)
			ColTemp[3] = Image->Data[i + 3];

		BucketPos = *(ILuint*)ColTemp % 8192;

		// Add to hash table
		if (Buckets[BucketPos].Next == NULL) {
			NumCols++;
			//Buckets[BucketPos].Next = (BUCKET*)ialloc(sizeof(BUCKET));
			Buckets[BucketPos].Next = Heap[HeapPos] + HeapPtr++;
			if (HeapPtr >= HeapSize) {
				Heap[++HeapPos] = (BUCKET*)ialloc(HeapSize * sizeof(BUCKET));
				if (Heap[HeapPos] == NULL)
					goto alloc_error;
				HeapPtr = 0;
			}
			*(ILuint*)Buckets[BucketPos].Next->Colours = *(ILuint*)ColTemp;
			Buckets[BucketPos].Next->Next = NULL;
		}
		else {
			Matched = IL_FALSE;
			Temp = Buckets[BucketPos].Next;

			ColVal = *(ILuint*)ColTemp;
			while (Temp->Next != NULL) {
				if (ColVal == *(ILuint*)Temp->Colours) {
					Matched = IL_TRUE;
					break;
				}
				Temp = Temp->Next;
			}
			if (!Matched) {
				if (ColVal != *(ILuint*)Temp->Colours) {  // Check against last entry
					NumCols++;
					Temp = Buckets[BucketPos].Next;
					//Buckets[BucketPos].Next = (BUCKET*)ialloc(sizeof(BUCKET));
					Buckets[BucketPos].Next = Heap[HeapPos] + HeapPtr++;
					if (HeapPtr >= HeapSize) {
						Heap[++HeapPos] = (BUCKET*)ialloc(HeapSize * sizeof(BUCKET));
						if (Heap[HeapPos] == NULL)
							goto alloc_error;
						HeapPtr = 0;
					}
					Buckets[BucketPos].Next->Next = Temp;
					*(ILuint*)Buckets[BucketPos].Next->Colours = *(ILuint*)ColTemp;
				}
			}
		}
	}

	// Delete our mini heap.
	for (i = 0; i < 9; i++) {
		if (Heap[i] == NULL)
			break;
		ifree(Heap[i]);
	}

	return NumCols;

alloc_error:
	for (i = 0; i < 9; i++) {
		ifree(Heap[i]);
	}

	return 0;
}


ILboolean ILAPIENTRY iluCompareImage(ILimage *Image1, ILimage *Image2)
{
	ILuint		i;
	ILboolean	Same = IL_TRUE;

	if (Image1 == NULL || Image2 == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return 0;
	}

	// @TODO:  Should we check palettes, too?
	if (Image1->Bpp != Image2->Bpp       ||
		Image1->Depth != Image2->Depth   ||
		Image1->Format != Image2->Format ||
		Image1->Height != Image2->Height ||
		Image1->Origin != Image2->Origin ||
		Image1->SizeOfData != Image2->SizeOfData ||
		Image1->Type != Image2->Type ||
		Image1->Width != Image2->Width) {
			return IL_FALSE;
	}

	// Image1->SizeOfData and Image2->SizeOfData are guaranteed to
	//  be the same at this point.
	for (i = 0; i < Image1->SizeOfData; i++) {
		if (Image1->Data[i] != Image2->Data[i]) {
			Same = IL_FALSE;
			break;
		}
	}

	return Same;
}


// @TODO:  FIX ILGETCLEARCALL!
ILboolean ILAPIENTRY iluReplaceColour(ILimage *Image, ILubyte Red, ILubyte Green, ILubyte Blue, ILfloat Tolerance)
{
	ILubyte	ClearCol[4];
	ILint	TolVal, Distance, Dist1, Dist2, Dist3;
	ILuint	i, NumPix;

	if (Image == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return 0;
	}

	ilGetClear(ClearCol, IL_RGBA, IL_UNSIGNED_BYTE);
	if (Tolerance > 1.0f || Tolerance < -1.0f)
		Tolerance = 1.0f;  // Clamp it.
	TolVal = (ILuint)(fabs(Tolerance) * UCHAR_MAX);  // To be changed.
	NumPix = Image->Width * Image->Height * Image->Depth;

	if (Tolerance <= FLT_EPSILON && Tolerance >= 0) {
 			
            //@TODO what is this?
	}
	else {
		switch (Image->Format)
		{
			case IL_RGB:
			case IL_RGBA:
				for (i = 0; i < Image->SizeOfData; i += Image->Bpp) {
					Dist1 = (ILint)Image->Data[i] - (ILint)ClearCol[0];
					Dist2 = (ILint)Image->Data[i+1] - (ILint)ClearCol[1];
					Dist3 = (ILint)Image->Data[i+2] - (ILint)ClearCol[2];
					Distance = (ILint)sqrt((float)(Dist1 * Dist1 + Dist2 * Dist2 + Dist3 * Dist3));
					if (Distance >= -TolVal && Distance <= TolVal) {
						Image->Data[i] = Red;
						Image->Data[i+1] = Green;
						Image->Data[i+2] = Blue;
					}
				}
				break;
			case IL_BGR:
			case IL_BGRA:
				for (i = 0; i < Image->SizeOfData; i += Image->Bpp) {
					Dist1 = (ILint)Image->Data[i] - (ILint)ClearCol[0];
					Dist2 = (ILint)Image->Data[i+1] - (ILint)ClearCol[1];
					Dist3 = (ILint)Image->Data[i+2] - (ILint)ClearCol[2];
					Distance = (ILint)sqrt((float)(Dist1 * Dist1 + Dist2 * Dist2 + Dist3 * Dist3));
					if (Distance >= -TolVal && Distance <= TolVal) {
						Image->Data[i+2] = Red;
						Image->Data[i+1] = Green;
						Image->Data[i] = Blue;
					}
				}
				break;
			case IL_LUMINANCE:
			case IL_LUMINANCE_ALPHA:
				for (i = 0; i < Image->SizeOfData; i += Image->Bpp) {
					Dist1 = (ILint)Image->Data[i] - (ILint)ClearCol[0];
					if (Dist1 >= -TolVal && Dist1 <= TolVal) {
						Image->Data[i] = Blue;
					}
				}
				break;
			//case IL_COLOUR_INDEX:  // @TODO
		}
	}

	return IL_TRUE;
}


// Credit goes to Lionel Brits for this (refer to credits.txt)
ILboolean ILAPIENTRY iluEqualize(ILimage *Image)
{
	ILuint	Histogram[256]; // image Histogram
	ILuint	SumHistm[256]; // normalized Histogram and LUT
	ILuint	i = 0; // index variable
	ILuint	j = 0; // index variable
	ILuint	Sum=0;
	ILuint	NumPixels, Bpp;
	ILint	Intensity;
	ILfloat	Scale;
	ILint	IntensityNew;
	ILimage	*LumImage;
	ILuint	NewColour[4];
	ILubyte		*BytePtr;
	ILushort	*ShortPtr;
	ILuint		*IntPtr;

	NewColour[0] = NewColour[1] = NewColour[2] = NewColour[3] = 0;

	if (Image == NULL) {
		ilSetError(ILU_ILLEGAL_OPERATION);
		return 0;
	}

	// @TODO:  Change to work with other types!
	if (Image->Bpc > 1) {
		ilSetError(ILU_INTERNAL_ERROR);
		return IL_FALSE;
	}

	if (Image->Format == IL_COLOUR_INDEX) {
		NumPixels = Image->Pal.PalSize / ilGetBppPal(Image->Pal.PalType);
		Bpp = ilGetBppPal(Image->Pal.PalType);
	} else {
		NumPixels = Image->Width * Image->Height * Image->Depth;
		Bpp = Image->Bpp;
	}

	// Clear the tables.
	imemclear(Histogram, 256 * sizeof(ILuint));
	imemclear(SumHistm,  256 * sizeof(ILuint));

	LumImage = iConvertImage(Image, IL_LUMINANCE, IL_UNSIGNED_BYTE); // the type must be left as it is!
	if (LumImage == NULL)
		return IL_FALSE;
	for (i = 0; i < NumPixels; i++) {
		Histogram[LumImage->Data[i]]++;
	}

	// Calculate normalized Sum of Histogram.
	for (i = 0; i < 256; i++) {
		for (j = 0; j < i; j++)
			Sum += Histogram[j];

		SumHistm[i] = (Sum << 8) / NumPixels;
		Sum = 0;
	}


	BytePtr = (Image->Format == IL_COLOUR_INDEX) ? Image->Pal.Palette : Image->Data;
	ShortPtr = (ILushort*)Image->Data;
	IntPtr = (ILuint*)Image->Data;

	// Transform image using new SumHistm as a LUT
	for (i = 0; i < NumPixels; i++) {
		Intensity = LumImage->Data[i];

		// Look up the normalized intensity
		IntensityNew = (ILint)SumHistm[Intensity];

		// Find out by how much the intensity has been Scaled
		Scale = (ILfloat)IntensityNew / (ILfloat)Intensity;

		switch (Image->Bpc)
		{
			case 1:
				// Calculate new pixel(s)
				NewColour[0] = (ILuint)(BytePtr[i * Image->Bpp] * Scale);
				if (Bpp >= 3) {
					NewColour[1] = (ILuint)(BytePtr[i * Image->Bpp + 1] * Scale);
					NewColour[2] = (ILuint)(BytePtr[i * Image->Bpp + 2] * Scale);
				}

				// Clamp values
				if (NewColour[0] > UCHAR_MAX)
					NewColour[0] = UCHAR_MAX;
				if (Bpp >= 3) {
					if (NewColour[1] > UCHAR_MAX)
						NewColour[1] = UCHAR_MAX;
					if (NewColour[2] > UCHAR_MAX)
						NewColour[2] = UCHAR_MAX;
				}

				// Store pixel(s)
				BytePtr[i * Image->Bpp] = (ILubyte)NewColour[0];
				if (Bpp >= 3) {
					BytePtr[i * Image->Bpp + 1]	= (ILubyte)NewColour[1];
					BytePtr[i * Image->Bpp + 2]	= (ILubyte)NewColour[2];
				}
				break;

			/*case 2:
				// Calculate new pixel
				NewColour[0] = (ILuint)(ShortPtr[i * Image->Bpp] * Scale);
				NewColour[1] = (ILuint)(ShortPtr[i * Image->Bpp + 1] * Scale);
				NewColour[2] = (ILuint)(ShortPtr[i * Image->Bpp + 2] * Scale);

				// Clamp values
				if (NewColour[0] > USHRT_MAX)
					NewColour[0] = USHRT_MAX;
				if (NewColour[1] > USHRT_MAX)
					NewColour[1] = USHRT_MAX;
				if (NewColour[2] > USHRT_MAX)
					NewColour[2] = USHRT_MAX;

				// Store pixel
				ShortPtr[i * Image->Bpp]		= (ILushort)NewColour[0];
				ShortPtr[i * Image->Bpp + 1]	= (ILushort)NewColour[1];
				ShortPtr[i * Image->Bpp + 2]	= (ILushort)NewColour[2];
				break;

			case 4:
				// Calculate new pixel
				NewColour[0] = (ILuint)(IntPtr[i * Image->Bpp] * Scale);
				NewColour[1] = (ILuint)(IntPtr[i * Image->Bpp + 1] * Scale);
				NewColour[2] = (ILuint)(IntPtr[i * Image->Bpp + 2] * Scale);

				// Clamp values
				if (NewColour[0] > UINT_MAX)
					NewColour[0] = UINT_MAX;
				if (NewColour[1] > UINT_MAX)
					NewColour[1] = UINT_MAX;
				if (NewColour[2] > UINT_MAX)
					NewColour[2] = UINT_MAX;

				// Store pixel
				IntPtr[i * 4 * Image->Bpp]		= NewColour[0];
				IntPtr[i * 4 * Image->Bpp + 1]	= NewColour[1];
				IntPtr[i * 4 * Image->Bpp + 2]	= NewColour[2];
				break;*/
		}
	}

	ilCloseImage(LumImage);

	return IL_TRUE;
}
