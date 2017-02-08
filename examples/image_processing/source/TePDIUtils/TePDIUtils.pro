include( ../base/base.pro )

SOURCES += TePDIUtils_test.cpp

QMAKE_CLEAN += \
  ../../bin/TeAllocMemoryMappedRaster_test.tif \
  ../../bin/TeAllocRAMRaster_test.tif \
  ../../bin/TeRaster2Geotiff_test.tif \
  ../../bin/TeResetRaster_test.tif \
  ../../bin/TePDIUtils_TeAllocRAMRaster3_test.tif \
  ../../bin/TePDIUtils_exportPS2SHP_test.dbf \
  ../../bin/TePDIUtils_exportPS2SHP_test.shp \
  ../../bin/TePDIUtils_exportPS2SHP_test.shx