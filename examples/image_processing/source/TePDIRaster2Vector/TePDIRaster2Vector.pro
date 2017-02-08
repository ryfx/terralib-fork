include( ../base/base.pro )


SOURCES += \
  TePDIRaster2Vector_test.cpp

QMAKE_CLEAN += \
  ../../bin/Raster2Vector*.dbf \
  ../../bin/Raster2Vector*.shp \
  ../../bin/Raster2Vector*.shx \
  ../../bin/Raster2Vector*.tif
