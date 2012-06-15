//-----------------------------------------------------------------------------
//
// ImageLib Utility Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 11/08/2008
//
// Filename: src-ILU/include/ilu_error/ilu_err_english.h
//
// Description: Error functions in English.
//
//-----------------------------------------------------------------------------


#ifndef IL_ERROR_H
#define IL_ERROR_H

#include "il_internal.h"

ILconst_string ilErrors[IL_FILE_READ_ERROR - IL_INVALID_ENUM + 1] = {
	/* TRANSLATORS: Enumerant is a parameter passed to a function */
	gettext_noop("invalid enumerant"),
	gettext_noop("out of memory"),
	gettext_noop("format not supported yet"),
	gettext_noop("internal error"),
	gettext_noop("invalid value"),
	gettext_noop("illegal operation"),
	/* TRANSLATORS: Something like "wrong kind of a supported format" */
	gettext_noop("illegal file value"),
	gettext_noop("invalid file header"),
	gettext_noop("invalid parameter"),
	gettext_noop("could not open file"),
	gettext_noop("invalid extension"),
	gettext_noop("file already exists"),
	gettext_noop("stack overflow"),
	gettext_noop("stack underflow"),
	gettext_noop("invalid conversion"),
	gettext_noop("bad dimensions"),
	gettext_noop("file read error")
};

ILconst_string ilLibErrors[IL_LIB_EXR_ERROR - IL_LIB_GIF_ERROR + 1] = {
	/* TRANSLATORS: Sorry, these repetitions can't be done in a smarter way without any losses */
	gettext_noop("gif library error"),
	gettext_noop("jpeg library error"),
	gettext_noop("png library error"),
	gettext_noop("tiff library error"),
	gettext_noop("mng library error"),
	gettext_noop("jp2 library error"),
	gettext_noop("exr library error")
};

ILconst_string ilMiscErrors[2] = {
	gettext_noop("no error"),
	gettext_noop("unknown error")
};

#endif//IL_ERROR_H
