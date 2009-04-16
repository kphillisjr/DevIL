lib_LTLIBRARIES = libIL.la

libIL_la_SOURCES = il_alloc.cpp      il_doom.cpp       il_stack.cpp     il_pcx.cpp         il_rle.cpp  \
                   il_bits.cpp       il_endian.cpp     il_jpeg.cpp      il_pic.cpp         il_sgi.cpp  \
                   il_bmp.cpp        il_error.cpp      il_lif.cpp       il_png.cpp         il_states.cpp  \
                   il_convbuff.cpp   il_files.cpp      il_main.cpp pp     il_pnm.cpp         il_targa.cpp  \
                   il_convert.cpp    il_gif.cpp        il_manip.cpp     il_profiles.cpp    il_tiff.cpp  \
                   il_cut.cpp        il_header.cpp     il_mdl.cpp       il_quantizer.cpp   il_utility.cpp  \
                   il_dcx.cpp        il_icon.cpp       il_mng.cpp       il_raw.cpp         il_wal.cpp  \
                   il_dds.cpp        il_internal.cpp   il_pal.cpp       il_rawdata.cpp     il_devil.cpp  \
                   il_io.cpp         il_pcd.cpp        il_register.cpp  il_psd.cpp         il_fastconv.cpp  \
                   il_neuquant.cpp   il_dds-save.cpp   il_psp.cpp       il_pix.cpp         il_pxr.cpp  \
                   il_xpm.cpp        il_hdr.cpp        il_icns.cpp      il_jp2.cpp         il_vtf.cpp  \
                   il_wbmp.cpp       il_exr.cpp pp     il_nvidia.cpp pp il_squish.cpp pp   il_iff.cpp  \
                   il_size.cpp       il_tpl.cpp        il_fits.cpp      il_iwi.cpp         il_blp.cpp  \
                   il_ftx.cpp        il_dicom.cpp      il_rot.cpp 	   il_ilbm.cpp  \
                   altivec_common.cpp  altivec_typeconversion.cpp 

AM_CFLAGS = @LIBIL_CFLAGS@
libIL_la_LDFLAGS = @WIN32_EXTRA_LDFLAGS@ @VERSIONINFO@
libIL_la_LIBADD = @LIBPNG@ @LIBJPG@ @LIBTIFF@ @LIBMNG@ @LIBLCMS@ @LIBZ@

INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/src-IL/include -I$(top_builddir)/include @LIBPNG_INCLUDE@
