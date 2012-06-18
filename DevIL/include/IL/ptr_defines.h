#ifndef PTR_DEFINES_H
#define PTR_DEFINES_H

#include "IL/devil_internal_exports.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Now examining format BMP */
#ifndef IL_NO_BMP /* we support that format */
ILboolean ilLoad_BMP(ILconst_string);
#define ilLoad_BMP_PTR & ilLoad_BMP

ILboolean ilLoadF_BMP(ILHANDLE);
#define ilLoadF_BMP_PTR & ilLoadF_BMP

ILboolean ilLoadL_BMP(const void *, ILuint);
#define ilLoadL_BMP_PTR & ilLoadL_BMP

ILboolean ilSave_BMP(ILconst_string);
#define ilSave_BMP_PTR & ilSave_BMP

ILuint ilSaveF_BMP(ILHANDLE);
#define ilSaveF_BMP_PTR & ilSaveF_BMP

ILuint ilSaveL_BMP(void *, ILuint);
#define ilSaveL_BMP_PTR & ilSaveL_BMP

ILboolean ilIsValid_BMP(ILconst_string);
#define ilIsValid_BMP_PTR & ilIsValid_BMP

ILboolean ilIsValidF_BMP(ILHANDLE);
#define ilIsValidF_BMP_PTR & ilIsValidF_BMP

ILboolean ilIsValidL_BMP(const void *, ILuint);
#define ilIsValidL_BMP_PTR & ilIsValidL_BMP

#else /* we don't support format BMP */
#define ilLoad_BMP_PTR NULL
#define ilLoadF_BMP_PTR NULL
#define ilLoadL_BMP_PTR NULL
#define ilSave_BMP_PTR NULL
#define ilSaveF_BMP_PTR NULL
#define ilSaveL_BMP_PTR NULL
#define ilIsValid_BMP_PTR NULL
#define ilIsValidF_BMP_PTR NULL
#define ilIsValidL_BMP_PTR NULL
#endif /* IL_NO_BMP */

/* Finished examining BMP */

/* Now examining format CUT */
#ifndef IL_NO_CUT /* we support that format */
ILboolean ilLoad_CUT(ILconst_string);
#define ilLoad_CUT_PTR & ilLoad_CUT

ILboolean ilLoadF_CUT(ILHANDLE);
#define ilLoadF_CUT_PTR & ilLoadF_CUT

ILboolean ilLoadL_CUT(const void *, ILuint);
#define ilLoadL_CUT_PTR & ilLoadL_CUT

#else /* we don't support format CUT */
#define ilLoad_CUT_PTR NULL
#define ilLoadF_CUT_PTR NULL
#define ilLoadL_CUT_PTR NULL
#endif /* IL_NO_CUT */

/* These are always NULL */
#define ilSave_CUT_PTR NULL
#define ilSaveF_CUT_PTR NULL
#define ilSaveL_CUT_PTR NULL
#define ilIsValid_CUT_PTR NULL
#define ilIsValidF_CUT_PTR NULL
#define ilIsValidL_CUT_PTR NULL

/* Finished examining CUT */

/* Now examining format CHEAD */
#ifndef IL_NO_CHEAD /* we support that format */
ILboolean ilSave_CHEAD(ILconst_string);
#define ilSave_CHEAD_PTR & ilSave_CHEAD

#else /* we don't support format CHEAD */
#define ilSave_CHEAD_PTR NULL
#endif /* IL_NO_CHEAD */

/* These are always NULL */
#define ilLoad_CHEAD_PTR NULL
#define ilLoadF_CHEAD_PTR NULL
#define ilLoadL_CHEAD_PTR NULL
#define ilSaveF_CHEAD_PTR NULL
#define ilSaveL_CHEAD_PTR NULL
#define ilIsValid_CHEAD_PTR NULL
#define ilIsValidF_CHEAD_PTR NULL
#define ilIsValidL_CHEAD_PTR NULL

/* Finished examining CHEAD */

/* Now examining format DCX */
#ifndef IL_NO_DCX /* we support that format */
ILboolean ilLoad_DCX(ILconst_string);
#define ilLoad_DCX_PTR & ilLoad_DCX

ILboolean ilLoadF_DCX(ILHANDLE);
#define ilLoadF_DCX_PTR & ilLoadF_DCX

ILboolean ilLoadL_DCX(const void *, ILuint);
#define ilLoadL_DCX_PTR & ilLoadL_DCX

ILboolean ilIsValid_DCX(ILconst_string);
#define ilIsValid_DCX_PTR & ilIsValid_DCX

ILboolean ilIsValidF_DCX(ILHANDLE);
#define ilIsValidF_DCX_PTR & ilIsValidF_DCX

ILboolean ilIsValidL_DCX(const void *, ILuint);
#define ilIsValidL_DCX_PTR & ilIsValidL_DCX

#else /* we don't support format DCX */
#define ilLoad_DCX_PTR NULL
#define ilLoadF_DCX_PTR NULL
#define ilLoadL_DCX_PTR NULL
#define ilIsValid_DCX_PTR NULL
#define ilIsValidF_DCX_PTR NULL
#define ilIsValidL_DCX_PTR NULL
#endif /* IL_NO_DCX */

/* These are always NULL */
#define ilSave_DCX_PTR NULL
#define ilSaveF_DCX_PTR NULL
#define ilSaveL_DCX_PTR NULL

/* Finished examining DCX */

/* Now examining format DDS */
#ifndef IL_NO_DDS /* we support that format */
ILboolean ilLoad_DDS(ILconst_string);
#define ilLoad_DDS_PTR & ilLoad_DDS

ILboolean ilLoadF_DDS(ILHANDLE);
#define ilLoadF_DDS_PTR & ilLoadF_DDS

ILboolean ilLoadL_DDS(const void *, ILuint);
#define ilLoadL_DDS_PTR & ilLoadL_DDS

ILboolean ilSave_DDS(ILconst_string);
#define ilSave_DDS_PTR & ilSave_DDS

ILuint ilSaveF_DDS(ILHANDLE);
#define ilSaveF_DDS_PTR & ilSaveF_DDS

ILuint ilSaveL_DDS(void *, ILuint);
#define ilSaveL_DDS_PTR & ilSaveL_DDS

ILboolean ilIsValid_DDS(ILconst_string);
#define ilIsValid_DDS_PTR & ilIsValid_DDS

ILboolean ilIsValidF_DDS(ILHANDLE);
#define ilIsValidF_DDS_PTR & ilIsValidF_DDS

ILboolean ilIsValidL_DDS(const void *, ILuint);
#define ilIsValidL_DDS_PTR & ilIsValidL_DDS

#else /* we don't support format DDS */
#define ilLoad_DDS_PTR NULL
#define ilLoadF_DDS_PTR NULL
#define ilLoadL_DDS_PTR NULL
#define ilSave_DDS_PTR NULL
#define ilSaveF_DDS_PTR NULL
#define ilSaveL_DDS_PTR NULL
#define ilIsValid_DDS_PTR NULL
#define ilIsValidF_DDS_PTR NULL
#define ilIsValidL_DDS_PTR NULL
#endif /* IL_NO_DDS */

/* Finished examining DDS */

/* Now examining format DOOM */
#ifndef IL_NO_DOOM /* we support that format */
ILboolean ilLoad_DOOM(ILconst_string);
#define ilLoad_DOOM_PTR & ilLoad_DOOM

ILboolean ilLoadF_DOOM(ILHANDLE);
#define ilLoadF_DOOM_PTR & ilLoadF_DOOM

ILboolean ilLoadL_DOOM(const void *, ILuint);
#define ilLoadL_DOOM_PTR & ilLoadL_DOOM

#else /* we don't support format DOOM */
#define ilLoad_DOOM_PTR NULL
#define ilLoadF_DOOM_PTR NULL
#define ilLoadL_DOOM_PTR NULL
#endif /* IL_NO_DOOM */

/* These are always NULL */
#define ilSave_DOOM_PTR NULL
#define ilSaveF_DOOM_PTR NULL
#define ilSaveL_DOOM_PTR NULL
#define ilIsValid_DOOM_PTR NULL
#define ilIsValidF_DOOM_PTR NULL
#define ilIsValidL_DOOM_PTR NULL

/* Finished examining DOOM */

/* Now examining format EXR */
#ifndef IL_NO_EXR /* we support that format */
ILboolean ilLoad_EXR(ILconst_string);
#define ilLoad_EXR_PTR & ilLoad_EXR

ILboolean ilLoadF_EXR(ILHANDLE);
#define ilLoadF_EXR_PTR & ilLoadF_EXR

ILboolean ilLoadL_EXR(const void *, ILuint);
#define ilLoadL_EXR_PTR & ilLoadL_EXR

ILboolean ilSave_EXR(ILconst_string);
#define ilSave_EXR_PTR & ilSave_EXR

ILuint ilSaveF_EXR(ILHANDLE);
#define ilSaveF_EXR_PTR & ilSaveF_EXR

ILuint ilSaveL_EXR(void *, ILuint);
#define ilSaveL_EXR_PTR & ilSaveL_EXR

ILboolean ilIsValid_EXR(ILconst_string);
#define ilIsValid_EXR_PTR & ilIsValid_EXR

ILboolean ilIsValidF_EXR(ILHANDLE);
#define ilIsValidF_EXR_PTR & ilIsValidF_EXR

ILboolean ilIsValidL_EXR(const void *, ILuint);
#define ilIsValidL_EXR_PTR & ilIsValidL_EXR

#else /* we don't support format EXR */
#define ilLoad_EXR_PTR NULL
#define ilLoadF_EXR_PTR NULL
#define ilLoadL_EXR_PTR NULL
#define ilSave_EXR_PTR NULL
#define ilSaveF_EXR_PTR NULL
#define ilSaveL_EXR_PTR NULL
#define ilIsValid_EXR_PTR NULL
#define ilIsValidF_EXR_PTR NULL
#define ilIsValidL_EXR_PTR NULL
#endif /* IL_NO_EXR */

/* Finished examining EXR */

/* Now examining format FITS */
#ifndef IL_NO_FITS /* we support that format */
ILboolean ilLoad_FITS(ILconst_string);
#define ilLoad_FITS_PTR & ilLoad_FITS

ILboolean ilLoadF_FITS(ILHANDLE);
#define ilLoadF_FITS_PTR & ilLoadF_FITS

ILboolean ilLoadL_FITS(const void *, ILuint);
#define ilLoadL_FITS_PTR & ilLoadL_FITS

ILboolean ilIsValid_FITS(ILconst_string);
#define ilIsValid_FITS_PTR & ilIsValid_FITS

ILboolean ilIsValidF_FITS(ILHANDLE);
#define ilIsValidF_FITS_PTR & ilIsValidF_FITS

ILboolean ilIsValidL_FITS(const void *, ILuint);
#define ilIsValidL_FITS_PTR & ilIsValidL_FITS

#else /* we don't support format FITS */
#define ilLoad_FITS_PTR NULL
#define ilLoadF_FITS_PTR NULL
#define ilLoadL_FITS_PTR NULL
#define ilIsValid_FITS_PTR NULL
#define ilIsValidF_FITS_PTR NULL
#define ilIsValidL_FITS_PTR NULL
#endif /* IL_NO_FITS */

/* These are always NULL */
#define ilSave_FITS_PTR NULL
#define ilSaveF_FITS_PTR NULL
#define ilSaveL_FITS_PTR NULL

/* Finished examining FITS */

/* Now examining format GIF */
#ifndef IL_NO_GIF /* we support that format */
ILboolean ilLoad_GIF(ILconst_string);
#define ilLoad_GIF_PTR & ilLoad_GIF

ILboolean ilLoadF_GIF(ILHANDLE);
#define ilLoadF_GIF_PTR & ilLoadF_GIF

ILboolean ilLoadL_GIF(const void *, ILuint);
#define ilLoadL_GIF_PTR & ilLoadL_GIF

ILboolean ilIsValid_GIF(ILconst_string);
#define ilIsValid_GIF_PTR & ilIsValid_GIF

ILboolean ilIsValidF_GIF(ILHANDLE);
#define ilIsValidF_GIF_PTR & ilIsValidF_GIF

ILboolean ilIsValidL_GIF(const void *, ILuint);
#define ilIsValidL_GIF_PTR & ilIsValidL_GIF

#else /* we don't support format GIF */
#define ilLoad_GIF_PTR NULL
#define ilLoadF_GIF_PTR NULL
#define ilLoadL_GIF_PTR NULL
#define ilIsValid_GIF_PTR NULL
#define ilIsValidF_GIF_PTR NULL
#define ilIsValidL_GIF_PTR NULL
#endif /* IL_NO_GIF */

/* These are always NULL */
#define ilSave_GIF_PTR NULL
#define ilSaveF_GIF_PTR NULL
#define ilSaveL_GIF_PTR NULL

/* Finished examining GIF */

/* Now examining format HDR */
#ifndef IL_NO_HDR /* we support that format */
ILboolean ilLoad_HDR(ILconst_string);
#define ilLoad_HDR_PTR & ilLoad_HDR

ILboolean ilLoadF_HDR(ILHANDLE);
#define ilLoadF_HDR_PTR & ilLoadF_HDR

ILboolean ilLoadL_HDR(const void *, ILuint);
#define ilLoadL_HDR_PTR & ilLoadL_HDR

ILboolean ilSave_HDR(ILconst_string);
#define ilSave_HDR_PTR & ilSave_HDR

ILuint ilSaveF_HDR(ILHANDLE);
#define ilSaveF_HDR_PTR & ilSaveF_HDR

ILuint ilSaveL_HDR(void *, ILuint);
#define ilSaveL_HDR_PTR & ilSaveL_HDR

ILboolean ilIsValid_HDR(ILconst_string);
#define ilIsValid_HDR_PTR & ilIsValid_HDR

ILboolean ilIsValidF_HDR(ILHANDLE);
#define ilIsValidF_HDR_PTR & ilIsValidF_HDR

ILboolean ilIsValidL_HDR(const void *, ILuint);
#define ilIsValidL_HDR_PTR & ilIsValidL_HDR

#else /* we don't support format HDR */
#define ilLoad_HDR_PTR NULL
#define ilLoadF_HDR_PTR NULL
#define ilLoadL_HDR_PTR NULL
#define ilSave_HDR_PTR NULL
#define ilSaveF_HDR_PTR NULL
#define ilSaveL_HDR_PTR NULL
#define ilIsValid_HDR_PTR NULL
#define ilIsValidF_HDR_PTR NULL
#define ilIsValidL_HDR_PTR NULL
#endif /* IL_NO_HDR */

/* Finished examining HDR */

/* Now examining format ICNS */
#ifndef IL_NO_ICNS /* we support that format */
ILboolean ilLoad_ICNS(ILconst_string);
#define ilLoad_ICNS_PTR & ilLoad_ICNS

ILboolean ilLoadF_ICNS(ILHANDLE);
#define ilLoadF_ICNS_PTR & ilLoadF_ICNS

ILboolean ilLoadL_ICNS(const void *, ILuint);
#define ilLoadL_ICNS_PTR & ilLoadL_ICNS

ILboolean ilIsValid_ICNS(ILconst_string);
#define ilIsValid_ICNS_PTR & ilIsValid_ICNS

ILboolean ilIsValidF_ICNS(ILHANDLE);
#define ilIsValidF_ICNS_PTR & ilIsValidF_ICNS

ILboolean ilIsValidL_ICNS(const void *, ILuint);
#define ilIsValidL_ICNS_PTR & ilIsValidL_ICNS

#else /* we don't support format ICNS */
#define ilLoad_ICNS_PTR NULL
#define ilLoadF_ICNS_PTR NULL
#define ilLoadL_ICNS_PTR NULL
#define ilIsValid_ICNS_PTR NULL
#define ilIsValidF_ICNS_PTR NULL
#define ilIsValidL_ICNS_PTR NULL
#endif /* IL_NO_ICNS */

/* These are always NULL */
#define ilSave_ICNS_PTR NULL
#define ilSaveF_ICNS_PTR NULL
#define ilSaveL_ICNS_PTR NULL

/* Finished examining ICNS */

/* Now examining format ICON */
#ifndef IL_NO_ICON /* we support that format */
ILboolean ilLoad_ICON(ILconst_string);
#define ilLoad_ICON_PTR & ilLoad_ICON

ILboolean ilLoadF_ICON(ILHANDLE);
#define ilLoadF_ICON_PTR & ilLoadF_ICON

ILboolean ilLoadL_ICON(const void *, ILuint);
#define ilLoadL_ICON_PTR & ilLoadL_ICON

#else /* we don't support format ICON */
#define ilLoad_ICON_PTR NULL
#define ilLoadF_ICON_PTR NULL
#define ilLoadL_ICON_PTR NULL
#endif /* IL_NO_ICON */

/* These are always NULL */
#define ilSave_ICON_PTR NULL
#define ilSaveF_ICON_PTR NULL
#define ilSaveL_ICON_PTR NULL
#define ilIsValid_ICON_PTR NULL
#define ilIsValidF_ICON_PTR NULL
#define ilIsValidL_ICON_PTR NULL

/* Finished examining ICON */

/* Now examining format IFF */
#ifndef IL_NO_IFF /* we support that format */
ILboolean ilLoad_IFF(ILconst_string);
#define ilLoad_IFF_PTR & ilLoad_IFF

ILboolean ilLoadF_IFF(ILHANDLE);
#define ilLoadF_IFF_PTR & ilLoadF_IFF

ILboolean ilLoadL_IFF(const void *, ILuint);
#define ilLoadL_IFF_PTR & ilLoadL_IFF

#else /* we don't support format IFF */
#define ilLoad_IFF_PTR NULL
#define ilLoadF_IFF_PTR NULL
#define ilLoadL_IFF_PTR NULL
#endif /* IL_NO_IFF */

/* These are always NULL */
#define ilSave_IFF_PTR NULL
#define ilSaveF_IFF_PTR NULL
#define ilSaveL_IFF_PTR NULL
#define ilIsValid_IFF_PTR NULL
#define ilIsValidF_IFF_PTR NULL
#define ilIsValidL_IFF_PTR NULL

/* Finished examining IFF */

/* Now examining format ILBM */
#ifndef IL_NO_ILBM /* we support that format */
ILboolean ilLoad_ILBM(ILconst_string);
#define ilLoad_ILBM_PTR & ilLoad_ILBM

ILboolean ilLoadF_ILBM(ILHANDLE);
#define ilLoadF_ILBM_PTR & ilLoadF_ILBM

ILboolean ilLoadL_ILBM(const void *, ILuint);
#define ilLoadL_ILBM_PTR & ilLoadL_ILBM

ILboolean ilIsValid_ILBM(ILconst_string);
#define ilIsValid_ILBM_PTR & ilIsValid_ILBM

ILboolean ilIsValidF_ILBM(ILHANDLE);
#define ilIsValidF_ILBM_PTR & ilIsValidF_ILBM

ILboolean ilIsValidL_ILBM(const void *, ILuint);
#define ilIsValidL_ILBM_PTR & ilIsValidL_ILBM

#else /* we don't support format ILBM */
#define ilLoad_ILBM_PTR NULL
#define ilLoadF_ILBM_PTR NULL
#define ilLoadL_ILBM_PTR NULL
#define ilIsValid_ILBM_PTR NULL
#define ilIsValidF_ILBM_PTR NULL
#define ilIsValidL_ILBM_PTR NULL
#endif /* IL_NO_ILBM */

/* These are always NULL */
#define ilSave_ILBM_PTR NULL
#define ilSaveF_ILBM_PTR NULL
#define ilSaveL_ILBM_PTR NULL

/* Finished examining ILBM */

/* Now examining format JPEG */
#ifndef IL_NO_JPEG /* we support that format */
ILboolean ilLoad_JPEG(ILconst_string);
#define ilLoad_JPEG_PTR & ilLoad_JPEG

ILboolean ilLoadF_JPEG(ILHANDLE);
#define ilLoadF_JPEG_PTR & ilLoadF_JPEG

ILboolean ilLoadL_JPEG(const void *, ILuint);
#define ilLoadL_JPEG_PTR & ilLoadL_JPEG

ILboolean ilSave_JPEG(ILconst_string);
#define ilSave_JPEG_PTR & ilSave_JPEG

ILuint ilSaveF_JPEG(ILHANDLE);
#define ilSaveF_JPEG_PTR & ilSaveF_JPEG

ILuint ilSaveL_JPEG(void *, ILuint);
#define ilSaveL_JPEG_PTR & ilSaveL_JPEG

ILboolean ilIsValid_JPEG(ILconst_string);
#define ilIsValid_JPEG_PTR & ilIsValid_JPEG

ILboolean ilIsValidF_JPEG(ILHANDLE);
#define ilIsValidF_JPEG_PTR & ilIsValidF_JPEG

ILboolean ilIsValidL_JPEG(const void *, ILuint);
#define ilIsValidL_JPEG_PTR & ilIsValidL_JPEG

#else /* we don't support format JPEG */
#define ilLoad_JPEG_PTR NULL
#define ilLoadF_JPEG_PTR NULL
#define ilLoadL_JPEG_PTR NULL
#define ilSave_JPEG_PTR NULL
#define ilSaveF_JPEG_PTR NULL
#define ilSaveL_JPEG_PTR NULL
#define ilIsValid_JPEG_PTR NULL
#define ilIsValidF_JPEG_PTR NULL
#define ilIsValidL_JPEG_PTR NULL
#endif /* IL_NO_JPEG */

/* Finished examining JPEG */

/* Now examining format JP2 */
#ifndef IL_NO_JP2 /* we support that format */
ILboolean ilLoad_JP2(ILconst_string);
#define ilLoad_JP2_PTR & ilLoad_JP2

ILboolean ilLoadF_JP2(ILHANDLE);
#define ilLoadF_JP2_PTR & ilLoadF_JP2

ILboolean ilLoadL_JP2(const void *, ILuint);
#define ilLoadL_JP2_PTR & ilLoadL_JP2

ILboolean ilSave_JP2(ILconst_string);
#define ilSave_JP2_PTR & ilSave_JP2

ILuint ilSaveF_JP2(ILHANDLE);
#define ilSaveF_JP2_PTR & ilSaveF_JP2

ILuint ilSaveL_JP2(void *, ILuint);
#define ilSaveL_JP2_PTR & ilSaveL_JP2

ILboolean ilIsValid_JP2(ILconst_string);
#define ilIsValid_JP2_PTR & ilIsValid_JP2

ILboolean ilIsValidF_JP2(ILHANDLE);
#define ilIsValidF_JP2_PTR & ilIsValidF_JP2

ILboolean ilIsValidL_JP2(const void *, ILuint);
#define ilIsValidL_JP2_PTR & ilIsValidL_JP2

#else /* we don't support format JP2 */
#define ilLoad_JP2_PTR NULL
#define ilLoadF_JP2_PTR NULL
#define ilLoadL_JP2_PTR NULL
#define ilSave_JP2_PTR NULL
#define ilSaveF_JP2_PTR NULL
#define ilSaveL_JP2_PTR NULL
#define ilIsValid_JP2_PTR NULL
#define ilIsValidF_JP2_PTR NULL
#define ilIsValidL_JP2_PTR NULL
#endif /* IL_NO_JP2 */

/* Finished examining JP2 */

/* Now examining format LIF */
#ifndef IL_NO_LIF /* we support that format */
ILboolean ilLoad_LIF(ILconst_string);
#define ilLoad_LIF_PTR & ilLoad_LIF

ILboolean ilLoadF_LIF(ILHANDLE);
#define ilLoadF_LIF_PTR & ilLoadF_LIF

ILboolean ilLoadL_LIF(const void *, ILuint);
#define ilLoadL_LIF_PTR & ilLoadL_LIF

ILboolean ilIsValid_LIF(ILconst_string);
#define ilIsValid_LIF_PTR & ilIsValid_LIF

ILboolean ilIsValidF_LIF(ILHANDLE);
#define ilIsValidF_LIF_PTR & ilIsValidF_LIF

ILboolean ilIsValidL_LIF(const void *, ILuint);
#define ilIsValidL_LIF_PTR & ilIsValidL_LIF

#else /* we don't support format LIF */
#define ilLoad_LIF_PTR NULL
#define ilLoadF_LIF_PTR NULL
#define ilLoadL_LIF_PTR NULL
#define ilIsValid_LIF_PTR NULL
#define ilIsValidF_LIF_PTR NULL
#define ilIsValidL_LIF_PTR NULL
#endif /* IL_NO_LIF */

/* These are always NULL */
#define ilSave_LIF_PTR NULL
#define ilSaveF_LIF_PTR NULL
#define ilSaveL_LIF_PTR NULL

/* Finished examining LIF */

/* Now examining format MDL */
#ifndef IL_NO_MDL /* we support that format */
ILboolean ilLoad_MDL(ILconst_string);
#define ilLoad_MDL_PTR & ilLoad_MDL

ILboolean ilLoadF_MDL(ILHANDLE);
#define ilLoadF_MDL_PTR & ilLoadF_MDL

ILboolean ilLoadL_MDL(const void *, ILuint);
#define ilLoadL_MDL_PTR & ilLoadL_MDL

ILboolean ilIsValid_MDL(ILconst_string);
#define ilIsValid_MDL_PTR & ilIsValid_MDL

ILboolean ilIsValidF_MDL(ILHANDLE);
#define ilIsValidF_MDL_PTR & ilIsValidF_MDL

ILboolean ilIsValidL_MDL(const void *, ILuint);
#define ilIsValidL_MDL_PTR & ilIsValidL_MDL

#else /* we don't support format MDL */
#define ilLoad_MDL_PTR NULL
#define ilLoadF_MDL_PTR NULL
#define ilLoadL_MDL_PTR NULL
#define ilIsValid_MDL_PTR NULL
#define ilIsValidF_MDL_PTR NULL
#define ilIsValidL_MDL_PTR NULL
#endif /* IL_NO_MDL */

/* These are always NULL */
#define ilSave_MDL_PTR NULL
#define ilSaveF_MDL_PTR NULL
#define ilSaveL_MDL_PTR NULL

/* Finished examining MDL */

/* Now examining format MNG */
#ifndef IL_NO_MNG /* we support that format */
ILboolean ilLoad_MNG(ILconst_string);
#define ilLoad_MNG_PTR & ilLoad_MNG

ILboolean ilLoadF_MNG(ILHANDLE);
#define ilLoadF_MNG_PTR & ilLoadF_MNG

ILboolean ilLoadL_MNG(const void *, ILuint);
#define ilLoadL_MNG_PTR & ilLoadL_MNG

ILboolean ilSave_MNG(ILconst_string);
#define ilSave_MNG_PTR & ilSave_MNG

ILuint ilSaveF_MNG(ILHANDLE);
#define ilSaveF_MNG_PTR & ilSaveF_MNG

ILuint ilSaveL_MNG(void *, ILuint);
#define ilSaveL_MNG_PTR & ilSaveL_MNG

#else /* we don't support format MNG */
#define ilLoad_MNG_PTR NULL
#define ilLoadF_MNG_PTR NULL
#define ilLoadL_MNG_PTR NULL
#define ilSave_MNG_PTR NULL
#define ilSaveF_MNG_PTR NULL
#define ilSaveL_MNG_PTR NULL
#endif /* IL_NO_MNG */

/* These are always NULL */
#define ilIsValid_MNG_PTR NULL
#define ilIsValidF_MNG_PTR NULL
#define ilIsValidL_MNG_PTR NULL

/* Finished examining MNG */

/* Now examining format PCX */
#ifndef IL_NO_PCX /* we support that format */
ILboolean ilLoad_PCX(ILconst_string);
#define ilLoad_PCX_PTR & ilLoad_PCX

ILboolean ilLoadF_PCX(ILHANDLE);
#define ilLoadF_PCX_PTR & ilLoadF_PCX

ILboolean ilLoadL_PCX(const void *, ILuint);
#define ilLoadL_PCX_PTR & ilLoadL_PCX

ILboolean ilSave_PCX(ILconst_string);
#define ilSave_PCX_PTR & ilSave_PCX

ILuint ilSaveF_PCX(ILHANDLE);
#define ilSaveF_PCX_PTR & ilSaveF_PCX

ILuint ilSaveL_PCX(void *, ILuint);
#define ilSaveL_PCX_PTR & ilSaveL_PCX

ILboolean ilIsValid_PCX(ILconst_string);
#define ilIsValid_PCX_PTR & ilIsValid_PCX

ILboolean ilIsValidF_PCX(ILHANDLE);
#define ilIsValidF_PCX_PTR & ilIsValidF_PCX

ILboolean ilIsValidL_PCX(const void *, ILuint);
#define ilIsValidL_PCX_PTR & ilIsValidL_PCX

#else /* we don't support format PCX */
#define ilLoad_PCX_PTR NULL
#define ilLoadF_PCX_PTR NULL
#define ilLoadL_PCX_PTR NULL
#define ilSave_PCX_PTR NULL
#define ilSaveF_PCX_PTR NULL
#define ilSaveL_PCX_PTR NULL
#define ilIsValid_PCX_PTR NULL
#define ilIsValidF_PCX_PTR NULL
#define ilIsValidL_PCX_PTR NULL
#endif /* IL_NO_PCX */

/* Finished examining PCX */

/* Now examining format PCD */
#ifndef IL_NO_PCD /* we support that format */
ILboolean ilLoad_PCD(ILconst_string);
#define ilLoad_PCD_PTR & ilLoad_PCD

ILboolean ilLoadF_PCD(ILHANDLE);
#define ilLoadF_PCD_PTR & ilLoadF_PCD

ILboolean ilLoadL_PCD(const void *, ILuint);
#define ilLoadL_PCD_PTR & ilLoadL_PCD

#else /* we don't support format PCD */
#define ilLoad_PCD_PTR NULL
#define ilLoadF_PCD_PTR NULL
#define ilLoadL_PCD_PTR NULL
#endif /* IL_NO_PCD */

/* These are always NULL */
#define ilSave_PCD_PTR NULL
#define ilSaveF_PCD_PTR NULL
#define ilSaveL_PCD_PTR NULL
#define ilIsValid_PCD_PTR NULL
#define ilIsValidF_PCD_PTR NULL
#define ilIsValidL_PCD_PTR NULL

/* Finished examining PCD */

/* Now examining format PIC */
#ifndef IL_NO_PIC /* we support that format */
ILboolean ilLoad_PIC(ILconst_string);
#define ilLoad_PIC_PTR & ilLoad_PIC

ILboolean ilLoadF_PIC(ILHANDLE);
#define ilLoadF_PIC_PTR & ilLoadF_PIC

ILboolean ilLoadL_PIC(const void *, ILuint);
#define ilLoadL_PIC_PTR & ilLoadL_PIC

ILboolean ilIsValid_PIC(ILconst_string);
#define ilIsValid_PIC_PTR & ilIsValid_PIC

ILboolean ilIsValidF_PIC(ILHANDLE);
#define ilIsValidF_PIC_PTR & ilIsValidF_PIC

ILboolean ilIsValidL_PIC(const void *, ILuint);
#define ilIsValidL_PIC_PTR & ilIsValidL_PIC

#else /* we don't support format PIC */
#define ilLoad_PIC_PTR NULL
#define ilLoadF_PIC_PTR NULL
#define ilLoadL_PIC_PTR NULL
#define ilIsValid_PIC_PTR NULL
#define ilIsValidF_PIC_PTR NULL
#define ilIsValidL_PIC_PTR NULL
#endif /* IL_NO_PIC */

/* These are always NULL */
#define ilSave_PIC_PTR NULL
#define ilSaveF_PIC_PTR NULL
#define ilSaveL_PIC_PTR NULL

/* Finished examining PIC */

/* Now examining format PIX */
#ifndef IL_NO_PIX /* we support that format */
ILboolean ilLoad_PIX(ILconst_string);
#define ilLoad_PIX_PTR & ilLoad_PIX

ILboolean ilLoadF_PIX(ILHANDLE);
#define ilLoadF_PIX_PTR & ilLoadF_PIX

ILboolean ilLoadL_PIX(const void *, ILuint);
#define ilLoadL_PIX_PTR & ilLoadL_PIX

#else /* we don't support format PIX */
#define ilLoad_PIX_PTR NULL
#define ilLoadF_PIX_PTR NULL
#define ilLoadL_PIX_PTR NULL
#endif /* IL_NO_PIX */

/* These are always NULL */
#define ilSave_PIX_PTR NULL
#define ilSaveF_PIX_PTR NULL
#define ilSaveL_PIX_PTR NULL
#define ilIsValid_PIX_PTR NULL
#define ilIsValidF_PIX_PTR NULL
#define ilIsValidL_PIX_PTR NULL

/* Finished examining PIX */

/* Now examining format PNG */
#ifndef IL_NO_PNG /* we support that format */
ILboolean ilLoad_PNG(ILconst_string);
#define ilLoad_PNG_PTR & ilLoad_PNG

ILboolean ilLoadF_PNG(ILHANDLE);
#define ilLoadF_PNG_PTR & ilLoadF_PNG

ILboolean ilLoadL_PNG(const void *, ILuint);
#define ilLoadL_PNG_PTR & ilLoadL_PNG

ILboolean ilSave_PNG(ILconst_string);
#define ilSave_PNG_PTR & ilSave_PNG

ILuint ilSaveF_PNG(ILHANDLE);
#define ilSaveF_PNG_PTR & ilSaveF_PNG

ILuint ilSaveL_PNG(void *, ILuint);
#define ilSaveL_PNG_PTR & ilSaveL_PNG

ILboolean ilIsValid_PNG(ILconst_string);
#define ilIsValid_PNG_PTR & ilIsValid_PNG

ILboolean ilIsValidF_PNG(ILHANDLE);
#define ilIsValidF_PNG_PTR & ilIsValidF_PNG

ILboolean ilIsValidL_PNG(const void *, ILuint);
#define ilIsValidL_PNG_PTR & ilIsValidL_PNG

#else /* we don't support format PNG */
#define ilLoad_PNG_PTR NULL
#define ilLoadF_PNG_PTR NULL
#define ilLoadL_PNG_PTR NULL
#define ilSave_PNG_PTR NULL
#define ilSaveF_PNG_PTR NULL
#define ilSaveL_PNG_PTR NULL
#define ilIsValid_PNG_PTR NULL
#define ilIsValidF_PNG_PTR NULL
#define ilIsValidL_PNG_PTR NULL
#endif /* IL_NO_PNG */

/* Finished examining PNG */

/* Now examining format PNM */
#ifndef IL_NO_PNM /* we support that format */
ILboolean ilLoad_PNM(ILconst_string);
#define ilLoad_PNM_PTR & ilLoad_PNM

ILboolean ilLoadF_PNM(ILHANDLE);
#define ilLoadF_PNM_PTR & ilLoadF_PNM

ILboolean ilLoadL_PNM(const void *, ILuint);
#define ilLoadL_PNM_PTR & ilLoadL_PNM

ILboolean ilSave_PNM(ILconst_string);
#define ilSave_PNM_PTR & ilSave_PNM

ILuint ilSaveF_PNM(ILHANDLE);
#define ilSaveF_PNM_PTR & ilSaveF_PNM

ILuint ilSaveL_PNM(void *, ILuint);
#define ilSaveL_PNM_PTR & ilSaveL_PNM

ILboolean ilIsValid_PNM(ILconst_string);
#define ilIsValid_PNM_PTR & ilIsValid_PNM

ILboolean ilIsValidF_PNM(ILHANDLE);
#define ilIsValidF_PNM_PTR & ilIsValidF_PNM

ILboolean ilIsValidL_PNM(const void *, ILuint);
#define ilIsValidL_PNM_PTR & ilIsValidL_PNM

#else /* we don't support format PNM */
#define ilLoad_PNM_PTR NULL
#define ilLoadF_PNM_PTR NULL
#define ilLoadL_PNM_PTR NULL
#define ilSave_PNM_PTR NULL
#define ilSaveF_PNM_PTR NULL
#define ilSaveL_PNM_PTR NULL
#define ilIsValid_PNM_PTR NULL
#define ilIsValidF_PNM_PTR NULL
#define ilIsValidL_PNM_PTR NULL
#endif /* IL_NO_PNM */

/* Finished examining PNM */

/* Now examining format PSD */
#ifndef IL_NO_PSD /* we support that format */
ILboolean ilLoad_PSD(ILconst_string);
#define ilLoad_PSD_PTR & ilLoad_PSD

ILboolean ilLoadF_PSD(ILHANDLE);
#define ilLoadF_PSD_PTR & ilLoadF_PSD

ILboolean ilLoadL_PSD(const void *, ILuint);
#define ilLoadL_PSD_PTR & ilLoadL_PSD

ILboolean ilSave_PSD(ILconst_string);
#define ilSave_PSD_PTR & ilSave_PSD

ILuint ilSaveF_PSD(ILHANDLE);
#define ilSaveF_PSD_PTR & ilSaveF_PSD

ILuint ilSaveL_PSD(void *, ILuint);
#define ilSaveL_PSD_PTR & ilSaveL_PSD

ILboolean ilIsValid_PSD(ILconst_string);
#define ilIsValid_PSD_PTR & ilIsValid_PSD

ILboolean ilIsValidF_PSD(ILHANDLE);
#define ilIsValidF_PSD_PTR & ilIsValidF_PSD

ILboolean ilIsValidL_PSD(const void *, ILuint);
#define ilIsValidL_PSD_PTR & ilIsValidL_PSD

#else /* we don't support format PSD */
#define ilLoad_PSD_PTR NULL
#define ilLoadF_PSD_PTR NULL
#define ilLoadL_PSD_PTR NULL
#define ilSave_PSD_PTR NULL
#define ilSaveF_PSD_PTR NULL
#define ilSaveL_PSD_PTR NULL
#define ilIsValid_PSD_PTR NULL
#define ilIsValidF_PSD_PTR NULL
#define ilIsValidL_PSD_PTR NULL
#endif /* IL_NO_PSD */

/* Finished examining PSD */

/* Now examining format PSP */
#ifndef IL_NO_PSP /* we support that format */
ILboolean ilLoad_PSP(ILconst_string);
#define ilLoad_PSP_PTR & ilLoad_PSP

ILboolean ilLoadF_PSP(ILHANDLE);
#define ilLoadF_PSP_PTR & ilLoadF_PSP

ILboolean ilLoadL_PSP(const void *, ILuint);
#define ilLoadL_PSP_PTR & ilLoadL_PSP

ILboolean ilIsValid_PSP(ILconst_string);
#define ilIsValid_PSP_PTR & ilIsValid_PSP

ILboolean ilIsValidF_PSP(ILHANDLE);
#define ilIsValidF_PSP_PTR & ilIsValidF_PSP

ILboolean ilIsValidL_PSP(const void *, ILuint);
#define ilIsValidL_PSP_PTR & ilIsValidL_PSP

#else /* we don't support format PSP */
#define ilLoad_PSP_PTR NULL
#define ilLoadF_PSP_PTR NULL
#define ilLoadL_PSP_PTR NULL
#define ilIsValid_PSP_PTR NULL
#define ilIsValidF_PSP_PTR NULL
#define ilIsValidL_PSP_PTR NULL
#endif /* IL_NO_PSP */

/* These are always NULL */
#define ilSave_PSP_PTR NULL
#define ilSaveF_PSP_PTR NULL
#define ilSaveL_PSP_PTR NULL

/* Finished examining PSP */

/* Now examining format PXR */
#ifndef IL_NO_PXR /* we support that format */
ILboolean ilLoad_PXR(ILconst_string);
#define ilLoad_PXR_PTR & ilLoad_PXR

ILboolean ilLoadF_PXR(ILHANDLE);
#define ilLoadF_PXR_PTR & ilLoadF_PXR

ILboolean ilLoadL_PXR(const void *, ILuint);
#define ilLoadL_PXR_PTR & ilLoadL_PXR

#else /* we don't support format PXR */
#define ilLoad_PXR_PTR NULL
#define ilLoadF_PXR_PTR NULL
#define ilLoadL_PXR_PTR NULL
#endif /* IL_NO_PXR */

/* These are always NULL */
#define ilSave_PXR_PTR NULL
#define ilSaveF_PXR_PTR NULL
#define ilSaveL_PXR_PTR NULL
#define ilIsValid_PXR_PTR NULL
#define ilIsValidF_PXR_PTR NULL
#define ilIsValidL_PXR_PTR NULL

/* Finished examining PXR */

/* Now examining format RAW */
#ifndef IL_NO_RAW /* we support that format */
ILboolean ilLoad_RAW(ILconst_string);
#define ilLoad_RAW_PTR & ilLoad_RAW

ILboolean ilLoadF_RAW(ILHANDLE);
#define ilLoadF_RAW_PTR & ilLoadF_RAW

ILboolean ilLoadL_RAW(const void *, ILuint);
#define ilLoadL_RAW_PTR & ilLoadL_RAW

ILboolean ilSave_RAW(ILconst_string);
#define ilSave_RAW_PTR & ilSave_RAW

ILuint ilSaveF_RAW(ILHANDLE);
#define ilSaveF_RAW_PTR & ilSaveF_RAW

ILuint ilSaveL_RAW(void *, ILuint);
#define ilSaveL_RAW_PTR & ilSaveL_RAW

#else /* we don't support format RAW */
#define ilLoad_RAW_PTR NULL
#define ilLoadF_RAW_PTR NULL
#define ilLoadL_RAW_PTR NULL
#define ilSave_RAW_PTR NULL
#define ilSaveF_RAW_PTR NULL
#define ilSaveL_RAW_PTR NULL
#endif /* IL_NO_RAW */

/* These are always NULL */
#define ilIsValid_RAW_PTR NULL
#define ilIsValidF_RAW_PTR NULL
#define ilIsValidL_RAW_PTR NULL

/* Finished examining RAW */

/* Now examining format ROT */
#ifndef IL_NO_ROT /* we support that format */
ILboolean ilLoad_ROT(ILconst_string);
#define ilLoad_ROT_PTR & ilLoad_ROT

ILboolean ilLoadF_ROT(ILHANDLE);
#define ilLoadF_ROT_PTR & ilLoadF_ROT

ILboolean ilLoadL_ROT(const void *, ILuint);
#define ilLoadL_ROT_PTR & ilLoadL_ROT

#else /* we don't support format ROT */
#define ilLoad_ROT_PTR NULL
#define ilLoadF_ROT_PTR NULL
#define ilLoadL_ROT_PTR NULL
#endif /* IL_NO_ROT */

/* These are always NULL */
#define ilSave_ROT_PTR NULL
#define ilSaveF_ROT_PTR NULL
#define ilSaveL_ROT_PTR NULL
#define ilIsValid_ROT_PTR NULL
#define ilIsValidF_ROT_PTR NULL
#define ilIsValidL_ROT_PTR NULL

/* Finished examining ROT */

/* Now examining format SGI */
#ifndef IL_NO_SGI /* we support that format */
ILboolean ilLoad_SGI(ILconst_string);
#define ilLoad_SGI_PTR & ilLoad_SGI

ILboolean ilLoadF_SGI(ILHANDLE);
#define ilLoadF_SGI_PTR & ilLoadF_SGI

ILboolean ilLoadL_SGI(const void *, ILuint);
#define ilLoadL_SGI_PTR & ilLoadL_SGI

ILboolean ilSave_SGI(ILconst_string);
#define ilSave_SGI_PTR & ilSave_SGI

ILuint ilSaveF_SGI(ILHANDLE);
#define ilSaveF_SGI_PTR & ilSaveF_SGI

ILuint ilSaveL_SGI(void *, ILuint);
#define ilSaveL_SGI_PTR & ilSaveL_SGI

ILboolean ilIsValid_SGI(ILconst_string);
#define ilIsValid_SGI_PTR & ilIsValid_SGI

ILboolean ilIsValidF_SGI(ILHANDLE);
#define ilIsValidF_SGI_PTR & ilIsValidF_SGI

ILboolean ilIsValidL_SGI(const void *, ILuint);
#define ilIsValidL_SGI_PTR & ilIsValidL_SGI

#else /* we don't support format SGI */
#define ilLoad_SGI_PTR NULL
#define ilLoadF_SGI_PTR NULL
#define ilLoadL_SGI_PTR NULL
#define ilSave_SGI_PTR NULL
#define ilSaveF_SGI_PTR NULL
#define ilSaveL_SGI_PTR NULL
#define ilIsValid_SGI_PTR NULL
#define ilIsValidF_SGI_PTR NULL
#define ilIsValidL_SGI_PTR NULL
#endif /* IL_NO_SGI */

/* Finished examining SGI */

/* Now examining format SUN */
#ifndef IL_NO_SUN /* we support that format */
ILboolean ilLoad_SUN(ILconst_string);
#define ilLoad_SUN_PTR & ilLoad_SUN

ILboolean ilLoadF_SUN(ILHANDLE);
#define ilLoadF_SUN_PTR & ilLoadF_SUN

ILboolean ilLoadL_SUN(const void *, ILuint);
#define ilLoadL_SUN_PTR & ilLoadL_SUN

ILboolean ilIsValid_SUN(ILconst_string);
#define ilIsValid_SUN_PTR & ilIsValid_SUN

ILboolean ilIsValidF_SUN(ILHANDLE);
#define ilIsValidF_SUN_PTR & ilIsValidF_SUN

ILboolean ilIsValidL_SUN(const void *, ILuint);
#define ilIsValidL_SUN_PTR & ilIsValidL_SUN

#else /* we don't support format SUN */
#define ilLoad_SUN_PTR NULL
#define ilLoadF_SUN_PTR NULL
#define ilLoadL_SUN_PTR NULL
#define ilIsValid_SUN_PTR NULL
#define ilIsValidF_SUN_PTR NULL
#define ilIsValidL_SUN_PTR NULL
#endif /* IL_NO_SUN */

/* These are always NULL */
#define ilSave_SUN_PTR NULL
#define ilSaveF_SUN_PTR NULL
#define ilSaveL_SUN_PTR NULL

/* Finished examining SUN */

/* Now examining format TEXTURE */
#ifndef IL_NO_TEXTURE /* we support that format */
ILboolean ilLoad_TEXTURE(ILconst_string);
#define ilLoad_TEXTURE_PTR & ilLoad_TEXTURE

ILboolean ilLoadF_TEXTURE(ILHANDLE);
#define ilLoadF_TEXTURE_PTR & ilLoadF_TEXTURE

ILboolean ilLoadL_TEXTURE(const void *, ILuint);
#define ilLoadL_TEXTURE_PTR & ilLoadL_TEXTURE

#else /* we don't support format TEXTURE */
#define ilLoad_TEXTURE_PTR NULL
#define ilLoadF_TEXTURE_PTR NULL
#define ilLoadL_TEXTURE_PTR NULL
#endif /* IL_NO_TEXTURE */

/* These are always NULL */
#define ilSave_TEXTURE_PTR NULL
#define ilSaveF_TEXTURE_PTR NULL
#define ilSaveL_TEXTURE_PTR NULL
#define ilIsValid_TEXTURE_PTR NULL
#define ilIsValidF_TEXTURE_PTR NULL
#define ilIsValidL_TEXTURE_PTR NULL

/* Finished examining TEXTURE */

/* Now examining format TGA */
#ifndef IL_NO_TGA /* we support that format */
ILboolean ilLoad_TGA(ILconst_string);
#define ilLoad_TGA_PTR & ilLoad_TGA

ILboolean ilLoadF_TGA(ILHANDLE);
#define ilLoadF_TGA_PTR & ilLoadF_TGA

ILboolean ilLoadL_TGA(const void *, ILuint);
#define ilLoadL_TGA_PTR & ilLoadL_TGA

ILboolean ilSave_TGA(ILconst_string);
#define ilSave_TGA_PTR & ilSave_TGA

ILuint ilSaveF_TGA(ILHANDLE);
#define ilSaveF_TGA_PTR & ilSaveF_TGA

ILuint ilSaveL_TGA(void *, ILuint);
#define ilSaveL_TGA_PTR & ilSaveL_TGA

ILboolean ilIsValid_TGA(ILconst_string);
#define ilIsValid_TGA_PTR & ilIsValid_TGA

ILboolean ilIsValidF_TGA(ILHANDLE);
#define ilIsValidF_TGA_PTR & ilIsValidF_TGA

ILboolean ilIsValidL_TGA(const void *, ILuint);
#define ilIsValidL_TGA_PTR & ilIsValidL_TGA

#else /* we don't support format TGA */
#define ilLoad_TGA_PTR NULL
#define ilLoadF_TGA_PTR NULL
#define ilLoadL_TGA_PTR NULL
#define ilSave_TGA_PTR NULL
#define ilSaveF_TGA_PTR NULL
#define ilSaveL_TGA_PTR NULL
#define ilIsValid_TGA_PTR NULL
#define ilIsValidF_TGA_PTR NULL
#define ilIsValidL_TGA_PTR NULL
#endif /* IL_NO_TGA */

/* Finished examining TGA */

/* Now examining format TIFF */
#ifndef IL_NO_TIFF /* we support that format */
ILboolean ilLoad_TIFF(ILconst_string);
#define ilLoad_TIFF_PTR & ilLoad_TIFF

ILboolean ilLoadF_TIFF(ILHANDLE);
#define ilLoadF_TIFF_PTR & ilLoadF_TIFF

ILboolean ilLoadL_TIFF(const void *, ILuint);
#define ilLoadL_TIFF_PTR & ilLoadL_TIFF

ILboolean ilSave_TIFF(ILconst_string);
#define ilSave_TIFF_PTR & ilSave_TIFF

ILuint ilSaveF_TIFF(ILHANDLE);
#define ilSaveF_TIFF_PTR & ilSaveF_TIFF

ILuint ilSaveL_TIFF(void *, ILuint);
#define ilSaveL_TIFF_PTR & ilSaveL_TIFF

ILboolean ilIsValid_TIFF(ILconst_string);
#define ilIsValid_TIFF_PTR & ilIsValid_TIFF

ILboolean ilIsValidF_TIFF(ILHANDLE);
#define ilIsValidF_TIFF_PTR & ilIsValidF_TIFF

ILboolean ilIsValidL_TIFF(const void *, ILuint);
#define ilIsValidL_TIFF_PTR & ilIsValidL_TIFF

#else /* we don't support format TIFF */
#define ilLoad_TIFF_PTR NULL
#define ilLoadF_TIFF_PTR NULL
#define ilLoadL_TIFF_PTR NULL
#define ilSave_TIFF_PTR NULL
#define ilSaveF_TIFF_PTR NULL
#define ilSaveL_TIFF_PTR NULL
#define ilIsValid_TIFF_PTR NULL
#define ilIsValidF_TIFF_PTR NULL
#define ilIsValidL_TIFF_PTR NULL
#endif /* IL_NO_TIFF */

/* Finished examining TIFF */

/* Now examining format TPL */
#ifndef IL_NO_TPL /* we support that format */
ILboolean ilLoad_TPL(ILconst_string);
#define ilLoad_TPL_PTR & ilLoad_TPL

ILboolean ilLoadF_TPL(ILHANDLE);
#define ilLoadF_TPL_PTR & ilLoadF_TPL

ILboolean ilLoadL_TPL(const void *, ILuint);
#define ilLoadL_TPL_PTR & ilLoadL_TPL

ILboolean ilIsValid_TPL(ILconst_string);
#define ilIsValid_TPL_PTR & ilIsValid_TPL

ILboolean ilIsValidF_TPL(ILHANDLE);
#define ilIsValidF_TPL_PTR & ilIsValidF_TPL

ILboolean ilIsValidL_TPL(const void *, ILuint);
#define ilIsValidL_TPL_PTR & ilIsValidL_TPL

#else /* we don't support format TPL */
#define ilLoad_TPL_PTR NULL
#define ilLoadF_TPL_PTR NULL
#define ilLoadL_TPL_PTR NULL
#define ilIsValid_TPL_PTR NULL
#define ilIsValidF_TPL_PTR NULL
#define ilIsValidL_TPL_PTR NULL
#endif /* IL_NO_TPL */

/* These are always NULL */
#define ilSave_TPL_PTR NULL
#define ilSaveF_TPL_PTR NULL
#define ilSaveL_TPL_PTR NULL

/* Finished examining TPL */

/* Now examining format UTX */
#ifndef IL_NO_UTX /* we support that format */
ILboolean ilLoad_UTX(ILconst_string);
#define ilLoad_UTX_PTR & ilLoad_UTX

ILboolean ilLoadF_UTX(ILHANDLE);
#define ilLoadF_UTX_PTR & ilLoadF_UTX

ILboolean ilLoadL_UTX(const void *, ILuint);
#define ilLoadL_UTX_PTR & ilLoadL_UTX

#else /* we don't support format UTX */
#define ilLoad_UTX_PTR NULL
#define ilLoadF_UTX_PTR NULL
#define ilLoadL_UTX_PTR NULL
#endif /* IL_NO_UTX */

/* These are always NULL */
#define ilSave_UTX_PTR NULL
#define ilSaveF_UTX_PTR NULL
#define ilSaveL_UTX_PTR NULL
#define ilIsValid_UTX_PTR NULL
#define ilIsValidF_UTX_PTR NULL
#define ilIsValidL_UTX_PTR NULL

/* Finished examining UTX */

/* Now examining format VTF */
#ifndef IL_NO_VTF /* we support that format */
ILboolean ilLoad_VTF(ILconst_string);
#define ilLoad_VTF_PTR & ilLoad_VTF

ILboolean ilLoadF_VTF(ILHANDLE);
#define ilLoadF_VTF_PTR & ilLoadF_VTF

ILboolean ilLoadL_VTF(const void *, ILuint);
#define ilLoadL_VTF_PTR & ilLoadL_VTF

ILboolean ilIsValid_VTF(ILconst_string);
#define ilIsValid_VTF_PTR & ilIsValid_VTF

ILboolean ilIsValidF_VTF(ILHANDLE);
#define ilIsValidF_VTF_PTR & ilIsValidF_VTF

ILboolean ilIsValidL_VTF(const void *, ILuint);
#define ilIsValidL_VTF_PTR & ilIsValidL_VTF

#else /* we don't support format VTF */
#define ilLoad_VTF_PTR NULL
#define ilLoadF_VTF_PTR NULL
#define ilLoadL_VTF_PTR NULL
#define ilIsValid_VTF_PTR NULL
#define ilIsValidF_VTF_PTR NULL
#define ilIsValidL_VTF_PTR NULL
#endif /* IL_NO_VTF */

/* These are always NULL */
#define ilSave_VTF_PTR NULL
#define ilSaveF_VTF_PTR NULL
#define ilSaveL_VTF_PTR NULL

/* Finished examining VTF */

/* Now examining format WAL */
#ifndef IL_NO_WAL /* we support that format */
ILboolean ilLoad_WAL(ILconst_string);
#define ilLoad_WAL_PTR & ilLoad_WAL

ILboolean ilLoadF_WAL(ILHANDLE);
#define ilLoadF_WAL_PTR & ilLoadF_WAL

ILboolean ilLoadL_WAL(const void *, ILuint);
#define ilLoadL_WAL_PTR & ilLoadL_WAL

#else /* we don't support format WAL */
#define ilLoad_WAL_PTR NULL
#define ilLoadF_WAL_PTR NULL
#define ilLoadL_WAL_PTR NULL
#endif /* IL_NO_WAL */

/* These are always NULL */
#define ilSave_WAL_PTR NULL
#define ilSaveF_WAL_PTR NULL
#define ilSaveL_WAL_PTR NULL
#define ilIsValid_WAL_PTR NULL
#define ilIsValidF_WAL_PTR NULL
#define ilIsValidL_WAL_PTR NULL

/* Finished examining WAL */

/* Now examining format WBMP */
#ifndef IL_NO_WBMP /* we support that format */
ILboolean ilLoad_WBMP(ILconst_string);
#define ilLoad_WBMP_PTR & ilLoad_WBMP

ILboolean ilLoadF_WBMP(ILHANDLE);
#define ilLoadF_WBMP_PTR & ilLoadF_WBMP

ILboolean ilLoadL_WBMP(const void *, ILuint);
#define ilLoadL_WBMP_PTR & ilLoadL_WBMP

ILboolean ilSave_WBMP(ILconst_string);
#define ilSave_WBMP_PTR & ilSave_WBMP

ILuint ilSaveF_WBMP(ILHANDLE);
#define ilSaveF_WBMP_PTR & ilSaveF_WBMP

ILuint ilSaveL_WBMP(void *, ILuint);
#define ilSaveL_WBMP_PTR & ilSaveL_WBMP

#else /* we don't support format WBMP */
#define ilLoad_WBMP_PTR NULL
#define ilLoadF_WBMP_PTR NULL
#define ilLoadL_WBMP_PTR NULL
#define ilSave_WBMP_PTR NULL
#define ilSaveF_WBMP_PTR NULL
#define ilSaveL_WBMP_PTR NULL
#endif /* IL_NO_WBMP */

/* These are always NULL */
#define ilIsValid_WBMP_PTR NULL
#define ilIsValidF_WBMP_PTR NULL
#define ilIsValidL_WBMP_PTR NULL

/* Finished examining WBMP */

/* Now examining format WDP */
#ifndef IL_NO_WDP /* we support that format */
ILboolean ilLoad_WDP(ILconst_string);
#define ilLoad_WDP_PTR & ilLoad_WDP

ILboolean ilLoadF_WDP(ILHANDLE);
#define ilLoadF_WDP_PTR & ilLoadF_WDP

ILboolean ilLoadL_WDP(const void *, ILuint);
#define ilLoadL_WDP_PTR & ilLoadL_WDP

#else /* we don't support format WDP */
#define ilLoad_WDP_PTR NULL
#define ilLoadF_WDP_PTR NULL
#define ilLoadL_WDP_PTR NULL
#endif /* IL_NO_WDP */

/* These are always NULL */
#define ilSave_WDP_PTR NULL
#define ilSaveF_WDP_PTR NULL
#define ilSaveL_WDP_PTR NULL
#define ilIsValid_WDP_PTR NULL
#define ilIsValidF_WDP_PTR NULL
#define ilIsValidL_WDP_PTR NULL

/* Finished examining WDP */

/* Now examining format XPM */
#ifndef IL_NO_XPM /* we support that format */
ILboolean ilLoad_XPM(ILconst_string);
#define ilLoad_XPM_PTR & ilLoad_XPM

ILboolean ilLoadF_XPM(ILHANDLE);
#define ilLoadF_XPM_PTR & ilLoadF_XPM

ILboolean ilLoadL_XPM(const void *, ILuint);
#define ilLoadL_XPM_PTR & ilLoadL_XPM

ILboolean ilIsValid_XPM(ILconst_string);
#define ilIsValid_XPM_PTR & ilIsValid_XPM

ILboolean ilIsValidF_XPM(ILHANDLE);
#define ilIsValidF_XPM_PTR & ilIsValidF_XPM

ILboolean ilIsValidL_XPM(const void *, ILuint);
#define ilIsValidL_XPM_PTR & ilIsValidL_XPM

#else /* we don't support format XPM */
#define ilLoad_XPM_PTR NULL
#define ilLoadF_XPM_PTR NULL
#define ilLoadL_XPM_PTR NULL
#define ilIsValid_XPM_PTR NULL
#define ilIsValidF_XPM_PTR NULL
#define ilIsValidL_XPM_PTR NULL
#endif /* IL_NO_XPM */

/* These are always NULL */
#define ilSave_XPM_PTR NULL
#define ilSaveF_XPM_PTR NULL
#define ilSaveL_XPM_PTR NULL

/* Finished examining XPM */

/* Now examining format BLP */
#ifndef IL_NO_BLP /* we support that format */
ILboolean ilLoad_BLP(ILconst_string);
#define ilLoad_BLP_PTR & ilLoad_BLP

ILboolean ilLoadF_BLP(ILHANDLE);
#define ilLoadF_BLP_PTR & ilLoadF_BLP

ILboolean ilLoadL_BLP(const void *, ILuint);
#define ilLoadL_BLP_PTR & ilLoadL_BLP

ILboolean ilIsValid_BLP(ILconst_string);
#define ilIsValid_BLP_PTR & ilIsValid_BLP

ILboolean ilIsValidF_BLP(ILHANDLE);
#define ilIsValidF_BLP_PTR & ilIsValidF_BLP

ILboolean ilIsValidL_BLP(const void *, ILuint);
#define ilIsValidL_BLP_PTR & ilIsValidL_BLP

#else /* we don't support format BLP */
#define ilLoad_BLP_PTR NULL
#define ilLoadF_BLP_PTR NULL
#define ilLoadL_BLP_PTR NULL
#define ilIsValid_BLP_PTR NULL
#define ilIsValidF_BLP_PTR NULL
#define ilIsValidL_BLP_PTR NULL
#endif /* IL_NO_BLP */

/* These are always NULL */
#define ilSave_BLP_PTR NULL
#define ilSaveF_BLP_PTR NULL
#define ilSaveL_BLP_PTR NULL

/* Finished examining BLP */

/* Now examining format IWI */
#ifndef IL_NO_IWI /* we support that format */
ILboolean ilLoad_IWI(ILconst_string);
#define ilLoad_IWI_PTR & ilLoad_IWI

ILboolean ilLoadF_IWI(ILHANDLE);
#define ilLoadF_IWI_PTR & ilLoadF_IWI

ILboolean ilLoadL_IWI(const void *, ILuint);
#define ilLoadL_IWI_PTR & ilLoadL_IWI

ILboolean ilIsValid_IWI(ILconst_string);
#define ilIsValid_IWI_PTR & ilIsValid_IWI

ILboolean ilIsValidF_IWI(ILHANDLE);
#define ilIsValidF_IWI_PTR & ilIsValidF_IWI

ILboolean ilIsValidL_IWI(const void *, ILuint);
#define ilIsValidL_IWI_PTR & ilIsValidL_IWI

#else /* we don't support format IWI */
#define ilLoad_IWI_PTR NULL
#define ilLoadF_IWI_PTR NULL
#define ilLoadL_IWI_PTR NULL
#define ilIsValid_IWI_PTR NULL
#define ilIsValidF_IWI_PTR NULL
#define ilIsValidL_IWI_PTR NULL
#endif /* IL_NO_IWI */

/* These are always NULL */
#define ilSave_IWI_PTR NULL
#define ilSaveF_IWI_PTR NULL
#define ilSaveL_IWI_PTR NULL

/* Finished examining IWI */

/* Now examining format FTX */
#ifndef IL_NO_FTX /* we support that format */
ILboolean ilLoad_FTX(ILconst_string);
#define ilLoad_FTX_PTR & ilLoad_FTX

ILboolean ilLoadF_FTX(ILHANDLE);
#define ilLoadF_FTX_PTR & ilLoadF_FTX

ILboolean ilLoadL_FTX(const void *, ILuint);
#define ilLoadL_FTX_PTR & ilLoadL_FTX

#else /* we don't support format FTX */
#define ilLoad_FTX_PTR NULL
#define ilLoadF_FTX_PTR NULL
#define ilLoadL_FTX_PTR NULL
#endif /* IL_NO_FTX */

/* These are always NULL */
#define ilSave_FTX_PTR NULL
#define ilSaveF_FTX_PTR NULL
#define ilSaveL_FTX_PTR NULL
#define ilIsValid_FTX_PTR NULL
#define ilIsValidF_FTX_PTR NULL
#define ilIsValidL_FTX_PTR NULL

/* Finished examining FTX */

/* Now examining format DICOM */
#ifndef IL_NO_DICOM /* we support that format */
ILboolean ilLoad_DICOM(ILconst_string);
#define ilLoad_DICOM_PTR & ilLoad_DICOM

ILboolean ilLoadF_DICOM(ILHANDLE);
#define ilLoadF_DICOM_PTR & ilLoadF_DICOM

ILboolean ilLoadL_DICOM(const void *, ILuint);
#define ilLoadL_DICOM_PTR & ilLoadL_DICOM

ILboolean ilIsValid_DICOM(ILconst_string);
#define ilIsValid_DICOM_PTR & ilIsValid_DICOM

ILboolean ilIsValidF_DICOM(ILHANDLE);
#define ilIsValidF_DICOM_PTR & ilIsValidF_DICOM

ILboolean ilIsValidL_DICOM(const void *, ILuint);
#define ilIsValidL_DICOM_PTR & ilIsValidL_DICOM

#else /* we don't support format DICOM */
#define ilLoad_DICOM_PTR NULL
#define ilLoadF_DICOM_PTR NULL
#define ilLoadL_DICOM_PTR NULL
#define ilIsValid_DICOM_PTR NULL
#define ilIsValidF_DICOM_PTR NULL
#define ilIsValidL_DICOM_PTR NULL
#endif /* IL_NO_DICOM */

/* These are always NULL */
#define ilSave_DICOM_PTR NULL
#define ilSaveF_DICOM_PTR NULL
#define ilSaveL_DICOM_PTR NULL

/* Finished examining DICOM */

#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus*/
#endif /* PTR_DEFINES_H */

