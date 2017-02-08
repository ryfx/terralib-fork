#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = EBEbin
include( ../base.pri )
TRANSLATIONS = ebebin_pt.ts \
		ebebin_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/ebebinplugin.h \
	./include/ebebinPluginCode.h \
	./include/EBEbinomial.h \
	./include/EBEBinPluginWindow.h
SOURCES +=  ./src/ebebinplugin.cpp \
	./src/ebebinPluginCode.cpp \
	./src/EBEbinomial.cpp \
	./src/EBEBinPluginWindow.cpp
FORMS += ./ui/EBEbinForm.ui