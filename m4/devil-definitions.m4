
AC_DEFUN([ADD_CFLAGS],
	 [GENERAL_CFLAGS="$GENERAL_CFLAGS $1"])

dnl 
dnl Adds CFLAGS to the appropriate module, good since it stores stuff 
dnl so it can be accessed on other places in the configure script
dnl Usage:
dnl 	ADD_CFLAGS_MODULE(<CFLAGS>, <class name>)
dnl Example:
dnl 	ADD_CFLAGS_MODULE([-O0 -g], [jpeg])
AC_DEFUN([ADD_CFLAGS_MODULE],
	 [STR_TO_INDEX([CLASS_NAMES], [$2], [index])
	  [eval MODULES_CFLAGS_$index=\"\$MODULES_CFLAGS_$index \$1\"]
	  TO_UPPERCASE([$2])_CFLAGS="${TO_UPPERCASE([$2])_CFLAGS} $1" ])

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

dnl there must be NO SPACE between the ']); and ending '])'
AC_DEFUN([TO_UPPERCASE],
	 [m4_translit([$1], [a-z], [A-Z])])

dnl
dnl Modules
dnl
dnl Usage: 
dnl 	ADD_CLASS(<name>, <enabled by default?>, <short description>)
dnl Concrete example:
dnl 	ADD_CLASS([base], [yes], [Basic must-have supported formats that don't have external dependencies])
dnl 	echo "The first added class: ${CLASS_NAMES[0]} -- ${CLASS_DESCRIPTIONS_0}"
dnl dnl 	echo "The first added class: ${CLASS_NAMES[0]} -- ${CLASS_DESCRIPTIONS[0]}"
dnl
NUM_CLASSES=0
AC_DEFUN([ADD_CLASS],	 
	 [dnl We set the number of classes array size for the first time
          dnl[NUM_CLASSES=${#CLASS_NAMES[@]}]
	  dnl Do we really want to add this class?
	  AC_ARG_ENABLE([$1],
			[AC_HELP_STRING([--enable-$1],
					[Compile the $1 class. $3 (default=$2) ]) ],
		        [],
			[enable_$1="$2"]) 
	  test -z "$NUM_CLASSES" && NUM_CLASSES=0
	  $1_index="$NUM_CLASSES"
	  dnl Automake conditional telling to make that we should bother to compile the thing.
	  dnl TODO: don't compile it if it does not contain any formats...
	  AM_CONDITIONAL(TO_UPPERCASE([BUILD_$1]),
			 [test "x$enable_$1" = "xyes" ])
	  AC_MSG_CHECKING([whether we would like to build the '$1' class])
          AS_IF([test "x$enable_$1" = "xyes"],
                [dnl Yes, we want to add a new class!
		 dnl CLASS_NAMES[[$$1_index]]="$1"
		 param="$1"
		 eval CLASS_NAMES_$$1_index=\$param
		 dnl CLASS_DESCRIPTIONS[[$$1_index]]="$3"
		 param="$3"
		 eval "CLASS_DESCRIPTIONS_$$1_index=\"\$param\""
		 dnl CLASS_DEFINE[[$$1_index]]="TO_UPPERCASE([$1_CLASS])"
		 eval "CLASS_DEFINE_$$1_index=\"TO_UPPERCASE([$1_CLASS])\""
		 eval "class_formats=\"\$CLASS_FORMATS_$$1_index\""
dnl	  	 AC_DEFINE_UNQUOTED(TO_UPPERCASE([$1_CLASS])_FORMATS,
				    dnl [[${CLASS_FORMATS[$$1_index]}]],
				    dnl This can't be abbreviated...
dnl				    [$(eval "AS_ECHO_N([\"\$CLASS_FORMATS$$1_index\"])")],
dnl				    [Formats we support])
		 dnl We also want to substitute its name in Makefile.am and friends
		 eval "subst_$1_CLASS=\"\${lib_prefix}\${CLASS_NAMES_$$1_index}\""
		 AC_SUBST( TO_UPPERCASE([$1_CLASS]),
			  [$subst_$1_CLASS ])
			  dnl [[${lib_prefix}${CLASS_NAMES[$$1_index]} ]])
		 AC_SUBST( TO_UPPERCASE([$1_CPPFLAGS]))
		 AC_SUBST( TO_UPPERCASE([$1_CFLAGS]))
		 AC_SUBST( TO_UPPERCASE([$1_CXXFLAGS]))
		 AC_SUBST( TO_UPPERCASE([$1_LIBS]))
		 AC_SUBST( TO_UPPERCASE([$1_LDFLAGS]))
		 NUM_CLASSES=$(expr $NUM_CLASSES '+' 1)
		 AC_MSG_RESULT([yes]) ], 
                [AC_MSG_RESULT([no]) ]) ])

dnl
dnl Converts string to an index in an array. 
dnl If the string matches to an array member, the index is returned.
dnl Usage: 
dnl 	STR_TO_INDEX(<array>, <string>, <index>)
dnl Example:
dnl 	STR_TO_INDEX([CLASS_NAMES], [base], [index])
dnl 	CLASS_FORMATS[$index]="CLASS_FORMATS[$index] JPG"
dnl
AC_DEFUN([STR_TO_INDEX],
	 [dnl I think that this is called "making STR_TO_INDEX reentrant"
	  $3=""
	  loop_index=0
	  dnl for loop_i in "${$1[[@]]}"
	  dnl for ii in  $(seq 0 $NUM_CLASSES)
	  ii=0
	  while test $ii -lt $NUM_CLASSES
	  do
		dnl Yes, we have it! Let's assign it then...
		dnl test "x$loop_i" = "x$2" && $3=$loop_index;
		dnl (( loop_index++ ))
		eval "test \"x\$$1_$ii\" = \"x$2\"" && $3=$ii;
		ii=$(expr $ii + 1)
	  done ])

dnl
dnl We may want to know what CFLAGS / LIBS have we used to build modules
dnl So let's make a macro that does that...
dnl Usage:
dnl 	DEFINE_MODULES_FLAGS([<CFLAGS or LIBS?>], [<name of the #define>])
dnl Example:
dnl 	DEFINE_MODULES_FLAGS([CFLAGS])
dnl creates MODULES_CFLAGS #define
dnl 	DEFINE_MODULES_FLAGS([LIBS])
dnl creates MODULES_LIBS #define
dnl
AC_DEFUN([DEFINE_MODULES_FLAGS],
	 [dnl Dont mess with [ and ] brackets if you don't know what are you doing
	  OUT_MODULES_$1=""
	  dnl Don't do anything if there is no information to tell (in the quoted for loop)
	  dnl [for ((i = 0; i < ${#CLASS_NAMES[@]}; i++))]
	  i=0
	  while test $i -lt $NUM_CLASSES
	  dnl for i in $(seq 0 $NUM_CLASSES)
	  do
		dnl test "x${MODULES_$1[$i]}" != x && OUT_MODULES_$1="${OUT_MODULES_$1} ${CLASS_NAMES[$i]}: ${MODULES_$1[$i]} ;"
		eval "test \"x\$MODULES_$1_$i\" != x" && dnl
			eval "OUT_MODULES_$1=\"\$OUT_MODULES_$1 \$CLASS_NAMES_$i: \$MODULES_$1_$i ;\""
		i=$(expr $i + 1)
	  done
	  AC_DEFINE_UNQUOTED([MODULES_$1_STR], 
			     ["$OUT_MODULES_$1"],
			     [Should be $1 used to build modules]) ])

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
	 dnl If the fourth argument was passed (typically a check macro) and if we want the format, execute it..
         AS_IF([test $# -eq 4 -a "x$enable_$1" = "xyes"],
               [$4]) 
	 dnl Add the format name to the list of mentioned formats. We will take some actions on them even if we won't support them...
	 MENTIONED_FORMATS="$MENTIONED_FORMATS $2"
         AC_MSG_CHECKING([whether we would like to have support for $1 format])
         AS_IF([test "x$enable_$1" = "xno" -o "x$lib_test_result" = "xno"],
               [AC_MSG_RESULT([no])
                dnl Let's tell everybody that we don't want this format
                AC_DEFINE([IL_NO_$2],
                          [1],
                          [$1 support ($3) ]) ],
               [dnl Magically detect what class does the module belong to...
                DETECT_FORMAT_CLASS([$2])
		dnl the $CLASSNAME comes from the previous macro..
		STR_TO_INDEX([CLASS_NAMES], [$CLASSNAME], [index])
		dnl Have we found the index?
		AS_IF([test -z "$index" ],
		      [AC_MSG_RESULT([no])
			AC_DEFINE([IL_NO_$2],
				  [1],
				  [$1 support ($3) ])
	 	       AS_IF([test -z "$CLASSNAME"],
			     [dnl The class name is actually unknown - someone should update the build system then...
			      AC_MSG_WARN([The $2 format has trouble with the class it should belong into]) ],
		             [AC_MSG_NOTICE([The $CLASSNAME module/class won't be built, so the $1 format support won't be compiled in]) ]) ],
                      [dnl Amazing, everything went +- fine...
                       AC_MSG_RESULT([yes]) 
                       dnl Append the list of formats in the same class. Carefully with m4 quoting!
		       dnl [CLASS_FORMATS[$index]="${CLASS_FORMATS[$index]}] $2" 
		       eval "CLASS_FORMATS_$index=\"\$CLASS_FORMATS_$index $2\"" 
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
	  [PATH_TO_SOME_DEFINITION=$($GREP -l 'il[^_]*_$1[[:blank:]]*([^;(]*$' ${srcdir}/src-IL/src/*.c ${srcdir}/src-IL/src/*.cpp) ]
	  dnl To partially fix parenthesis matching for the editor: )
	  dnl Oh yes, now we take only the filename without the path to it (we have to chop everything before the last '/')
	  dnl Note: protective quoting '[' ...  ']' is needed here.
          [ SOME_DEFN_FILENAME=$(echo $PATH_TO_SOME_DEFINITION | sed -e 's|.*/\([^/]*\)|\1|') ]
	  dnl now we have to get rid of the "\\\n" characters. To be honest I don't feel comfortably with that command.
	  [ CLASSNAME=$(sed -e :a -e '/\\$/N; s/\\\n[[:blank:]]*/ /; ta' ${srcdir}/lib/IL.am] dnl
	  | $GREP $SOME_DEFN_FILENAME dnl 
	  dnl Get the right line, there can be more of them
	  [| sed -n '/@\([A-Z0-9]*\)_/p'] dnl 
	  dnl Finally extract the classname.
          [| sed -e "s/[^@]*@\([A-Z0-9]*\)_.*/\1/" ]  dnl
	  dnl Output it in lowercase...
          | tr A-Z a-z ) ])

dnl 
dnl This macro appends the stuff to _REQUIRED suffixed variables
dnl Used standalone and in MAYBE_OPTIONAL_DEPENDENCY
dnl
AC_DEFUN([REQUIRED_DEPENDENCY],
	 [AS_IF([test "x$lib_test_result" != "xno"],
		[$1_LIBS_REQUIRED="${$1_LIBS_REQUIRED} $2"],
		[$1_LIBS_MISSING_REQUIRED="${$1_LIBS_MISSING_REQUIRED} $2"]) ])

dnl 
dnl This macro appends the stuff to _OPTIONAL suffixed variables
dnl Used standalone and in MAYBE_OPTIONAL_DEPENDENCY
dnl
AC_DEFUN([OPTIONAL_DEPENDENCY],
	 [AS_IF([test "x$lib_test_result" != "xno"],
		[$1_LIBS_OPTIONAL="${$1_LIBS_OPTIONAL} $2"],
		[$1_LIBS_MISSING_OPTIONAL="${$1_LIBS_MISSING_OPTIONAL} $2"]) ])
dnl
dnl Adds a library to human-readable list of dependencies
dnl It tries to look whether we actually have the library 
dnl and then appends its name to a variable depending on whether 
dnl we have a modular or static build going on
dnl
dnl Usage:
dnl MAYBE_OPTIONAL_DEPENDENCY(<library part prefix>, <library name> [, <module name>])
dnl for example:
dnl MAYBE_OPTIONAL_DEPENDENCY([IL], [libjpeg], [base])
dnl MAYBE_OPTIONAL_DEPENDENCY([ILUT], [allegro])
dnl
AC_DEFUN([MAYBE_OPTIONAL_DEPENDENCY],
         [AS_IF([test "x$enable_modules" = "xyes"],
		[OPTIONAL_DEPENDENCY([$1], [$2]) ],
	        [REQUIRED_DEPENDENCY([$1], [$2]) ]) 
	  dnl If we have supplied the module name, just append the lib name into the module libs array
	  dnl so they are easily accessible later if we need it
	  AS_IF([test $# = 3],
		[STR_TO_INDEX([CLASS_NAMES], [$3], [index]) 
		 eval "MODULES_LIBS_$index=\"\${MODULES_LIBS_$index} \$TO_UPPERCASE([$3])_LIBS\"" ]) ])
		dnl  MODULES_LIBS[[$index]]="${MODULES_LIBS[[$index]]} $TO_UPPERCASE([$3])_LIBS" ]) ])

dnl
dnl Check for libraries
dnl
dnl Usage:
dnl DEVIL_LIB(<include>, <library>, <code>, <class name>[, define])
dnl 	the <library> is appended to <class_name>_LIBS, sets have_<library> to yes/no
dnl dnl If we don't detect the LIB, optionally #define <class_name>_NO_<define>
dnl Nothing else is done, see MAYBE_OPTIONAL_DEPENDENCY macro...
dnl Use 'IL' as class if it is for the main library. You should be able to use 'ILU' and 'ILUT' as well...
dnl Example:
dnl 	DEVIL_LIB([jpeglib.h], [jpeg], [jpeg_read_header(NULL, 0)], [JPEG])
dnl
AC_DEFUN([DEVIL_LIB],
         [AC_ARG_WITH([$2-prefix],
		      [AS_HELP_STRING([--with-$2-prefix],
				      [Use if you need to specify the prefix where the $2 library (from $4 class) is installed]) ],
		      [$4_CPPFLAGS="-I $with_$2_prefix ${$4_CPPFLAGS}"
		       $4_LDFLAGS="-L $with_$2_prefix ${$4_LDFLAGS}"])
	  dnl We create a special test environement and we restore the original at the end
	  OLD_CPPFLAGS=$CPPFLAGS
	  CPPFLAGS="${$4_CPPFLAGS} $CPPFLAGS"
	  OLD_LDFLAGS=$LDFLAGS
	  LDFLAGS="${$4_LDFLAGS} $LDFLAGS"
	  CHECK_LIBS_EX([$2], [<$1>], [$3], [${$4_LDFLAGS} -l$2], [${$4_CPPFLAGS}])
	  dnl I have thought of emptying those LDFLAGS and CPPFLAGS if the lib is not found, but 
	  dnl it is not a good idea.
	  dnl Does anyone know why? It actually seems to be a very good idea!
dnl	  AC_CHECK_HEADER([$1],
dnl			  [AC_CHECK_LIB([$2],
dnl                                     [main],
dnl                                     [$4_LIBS="-l$2 ${$4_LIBS}"
dnl                                      have_$2="yes"
dnl				 dnl Add lib to the array of libs
dnl				 ],
dnl                                      [have_$2="no"]) ],
dnl			  [have_$2="no"])
	  AS_IF([test "x${$2_links}" = "xyes"],
		[have_$2="yes"
		 $4_LIBS="-l$2 ${$4_LIBS}"],
		[have_$2="no"])
	  lib_test_result="$have_$2"
	  CPPFLAGS=$OLD_CPPFLAGS
	  LDFLAGS=$OLD_LDFLAGS
	  AS_IF([test "x$have_$2" = "xno"],
	        [AS_IF([test "$#" = "5"],
	               [AC_DEFINE([$4_NO_$5],
				  [],
				  [$2 support ]) ])
	         dnl Clear provided flags if they are usless here
	         dnl They could confuse the rest
	         $4_LDFLAGS=""
	         $4_CPPFLAGS=""]) ])

dnl
dnl Checks for squish library = GPU accelerated DXT compression
dnl Can be used along with nvidia texture tools
dnl
AC_DEFUN([DEVIL_CHECK_LIBSQUISH],
         [AC_LANG_PUSH([C++])
          DEVIL_LIB([squish.h],
                    [squish],
                    [;],
		    [DDS])
          lib_test_result="$have_squish"
          AS_IF([test "x$lib_test_result" = "xyes"],
                [AC_DEFINE([IL_USE_DXTC_SQUISH],
                           [1],
                           [Define if you have libsquish installed]) 
                 MAYBE_OPTIONAL_DEPENDENCY([IL], 
                                           [libsquish]) ])
          AC_LANG_POP([C++]) ])

dnl
dnl Checks for nvidia texture tools library - GPU acceleration of DXT compression
dnl Can be used along with libsquish
dnl 
AC_DEFUN([DEVIL_CHECK_NVIDIA_TEXTOOLS],
         [AC_LANG_PUSH([C++])
          DEVIL_LIB([nvtt/nvtt.h],
                    [nvtt],
                    [;],
		    [DDS])
          lib_test_result="$have_nvtt"
	  MAYBE_OPTIONAL_DEPENDENCY([IL], 
				    [nvidia_texture_tools])
          AS_IF([test "x$lib_test_result" = "xyes"],
                [AC_DEFINE([IL_USE_DXTC_NVIDIA],
                           [1],
                           [Define if you have nvidia texture tools library installed]) 
                 MAYBE_OPTIONAL_DEPENDENCY([IL], 
                                           [libnvtt-nvidia_texture_tools]) ])
          AC_LANG_POP([C++]) ])

dnl LCMS specific: The headers can be in 'lcms' include subdirectory.
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
		 lib_test_result="yes"],
		[lib_test_result="no"
		 AC_DEFINE([IL_NO_LCMS],
			   [1],
			   [We don't have LCMS]) ])
	  REQUIRED_DEPENDENCY([IL], 
			      [lcms]) ])

AC_DEFUN([SETTLE_EXR],
         [AC_LANG_PUSH([C++])
          PKG_CHECK_MODULES([OPENEXR], 
                            [OpenEXR],
                            [have_openexr="maybe"],
                            [have_openexr="no"])
	  AS_IF([test "x$have_openexr" = "xmaybe"],
	        [CHECK_LIBS_EX([exr], [<ImfIO.h>], [;], [$OPENEXR_LIBS], [$OPENEXR_CFLAGS]) ]) 
	  AS_IF([test "x$exr_links" = "xyes"],
		[have_openexr="yes"
		 EXR_LIBS="$OPENEXR_LIBS"
		 ADD_CFLAGS_MODULE([$OPENEXR_CFLAGS],
				   [exr])
		 dnl Add -pthreads or alternatives to the whole chain. Double quoted
		 [IL_LIBS="$IL_LIBS $(echo $EXR_LIBS | $GREP -o -e '-[^ ]*thread[^ ]*')"] ],
		[have_openexr="no"])
          lib_test_result="$have_openexr"
          MAYBE_OPTIONAL_DEPENDENCY([IL],
                                    [OpenEXR],
				    [exr])
          AC_LANG_POP([C++]) ])

AC_DEFUN([SETTLE_JPEG],
         [DEVIL_LIB([jpeglib.h],
                    [jpeg], 
                    [jpeg_read_header(NULL, 0);], 
		    [JPEG])
          AC_DEFINE([IL_USE_JPEGLIB_UNMODIFIED],
                    [1],
                    [Use libjpeg without modification. always enabled.])
          lib_test_result="$have_jpeg"
	  MAYBE_OPTIONAL_DEPENDENCY([IL],
				    [libjpeg],
				    [jpeg]) ])

AC_DEFUN([SETTLE_JASPER],
         [DEVIL_LIB([jasper/jasper.h],
                    [jasper],
                    [;],
                    [JP2])
          AS_IF([test "x$have_jasper" != "xyes"],
                [DEVIL_LIB([jasper/jasper.h],
                           [jp2],
                           [;],
                           [JP2]) ],
                [lib_test_result="yes"]) 
	 MAYBE_OPTIONAL_DEPENDENCY([IL],
				   [JasPer],
				   [jp2]) ])

AC_DEFUN([SETTLE_MNG],
         [DEVIL_LIB([libmng.h],
                    [mng],
                    [;],
                    [MNG])
	  MAYBE_OPTIONAL_DEPENDENCY([IL],
				    [libmng],
				    [mng]) ])

dnl PNG specific: The library could be just 'libpng' or 'libpng12'
AC_DEFUN([SETTLE_PNG],
         [PKG_CHECK_MODULES([LIBPNG], 
                            [libpng],
                            [have_libpng="maybe"],
                            [have_libpng="no"])
	  AS_IF([test "x$have_libpng" == "xmaybe"],
		[CHECK_LIBS_EX([png], [<png.h>], [png_sig_cmp(NULL, 0, 8);], [$LIBPNG_LIBS], [$LIBPNG_CFLAGS]) ]) 
		 AS_IF([test "x$png_links" = "xyes"],
		       [have_libpng="yes"
			PNG_LIBS="$LIBPNG_LIBS"
			ADD_CFLAGS_MODULE([$PNG_CFLAGS],
					  [png]) ])

	  AS_IF([test "x$have_libpng" != "xyes"],
                [DEVIL_LIB([png.h],
                           [png], 
                           [png_sig_cmp(NULL, 0, 8);], 
                           [PNG]) 
                 lib_test_result="$have_libpng"])
	  MAYBE_OPTIONAL_DEPENDENCY([IL],
				    [libpng],
				    [png]) ])

AC_DEFUN([SETTLE_TIFF],
         [DEVIL_LIB([tiffio.h],
                    [tiff],
                    [TIFFOpen(NULL, NULL);],
                    [TIFF])
	  MAYBE_OPTIONAL_DEPENDENCY([IL],
				    [libtiff],
				    [tiff]) ])

AC_DEFUN([SETTLE_LIBWMP],
         [DEVIL_LIB([WMPGlue.h],
                    [wmp],
                    [;],
                    [OTHERS])
	  MAYBE_OPTIONAL_DEPENDENCY([IL],
				    [libwmp]) ])

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
                             ADD_CFLAGS([-std=gnu99 -fgnu89-inline]) ],
			    [AC_MSG_RESULT([no])
	                     CFLAGS="$TMP_CFLAGS"]) ])
