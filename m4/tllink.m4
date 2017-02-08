# The following variables are used by this file
# Their values can be set before this file includes
#
# srcdirprefix - Points to the relative terralib source path (default:empty)
# bindirprefix - Points to the relative terralib source path (default:empty)

# ------------------------------------------------------------
# Solving Terralib basic mandatory modules depencencies

AM_LDFLAGS="-lterralib $AM_LDFLAGS"

AM_LDFLAGS="-lterralib_spl $AM_LDFLAGS"

# ------------------------------------------------------------
# Solving Terralib optional modules depencencies

AS_IF(
  [test "x$tlpkgs_shapelib_status" == xyes],
  [
    AM_LDFLAGS="-lterralib_shp $AM_LDFLAGS"
  ]  
)

# ------------------------------------------------------------
# Configuring the output variable used when building and executing binaries

AM_LDFLAGS="-L../$bindirprefix/bin $AM_LDFLAGS"
AC_SUBST(AM_LDFLAGS,[$AM_LDFLAGS])

AM_CPPFLAGS="-I$srcdir/../$srcdirprefix/src $AM_CPPFLAGS"
AC_SUBST(AM_CPPFLAGS,[$AM_CPPFLAGS])
