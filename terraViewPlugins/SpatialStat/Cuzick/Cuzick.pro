#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = Cuzick
include( ../base.pri )
TRANSLATIONS = cuzick_pt.ts \
		cuzick_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/cuzickplugin.h \
	./include/cuzickPluginCode.h \
	./include/Cuzick.h \
	./include/CuzickPluginWindow.h 
SOURCES +=  ./src/cuzickplugin.cpp \
	./src/cuzickPluginCode.cpp \
	./src/Cuzick.cpp \
	./src/CuzickPluginWindow.cpp
FORMS += ./ui/CuzickForm.ui