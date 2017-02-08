m4_include(../../m4/tlglobals.m4)

AC_INIT([terralib_examples_image_processing],TL_VERSION_MACRO)
AC_CONFIG_AUX_DIR([.])
AM_PROG_CC_C_O
AM_INIT_AUTOMAKE([subdir-objects])
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_LANG(C++)
AC_CONFIG_MACRO_DIR([m4])

# ------------------------------------------------------------
# Getting used defined options

m4_include(../../m4/tloptions.m4)

srcdirprefix="../"
bindirprefix="../"
m4_include(../../m4/tllink.m4)

# ------------------------------------------------------------
# Global examples defines

AM_CONDITIONAL([TE_BUILD_PDIEXAMPLE],[test x$tlpkgs_examples_status = xyes])

AM_CONDITIONAL([TE_BUILD_TIFFBASED_PDIEXAMPLE],[test x$tlpkgs_examples_status = xyes &&
test x$tlpkgs_geotiff_status = xyes])

AM_CONDITIONAL([TE_BUILD_JPEGBASED_PDIEXAMPLE],[test x$tlpkgs_examples_status = xyes &&
test x$tlpkgs_jpeg_status = xyes])

AM_CONDITIONAL([TE_BUILD_FFTWBASED_PDIEXAMPLE],[test x$tlpkgs_examples_status = xyes &&
test x$tlpkgs_fftw_status = xyes])

AC_DEFINE(
  [TE_DATA_PDIEXAMPLES_LOCALE],
  ["./resources/"],
  [This flag tells where is the base folder for the data used in examples]
)

AC_DEFINE(
  [TE_DATA_PDIEXAMPLES_BIN],
  ["../../bin/"],
  [This flag tells where is the base folder for the binaries used/generated in examples]
)

# ------------------------------------------------------------
# Ad-doc solution for incorrect examples using wrong include paths

AM_CPPFLAGS="-I$srcdir/source/base $AM_CPPFLAGS"

# Using the internal modified Frank-Shapelib
AM_CPPFLAGS="-I$srcdir/../../src/shapelib/ $AM_CPPFLAGS"

AM_CPPFLAGS="-I$srcdir/../../src/terralib/kernel/ $AM_CPPFLAGS"
AM_CPPFLAGS="-I$srcdir/../../src/terralib/image_processing $AM_CPPFLAGS"

# ------------------------------------------------------------
# Generate outputs

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
