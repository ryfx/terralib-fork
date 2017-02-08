#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = oracleSpatialAdapter
include( ../base/base.pro )
TRANSLATIONS = oracleSpatialAdapter_pt.ts \
		oracleSpatialAdapter_es.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/oraclespatialadapterPlugin.h \
	./include/oraclespatialadapterPluginCode.h
SOURCES +=  ./src/oraclespatialadapterPlugin.cpp \
	./src/oraclespatialadapterPluginCode.cpp
FORMS += ./ui/OracleSpatialAdapterWindow.ui