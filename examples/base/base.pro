TERRALIBPATH = ../..

include ($$TERRALIBPATH/build/qt/config.pri)

TEMPLATE = app

# Define your mysql client path
unix:MYSQL_CLIENT = $$TERRALIBPATH/dependencies/linux/MySQL/lib
win32 {
	win32-g++:MYSQL_CLIENT=$$TERRALIBPATH/dependencies/win32/MySQL/lib/mingw
	else:MYSQL_CLIENT=$$TERRALIBPATH/dependencies/win32/MySQL/lib/ms
}

INCLUDEPATH = \
  . \
  .. \
  $$TERRALIBPATH/src/terralib/kernel \
  $$TERRALIBPATH/src/terralib/functions \
  $$TERRALIBPATH/src/terralib/stat \
  $$TERRALIBPATH/src/terralib/utils \
  $$TERRALIBPATH/src/terralib/drivers/shapelib \
  $$TERRALIBPATH/src/terralib/drivers/MySQL \
  $$TERRALIBPATH/src/terralib/drivers/MySQL/include \
  $$TERRALIBPATH/src/terralib/drivers/shapelib \
  $$TERRALIBPATH/src/shapelib \
  $$TERRALIBPATH/src/zlib \
  $$TERRALIBPATH/src/tiff \
  $$INCLUDEPATH

debug:CONFIGOUT = Debug
release:CONFIGOUT = Release

unix {
  LIBS += \
    -L/usr/local/lib \
    -L/usr/lib \
    -L$$MYSQL_CLIENT \
    -ljpeg \
    -lshapelib \
    -lz \
    -lterralibtiff \
    -lcrypt \
    -lterralib \
    -lstat
  DESTDIR = linux-g++
}

win32 {
	win32-g++:DESTDIR = win32-g++
}
 
LIBS += -ljpeg -lshapelib -lterralibtiff -lterralib -lstat -lte_mysql
unix:LIBS += -lz -lcrypt

DESTDIR = $$TERRALIBPATH/$$CONFIGOUT/examples/$$DESTDIR/$$TARGET
 