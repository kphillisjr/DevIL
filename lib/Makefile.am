all_include = $(srcdir)/../include

il_src = $(srcdir)/../src-IL/src
il_include = $(srcdir)/../src-IL/include
ilu_src = $(srcdir)/../src-ILU/src
ilu_include = $(srcdir)/../src-ILU/include
ilut_src = $(srcdir)/../src-ILUT/src
ilut_include = $(srcdir)/../src-ILUT/include

ildir = $(includedir)/IL

lib_LTLIBRARIES =
il_HEADERS =  $(all_include)/IL/devil_cpp_wrapper.hpp
AM_LDFLAGS = -version-info @DEVIL_LTVERSION@
AM_CFLAGS = $(GENERAL_CFLAGS)

if WINDOWS_BUILD
AM_LDFLAGS += -no-undefined
endif #WINDOWS_BUILD

if BUILD_IL

lib_LTLIBRARIES += libIL.la 
il_HEADERS += $(all_include)/IL/il.h

endif #BUILD_IL

if BUILD_ILU

lib_LTLIBRARIES += libILU.la 
il_HEADERS += $(all_include)/IL/ilu.h $(all_include)/IL/ilu_region.h

endif #BUILD_ILU

if BUILD_ILUT

lib_LTLIBRARIES += libILUT.la 
il_HEADERS += $(all_include)/IL/ilut.h $(all_include)/IL/ilut_config.h

endif #BUILD_ILUT

libIL_la_CPPFLAGS = -I $(il_include) -I $(all_include)
libIL_la_CFLAGS = $(AM_CFLAGS) @IL_CFLAGS@
libIL_la_CXXFLAGS = $(AM_CFLAGS) @IL_CFLAGS@

libIL_la_SOURCES = $(il_src)/il_alloc.cpp $(il_src)/il_doom.cpp $(il_src)/il_stack.cpp $(il_src)/il_pcx.cpp $(il_src)/il_rle.cpp $(il_src)/il_bits.cpp $(il_src)/il_endian.cpp $(il_src)/il_exr.cpp $(il_src)/il_jpeg.cpp $(il_src)/il_pic.cpp $(il_src)/il_sgi.cpp $(il_src)/il_bmp.cpp $(il_src)/il_error.cpp $(il_src)/il_lif.cpp $(il_src)/il_png.cpp $(il_src)/il_states.cpp $(il_src)/il_convbuff.cpp $(il_src)/il_files.cpp $(il_src)/il_main.cpp $(il_src)/il_pnm.cpp $(il_src)/il_targa.cpp $(il_src)/il_wdp.cpp $(il_src)/il_convert.cpp $(il_src)/il_gif.cpp $(il_src)/il_manip.cpp $(il_src)/il_profiles.cpp $(il_src)/il_tiff.cpp $(il_src)/il_cut.cpp $(il_src)/il_header.cpp $(il_src)/il_mdl.cpp $(il_src)/il_quantizer.cpp $(il_src)/il_utility.cpp $(il_src)/il_dcx.cpp $(il_src)/il_icon.cpp $(il_src)/il_mng.cpp $(il_src)/il_wal.cpp $(il_src)/il_dds.cpp $(il_src)/il_internal.cpp $(il_src)/il_pal.cpp $(il_src)/il_rawdata.cpp $(il_src)/il_devil.cpp $(il_src)/il_io.cpp $(il_src)/il_pcd.cpp $(il_src)/il_register.cpp $(il_src)/il_psd.cpp $(il_src)/il_fastconv.cpp $(il_src)/il_neuquant.cpp $(il_src)/il_dds-save.cpp $(il_src)/il_psp.cpp $(il_src)/il_pix.cpp $(il_src)/il_pxr.cpp $(il_src)/il_vtf.cpp $(il_src)/il_xpm.cpp $(il_src)/il_hdr.cpp $(il_src)/il_icns.cpp $(il_src)/il_jp2.cpp $(il_src)/altivec_common.cpp $(il_src)/altivec_typeconversion.cpp $(il_src)/il_sun.cpp $(il_src)/il_nvidia.cpp $(il_src)/il_wbmp.cpp $(il_src)/il_squish.cpp $(il_src)/il_iff.cpp $(il_src)/il_ilbm.cpp $(il_src)/il_size.cpp $(il_src)/il_tpl.cpp $(il_src)/il_fits.cpp $(il_src)/il_iwi.cpp $(il_src)/il_blp.cpp $(il_src)/il_ftx.cpp $(il_src)/il_dicom.cpp $(il_src)/il_rot.cpp $(il_src)/il_texture.cpp $(il_src)/il_dpx.cpp $(il_src)/il_utx.cpp $(il_src)/il_mp3.cpp $(il_src)/il_sin.cpp $(il_src)/il_tex.cpp


libIL_la_SOURCES += $(il_include)/altivec_common.h $(il_include)/altivec_typeconversion.h $(il_include)/il_alloc.h $(il_include)/il_bits.h $(il_include)/il_bmp.h $(il_include)/il_dcx.h $(il_include)/il_dds.h $(il_include)/il_doompal.h $(il_include)/il_dpx.h $(il_include)/il_endian.h $(il_include)/il_exr.h $(il_include)/il_files.h $(il_include)/il_gif.h $(il_include)/il_hdr.h $(il_include)/il_icns.h $(il_include)/il_icon.h $(il_include)/il_internal.h $(il_include)/il_jp2.h $(il_include)/il_jpeg.h $(il_include)/il_lif.h $(il_include)/il_manip.h $(il_include)/il_mdl.h $(il_include)/il_pal.h $(il_include)/il_pcx.h $(il_include)/il_pic.h $(il_include)/il_pnm.h $(il_include)/il_psd.h $(il_include)/il_psp.h $(il_include)/il_q2pal.h $(il_include)/il_register.h $(il_include)/il_rle.h $(il_include)/il_sgi.h $(il_include)/il_stack.h $(il_include)/il_states.h $(il_include)/il_targa.h $(il_include)/il_utx.h $(il_include)/il_vtf.h $(il_include)/il_wdp.h $(all_include)/IL/devil_internal_exports.h
libIL_la_LDFLAGS = @IL_LIBS@ $(AM_LDFLAGS)

libILU_la_CPPFLAGS = -I $(ilu_include) -I $(all_include)
libILU_la_CFLAGS = $(AM_CFLAGS) @ILU_CFLAGS@
libILU_la_SOURCES = $(ilu_src)/ilu_alloc.cpp $(ilu_src)/ilu_internal.cpp $(ilu_src)/ilu_noise.cpp $(ilu_src)/ilu_scale.cpp $(ilu_src)/ilu_error.cpp $(ilu_src)/ilu_main.cpp $(ilu_src)/ilu_rotate.cpp $(ilu_src)/ilu_scaling.cpp $(ilu_src)/ilu_filter.cpp $(ilu_src)/ilu_manip.cpp $(ilu_src)/ilu_scale2d.cpp $(ilu_src)/ilu_states.cpp $(ilu_src)/ilu_filter_rcg.cpp $(ilu_src)/ilu_mipmap.cpp $(ilu_src)/ilu_scale3d.cpp $(ilu_src)/ilu_utilities.cpp $(ilu_src)/ilu_region.cpp

libILU_la_SOURCES += $(ilu_include)/ilu_alloc.h $(ilu_include)/ilu_filter.h $(ilu_include)/ilu_internal.h $(ilu_include)/ilu_region.h $(ilu_include)/ilu_states.h $(all_include)/IL/devil_internal_exports.h $(all_include)/IL/ilu_region.h $(ilu_include)/ilu_error/ilu_err-arabic.h $(ilu_include)/ilu_error/ilu_err-dutch.h $(ilu_include)/ilu_error/ilu_err-english.h $(ilu_include)/ilu_error/ilu_err-french.h $(ilu_include)/ilu_error/ilu_err-german.h $(ilu_include)/ilu_error/ilu_err-japanese.h $(ilu_include)/ilu_error/ilu_err-spanish.h 

libILU_la_LDFLAGS = @ILU_LIBS@ $(AM_LDFLAGS)
libILU_la_LIBADD = libIL.la

libILUT_la_CPPFLAGS = -I $(ilut_include) -I $(all_include)
libILUT_la_CFLAGS = $(AM_CFLAGS) @ILUT_CFLAGS@
libILUT_la_SOURCES = $(ilut_src)/ilut_main.cpp $(ilut_src)/ilut_states.cpp $(ilut_src)/ilut_internal.cpp  
libILUT_la_SOURCES += $(ilut_include)/ilut_internal.h $(ilut_include)/ilut_states.h $(all_include)/IL/devil_internal_exports.h
libILUT_la_LDFLAGS = @ILUT_LIBS@ $(AM_LDFLAGS)
libILUT_la_LIBADD = libILU.la

if USE_ALLEGRO
libILUT_la_SOURCES += $(ilut_src)/ilut_allegro.cpp $(ilut_include)/ilut_allegro.h 
endif #USE_ALLEG

if USE_DIRECTX
libILUT_la_SOURCES += $(ilut_src)/ilut_directx.cpp
endif #USE_DIRECTX

if USE_DIRECTX9
libILUT_la_SOURCES += $(ilut_src)/ilut_directx9.cpp
endif #USE_DIRECTX

if USE_SDL
libILUT_la_SOURCES += $(ilut_src)/ilut_sdlsurface.cpp 
endif #USE_SDL

if USE_OPENGL
libILUT_la_SOURCES += $(ilut_include)/ilut_opengl.h $(ilut_src)/ilut_opengl.cpp 
endif #USE_OPENGL

if USE_X11
libILUT_la_SOURCES += $(ilut_src)/ilut_x11.cpp
endif #USE_X11

if USE_W32
libILUT_la_SOURCES += $(ilut_src)/ilut_win32.cpp  
endif #USE_W32

