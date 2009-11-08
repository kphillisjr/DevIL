AC_DEFUN([IL_REPORT],
	 [AC_MSG_NOTICE([+----------------------------------------+])
	  AS_IF([test "x$enable_IL" = "xyes"],
		[AC_MSG_NOTICE([\| IL library  Supported formats \|])
		 AC_MSG_NOTICE([+----------------------------------------+-------+])
		 AC_MSG_NOTICE([$SUPPORTED_FORMATS]) 
		 AC_MSG_NOTICE([+----------------------------------------+])
		 AC_MSG_NOTICE([\| IL library external dependencies \|]) 
		 AC_MSG_NOTICE([+----------------------------------------+-------+])
		 AC_MSG_NOTICE([Libs required during runtime: $IL_LIBS_REQUIRED])
		 test -n "$IL_LIBS_MISSING_REQUIRED" && AC_MSG_NOTICE([Libraries that could be used but are missing: $IL_LIBS_MISSING_REQUIRED])
 		 AS_IF([test "x$enable_modules" = "xyes"],
		       [AC_MSG_NOTICE([+----------------------------------------+-------+])
			AC_MSG_NOTICE([Optional dependencies: $IL_LIBS_OPTIONAL])
			test -n "$IL_LIBS_MISSING_OPTIONAL" && AC_MSG_NOTICE([Missing optional dependencies: $IL_LIBS_MISSING_OPTIONAL]) ]) ],
		[AC_MSG_NOTICE([\| IL disabled (huh? good luck!) \|]) ]) ])

AC_DEFUN([ILU_REPORT],
[AC_MSG_NOTICE([+----------------------------------------+])
AS_IF([test "x$enable_ILU" = "xyes"],
      [AC_MSG_NOTICE([\|      ILU part enabled        \|]) ],
      [AC_MSG_NOTICE([\|      ILU part disabled       \|]) 
       AC_MSG_NOTICE([+----------------------------------------+-------+])
       AC_MSG_NOTICE([Pass --enable-ILU option to the configure script if you want to build ILU]) ]) ])

AC_DEFUN([ILUT_REPORT],
[AC_MSG_NOTICE([+----------------------------------------+])
AS_IF([test "x$enable_ILUT" = "xyes"],
      [AC_MSG_NOTICE([\| ILUT library  Supported APIs \|])
       AC_MSG_NOTICE([+----------------------------------------+-------+])
       AC_MSG_NOTICE([$SUPPORTED_API])],
      [AC_MSG_NOTICE([\|      ILUT part disabled      \|])
       AC_MSG_NOTICE([+----------------------------------------+-------+])
       AC_MSG_NOTICE([Pass --enable-ILUT option to the configure script if you want to build ILUT]) ]) ])

AC_DEFUN([EXTENSIONS_REPORT],
[AC_MSG_NOTICE([+----------------------------------------+])
       AC_MSG_NOTICE([\| Detected Machine Extensions  \|])
AC_MSG_NOTICE([+----------------------------------------+-------+])
AS_IF([test "x$enable_debug" = "xyes"],
[AC_MSG_NOTICE([Debug mode: Machine extensions are (or at least should be) disabled]) ],
[AC_MSG_NOTICE([$SUPPORTED_EXTENSION]) ]) ])

AC_DEFUN([EXAMPLES_REPORT],
[AC_MSG_NOTICE([+----------------------------------------+])
AS_IF([test "x$with_examples" = "xyes"],
      [AC_MSG_NOTICE([\|      Building examples       \|]) 
       AC_MSG_NOTICE([+----------------------------------------+-------+])
       AC_MSG_NOTICE([$EXAMPLES_TO_BUILD])
       AS_IF([test "x$build" != "x$host"],
             [AC_MSG_NOTICE([Note: You may get some errors when cross-compiling when using OpenEXR, Allegro and/or SDL. This is because you have to set up pkg-config to be able to use it for cross-compilation. You can also disable build of problematic parts, see ./configure --help]) ]) ],
      [AC_MSG_NOTICE([\|  Not building any examples   \|]) 
       AC_MSG_NOTICE([+----------------------------------------+-------+])
       AC_MSG_NOTICE([Pass --with-examples option to the configure script if you want to build examples.]) ])
AC_MSG_NOTICE([+----------------------------------------+]) ])
