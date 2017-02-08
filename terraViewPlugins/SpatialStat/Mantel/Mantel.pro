#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = Mantel
include( ../base.pri )
TRANSLATIONS = mantel_pt.ts \
		mantel_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/mantelplugin.h \
	./include/mantelPluginCode.h \
	./include/Mantel.h \
	./include/MantelPluginWindow.h \
	./include/ranker.h 
SOURCES +=  ./src/mantelplugin.cpp \
	./src/mantelPluginCode.cpp \
	./src/MantelPluginWindow.cpp
FORMS += ./ui/MantelForm.ui