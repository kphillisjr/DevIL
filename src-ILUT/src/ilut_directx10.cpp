//-----------------------------------------------------------------------------
//
// ImageLib Utility Toolkit Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 04/11/2009
//
// Filename: src-ILUT/src/ilut_directx10.c
//
// Description: DirectX 10 functions for textures
//
//-----------------------------------------------------------------------------


// Needed to use the C interface to DirectX 10.
#define COBJMACROS

#include "ilut_internal.h"
#ifdef ILUT_USE_DIRECTX10

#include <d3d10.h>

#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")


ILimage*	MakeD3D10Compliant(ILimage *Image, ID3D10Device *Device, DXGI_FORMAT *DestFormat);
ILenum		GetD3D10Compat(ILenum Format);
//D3DFORMAT	GetD3D10Format(ILenum Format);
D3DFORMAT	D3DGetDXTCNumDX10(ILenum DXTCFormat);
ILenum		D3DGetDXTCFormat(D3DFORMAT DXTCNum);
ILboolean	iD3D10CreateMipmaps(IDirect3DTexture9 *Texture, ILimage *Image);
//IDirect3DTexture10* iD3DMakeTexture( IDirect3DDevice9 *Device, void *Data, ILuint DLen, ILuint Width, ILuint Height, D3DFORMAT Format, D3DPOOL Pool, ILuint Levels );


#define ILUT_TEXTUREFORMAT_D3D10_COUNT		7
ILboolean	FormatsDX10Checked = IL_FALSE;
ILboolean	FormatsDX10supported[ILUT_TEXTUREFORMAT_D3D10_COUNT] =
	{ IL_FALSE, IL_FALSE, IL_FALSE, IL_FALSE, IL_FALSE, IL_FALSE, IL_FALSE };
D3DFORMAT	FormatsDX10[ILUT_TEXTUREFORMAT_D3D10_COUNT] =
{ DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_R8G8B8A8_UINT, DXGI_FORMAT_R8_UINT, DXGI_FORMAT_BC1_UNORM_SRGB, DXGI_FORMAT_BC3_UNORM_SRGB, DXGI_FORMAT_BC5_UNORM};


// Nothing to do at the moment
ILboolean ilutD3D10Init()
{

	return IL_TRUE;
}


void CheckFormatsDX10(ID3D10Device *Device)
{
	ILuint	i;
	UINT	FormatSupport;

	for (i = 0; i < ILUT_TEXTUREFORMAT_D3D10_COUNT; i++) {
		ID3D10Device_CheckFormatSupport(Device, FormatsDX10[i], &FormatSupport);
		FormatsDX10supported[i] = (FormatSupport & D3D10_FORMAT_SUPPORT_TEXTURE2D) ? IL_TRUE : IL_FALSE;
	}
	FormatsDX10Checked = IL_TRUE;

	return;
}

#ifndef _WIN32_WCE
ILboolean ILAPIENTRY ilutD3D10TexFromFile(ID3D10Device *Device, ILconst_string FileName, ID3D10Texture2D **Texture)
{
	ILimage *Image = ilGenImage();
	if (!ilLoadImage(Image, FileName)) {
		ilCloseImage(Image);
		return IL_FALSE;
	}

	*Texture = ilutD3D10Texture(Image, Device);

	ilCloseImage(Image);
	return IL_TRUE;
}
#endif//_WIN32_WCE


ILboolean ILAPIENTRY ilutD3D10TexFromFileInMemory(ID3D10Device *Device, void *Lump, ILuint Size, ID3D10Texture2D **Texture)
{
	ILimage *Image = ilGenImage();
	if (!ilLoadL(Image, IL_TYPE_UNKNOWN, Lump, Size)) {
		ilCloseImage(Image);
		return IL_FALSE;
	}

	*Texture = ilutD3D10Texture(Image, Device);

	ilCloseImage(Image);
	return IL_TRUE;
}


ILboolean ILAPIENTRY ilutD3D10TexFromResource(ID3D10Device *Device, HMODULE SrcModule, ILconst_string SrcResource, ID3D10Texture2D **Texture)
{
	HRSRC	Resource;
	ILubyte	*Data;
	ILimage *Image = ilGenImage();

	Resource = (HRSRC)LoadResource(SrcModule, FindResource(SrcModule, SrcResource, RT_BITMAP));
	Data = (ILubyte*)LockResource(Resource);
	if (!ilLoadL(Image, IL_TYPE_UNKNOWN, Data, SizeofResource(SrcModule, FindResource(SrcModule, SrcResource, RT_BITMAP)))) {
		ilCloseImage(Image);
		return IL_FALSE;
	}

	*Texture = ilutD3D10Texture(Image, Device);

	ilCloseImage(Image);
	return IL_TRUE;
}


ILboolean ILAPIENTRY ilutD3D10TexFromFileHandle(ID3D10Device *Device, ILHANDLE File, ID3D10Texture2D **Texture)
{
	ILimage *Image = ilGenImage();
	if (!ilLoadF(Image, IL_TYPE_UNKNOWN, File)) {
		ilCloseImage(Image);
		return IL_FALSE;
	}

	*Texture = ilutD3D10Texture(Image, Device);

	ilCloseImage(Image);
	return IL_TRUE;
}


D3DFORMAT D3DGetDXTCNumDX10(ILenum DXTCFormat)
{
	switch (DXTCFormat)
	{
		case IL_DXT1:
			return D3DFMT_DXT1;
		case IL_DXT3:
			return D3DFMT_DXT3;
		case IL_DXT5:
			return D3DFMT_DXT5;
	}

	return D3DFMT_UNKNOWN;
}


//ILenum D3DGetDXTCFormat(D3DFORMAT DXTCNum)
//{
//	switch (DXTCNum)
//	{
//		case D3DFMT_DXT1:
//			return IL_DXT1;
//		case D3DFMT_DXT3:
//			return IL_DXT3;
//		case D3DFMT_DXT5:
//			return IL_DXT5;
//	}
//
//	return D3DFMT_UNKNOWN;
//}


// Code from http://msdn.microsoft.com/en-us/library/bb205131(VS.85).aspx#Creating_Empty_Textures
ID3D10Texture2D* iD3D10MakeTexture(ID3D10Device *Device, void *Data, ILuint Width, ILuint Height, DXGI_FORMAT Format, ILuint Levels)
{
	ID3D10Texture2D *Texture = NULL;
	D3D10_TEXTURE2D_DESC Desc;
	D3D10_SUBRESOURCE_DATA InitialData;
	HRESULT Result;

	Levels;

	Desc.Width = Width;
	Desc.Height = Height;
	Desc.MipLevels = 1;  //@TODO: Change for mipmaps.
	Desc.ArraySize = 1;  //@TODO: Change for mipmaps.
	Desc.Format = Format;
	Desc.SampleDesc.Count = 1;
	Desc.Usage = D3D10_USAGE_DEFAULT;
	Desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	Desc.MiscFlags = 0;
	Desc.CPUAccessFlags = 0;  // Only need to put the data in initially.

	InitialData.pSysMem = Data;
	InitialData.SysMemPitch = 0;
	InitialData.SysMemSlicePitch = 0;


	Result = ID3D10Device_CreateTexture2D(Device, &Desc, /*&InitialData*/ NULL, &Texture);
	if (FAILED(Result))
		return NULL;

	return Texture;
}

ID3D10Texture2D* ILAPIENTRY ilutD3D10Texture(ILimage *Image, ID3D10Device *Device)
{
	ID3D10Texture2D *Texture;
	DXGI_FORMAT Format;
	ILimage	*CurImage;

	CurImage = Image;
	if (Image == NULL) {
		ilSetError(ILUT_ILLEGAL_OPERATION);
		return NULL;
	}

	if (!FormatsDX10Checked)
		CheckFormatsDX10(Device);

	CurImage = MakeD3D10Compliant(Image, Device, &Format);
	if (CurImage == NULL)
		return NULL;

	Texture = iD3D10MakeTexture(Device, CurImage->Data, CurImage->Width, CurImage->Height, Format, 1);  //@TODO: The 1 should be ilutGetInteger(ILUT_D3D_MIPLEVELS).
	if (!Texture)
		return NULL;
	//iD3D10CreateMipmaps(Texture, Image);

//success:

	if (CurImage != Image)
		ilCloseImage(CurImage);

	return Texture;
}


ILimage *MakeD3D10Compliant(ILimage *Image, ID3D10Device *Device, DXGI_FORMAT *DestFormat)
{
	ILimage	*Converted, *Scaled, *CurImage;
	ILuint nConversionType, ilutFormat;
	ILboolean bForceIntegerFormat = ilutGetBoolean(ILUT_FORCE_INTEGER_FORMAT);

	Device;

	CurImage = Image;
	if (!Image)
		return NULL;
	ilutFormat = Image->Format;
	nConversionType = Image->Type;

	switch (CurImage->Type)
	{
		case IL_UNSIGNED_BYTE:
		case IL_BYTE:
		case IL_SHORT:
		case IL_UNSIGNED_SHORT:
		case IL_INT:
		case IL_UNSIGNED_INT:
			*DestFormat     = DXGI_FORMAT_R8G8B8A8_UINT;
			nConversionType = IL_UNSIGNED_BYTE;
			ilutFormat      = IL_RGBA;
			break;
		case IL_FLOAT:
		case IL_DOUBLE:
		case IL_HALF:
			if (bForceIntegerFormat || (!FormatsDX10supported[6]))
			{
				*DestFormat     = DXGI_FORMAT_R8G8B8A8_UINT;
				nConversionType = IL_UNSIGNED_BYTE;
				ilutFormat      = IL_RGBA;
			}
			else
			{
				*DestFormat     = DXGI_FORMAT_R32G32B32A32_FLOAT;
				nConversionType = IL_HALF;
				ilutFormat      = IL_RGBA;
			}
			break;
	}

	// Images must be in BGRA format.
	if (((CurImage->Format != ilutFormat))
		|| (CurImage->Type != nConversionType)) 
	{
		Converted = iConvertImage(CurImage, ilutFormat, nConversionType);
		if (Converted == NULL)
			return NULL;
	}
	else 
	{
		Converted = CurImage;
	}

	// Images must have their origin in the upper left.
	if (Converted->Origin != IL_ORIGIN_UPPER_LEFT) 
		iluFlipImage(Converted);

	// Images must have powers-of-2 dimensions.
	if (ilNextPower2(CurImage->Width) != CurImage->Width ||
		ilNextPower2(CurImage->Height) != CurImage->Height ||
		ilNextPower2(CurImage->Depth) != CurImage->Depth) {
			Scaled = iluScale_(Converted, ilNextPower2(CurImage->Width),
						ilNextPower2(CurImage->Height), 1);  //@TODO: 1 should be ilNextPower2(CurImage->Depth)
			if (Converted != CurImage) {
				ilCloseImage(Converted);
			}
			if (Scaled == NULL) {
				return NULL;
			}
			Converted = Scaled;
	}

	return Converted;
}


#endif//ILUT_USE_DIRECTX10
