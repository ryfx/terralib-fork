include( ../base/base.pro )

SOURCES += \
  TePDIMorfFilter_test.cpp

QMAKE_CLEAN += \
  ../../bin/Dilation*.tif \
  ../../bin/Erosion*.tif \
  ../../bin/Median*.tif