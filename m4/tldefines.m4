# ------------------------------------------------------------
# shapelib support
# These are the defines set
# USE_SHAPELIB

AS_IF(
  [test "x$tlpkgs_shapelib_status" == xyes],
  [
    AC_DEFINE(
      [USE_SHAPELIB],
      [1],
      [Define to enable shapelib support]
    )  
  ]  
)
# ------------------------------------------------------------
# geotiff support
# These are the defines set
# USE_GEOTIFF

AS_IF(
  [test "x$tlpkgs_geotiff_status" == xyes],
  [
    AC_DEFINE(
      [USE_GEOTIFF],
      [1],
      [Define to enable geotiff support]
    )  
  ]  
)
# ------------------------------------------------------------
# jpeg support
# These are the defines set
# USE_JPEG

AS_IF(
  [test "x$tlpkgs_jpeg_status" == xyes],
  [
    AC_DEFINE(
      [USE_JPEG],
      [1],
      [Define to enable jpeg support]
    )  
  ]  
)
# ------------------------------------------------------------
# mysql support
# These are the defines set
# USE_MYSQL

AS_IF(
  [test "x$tlpkgs_mysql_status" == xyes],
  [
    AC_DEFINE(
      [USE_MYSQL],
      [1],
      [Define to enable mysql support]
    )  
  ]  
)
# ------------------------------------------------------------
# fftw support
# These are the defines set
# USE_FFTW

AS_IF(
  [test "x$tlpkgs_fftw_status" == xyes],
  [
    AC_DEFINE(
      [USE_FFTW],
      [1],
      [Define to enable FFTW support]
    )  
  ]  
)
# ------------------------------------------------------------
# PostGIS support
# These are the defines set
# USE_POSTGIS

AS_IF(
  [test "x$tlpkgs_postgis_status" == xyes],
  [
    AC_DEFINE(
      [USE_PGIS],
      [1],
      [Define to enable PostGIS support]
    )  
  ]  
)
# ------------------------------------------------------------
# QT support
# These are the defines set
# USE_QT

AS_IF(
  [test "x$tlpkgs_qt_status" == xyes],
  [
    AC_DEFINE(
      [USE_QT],
      [1],
      [Define to enable QT support]
    )  
  ]  
)
# ------------------------------------------------------------
# GDAL support
# These are the defines set
# USE_GDAL

AS_IF(
  [test "x$tlpkgs_gdal_status" == xyes],
  [
    AC_DEFINE(
      [USE_GDAL],
      [1],
      [Define to enable GDAL support]
    )  
  ]  
)
# ------------------------------------------------------------
# LibNetPBM support
# These are the defines set
# USE_NETPBM

AS_IF(
  [test "x$tlpkgs_netpbm_status" == xyes],
  [
    AC_DEFINE(
      [USE_NETPBM],
      [1],
      [Define to enable LibNetPBM support]
    )  
  ]  
)
# ------------------------------------------------------------
# QWT support
# These are the defines set
# USE_QWT

AS_IF(
  [test "x$tlpkgs_qwt_status" == xyes],
  [
    AC_DEFINE(
      [USE_QWT],
      [1],
      [Define to enable QWT support]
    )  
  ]  
)
# ------------------------------------------------------------
# OpenMP support
# These are the defines set
# USE_OPENMP

AS_IF(
  [test "x$tlpkgs_openmp_status" == xyes],
  [
    AC_DEFINE(
      [USE_OPENMP],
      [1],
      [Define to enable OpenMP support]
    )  
  ]  
)

# ------------------------------------------------------------
# MrSID support
# These are the defines set
# USE_MRSID

AS_IF(
  [test "x$tlpkgs_mrsid_status" == xyes],
  [
    AC_DEFINE(
      [USE_MRSID],
      [1],
      [Define to enable MrSID support]
    )  
  ]  
)
