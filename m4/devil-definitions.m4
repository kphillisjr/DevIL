
AC_DEFUN([ADD_CFLAGS],
	 [GENERAL_CFLAGS="$GENERAL_CFLAGS $1"])
dnl          IL_CFLAGS="$IL_CFLAGS $1"])
dnl          ILU_CFLAGS="$ILU_CFLAGS $1"
dnl          ILUT_CFLAGS="$ILUT_CFLAGS $1"])
dnl IL_CFLAGS are present everywhere


dnl
dnl Check CPU Extensions
dnl
AC_DEFUN([TEST_EXT],
	 [AC_ARG_ENABLE([$6],
			[AC_HELP_STRING([--enable-$6],
                                        [Doesn't compile $1 support. Overrides general enable of ASM])],
			[use_$1=$enable_$6
                         test "x$enable_$6" = "xyes" && enable_asm="yes"],
			[use_$1=test])
         AC_MSG_CHECKING([$1])
         AS_IF([test "$use_$1" = "yes"],
               [AC_DEFINE([$4],
                          [1],
                          [$1 extension found])
                AC_DEFINE([VECTORMEM],
                          [1],
                          [Memory must be vector aligned])
                ADD_CFLAGS([$2])
                AC_MSG_RESULT([yes])
                SUPPORTED_EXTENSION=$SUPPORTED_EXTENSION"$1 "],
               [test "$use_$1" = "test" -a "x$enable_asm" = "xyes"],
               [CFLAGS_TMP=$CFLAGS
                CFLAGS="$CFLAGS $2"
                AC_COMPILE_IFELSE([$5
                                   int main() 
                                   {$3;return 0;}],
                                  [AC_DEFINE([$4],
                                             [],
                                             [$1 extension found])
                                   AC_DEFINE([VECTORMEM],
                                             [],
                                             [Memory must be vector aligned])
                                   ADD_CFLAGS([$2])
                                   AC_MSG_RESULT([yes])
                                   SUPPORTED_EXTENSION=$SUPPORTED_EXTENSION"$1 "],
                                  [AC_MSG_RESULT([no]) ])
                CFLAGS="$CFLAGS_TMP"],
               [AC_MSG_RESULT(disabled)]) ])

dnl
dnl Check CPU Assembly
dnl
dnl add test to have only an ASM for compilation
AC_DEFUN([TEST_ASM],
	 [AC_ARG_ENABLE([$4],
			[AC_HELP_STRING([--enable-$4],
					[Compile with $4 support])],
                        [use_$4=$enable_$4],[use_$4=test])
          AC_MSG_CHECKING([$4])
          AS_IF([test "$use_$4" = "yes"],
                [SUPPORTED_EXTENSION=$SUPPORTED_EXTENSION"$2 "
		 AC_DEFINE([$3],
			   [],
			   [$2 assembly found]) 
		 AC_MSG_RESULT([yes])],
                [test "$use_$4" = "no"],
                [AC_MSG_RESULT(no)],
		[AS_CASE([$host_cpu], 
	                 [$1],
			 [SUPPORTED_EXTENSION=$SUPPORTED_EXTENSION"$2 "
		          AC_DEFINE([$3],
                                    [],
                                    [$2 assembly found]) 
		          AC_MSG_RESULT([yes])],
	                 [AC_MSG_RESULT([no]) ]) ]) ])

dnl
dnl Setup Libraries
dnl SET_LIB([libname],[if-found],[if-not-found])
dnl
AC_DEFUN([SET_LIB],
	 [AC_ARG_WITH([$1],
		      [AC_HELP_STRING([--with-$1=[[yes/no]]],
				      [Do wou want to use lib$1?])],
	              [],
	              [enable_$1="yes"])
          AS_IF([test "$enable_$1" = "yes"],
	        [AC_CHECK_LIB([$1],
                              [main],
                              [LIBS="-l$1 $LIBS"
                               support_$1="yes"],
                              [support_$1="no"]) 
          AS_IF([test "x$support_$1" = "xyes" -a -n '$2'],
                [$2],
                [test "x$support_$1" = "xno" -a -n '$3'],
                [$3]) ]) ])

AC_DEFUN([TO_UPPERCASE],
	 [m4_translit([$1], [a-z], [A-Z])])

dnl
dnl Modules
dnl
dnl Usage: 
dnl 	ADD_CLASS(<name>, <enabled by default?>, <short description>)
dnl Concrete example:
dnl 	ADD_CLASS([base], [yes], [Basic must-have supported formats that don't have external dependencies])
dnl 	echo "The first added class: ${CLASS_NAMES[0]} -- ${CLASS_DESCRIPTIONS[0]}"
dnl
AC_DEFUN([ADD_CLASS],	 
	 [dnl We set the number of classes array size for the first time
          NUM_CLASSES=${#CLASS_NAMES[@]}
	  dnl Do we really want to add this class?
	  AC_ARG_ENABLE([$1],
			[AC_HELP_STRING([--enable-$1],
					[Compile the $1 class. $3 (default=$2) ]) ],
		        [],
			[enable_$1="$2"]) 
	  dnl Automake conditional telling to make that we should bother to compile the thing.
	  AM_CONDITIONAL( TO_UPPERCASE([BUILD_$1]),
			 [test "x$enable_$1" = "xyes"])
	  AC_MSG_CHECKING([whether we would like to build the '$1' class])
          AS_IF([test "x$enable_$1" = "xyes"],
                [dnl Yes, we want to add a new class!
		 CLASS_NAMES[[$NUM_CLASSES]]="$1"
		 CLASS_DESCRIPTIONS[[$NUM_CLASSES]]="$3"
		 dnl We also want to substitute its name in Makefile.am
		 AC_SUBST( TO_UPPERCASE([$1_CLASS]),
			  [${lib_prefix}${CLASS_NAMES[[$NUM_CLASSES]]} ])
		 (( NUM_CLASSES++ ))
		 AC_MSG_RESULT([yes]) ], 
                [AC_MSG_RESULT([no]) ]) ])

dnl
dnl Converts string to an index in an array. 
dnl If the string matches to an array member, the index is returned.
dnl Usage: 
dnl 	STR_TO_INDEX(<array>, <string>, <index>)
dnl Example:
dnl 	STR_TO_INDEX([CLASS_NAME], [base], [index])
dnl 	CLASS_FORMATS[$index]="CLASS_FORMATS[$index] JPG"
dnl
AC_DEFUN([STR_TO_INDEX],
	 [dnl I think that this is called "making STR_TO_INDEX reentrant"
	  $3=""
	  loop_index=0
	  for loop_i in "${$1[[@]]}"
	  do
		dnl Yes, we have it! Let's assign it then...
		test "x$loop_i" = "x$2" && $3=$loop_index;
		(( loop_index++ ))
	  done ])

dnl
dnl Formats
dnl
dnl Usage:
dnl 	TEST_FORMAT(<name>, <varname>[, <brief description>, [, <check for a lib>] ] )
dnl Concrete example:
dnl 	TEST_FORMAT([jpeg], [JPG], [JPEG is most common lossy format. libjpeg powered], [SETTLE_JPEG])
dnl
AC_DEFUN([TEST_FORMAT],
	 [dnl Let's ask politely whether we want to compile this format
	  AC_ARG_ENABLE([$1],
			[AC_HELP_STRING([--enable-$1],
					[Compile $1 support. $3 (default=yes) ])],
		        [],
			[enable_$1="yes"]) 
         AS_IF([test $# -eq 4 -a "x$enable_$1" = "xyes"],
               [$4]) 
	 dnl Add the format name to the list of mentioned formats. We will take some actions on them even if we won't support them...
	 MENTIONED_FORMATS="$MENTIONED_FORMATS $2"
         AC_MSG_CHECKING([whether we would like to have support for $1 format])
         AS_IF([test "x$enable_$1" = "xno" -o "x$lib_test_result" = "xno"],
               [AC_MSG_RESULT([no])
                dnl Let's tell everybody that we don't want this format
                AC_DEFINE([IL_NO_$2],
                          [],
                          [$1 support ($3)])],
               [dnl Magically detect what class does the module belong to...
                DETECT_FORMAT_CLASS([$2])
		STR_TO_INDEX([CLASS_NAMES], [$CLASSNAME], [index])
		dnl Have we found the index?
		AS_IF([test -z "$index" ],
		      [AC_MSG_RESULT([no])
	 	       AS_IF([test -z "$CLASSNAME"],
			     [dnl The class name is actually unknown - someone should update the build system then...
			      AC_MSG_WARN([The $2 format has trouble with the class it should belong into]) ],
		             [AC_MSG_NOTICE([The $CLASSNAME module/class won't be built, so the $1 format support won't be compiled in]) ]) ],
                      [dnl Amazing, everything went +- fine...
                       AC_MSG_RESULT([yes]) 
                       dnl Append the list of formats in the same class. Carefully with m4 quoting!
		       [CLASS_FORMATS[$index]="${CLASS_FORMATS[$index]}] $2" 
		       dnl And also update the cumulative list of supported formats to amaze users.
		       SUPPORTED_FORMATS="$SUPPORTED_FORMATS $2" ]) ]) 
         lib_test_result="" ])

dnl
dnl Given a format CODE, tell what class does it belong to and do the useful stuf:
dnl 	0. Find out what class does the format belong to
dnl 	1. If the class was disabled, tell the user not to expect the format
dnl 	2. If the class is enabled, add the format ID to the class format list
dnl 	3. Tell us whether the class was enabled at all...
dnl
dnl Usage:
dnl 	DETECT_FORMAT_CLASS(<format>)
dnl Example:
dnl 	DETECT_FORMAT_CLASS([JPEG])
dnl 	echo "JPEG belongs to $CLASSNAME"
dnl
dnl Output: 
dnl 	class_enabled=yes/no
dnl
AC_DEFUN([DETECT_FORMAT_CLASS],
	 [dnl We try to find the file where some functions containing
	  dnl format name are DEFINED (for example ilLoad_JPEG, ilIsValid_JPEG)
	  PATH_TO_SOME_DEFINITION=$(grep -l 'il[[^_]]*_$1\s*([[^;(]]*$' src-IL/src/*) 
	  dnl To partially fix parenthesis matching for the editor: )
	  dnl Oh yes, now we take only the filename without the path to it (we have to chop everything before the last '/')
          SOME_DEFN_FILENAME=$(echo $PATH_TO_SOME_DEFINITION | sed -e 's/.*\/\([[^\/]]\+\)/\1/') 
	  CLASSNAME=$(grep $SOME_DEFN_FILENAME lib/Makefile.am dnl 
	  dnl Get the right line, there can be more of them
	  | sed -n '/@\([[A-Z0-9]]*\)_/p' dnl 
	  dnl Finally extract the classname. Output it in lowercase...
          | sed -e "s/[^@]*@\([[A-Z0-9]]*\)_.*/\L\1\E/" ) ])

dnl
dnl Adds a library to human-readable list of dependencies
dnl
dnl Usage:
dnl MAYBE_OPTIONAL_DEPENDENCY(<library part prefix>, <library name>)
dnl for example:
dnl MAYBE_OPTIONAL_DEPENDENCY([IL], [libjpeg])
dnl MAYBE_OPTIONAL_DEPENDENCY([ILUT], [allegro])
dnl
AC_DEFUN([MAYBE_OPTIONAL_DEPENDENCY],
         [AS_IF([test "x$enable_modules" = "xyes"],
                [$1_LIBS_OPTIONAL="${$1_LIBS_OPTIONAL} $2"],
                [$1_LIBS_REQUIRED="${$1_LIBS_REQUIRED} $2"]) ])

dnl
dnl Check for libraries
dnl
dnl Usage:
dnl DEVIL_IL_LIB(<include>, <library>, <class name>)
dnl 	the <library> is appended to <class_name>_LIBS, sets have_<library> to yes/no
dnl Nothing else is done, see MAYBE_OPTIONAL_DEPENDENCY macro...
dnl
AC_DEFUN([DEVIL_IL_LIB],
         [AC_CHECK_HEADER([$1],
                          [AC_CHECK_LIB([$2],
                                        [main],
                                        [$3_LIBS="-l$2 ${$3_LIBS}"
                                         have_$2="yes"],
                                        [have_$2="no"])],
                          [have_$2="no"]) ])

dnl
dnl Checks for squish library = GPU accelerated DXT compression
dnl Can be used along with nvidia texture tools
dnl
AC_DEFUN([DEVIL_CHECK_LIBSQUISH],
         [DEVIL_IL_LIB([squish.h],
                       [squish],
		       [DDS])
          lib_test_result="$have_squish"
          AS_IF([test "x$lib_test_result" = "xyes"],
                [AC_SUBST([DDS_LIBS])
                 AC_DEFINE([IL_USE_DXTC_SQUISH],
                           [1],
                           [Define if you have libsquish installed]) 
                 MAYBE_OPTIONAL_DEPENDENCY([IL], 
                                           [libsquish]) ]) ])

dnl
dnl Checks for nvidia texture tools library - GPU acceleration of DXT compression
dnl Can be used along with libsquish
dnl 
AC_DEFUN([DEVIL_CHECK_NVIDIA_TEXTOOLS],
         [DEVIL_IL_LIB([nvtt/nvtt.h],
                       [nvtt],
		       [DDS])
          lib_test_result="$have_nvtt"
          AS_IF([test "x$lib_test_result" = "xyes"],
                [AC_SUBST([DDS_LIBS])
                 AC_DEFINE([IL_USE_DXTC_NVIDIA],
                           [1],
                           [Define if you have nvidia texture tools library installed]) 
                 MAYBE_OPTIONAL_DEPENDENCY([IL], 
                                           [libnvtt-nvidia_texture_tools]) ]) ])

AC_DEFUN([SETTLE_LCMS],
[AC_CHECK_LIB([lcms],
              [main],
              [have_lcms_lib="yes"
               IL_LIBS="-llcms $IL_LIBS"])
AC_CHECK_HEADER([lcms/lcms.h],
                [have_lcms_h="yes"])
AC_CHECK_HEADER([lcms.h],
                [have_lcms_h="yes"
                 lcms_nodirinclude="yes"]) 
AS_IF([test "x$lcms_nodirinclude" = "xyes"],
      [AC_DEFINE([LCMS_NODIRINCLUDE],
                 [1],
                 [LCMS include without lcms/ support]) ])
AS_IF([test "x$have_lcms_lib" = "xyes" -a "x$have_lcms_h" = "xyes"],
      [have_lcms="yes"
       IL_LIBS_REQUIRED="$IL_LIBS_REQUIRED lcms"
       lib_test_result="yes"],
      [lib_test_result="no"]) ])

AC_DEFUN([SETTLE_OPENEXR],
         [PKG_CHECK_MODULES([OPENEXR], 
                            [OpenEXR],
                            [have_openexr="yes"],
                            [have_openexr="no"])
          MAYBE_OPTIONAL_DEPENDENCY([IL],
                                    [OpenEXR])
          dnl IL_LIBS="$OPENEXR_LIBS $IL_LIBS"
	  AC_SUBST([OPENEXR_LIBS])
          dnl IL_CFLAGS="$OPENEXR_CFLAGS $IL_CFLAGS"
	  AC_SUBST([OPENEXR_CFLAGS])
          lib_test_result="$have_openexr"])

AC_DEFUN([SETTLE_JPEG],
         [DEVIL_IL_LIB([jpeglib.h],
                       [jpeg], 
		       [JPEG])
          AC_DEFINE([IL_USE_JPEGLIB_UNMODIFIED],
                    [1],
                    [Use libjpeg without modification. always enabled.])
          lib_test_result="$have_jpeg"
          AS_IF([test "x$lib_test_result" = "xyes"],
                [AC_SUBST([JPEG_LIBS])
                 MAYBE_OPTIONAL_DEPENDENCY([IL],
                                           [libjpeg]) ]) ]) 

AC_DEFUN([SETTLE_JASPER],
         [DEVIL_IL_LIB([jasper/jasper.h],
                       [jasper],
                       [JP2])
          AS_IF([test "x$have_jasper" != "xyes"],
                [DEVIL_IL_LIB([jasper/jasper.h],
                              [jp2],
                              [jp2])
                 lib_test_result="$have_jp2" ],
                [lib_test_result="yes"]) 
          AS_IF([test "x$lib_test_result" = "xyes"],
                [AC_SUBST([JP2_LIBS])
                 MAYBE_OPTIONAL_DEPENDENCY([IL],
                                           [JasPer]) ]) ])

AC_DEFUN([SETTLE_MNG],
         [DEVIL_IL_LIB([libmng.h],
                       [mng],
                       [MNG])
          lib_test_result="$have_mng" 
          AS_IF([test "x$lib_test_result" = "xyes"],
                [AC_SUBST([MNG_LIBS])
                 MAYBE_OPTIONAL_DEPENDENCY([IL],
                                           [libmng]) ]) ]) 

AC_DEFUN([SETTLE_PNG],
         [DEVIL_IL_LIB([png.h],
                       [png12], 
                       [PNG]) 
          AS_IF([test "x$have_png12" = "xno"],
                [DEVIL_IL_LIB([png.h],
                              [png], 
                              [PNG]) 
                 lib_test_result="$have_png"],
                [lib_test_result="$have_png12"]) 
          AS_IF([test "x$lib_test_result" = "xyes"],
                [AC_SUBST([PNG_LIBS])
                 MAYBE_OPTIONAL_DEPENDENCY([IL],
                                           [libpng]) ]) ]) 

AC_DEFUN([SETTLE_TIFF],
         [DEVIL_IL_LIB([tiffio.h],
                       [tiff],
                       [TIFF])
          lib_test_result="$have_tiff" 
          AS_IF([test "x$lib_test_result" = "xyes"],
                [AC_SUBST([TIFF_LIBS])
                 MAYBE_OPTIONAL_DEPENDENCY([IL],
                                           [libtiff]) ]) ]) 

AC_DEFUN([SETTLE_LIBWMP],
         [DEVIL_IL_LIB([WMPGlue.h],
                       [wmp],
                       [OTHERS])
          lib_test_result="$have_wmp" 
          AS_IF([test "x$lib_test_result" = "xyes"],
                [AC_SUBST([OTHERS_LIBS])
                 MAYBE_OPTIONAL_DEPENDENCY([IL],
                                           [libwmp]) ]) ]) 
dnl
dnl ILUT generic APIs checking
dnl
AC_DEFUN([TEST_API],
	 [AC_ARG_ENABLE([$1],
			[AC_HELP_STRING([--enable-$1],
					[Compiles $1 support for ILUT (default=yes) ])],
			[],
			[enable_$1="yes"]) ])

dnl Test for restrict c/c++ keyword
AC_DEFUN([DEVIL_CHECK_RESTRICT],
         [AC_MSG_CHECKING([restrict keyword])
	  AC_COMPILE_IFELSE([[void foo(void * restrict test){}]],
	            	    [AC_DEFINE([RESTRICT_KEYWORD],
				       [],
				       [restric keyword available])
                             restrict_keyword="found"
	                     AC_MSG_RESULT([yes]) ],
			    [AC_MSG_RESULT([no]) ]) ])

AC_DEFUN([DEVIL_CHECK_RESTRICT_GNU99],
         [TMP_CFLAGS="$CFLAGS"
          CFLAGS="$CFLAGS -std=gnu99 -fgnu89-inline"
          AC_MSG_CHECKING([restrict gnu99 keyword])
          AC_COMPILE_IFELSE([[void foo(void * restrict test){}]],
	                    [AC_DEFINE([RESTRICT_KEYWORD],
	  			       [1],
				       [restric keyword available])
	                     AC_MSG_RESULT([yes])
	                     CFLAGS="$TMP_CFLAGS"
                             ADD_CFLAGS(-std=gnu99 -fgnu89-inline)],
			    [AC_MSG_RESULT([no])
	                     CFLAGS="$TMP_CFLAGS"]) ])
