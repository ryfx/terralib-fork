m4_include(../m4/tlglobals.m4)

AC_INIT([terralib_examples],TL_VERSION_MACRO)
AC_CONFIG_AUX_DIR([.])
AM_PROG_CC_C_O
AM_INIT_AUTOMAKE([subdir-objects])
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_LANG(C++)
AC_CONFIG_MACRO_DIR([m4])

# ------------------------------------------------------------
# Getting used defined options

m4_include(../m4/tloptions.m4)
m4_include(../m4/tllink.m4)

# ------------------------------------------------------------
# Global examples defines

AM_CONDITIONAL([TE_BUILD_EXAMPLE],[test x$tlpkgs_examples_status = xyes])

AM_CONDITIONAL([TE_BUILD_SHAPELIB_BASED_EXAMPLE],[
test x$tlpkgs_examples_status = xyes && test x$tlpkgs_shapelib_status = xyes])

AM_CONDITIONAL([TE_BUILD_MYSQL_BASED_EXAMPLE],[
test x$tlpkgs_examples_status = xyes && test x$tlpkgs_mysql_status = xyes])

AM_CONDITIONAL([TE_BUILD_POSTGIS_BASED_EXAMPLE],[
test x$tlpkgs_examples_status = xyes && test x$tlpkgs_postgis_status = xyes])

AM_CONDITIONAL([TE_BUILD_OPENMP_BASED_EXAMPLE],[
test x$tlpkgs_examples_status = xyes && test x$tlpkgs_openmp_status = xyes])

AM_CONDITIONAL([TE_BUILD_JPEG_BASED_EXAMPLE],[
test x$tlpkgs_examples_status = xyes && test x$tlpkgs_jpeg_status = xyes])

AM_CONDITIONAL([TE_BUILD_GEOTIFF_BASED_EXAMPLE],[
test x$tlpkgs_examples_status = xyes && test x$tlpkgs_geotiff_status = xyes])


AC_DEFINE(
  [TE_DATA_EXAMPLES_LOCALE],
  ["../"],
  [This flag tells where is the base folder for the data used in examples]
)

# ------------------------------------------------------------
# Generate outputs

AC_CONFIG_SUBDIRS([image_processing])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
