//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 06/02/2007
//
// Filename: src-IL/src/il_error.c
//
// Description: The error functions
//
//-----------------------------------------------------------------------------


#include "il_error.h"

#define IL_ERROR_STACK_SIZE 32  // Needed elsewhere?

ILenum	ilErrorNum[IL_ERROR_STACK_SIZE];
ILint	ilErrorPlace = (-1);

// Sets the current error
//	If you go past the stack size for this, it cycles the errors, almost like a LRU algo.
ILAPI void ILAPIENTRY ilSetError(ILenum Error)
{
	ILuint i;

	ilErrorPlace++;
	if (ilErrorPlace >= IL_ERROR_STACK_SIZE) {
		for (i = 0; i < IL_ERROR_STACK_SIZE - 2; i++) {
			ilErrorNum[i] = ilErrorNum[i+1];
		}
		ilErrorPlace = IL_ERROR_STACK_SIZE - 1;
	}
	ilErrorNum[ilErrorPlace] = Error;

	return;
}

//! Gets the last error on the error stack
ILenum ILAPIENTRY ilGetError(void)
{
	ILenum ilReturn;

	if (ilErrorPlace >= 0) {
		ilReturn = ilErrorNum[ilErrorPlace];
		ilErrorPlace--;
	}
	else
		ilReturn = IL_NO_ERROR;

	return ilReturn;
}

ILconst_string ILAPIENTRY ilErrorString(ILenum Error)
{
	ILstring to_return = NULL;
	if (Error == IL_NO_ERROR) 
	{
		to_return =  _(ilMiscErrors[0]);
	}
	else if (Error == IL_UNKNOWN_ERROR) 
	{
		to_return = _(ilMiscErrors[1]);
	} 
	else if (Error >= IL_INVALID_ENUM && Error <= IL_FILE_READ_ERROR) 
	{
		to_return = _(ilErrors[Error - IL_INVALID_ENUM]);
	}
	else if (Error >= IL_LIB_GIF_ERROR && Error <= IL_LIB_EXR_ERROR) 
	{
		to_return = _(ilLibErrors[Error - IL_LIB_GIF_ERROR]);
	} 
	else
		to_return = _(ilMiscErrors[0]);
	return to_return;
}

/* The only IL function that begins with ilu :-) */
ILconst_string ILAPIENTRY iluErrorString(ILenum Error)
{ return ilErrorString(Error); }

/* also depreceated */
ILboolean ILAPIENTRY iluSetLanguage(ILenum Language)
{ return IL_TRUE; }

