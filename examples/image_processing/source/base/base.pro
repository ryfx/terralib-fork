
TERRALIB_PATH = ../../../../

win32 {
  error( "To compile LibPDI tests you must use the Visual Studio projects of each test" )
}

CONFIG = qt warn_on debug rtti exceptions

TEMPLATE = app

LANGUAGE = C++

DESTDIR = ../../bin

DEFINES += PDIAGN_DEBUG_MODE

debug {
  DEFINES += PDIAGN_ENABLE_STDOUT_LOG
}

INCLUDEPATH = \
  ../base \
  $$TERRALIB_PATH/src/tiff \
  $$TERRALIB_PATH/src/shapelib \
  $$TERRALIB_PATH/src/terralib/kernel \
  $$TERRALIB_PATH/src/terralib/functions \
  $$TERRALIB_PATH/src/terralib/image_processing \
  $$TERRALIB_PATH/src/terralib/image_processing/data_structs \
  $$INCLUDEPATH

# Library depencencies

LIBS = -L$$TERRALIB_PATH/Debug/linux-g++ -L$$TERRALIB_PATH/Release/linux-g++
LIBS += -lz
LIBS += -lterralibtiff 
LIBS += -lshapelib
LIBS += -lterralib
LIBS += -lterralibpdi

