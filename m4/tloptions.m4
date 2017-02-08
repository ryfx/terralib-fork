# ------------------------------------------------------------
# Debug build
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_debug_status=no

# Analysing the user options
AC_ARG_ENABLE( 
  [debug],
  [
    AS_HELP_STRING([--enable-debug],
      [Enable the Terralib debug mode build - default:disabled]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_debug_status],[yes])
    CXXFLAGS="$CXXFLAGS -g -Wall -O0"
    CFLAGS="$CFLAGS -g -Wall -O0"
    AC_MSG_NOTICE([Terralib debug mode build enabled])
  ],
  [
    CPPFLAGS="-DNDEBUG $CPPFLAGS"
    CXXFLAGS="$CXXFLAGS -w -O2"
    CFLAGS="$CFLAGS -w -O2"
    AC_MSG_NOTICE([Terralib debug mode build disabled])
  ]  
)

# ------------------------------------------------------------
# Shapelib support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_shapelib_status=no

# Analysing the user options
AC_ARG_ENABLE( 
  [shapelib],
  [
    AS_HELP_STRING([--enable-shapelib],
      [Enable the Terralib internal ShapeLib support - default:disabled]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_shapelib_status],[yes])
    AC_MSG_NOTICE([Terralib internal ShapeLib support enabled])
  ],
  [
    AC_MSG_NOTICE([Terralib internal ShapeLib support disabled])
  ]  
)

# ------------------------------------------------------------
# geotiff support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_geotiff_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_geotiff_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [geotiff],
  [
    AS_HELP_STRING([--with-geotiff<=PATH>],
      [Enable (default:disabled) the Terralib geotiff support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_geotiff_status],[check])
    AS_VAR_SET([tlpkgs_geotiff_arg],[$withval])
    LDFLAGS="-lgeotiff $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval -I$withval/include -I$withval/include/geotiff $CPPFLAGS"
        LDFLAGS="-L$withval -L$withval/lib $LDFLAGS"
      ]  
    )    
  ],
  [
    AS_VAR_SET([tlpkgs_geotiff_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_geotiff_status" == xcheck],
  [
    AC_CHECK_LIB(
      [geotiff],
      [XTIFFClientOpen],
      [
        AS_VAR_SET([tlpkgs_geotiff_status],[yes])
        AC_MSG_NOTICE([Terralib geotiff support enabled])
      ],
      [AC_MSG_FAILURE(geotiff not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib geotiff support disabled])
  ]
)

# ------------------------------------------------------------
# mysql support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_mysql_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_mysql_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [mysql],
  [
    AS_HELP_STRING([--with-mysql<=PATH>],
      [Enable (default:disabled) the Terralib mysql support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_mysql_status],[check])
    AS_VAR_SET([tlpkgs_mysql_arg],[$withval])
    LDFLAGS="-lmysqlclient $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval -I$withval/include $CPPFLAGS"
        LDFLAGS="-L$withval -L$withval/lib $LDFLAGS"
      ]  
    )    
  ],
  [
    AS_VAR_SET([tlpkgs_mysql_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_mysql_status" == xcheck],
  [
    AC_CHECK_LIB(
      [mysqlclient],
      [mysql_init],
      [
        AS_VAR_SET([tlpkgs_mysql_status],[yes])
        AC_MSG_NOTICE([Terralib mysql support enabled])
      ],
      [AC_MSG_FAILURE(mysql not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib mysql support disabled])
  ]
)

# ------------------------------------------------------------
# jpeg support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_jpeg_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_jpeg_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [jpeg],
  [
    AS_HELP_STRING([--with-jpeg<=PATH>],
      [Enable (default:disabled) the Terralib jpeg support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_jpeg_status],[check])
    AS_VAR_SET([tlpkgs_jpeg_arg],[$withval])
    LDFLAGS="-ljpeg $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval -I$withval/include $CPPFLAGS"
        LDFLAGS="-L$withval -L$withval/lib $LDFLAGS"
      ]  
    )    
  ],
  [
    AS_VAR_SET([tlpkgs_jpeg_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_jpeg_status" == xcheck],
  [
    AC_CHECK_LIB(
      [jpeg],
      [jpeg_read_header],
      [
        AS_VAR_SET([tlpkgs_jpeg_status],[yes])
        AC_MSG_NOTICE([Terralib jpeg support enabled])
      ],
      [AC_MSG_FAILURE(jpeg not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib jpeg support disabled])
  ]
)

# ------------------------------------------------------------
# CPPunit support options
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_cppunit_status=no
# Possible values: empty(default), the user given argument
tlpkgs_cppunit_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [cppunit],
  [
    AS_HELP_STRING([--with-cppunit<=PATH>],
      [Enable (default:disabled) the Terralib CPPUnit support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_cppunit_status],[check])
    AS_VAR_SET([tlpkgs_cppunit_arg],[$withval])
    LDFLAGS="-lcppunit $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval -I$withval/include $CPPFLAGS"
        LDFLAGS="-L$withval -L$withval/lib $LDFLAGS"
      ]  
    ) 
  ],
  [
    AS_VAR_SET([tlpkgs_cppunit_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_cppunit_status" == xcheck],
  [
    AC_CHECK_HEADERS(
      [cppunit/Outputter.h],
      [
        AS_VAR_SET([tlpkgs_cppunit_status],[yes])
        AC_MSG_NOTICE([Terralib CPPUnit support enabled])
      ],
      [AC_MSG_FAILURE(CPPUnit not found)],
      []
    )
  ],
  [
    AC_MSG_NOTICE([Terralib CPPUnit support disabled])
  ]
)

# ------------------------------------------------------------
# Examples build
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_examples_status=no

# Analysing the user options
AC_ARG_ENABLE( 
  [examples],
  [
    AS_HELP_STRING([--enable-examples],
      [Enable the Terralib examples build - default:disabled]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_examples_status],[yes])
    AC_MSG_NOTICE([Terralib examples build enabled])
  ],
  [
    AC_MSG_NOTICE([Terralib examples build disabled])
  ]  
)

# ------------------------------------------------------------
# fftw (Fast fourier transform library​) support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_fftw_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_fftw_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [fftw],
  [
    AS_HELP_STRING([--with-fftw<=PATH>],
      [Enable (default:disabled) the Terralib FFTW (Fast fourier transform library) support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_fftw_status],[check])
    AS_VAR_SET([tlpkgs_fftw_arg],[$withval])
    LDFLAGS="-lfftw3 $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval -I$withval/include $CPPFLAGS"
        LDFLAGS="-L$withval -L$withval/lib $LDFLAGS"
      ]  
    )    
  ],
  [
    AS_VAR_SET([tlpkgs_fftw_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_fftw_status" == xcheck],
  [
    AC_CHECK_LIB(
      [fftw3],
      [fftw_malloc],
      [
        AS_VAR_SET([tlpkgs_fftw_status],[yes])
        AC_MSG_NOTICE([Terralib FFTW support enabled])
      ],
      [AC_MSG_FAILURE(fftw not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib FFTW support disabled])
  ]
)

# ------------------------------------------------------------
# PostGIS support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_postgis_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_postgis_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [postgis],
  [
    AS_HELP_STRING([--with-postgis<=PATH>],
      [Enable (default:disabled) the Terralib PostGIS support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_postgis_status],[check])
    AS_VAR_SET([tlpkgs_postgis_arg],[$withval])
    LDFLAGS="-lpq $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval -I$withval/include $CPPFLAGS"
        LDFLAGS="-L$withval -L$withval/lib $LDFLAGS"
      ]  
    )    
  ],
  [
    AS_VAR_SET([tlpkgs_postgis_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_postgis_status" == xcheck],
  [
    AC_CHECK_LIB(
      [pq],
      [PQexec],
      [
        AS_VAR_SET([tlpkgs_postgis_status],[yes])
        AC_MSG_NOTICE([Terralib PostGIS support enabled])
      ],
      [AC_MSG_FAILURE(PostGIS not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib PostGIS support disabled])
  ]
)

# ------------------------------------------------------------
# GLU (OpenGL Utility Library support)
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_glu_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_glu_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [glu],
  [
    AS_HELP_STRING([--with-glu<=PATH>],
      [Enable (default:disabled) the Terralib GLU (OpenGL Utility Library) support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_glu_status],[check])
    AS_VAR_SET([tlpkgs_glu_arg],[$withval])
    LDFLAGS="-lGLU $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval -I$withval/include $CPPFLAGS"
        LDFLAGS="-L$withval -L$withval/lib $LDFLAGS"
      ]  
    )    
  ],
  [
    AS_VAR_SET([tlpkgs_glu_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_glu_status" == xcheck],
  [
    AC_CHECK_LIB(
      [GLU],
      [main],
      [
        AS_VAR_SET([tlpkgs_glu_status],[yes])
        AC_MSG_NOTICE([Terralib GLU support enabled])
      ],
      [AC_MSG_FAILURE(GLU not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib GLU support disabled])
  ]
)

# ------------------------------------------------------------
# QT support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_qt_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_qt_arg=""
# QT variables set (also exported to Makefile.am): 
#  QT_PATH - path to QT installed directory
#  MOC 
#  UIC 
#  RCC 

# Analysing the user options
AC_ARG_WITH( 
  [qt],
  [
    AS_HELP_STRING([--with-qt<=PATH>],
      [Enable (default:disabled) the Terralib QT support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_qt_status],[check])
    AS_VAR_SET([tlpkgs_qt_arg],[$withval])
    LDFLAGS="-lqt-mt $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval -I$withval/include $CPPFLAGS"
        LDFLAGS="-L$withval -L$withval/lib $LDFLAGS"
        QT_PATH="$withval"
        AC_MSG_NOTICE(Using QT path:$QT_PATH)
      ],
      [
        AS_IF(
          [test -d "$QTDIR"],
          [
            AC_MSG_NOTICE(Using QTDIR variable:$QTDIR)
            CPPFLAGS="-I$QTDIR/include $CPPFLAGS"
            LDFLAGS="-L$QTDIR/lib $LDFLAGS"
            QT_PATH="$QTDIR"
          ],
          [
            AC_MSG_NOTICE(QTDIR environment variable not present. The system default paths will be used.)
          ]
        )
      ]
    )    
  ],
  [
    AS_VAR_SET([tlpkgs_qt_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_qt_status" == xcheck],
  [
    #Checking for the GLU library
    AS_IF(
      [test "x$tlpkgs_glu_status" == xyes],
      [
        AC_MSG_NOTICE([Terralib QT GLU support enabled])
      ],
      [
        AC_MSG_FAILURE(TerraLib GLU support must be enabled)
      ]
    )
    # Checking the QT Library
    AC_CHECK_LIB(
      [qt-mt],
      [main],
      [
        # ----------------------------------------------------
        # QT flags
        CPPFLAGS="-DQT_SHARED -DQT_NO_DEBUG -DQT_THREAD_SUPPORT -D_REENTRANT $CPPFLAGS"
        # ----------------------------------------------------
        # Find moc (Meta Object Compiler).
        AC_ARG_VAR([MOC], [Qt Meta Object Compiler command])
        AC_PATH_PROGS([MOC], [moc moc-qt4 moc-qt3], [missing],
                      [$QT_PATH:$QT_PATH/bin]:$PATH)
        if test x"$MOC" = xmissing; then
          AC_MSG_ERROR([Cannot find moc (Meta Object Compiler) in your PATH. Try using --with-qt.])
        fi
        # ----------------------------------------------------
        # Find uic (User Interface Compiler).
        AC_ARG_VAR([UIC], [Qt User Interface Compiler command])
        AC_PATH_PROGS([UIC], [uic uic-qt4 uic-qt3 uic3], [missing],
                      [$QT_PATH:$QT_PATH/bin:$PATH])
        if test x"$UIC" = xmissing; then
          AC_MSG_ERROR([Cannot find uic (User Interface Compiler) in your PATH. Try using --with-qt.])
        fi
        # ----------------------------------------------------
        # Find rcc (Qt Resource Compiler).
        AC_ARG_VAR([RCC], [Qt Resource Compiler command])
        AC_PATH_PROGS([RCC], [rcc], [false], [$QT_PATH:$QT_PATH/bin:$PATH])
        if test x"$UIC" = xfalse; then
          AC_MSG_WARN([Cannot find rcc (Qt Resource Compiler) in your PATH. Try using --with-qt.])
        fi    
        # ----------------------------------------------------       
        AS_VAR_SET([tlpkgs_qt_status],[yes])
        AC_MSG_NOTICE([Terralib QT support enabled])
      ],
      [AC_MSG_FAILURE(QT not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib QT support disabled])
  ]
)

# ------------------------------------------------------------
# MrSID support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_mrsid_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_mrsid_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [mrsid],
  [
    AS_HELP_STRING([--with-mrsid<=PATH>],
      [Enable (default:disabled) the Terralib MrSID support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_mrsid_status],[check])
    AS_VAR_SET([tlpkgs_mrsid_arg],[$withval])
    LDFLAGS="-lltidsdk $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval -I$withval/include $CPPFLAGS"
        LDFLAGS="-L$withval -L$withval/lib $LDFLAGS"
      ]  
    )
  ],
  [
    AS_VAR_SET([tlpkgs_mrsid_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_mrsid_status" == xcheck],
  [
    AC_CHECK_LIB(
      [ltidsdk],
      [main],
      [
        AS_VAR_SET([tlpkgs_mrsid_status],[yes])
        AC_MSG_NOTICE([Terralib MrSID support enabled])
      ],
      [AC_MSG_FAILURE(ltidsd MrSID not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib MrSID support disabled])
  ]
)

# ------------------------------------------------------------
# GDAL support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_gdal_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_gdal_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [gdal],
  [
    AS_HELP_STRING([--with-gdal<=PATH>],
      [Enable (default:disabled) the Terralib GDAL support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_gdal_status],[check])
    AS_VAR_SET([tlpkgs_gdal_arg],[$withval])
    LDFLAGS="-lgdal $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval -I$withval/include $CPPFLAGS"
        LDFLAGS="-L$withval -L$withval/lib $LDFLAGS"
      ]  
    )
  ],
  [
    AS_VAR_SET([tlpkgs_gdal_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_gdal_status" == xcheck],
  [
    AC_CHECK_LIB(
      [gdal],
      [main],
      [
        AS_VAR_SET([tlpkgs_gdal_status],[yes])
        AC_MSG_NOTICE([Terralib GDAL support enabled])
      ],
      [AC_MSG_FAILURE(GDAL not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib GDAL support disabled])
  ]
)

# ------------------------------------------------------------
# NetPBM support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_netpbm_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_netpbm_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [netpbm],
  [
    AS_HELP_STRING([--with-netpbm<=PATH>],
      [Enable (default:disabled) the Terralib LibNetPBM support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_netpbm_status],[check])
    AS_VAR_SET([tlpkgs_netpbm_arg],[$withval])
    LDFLAGS="-lnetpbm $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval -I$withval/include $CPPFLAGS"
        LDFLAGS="-L$withval -L$withval/lib $LDFLAGS"
      ]  
    )
  ],
  [
    AS_VAR_SET([tlpkgs_netpbm_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_netpbm_status" == xcheck],
  [
    AC_CHECK_LIB(
      [netpbm],
      [main],
      [
        AS_VAR_SET([tlpkgs_netpbm_status],[yes])
        AC_MSG_NOTICE([Terralib LibNetPBM support enabled])
      ],
      [AC_MSG_FAILURE(LibNetPBM not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib LibNetPBM support disabled])
  ]
)

# ------------------------------------------------------------
# QWT support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_qwt_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_qwt_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [qwt],
  [
    AS_HELP_STRING([--with-qwt<=PATH>],
      [Enable (default:disabled) the Terralib QWT support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_qwt_status],[check])
    AS_VAR_SET([tlpkgs_qwt_arg],[$withval])
    LDFLAGS="-lqwt $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval -I$withval/include $CPPFLAGS"
        LDFLAGS="-L$withval -L$withval/lib $LDFLAGS"
      ]  
    )
  ],
  [
    AS_VAR_SET([tlpkgs_qwt_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_qwt_status" == xcheck],
  [
    AS_IF(
      [test "x$tlpkgs_qt_status" == xyes],
      [  
        AC_CHECK_LIB(
          [qwt],
          [main],
          [
            AS_VAR_SET([tlpkgs_qwt_status],[yes])
            AC_MSG_NOTICE([Terralib QWT support enabled])
          ],
          [AC_MSG_FAILURE(QWT not found)]
        )
      ],
      [AC_MSG_FAILURE(QT support must be enabled to use QWT)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib QWT support disabled])
  ]
)
# ------------------------------------------------------------
# OpenMP support
# These are the global variables set by reading the user input
# parameters:
# Possible values: no(default), yes
tlpkgs_openmp_status=no
# Possible values: empty(default), path to the user given argument
tlpkgs_openmp_arg=""

# Analysing the user options
AC_ARG_WITH( 
  [openmp],
  [
    AS_HELP_STRING([--with-openmp<=PATH>],
      [Enable (default:disabled) the Terralib OpenMP support using the library installed into the system default path or the library installed into the given optional PATH]
    )
  ],
  [
    AS_VAR_SET([tlpkgs_openmp_status],[check])
    AS_VAR_SET([tlpkgs_openmp_arg],[$withval])
    LDFLAGS="-lgomp $LDFLAGS"
    AS_IF(
      [test "x$withval" != xyes],
      [
        CPPFLAGS="-I$withval -I$withval/include $CPPFLAGS"
        LDFLAGS="-L$withval -L$withval/lib $LDFLAGS"
      ]  
    )
  ],
  [
    AS_VAR_SET([tlpkgs_openmp_status],[no])
  ]
)

#Checking for the required library
AS_IF(
  [test "x$tlpkgs_openmp_status" == xcheck],
  [
    AC_CHECK_LIB(
      [gomp],
      [GOMP_parallel_start],
      [
        AS_VAR_SET([tlpkgs_openmp_status],[yes])
        AC_MSG_NOTICE([Terralib OpenMP support enabled])
      ],
      [AC_MSG_FAILURE(OpenMP not found)]
    )
  ],
  [
    AC_MSG_NOTICE([Terralib OpenMP support disabled])
  ]
)


