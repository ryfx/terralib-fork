SOURCES	+= ./src/postgisadapter.cpp \
	./src/postgisadapterPluginCode.cpp
HEADERS	+= ./include/postgisadapter.h \
	./include/postgisadapterPluginCode.h
#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = PostGISAdapter
include( ../base/base.pro )
TRANSLATIONS = postgisadapter_pt.ts \
		postgisadapter_sp.ts
FORMS	= ui/PostGISAdapterWindow.ui
TEMPLATE	=app
INCLUDEPATH	+= ./include
LANGUAGE	= C++
