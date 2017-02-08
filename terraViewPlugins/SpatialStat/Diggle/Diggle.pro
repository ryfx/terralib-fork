#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = Diggle
include( ../base.pri )
TRANSLATIONS = diggle_pt.ts \
		diggle_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/diggleplugin.h \
	./include/digglePluginCode.h \
	./include/Diggle.h \
	./include/DigglePluginWindow.h
SOURCES +=  ./src/diggleplugin.cpp \
	./src/digglePluginCode.cpp \
	./src/Diggle.cpp \
	./src/DigglePluginWindow.cpp
FORMS += ./ui/DiggleForm.ui