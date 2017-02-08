#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = Besag
include( ../base.pri )
TRANSLATIONS = besag_pt.ts \
		besag_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/besageplugin.h \
	./include/besagPluginCode.h \
	./include/besagfunc.h \
	./include/BesagPluginWindow.h \
	./include/matrixbesag.h
SOURCES +=  ./src/besageplugin.cpp \
	./src/besagPluginCode.cpp \
	./src/besagfunc.cpp \
	./src/BesagPluginWindow.cpp
FORMS += ./ui/BesagForm.ui