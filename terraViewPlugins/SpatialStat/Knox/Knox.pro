#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = Knox
include( ../base.pri )
TRANSLATIONS = knox_pt.ts \
		knox_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/knoxplugin.h \
	./include/knoxPluginCode.h \
	./include/funcaux.h \
	./include/KnoxPluginWindow.h
SOURCES +=  ./src/knoxplugin.cpp \
	./src/knoxPluginCode.cpp \
	./src/KnoxPluginWindow.cpp
FORMS += ./ui/KnoxForm.ui